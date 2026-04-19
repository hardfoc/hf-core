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
#include <cstdint>
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
     * @brief Bus-wide discovery sweep.
     * @param[out] present_bitmap  31-byte bitmap; bit N = address N is alive.
     * @param      probe_timeout_ms Per-address probe timeout (ms).
     * @return  Number of present devices, or DriverError on bus failure.
     *
     * @note Disables the per-handler address temporarily; restored on exit.
     */
    alicat_basis2::DriverResult<std::uint8_t>
    Discover(std::uint8_t* present_bitmap, std::size_t bitmap_bytes,
             std::uint16_t probe_timeout_ms = 30) noexcept;

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
