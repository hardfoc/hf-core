/**
 * @file Fdo2Handler.h
 * @brief HAL-level handler for the PyroScience FDO2-G2 oxygen probe.
 *
 * @details Bridges the platform-agnostic templated `fdo2::Driver<UartT>`
 *          (from `hf-core-drivers/external/hf-fdo2-driver`) to a
 *          `BaseUart&` reference. Apps that already own a configured
 *          UART (acquired via `CommChannelsManager::AcquireUart`) can
 *          construct a handler with a single `BaseUart&` and the
 *          handler internally:
 *
 *            - instantiates the CRTP `fdo2::UartInterface<>` adapter
 *              that translates `BaseUart` calls into the byte-level
 *              read / write / flush primitives the driver needs,
 *            - owns the templated `fdo2::Driver<HalUartFdo2Comm>`,
 *            - serialises every transaction through a per-bus mutex
 *              (private when the handler owns the UART exclusively;
 *              shared when multiple PSUP devices live on one bus —
 *              not possible with the FDO2-G2 today, but kept for
 *              symmetry with `AlicatBasis2Handler`).
 *
 *          Public API is intentionally narrow:
 *            - `EnsureInitialized()`  → power-up settle + `#VERS` probe.
 *            - `IsPresent()`           → cached probe result.
 *            - `Identity()`            → cached `VersionInfo`.
 *            - `MeasureMoxy()` / `MeasureMraw()` → one PSUP transaction.
 *            - `ReadVersion()` / `ReadUniqueId()` → re-read identity / serial.
 *
 *          The handler does **not** spawn its own thread; the polling
 *          cadence and lock-free caching policy live in the app.
 *
 *          Pattern follows the rest of the HAL handler family
 *          (`AlicatBasis2Handler`, `Ads7952Handler`, `Tle92466edHandler`,
 *          `Max22200Handler`).
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#ifndef COMPONENT_HANDLER_FDO2_HANDLER_H_
#define COMPONENT_HANDLER_FDO2_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "RtosMutex.h"
#include "base/BaseUart.h"

#include "core/hf-core-drivers/external/hf-fdo2-driver/inc/fdo2.hpp"

//==============================================================================
// CRTP UART ADAPTER FOR BaseUart
//==============================================================================

/**
 * @brief CRTP adapter that turns a `BaseUart&` into a transport the
 *        templated `fdo2::Driver<UartT>` can drive.
 *
 * @par Notes
 *   - `BaseUart::Read` returns a status code only — not the actual byte
 *     count — so this adapter polls `BytesAvailable()` first (with the
 *     caller's timeout as the deadline for the **first** byte) then
 *     drains whatever is currently in the FIFO plus a small grace
 *     window for any in-flight bytes. PSUP lines are short
 *     (~80 chars; ~42 ms at 19200 baud) so the drain window is
 *     bounded.
 *   - All timing (delays, deadline tracking) goes through the
 *     `hf-utils-rtos-wrap` `os_*` primitives — the handler has zero
 *     direct ESP-IDF / FreeRTOS dependencies, so the same code path
 *     works against any RTOS the wrapper supports.
 */
class HalUartFdo2Comm : public fdo2::UartInterface<HalUartFdo2Comm> {
public:
    explicit HalUartFdo2Comm(BaseUart& uart) noexcept : uart_(uart) {}

    void write(const std::uint8_t* data, std::size_t length) noexcept;
    std::size_t read(std::uint8_t* out, std::size_t max,
                     std::uint32_t timeout_ms) noexcept;
    void flush_rx() noexcept;
    void delay_ms_impl(std::uint32_t ms) noexcept;

private:
    BaseUart& uart_;
};

//==============================================================================
// HANDLER CONFIGURATION
//==============================================================================

/// Construction-time configuration for one FDO2-G2 instance.
struct Fdo2HandlerConfig {
    /// Per-PSUP-transaction line read timeout (ms). Most commands
    /// resolve in well under 50 ms; #MOXY / #MRAW take ~150 ms.
    std::uint32_t line_timeout_ms{300};
    /// Specific timeout for the slow #MOXY / #MRAW measurement
    /// commands. Defaulted higher than `line_timeout_ms` because the
    /// device internally averages.
    std::uint32_t measure_timeout_ms{300};
    /// Logical index for diagnostics (multiple FDO2 instances on
    /// separate buses can share this handler class).
    std::uint8_t  device_index{0};
};

//==============================================================================
// HANDLER
//==============================================================================

/**
 * @brief HAL handler wrapping one FDO2-G2 oxygen probe on a `BaseUart`.
 *
 * Ownership: the handler holds a reference to the supplied `BaseUart`
 * (the HAL owns the actual `EspUart` via `CommChannelsManager`); the
 * handler owns the CRTP adapter + the templated driver instance.
 */
class Fdo2Handler {
public:
    using DriverType = fdo2::Driver<HalUartFdo2Comm>;

    /**
     * @brief Construct a handler around an already-configured UART.
     *
     * @param uart       Reference to a UART acquired via
     *                   `CommChannelsManager::AcquireUart`. Must outlive
     *                   the handler.
     * @param config     Per-instance timeouts + diagnostics index.
     * @param bus_mutex  Optional shared mutex (non-null) when more than
     *                   one PSUP device shares the UART. When null, the
     *                   handler uses its own private mutex.
     */
    explicit Fdo2Handler(BaseUart& uart,
                         const Fdo2HandlerConfig& config = Fdo2HandlerConfig{},
                         RtosMutex* bus_mutex = nullptr) noexcept;

    Fdo2Handler(const Fdo2Handler&)            = delete;
    Fdo2Handler& operator=(const Fdo2Handler&) = delete;
    Fdo2Handler(Fdo2Handler&&)                 = delete;
    Fdo2Handler& operator=(Fdo2Handler&&)      = delete;

    /**
     * @brief Wait the FDO2-G2 power-up settle (`kFdo2G2PowerUpSettleMs`)
     *        and probe the device with `#VERS`. Caches the returned
     *        `VersionInfo` on success. Idempotent.
     */
    bool EnsureInitialized() noexcept;

    /// True once `EnsureInitialized()` has succeeded.
    [[nodiscard]] bool IsPresent() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    /// Cached identity (zeroed until probe succeeds).
    [[nodiscard]] const fdo2::VersionInfo& Identity() const noexcept {
        return identity_;
    }

    //==========================================================================
    // Live measurement
    //==========================================================================

    /// One `#MOXY` round-trip — partial-pressure O2 + temperature + status.
    fdo2::DriverResult<fdo2::MoxyReading> MeasureMoxy() noexcept;

    /// One `#MRAW` round-trip — same as MOXY plus back-side ambient
    /// pressure, optical signal, and housing humidity. Use this when the
    /// caller wants true volume-percent O2
    /// (`100 · pO2 / pressure_mbar`).
    fdo2::DriverResult<fdo2::MrawReading> MeasureMraw() noexcept;

    /// Re-read `#VERS` (refreshes the cached `Identity`).
    fdo2::DriverResult<fdo2::VersionInfo> ReadVersion() noexcept;

    /// Read the device's 64-bit unique serial via `#IDNR`.
    fdo2::DriverResult<std::uint64_t>     ReadUniqueId() noexcept;

private:
    bool EnsureInitializedLocked() noexcept;

    Fdo2HandlerConfig          config_;
    HalUartFdo2Comm            comm_;
    std::unique_ptr<DriverType> driver_;

    fdo2::VersionInfo          identity_{};

    std::atomic<bool>          initialized_{false};

    RtosMutex                  private_mutex_;
    RtosMutex*                 bus_mutex_;
};

#endif  // COMPONENT_HANDLER_FDO2_HANDLER_H_
