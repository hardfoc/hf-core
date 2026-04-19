/**
 * @file AlicatBasis2Handler.h
 * @brief HAL-level handler for the Alicat BASIS-2 mass-flow instrument.
 *
 * @details Bridges the platform-agnostic `alicat_basis2::Driver<UartT>` to a
 *          `BaseUart` reference so any project that already owns a configured
 *          UART (in RS-485 half-duplex mode if needed) can talk to one or
 *          more BASIS-2 instruments on a shared bus.
 *
 *          Each handler instance corresponds to **one BASIS-2 slave** on
 *          the shared bus. Multiple handlers can share a single
 *          `BaseUart` — they coordinate access through the per-bus
 *          `RtosMutex` shared via `bus_mutex_`. The first handler created
 *          for a given UART seeds the mutex; later handlers must be
 *          constructed with a pointer to the same mutex object.
 *
 *          Pattern follows the rest of the HAL handler family
 *          (`Ads7952Handler`, `Tle92466edHandler`, `Max22200Handler`).
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#ifndef COMPONENT_HANDLER_ALICAT_BASIS2_HANDLER_H_
#define COMPONENT_HANDLER_ALICAT_BASIS2_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>

#include "RtosMutex.h"
#include "base/BaseUart.h"

#include "core/hf-core-drivers/external/hf-alicat-basis2-driver/inc/alicat_basis2.hpp"
#include "core/hf-core-drivers/external/hf-alicat-basis2-driver/inc/alicat_basis2_uart_interface.hpp"

//==============================================================================
// CRTP UART ADAPTER FOR BaseUart
//==============================================================================

/**
 * @brief CRTP adapter that turns a `BaseUart&` into a transport the
 *        `alicat_basis2::Driver` can drive.
 *
 * @par Notes
 *   - `BaseUart::Read` returns an error code, not a byte count, so we
 *     trust the contract that the underlying driver fulfills the request
 *     completely or returns a non-success error (timeout / no data).
 *   - `delay_ms_impl` uses `vTaskDelay` so the driver can satisfy the
 *     Modbus-RTU 3.5-character idle gap when needed.
 */
class HalUartAlicatBasis2Comm
    : public alicat_basis2::UartInterface<HalUartAlicatBasis2Comm> {
public:
    explicit HalUartAlicatBasis2Comm(BaseUart& uart) noexcept : uart_(uart) {}

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

/// Construction-time configuration for a single BASIS-2 slave.
struct AlicatBasis2HandlerConfig {
    std::uint8_t  modbus_address{1};       ///< 1..247 — must be unique on the bus.
    std::uint16_t timeout_ms{200};          ///< Per-transaction response timeout.
    std::uint8_t  device_index{0};          ///< Logical index for diagnostics / logging.
    /**
     * @brief Number of decimal places the instrument reports in its flow
     *        register (datasheet: factory-set per range). Cached at init
     *        from `ReadIdentity()` to scale the `flow` field correctly.
     */
    std::uint8_t  flow_decimals_default{3};
};

inline constexpr AlicatBasis2HandlerConfig MakeDefaultBasis2Config(std::uint8_t addr,
                                                                   std::uint8_t idx) noexcept {
    AlicatBasis2HandlerConfig c{};
    c.modbus_address       = addr;
    c.timeout_ms           = 200;
    c.device_index         = idx;
    c.flow_decimals_default = 3;
    return c;
}

//==============================================================================
// HANDLER CLASS
//==============================================================================

/**
 * @class AlicatBasis2Handler
 * @brief Thread-safe access layer to one BASIS-2 instrument on a shared bus.
 *
 * @par Lifecycle
 *   - Construct lightweight (no SPI / UART traffic).
 *   - `EnsureInitialized()` lazily probes the instrument and caches identity.
 *   - All read / write entry points are mutex-guarded by the per-bus mutex.
 *
 * @par Multi-instrument bus
 *   Pass the same `BaseUart` to every handler that shares the bus. The
 *   first handler created with a `nullptr` mutex pointer creates the
 *   shared mutex; subsequent handlers must be constructed with a pointer
 *   to that same mutex object so transactions are properly serialised.
 */
class AlicatBasis2Handler {
public:
    using DriverType = alicat_basis2::Driver<HalUartAlicatBasis2Comm>;

    /**
     * @param uart       Reference to the (already-configured) BaseUart.
     * @param config     Per-slave configuration.
     * @param bus_mutex  Optional shared mutex when several handlers share
     *                   the same bus. Pass `nullptr` for a private mutex.
     */
    AlicatBasis2Handler(BaseUart& uart,
                        const AlicatBasis2HandlerConfig& config,
                        RtosMutex* bus_mutex = nullptr) noexcept;

    AlicatBasis2Handler(const AlicatBasis2Handler&)            = delete;
    AlicatBasis2Handler& operator=(const AlicatBasis2Handler&) = delete;

    /// Lazy initialisation; reads identity, caches `flow_decimals_`.
    bool EnsureInitialized() noexcept;
    bool IsInitialized() const noexcept { return initialized_.load(std::memory_order_acquire); }

    std::uint8_t Address() const noexcept     { return config_.modbus_address; }
    std::uint8_t DeviceIndex() const noexcept { return config_.device_index; }

    /// Cached identity (filled by `EnsureInitialized`).
    const alicat_basis2::InstrumentIdentity& Identity() const noexcept { return identity_; }
    std::uint8_t FlowDecimals() const noexcept { return flow_decimals_; }

    //--------------------------------------------------------------------------
    // High-level operations (mutex-guarded)
    //--------------------------------------------------------------------------

    alicat_basis2::DriverResult<alicat_basis2::InstantaneousData>
    ReadInstantaneous() noexcept;

    alicat_basis2::DriverResult<alicat_basis2::InstrumentIdentity>
    RereadIdentity() noexcept;

    alicat_basis2::DriverResult<void> Tare() noexcept;
    alicat_basis2::DriverResult<void> SetGas(alicat_basis2::Gas g) noexcept;
    alicat_basis2::DriverResult<void> SetSetpoint(float user_units) noexcept;
    alicat_basis2::DriverResult<void> SetSetpointSource(alicat_basis2::SetpointSource s) noexcept;
    alicat_basis2::DriverResult<void> SetCommWatchdogMs(std::uint16_t ms) noexcept;
    alicat_basis2::DriverResult<void> SetMaxSetpointRamp(std::uint32_t pct_per_ms_x_10e7) noexcept;
    alicat_basis2::DriverResult<void> SetAutotareEnabled(bool enabled) noexcept;
    alicat_basis2::DriverResult<void> ResetTotalizer() noexcept;
    alicat_basis2::DriverResult<void> SetTotalizerLimitMode(alicat_basis2::TotalizerLimitMode m) noexcept;
    alicat_basis2::DriverResult<void> SetTotalizerBatch(std::uint32_t value_scaled) noexcept;
    alicat_basis2::DriverResult<void> SetFlowAveragingMs(std::uint16_t ms) noexcept;
    alicat_basis2::DriverResult<void> SetReferenceTemperatureC(float c) noexcept;
    alicat_basis2::DriverResult<void> ConfigureMeasurementTrigger(std::uint16_t bits) noexcept;
    alicat_basis2::DriverResult<void> StartMeasurementSamples(std::uint16_t n) noexcept;
    alicat_basis2::DriverResult<alicat_basis2::MeasurementData> ReadMeasurement() noexcept;
    alicat_basis2::DriverResult<void> SetModbusAddress(std::uint8_t addr) noexcept;
    alicat_basis2::DriverResult<void> SetAsciiUnitId(char id) noexcept;
    alicat_basis2::DriverResult<void> SetBaudRate(alicat_basis2::BaudRate br) noexcept;
    alicat_basis2::DriverResult<void> FactoryRestore() noexcept;

    /**
     * @brief Bus-wide discovery sweep at the **current host baud only**.
     * @param[out] present_bitmap  ≥32-byte bitmap; bit N = address N is alive.
     * @param      bitmap_bytes    Size of `present_bitmap` (must be ≥32).
     * @param      probe_timeout_ms Per-address probe timeout (ms).
     * @return  Number of present devices, or DriverError on bus failure.
     *
     * @note Disables the per-handler address temporarily; restored on exit.
     *       This finds devices that match the host UART's current baud
     *       only — use `DiscoverAcrossBauds()` to also find devices at
     *       other baud rates.
     */
    alicat_basis2::DriverResult<std::uint8_t>
    Discover(std::uint8_t* present_bitmap, std::size_t bitmap_bytes,
             std::uint16_t probe_timeout_ms = 30) noexcept;

    //--------------------------------------------------------------------------
    // Multi-baud discovery + bus normalisation
    //--------------------------------------------------------------------------

    /**
     * @brief One discovered instrument and the baud rate it was found at.
     */
    struct DiscoveredDevice {
        std::uint8_t  address{0};        ///< Modbus slave address (1..247).
        std::uint32_t baud_bps{0};       ///< Baud rate the device responded at.
    };

    /**
     * @brief Callback used by `DiscoverAcrossBauds` / `NormalizeBusBaud` to
     *        retune the **host UART** when probing a new baud rate.
     *
     * @return `true` if the host UART successfully switched to `bps`,
     *         `false` to skip that baud entry.
     *
     * @note This is intentionally injected from the application — the
     *       handler only knows about a `BaseUart&` (which has no
     *       runtime baud setter), so the caller wires in whichever
     *       MCU-specific call applies (e.g.
     *       `EspUart::SetBaudRate(bps)`).
     */
    using HostBaudSetter = std::function<bool(std::uint32_t bps)>;

    /**
     * @brief Sweep multiple baud rates, returning every BASIS-2 found
     *        on the bus along with the baud rate it answered at.
     *
     * @details For each baud in `baud_list` the handler:
     *            1. Calls `set_host_baud(baud)` to retune the host UART.
     *            2. Sleeps `settle_ms` so any stale RX bytes drain.
     *            3. Runs `DiscoverPresentAddresses()` at that baud.
     *            4. Records `{addr, baud}` for every responder that
     *               isn't already in `out` (first-seen-baud wins).
     *
     *          When `baud_list` is `nullptr` the handler iterates over
     *          every value in `alicat_basis2::BaudRate`
     *          (4800/9600/19200/38400/57600/115200).
     *
     *          On exit the host UART is restored to whatever baud was
     *          active **last** — typically you should follow the call
     *          with `NormalizeBusBaud()` (which retunes again) or an
     *          explicit `set_host_baud(target)`.
     *
     * @param[out] out               Destination buffer (caller-allocated).
     * @param      max_devices       Capacity of `out` in elements.
     * @param      set_host_baud     Callback that retunes the host UART.
     * @param      probe_timeout_ms  Per-address timeout passed to the sweep.
     * @param      settle_ms         Delay after each baud change to let
     *                               UART glitches subside (default 25 ms).
     * @param      baud_list_bps     Optional explicit baud-rate list in
     *                               bits-per-second. `nullptr` = all six
     *                               supported rates.
     * @param      baud_list_count   Number of entries in `baud_list_bps`.
     * @return     Number of devices populated into `out` on success;
     *             `BufferTooSmall` if `out` overflows; transport error
     *             if `set_host_baud` rejects every requested rate.
     */
    alicat_basis2::DriverResult<std::size_t>
    DiscoverAcrossBauds(DiscoveredDevice*       out,
                        std::size_t             max_devices,
                        const HostBaudSetter&   set_host_baud,
                        std::uint16_t           probe_timeout_ms = 30,
                        std::uint32_t           settle_ms        = 25,
                        const std::uint32_t*    baud_list_bps    = nullptr,
                        std::size_t             baud_list_count  = 0) noexcept;

    /**
     * @brief Normalise every device on the bus to the same baud rate.
     *
     * @details For every `{addr, baud}` in `devices` whose `baud != target_bps`:
     *            1. Retune the host UART to the device's current baud.
     *            2. Address that slave and issue `SetBaudRate(target)`.
     *               The device switches immediately — the response may
     *               be lost or garbled (datasheet calls this out
     *               explicitly), so the operation is logged but never
     *               aborts on a single failure.
     *            3. Retune the host UART to `target_bps`.
     *            4. Verify with a short identity read; on failure the
     *               device is left in `failed_addresses` (if provided).
     *
     *          Devices already at `target_bps` are skipped (verified
     *          only). On exit the host UART is left at `target_bps`.
     *
     * @param      target_bps        Final baud rate everyone should use.
     * @param      devices           Output of `DiscoverAcrossBauds()`.
     * @param      device_count      Length of `devices`.
     * @param      set_host_baud     Callback that retunes the host UART.
     * @param[out] failed_addresses  Optional buffer for slaves that
     *                               could not be normalised; use a
     *                               32-byte bitmap.
     * @param      verify_timeout_ms Per-device verify timeout (ms).
     * @return     Number of devices that successfully end up at
     *             `target_bps` (verified by identity read), or
     *             transport / parameter error.
     */
    alicat_basis2::DriverResult<std::size_t>
    NormalizeBusBaud(std::uint32_t           target_bps,
                     const DiscoveredDevice* devices,
                     std::size_t             device_count,
                     const HostBaudSetter&   set_host_baud,
                     std::uint8_t*           failed_addresses_bitmap = nullptr,
                     std::size_t             failed_bitmap_bytes     = 0,
                     std::uint16_t           verify_timeout_ms       = 100) noexcept;

    /// Direct access to the underlying driver (for advanced use; mutex must be held).
    DriverType* GetDriver() noexcept { return driver_.get(); }

private:
    AlicatBasis2HandlerConfig          config_;
    HalUartAlicatBasis2Comm            comm_;
    std::unique_ptr<DriverType>        driver_;
    alicat_basis2::InstrumentIdentity  identity_{};
    std::uint8_t                       flow_decimals_{3};

    std::atomic<bool>                  initialized_{false};

    RtosMutex                          private_mutex_;
    RtosMutex*                         bus_mutex_{&private_mutex_};

    bool EnsureInitializedLocked() noexcept;
};

#endif  // COMPONENT_HANDLER_ALICAT_BASIS2_HANDLER_H_
