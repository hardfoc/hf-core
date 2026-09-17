/**
 * @file Bmp581Handler.h
 * @brief Handler for the Bosch BMP581 barometric pressure sensor on a `BaseI2c`.
 *
 * @details Bridges the templated `bmp581::Driver<BusT>` (hf-bmp581-driver) to
 *          the HardFOC `BaseI2c` device interface through an internal CRTP
 *          register adapter, and exposes a narrow, mutex-protected API:
 *          probe (chip id + NVM ready), configure, read pressure/temperature.
 *
 *          Ownership: the caller owns the `BaseI2c` device (address 0x46/0x47
 *          pre-configured); the handler owns the adapter and the driver.
 *          Bring-up is lazy and idempotent.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_BMP581_HANDLER_H_
#define COMPONENT_HANDLER_BMP581_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "base/BaseI2c.h"
#include "core/hf-core-drivers/external/hf-bmp581-driver/inc/bmp581.hpp"
#include "RtosMutex.h"

//==============================================================================
// CRTP REGISTER ADAPTER
//==============================================================================

/**
 * @class HalI2cBmp581Comm
 * @brief CRTP register transport bridging `bmp581::BusInterface` to a `BaseI2c` device.
 *
 * @details Frames writes as `[reg, data…]` and reads as write-`reg` /
 *          read-`len` through member scratch buffers (internal SRAM).
 *
 * @note Does not own the `BaseI2c`; it must outlive the adapter.
 */
class HalI2cBmp581Comm : public bmp581::BusInterface<HalI2cBmp581Comm> {
public:
    explicit HalI2cBmp581Comm(BaseI2c& i2c_device) noexcept;

    /// @name CRTP-required methods (static dispatch from bmp581::BusInterface)
    /// @{
    bool WriteRegs(std::uint8_t reg, const std::uint8_t* data, std::size_t len) noexcept;
    bool ReadRegs(std::uint8_t reg, std::uint8_t* out, std::size_t len) noexcept;
    void DelayUs(std::uint32_t us) noexcept;
    bool EnsureInitialized() noexcept;
    /// @}

private:
    BaseI2c& i2c_device_;
    std::uint8_t tx_scratch_[8]{};
    std::uint8_t rx_scratch_[8]{};
    std::uint8_t reg_scratch_{0};
};

//==============================================================================
// HANDLER CONFIGURATION
//==============================================================================

/// Construction-time configuration for one BMP581.
struct Bmp581HandlerConfig {
    /// Sensor configuration applied by `EnsureInitialized()`.
    bmp581::Config config{bmp581::BarometricReferencePreset()};
    /// Soft reset before configuring (recommended; clears stale IIR / mode).
    bool soft_reset_on_init{true};
    /// Logical index for diagnostics.
    std::uint8_t device_index{0};
};

//==============================================================================
// HANDLER
//==============================================================================

/**
 * @brief Handler wrapping one BMP581 on a `BaseI2c` device.
 */
class Bmp581Handler {
public:
    using DriverType = bmp581::Driver<HalI2cBmp581Comm>;

    /**
     * @param i2c_device Configured `BaseI2c` device. Must outlive the handler.
     * @param config     Sensor preset + bring-up options.
     * @param bus_mutex  Optional shared mutex when several devices share the bus; private when null.
     */
    explicit Bmp581Handler(BaseI2c& i2c_device,
                           const Bmp581HandlerConfig& config = Bmp581HandlerConfig{},
                           RtosMutex* bus_mutex = nullptr) noexcept;

    Bmp581Handler(const Bmp581Handler&) = delete;
    Bmp581Handler& operator=(const Bmp581Handler&) = delete;
    Bmp581Handler(Bmp581Handler&&) = delete;
    Bmp581Handler& operator=(Bmp581Handler&&) = delete;

    /**
     * @brief Probe (chip id + NVM ready), optional soft reset, apply the
     *        configured preset. Idempotent.
     * @return true when the device answered and accepted the configuration.
     */
    bool EnsureInitialized() noexcept;

    /// True once `EnsureInitialized()` has succeeded.
    [[nodiscard]] bool IsPresent() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    /// Chip id read at probe (0 until present).
    [[nodiscard]] std::uint8_t ChipId() const noexcept { return chip_id_; }

    /// Effective OSR / ODR validity captured after configuration.
    [[nodiscard]] const bmp581::EffectiveOsr& EffectiveOsr() const noexcept { return effective_; }

    //==========================================================================
    // Live data
    //==========================================================================

    /// Whether a fresh sample is waiting (INT_STATUS.drdy).
    bmp581::DriverResult<bool> DataReady() noexcept;

    /// Read pressure [Pa] and temperature [°C].
    bmp581::DriverResult<bmp581::Sample> ReadSample() noexcept;

    /// Re-apply a new configuration (standby hop handled by the driver).
    bmp581::DriverResult<void> Configure(const bmp581::Config& cfg) noexcept;

    /// Soft reset; the handler re-applies the current configuration.
    bmp581::DriverResult<void> Reset() noexcept;

    /// Diagnostics counters since construction.
    struct Counters {
        std::uint32_t samples{0};
        std::uint32_t bus_errors{0};
    };
    [[nodiscard]] Counters GetCounters() const noexcept;

private:
    bool EnsureInitializedLocked() noexcept;

    Bmp581HandlerConfig config_;
    HalI2cBmp581Comm comm_;
    std::unique_ptr<DriverType> driver_;

    std::uint8_t chip_id_{0};
    bmp581::EffectiveOsr effective_{};
    std::atomic<bool> initialized_{false};

    std::atomic<std::uint32_t> samples_{0};
    std::atomic<std::uint32_t> bus_errors_{0};

    RtosMutex private_mutex_;
    RtosMutex* bus_mutex_;
};

#endif  // COMPONENT_HANDLER_BMP581_HANDLER_H_
