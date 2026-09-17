/**
 * @file SfmHandler.h
 * @brief Handler for Sensirion SF06-family gas mass flow meters (SFM4300, …) on a `BaseI2c`.
 *
 * @details Bridges the templated `sfm::Driver<I2cT>` (hf-sfm-flow-meter-driver)
 *          to the HardFOC `BaseI2c` device interface through an internal CRTP
 *          adapter, and exposes a narrow, mutex-protected API for the layers
 *          above: probe identity, start a gas table, read one frame, stop.
 *
 *          Ownership: the caller owns the `BaseI2c` device (address
 *          pre-configured, 0x2A default for SFM4300); the handler owns the
 *          adapter and the driver instance. Bring-up is lazy and idempotent.
 *
 *          Gas-table gating: after the product identifier is read the driver
 *          refuses tables the part does not carry (SFM4300-50-x has no CO2 or
 *          N2O tables). `Start()` reports `UnsupportedGas` in that case so the
 *          caller can choose a fallback and flag the reading — the handler
 *          never substitutes a table silently.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_SFM_HANDLER_H_
#define COMPONENT_HANDLER_SFM_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "base/BaseI2c.h"
#include "core/hf-core-drivers/external/hf-sfm-flow-meter-driver/inc/sfm.hpp"
#include "RtosMutex.h"

//==============================================================================
// CRTP I2C ADAPTER
//==============================================================================

/**
 * @class HalI2cSfmComm
 * @brief CRTP transport bridging `sfm::I2cInterface` to a `BaseI2c` device.
 *
 * @details SF06 parts use raw command writes and raw multi-byte reads (no
 *          register byte). The adapter validates the driver's 7-bit address
 *          against the device's configured address, frames through member
 *          scratch buffers (internal SRAM, not task stack), and maps a NACKed
 *          read to `false` so the driver can report `NoData`.
 *
 * @note Does not own the `BaseI2c`; it must outlive the adapter.
 */
class HalI2cSfmComm : public sfm::I2cInterface<HalI2cSfmComm> {
public:
    explicit HalI2cSfmComm(BaseI2c& i2c_device) noexcept;

    /// @name CRTP-required methods (static dispatch from sfm::I2cInterface)
    /// @{
    bool Write(std::uint8_t addr7, const std::uint8_t* data, std::size_t len) noexcept;
    bool Read(std::uint8_t addr7, std::uint8_t* out, std::size_t len) noexcept;
    void DelayMs(std::uint32_t ms) noexcept;
    bool EnsureInitialized() noexcept;
    /// @}

    /// Address configured on the bound `BaseI2c` device.
    [[nodiscard]] std::uint8_t DeviceAddress() const noexcept;

private:
    BaseI2c& i2c_device_;
    std::uint8_t tx_scratch_[8]{};
    std::uint8_t rx_scratch_[18]{};
};

//==============================================================================
// HANDLER CONFIGURATION
//==============================================================================

/// Construction-time configuration for one SF06 flow meter.
struct SfmHandlerConfig {
    /// Gas table requested at `Start()` when the caller passes none.
    sfm::Gas default_gas{sfm::Gas::Air};
    /// O2 volume fraction (‰) for mixture tables.
    std::uint16_t o2_permille{0};
    /// On-sensor averaging: 0 = average-until-read, 1…128 = fixed-N.
    std::uint16_t averaging_window{0};
    /// Issue a general-call soft reset during bring-up when the bus supports it.
    bool soft_reset_on_init{false};
    /// Logical index for diagnostics.
    std::uint8_t device_index{0};
};

//==============================================================================
// HANDLER
//==============================================================================

/**
 * @brief Handler wrapping one SF06 flow meter on a `BaseI2c` device.
 */
class SfmHandler {
public:
    using DriverType = sfm::Driver<HalI2cSfmComm>;

    /**
     * @param i2c_device Configured `BaseI2c` device (address = sensor address). Must outlive the handler.
     * @param config     Gas / averaging / diagnostics.
     * @param bus_mutex  Optional shared mutex when several devices share the bus; private when null.
     */
    explicit SfmHandler(BaseI2c& i2c_device,
                        const SfmHandlerConfig& config = SfmHandlerConfig{},
                        RtosMutex* bus_mutex = nullptr) noexcept;

    SfmHandler(const SfmHandler&) = delete;
    SfmHandler& operator=(const SfmHandler&) = delete;
    SfmHandler(SfmHandler&&) = delete;
    SfmHandler& operator=(SfmHandler&&) = delete;

    /**
     * @brief Probe the device: optional soft reset, stop any running
     *        measurement, read product identifier + serial. Idempotent.
     * @return true when the identifier was read (device present).
     */
    bool EnsureInitialized() noexcept;

    /// True once `EnsureInitialized()` has succeeded.
    [[nodiscard]] bool IsPresent() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    /// Cached identity (zeroed until probe succeeds).
    [[nodiscard]] const sfm::ProductInfo& Identity() const noexcept { return identity_; }

    /// Whether the probed variant carries a factory table for @p gas.
    [[nodiscard]] bool SupportsGas(sfm::Gas gas) const noexcept;

    /// Full-scale flow of the probed variant [slm] (0 when unknown).
    [[nodiscard]] float FullScaleSlm() const noexcept;

    //==========================================================================
    // Measurement session
    //==========================================================================

    /**
     * @brief Start continuous measurement on @p gas (reads scale/offset first)
     *        and apply the configured averaging window.
     * @return Driver result; `UnsupportedGas` when the variant lacks the table.
     */
    sfm::DriverResult<void> Start(sfm::Gas gas) noexcept;

    /// `Start(config.default_gas)`.
    sfm::DriverResult<void> Start() noexcept { return Start(config_.default_gas); }

    /// Stop continuous measurement (idle).
    sfm::DriverResult<void> Stop() noexcept;

    /// Read one flow / temperature / status frame (`NoData` when no fresh sample).
    sfm::DriverResult<sfm::Measurement> ReadMeasurement() noexcept;

    /// Change the averaging window while measuring (0 = average-until-read).
    sfm::DriverResult<void> ConfigureAveraging(std::uint16_t window) noexcept;

    /// Update the O2 fraction of a running mixture table.
    sfm::DriverResult<void> UpdateConcentration(std::uint16_t o2_permille) noexcept;

    /// Gas selected by the last successful `Start()`.
    [[nodiscard]] sfm::Gas ActiveGas() const noexcept;
    /// Scale / offset / unit captured at `Start()`.
    [[nodiscard]] sfm::Scaling ActiveScaling() const noexcept;
    /// True while a continuous measurement is running.
    [[nodiscard]] bool Measuring() const noexcept;

    /// Diagnostics counters since construction.
    struct Counters {
        std::uint32_t frames{0};
        std::uint32_t no_data{0};
        std::uint32_t crc_errors{0};
        std::uint32_t bus_errors{0};
    };
    [[nodiscard]] Counters GetCounters() const noexcept;

private:
    bool EnsureInitializedLocked() noexcept;

    SfmHandlerConfig config_;
    HalI2cSfmComm comm_;
    std::unique_ptr<DriverType> driver_;

    sfm::ProductInfo identity_{};
    std::atomic<bool> initialized_{false};

    std::atomic<std::uint32_t> frames_{0};
    std::atomic<std::uint32_t> no_data_{0};
    std::atomic<std::uint32_t> crc_errors_{0};
    std::atomic<std::uint32_t> bus_errors_{0};

    RtosMutex private_mutex_;
    RtosMutex* bus_mutex_;
};

#endif  // COMPONENT_HANDLER_SFM_HANDLER_H_
