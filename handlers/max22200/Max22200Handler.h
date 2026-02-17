/**
 * @file Max22200Handler.h
 * @brief Unified handler for MAX22200 octal solenoid and motor driver.
 *
 * @details
 * Provides HAL-level integration for the MAX22200 using BaseSpi and BaseGpio.
 * Features:
 * - CRTP SPI adapter bridging BaseSpi to the MAX22200 two-phase SPI protocol
 * - 8-channel high/low-side output control with CDR/VDR drive modes
 * - Channel-pair modes (independent, parallel, half-bridge, full-bridge)
 * - Comprehensive fault monitoring and DPM (Detect Plunger Movement)
 * - Convenience APIs with real-unit parameters (mA, %, ms)
 * - Thread-safe operations with RtosMutex
 * - Lazy initialization pattern
 * - Full driver access through GetDriver() for advanced operations
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_MAX22200_HANDLER_H_
#define COMPONENT_HANDLER_MAX22200_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include "core/hf-core-drivers/external/hf-max22200-driver/inc/max22200.hpp"
#include "core/hf-core-drivers/external/hf-max22200-driver/inc/max22200_spi_interface.hpp"
#include "core/hf-core-drivers/external/hf-max22200-driver/inc/max22200_types.hpp"
#include "base/BaseSpi.h"
#include "base/BaseGpio.h"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/utils/RtosMutex.h"

///////////////////////////////////////////////////////////////////////////////
/// @defgroup MAX22200_HAL_CommAdapter HAL Communication Adapter
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class HalSpiMax22200Comm
 * @brief CRTP SPI communication adapter for MAX22200 using BaseSpi and BaseGpio.
 *
 * Implements all methods required by max22200::SpiInterface<HalSpiMax22200Comm>.
 * Handles the two-phase CMD-pin SPI protocol.
 */
class HalSpiMax22200Comm : public max22200::SpiInterface<HalSpiMax22200Comm> {
public:
    /**
     * @brief Construct the SPI adapter.
     * @param spi    Reference to pre-configured BaseSpi (Mode 0, 8-bit).
     * @param enable BaseGpio for ENABLE pin (active HIGH).
     * @param cmd    BaseGpio for CMD pin (HIGH=command phase, LOW=data phase).
     * @param fault  Optional BaseGpio for nFAULT (active LOW, open-drain).
     */
    HalSpiMax22200Comm(BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
                        BaseGpio* fault = nullptr) noexcept;

    /// @name CRTP-Required Methods
    /// @{

    bool Initialize() noexcept;
    bool Transfer(const uint8_t* tx_data, uint8_t* rx_data, size_t length) noexcept;
    bool SetChipSelect(bool state) noexcept;
    bool Configure(uint32_t speed_hz, uint8_t mode, bool msb_first = true) noexcept;
    [[nodiscard]] bool IsReady() const noexcept;
    void DelayUs(uint32_t us) noexcept;
    void GpioSet(max22200::CtrlPin pin, max22200::GpioSignal signal) noexcept;
    bool GpioRead(max22200::CtrlPin pin, max22200::GpioSignal& signal) noexcept;

    /// @}

private:
    BaseSpi&   spi_;
    BaseGpio&  enable_;
    BaseGpio&  cmd_;
    BaseGpio*  fault_;
    bool       initialized_{false};
};

/// @}

///////////////////////////////////////////////////////////////////////////////
/// @defgroup MAX22200_Handler Main Handler Class
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Max22200Handler
 * @brief Unified handler for MAX22200 octal solenoid/motor driver.
 *
 * Provides thread-safe access to all MAX22200 driver features:
 * - 8-channel output control (enable/disable)
 * - CDR (Current Drive Regulation) and VDR (Voltage Drive Regulation) modes
 * - HIT/HOLD current and timing configuration
 * - Channel-pair modes (independent, parallel, half-bridge, full-bridge)
 * - Fault monitoring and diagnostics
 * - DPM (Detect Plunger Movement) configuration
 * - Convenience APIs with real-unit parameters
 */
class Max22200Handler {
public:
    /// @brief Driver type alias
    using DriverType = max22200::MAX22200<HalSpiMax22200Comm>;

    static constexpr uint8_t kNumChannels = 8;

    //=========================================================================
    // Construction
    //=========================================================================

    /**
     * @brief Construct MAX22200 handler.
     * @param spi    Reference to pre-configured BaseSpi (Mode 0).
     * @param enable BaseGpio for ENABLE pin (active HIGH).
     * @param cmd    BaseGpio for CMD pin.
     * @param fault  Optional BaseGpio for nFAULT (active LOW).
     */
    Max22200Handler(BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
                    BaseGpio* fault = nullptr) noexcept;

    ~Max22200Handler() noexcept;

    // Non-copyable
    Max22200Handler(const Max22200Handler&) = delete;
    Max22200Handler& operator=(const Max22200Handler&) = delete;

    // Non-movable
    Max22200Handler(Max22200Handler&&) = delete;
    Max22200Handler& operator=(Max22200Handler&&) = delete;

    //=========================================================================
    // Initialization
    //=========================================================================

    /**
     * @brief Initialize driver (ENABLE HIGH, read/clear STATUS, set ACTIVE).
     * @return DriverStatus::OK on success, specific error code on failure.
     */
    max22200::DriverStatus Initialize() noexcept;

    /**
     * @brief Ensure driver is initialized (lazy init entrypoint).
     * @return true if initialized and ready.
     */
    bool EnsureInitialized() noexcept;

    /**
     * @brief Initialize with board configuration.
     * @param board_config Board-specific IFS and safety limits.
     * @return DriverStatus::OK on success, specific error code on failure.
     */
    max22200::DriverStatus Initialize(const max22200::BoardConfig& board_config) noexcept;

    /** @brief Deinitialize — disable all channels, ACTIVE=0, ENABLE LOW. */
    max22200::DriverStatus Deinitialize() noexcept;

    /** @brief Check if initialized. */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    //=========================================================================
    // Channel Configuration
    //=========================================================================

    /**
     * @brief Configure a channel completely.
     * @param channel Channel number (0-7).
     * @param config  Full channel configuration.
     * @return DriverStatus::OK on success, specific error code on failure.
     */
    max22200::DriverStatus ConfigureChannel(uint8_t channel, const max22200::ChannelConfig& config) noexcept;

    /**
     * @brief Quick CDR setup with milliamp values.
     * @param channel Channel number (0-7).
     * @param hit_ma  HIT current in milliamps.
     * @param hold_ma HOLD current in milliamps.
     * @param hit_time_ms HIT time in milliseconds.
     * @return DriverStatus::OK on success, first failing operation's error code.
     */
    max22200::DriverStatus SetupCdrChannel(uint8_t channel, uint16_t hit_ma,
                         uint16_t hold_ma, float hit_time_ms) noexcept;

    /**
     * @brief Quick VDR setup with duty cycle percentages.
     * @param channel Channel number (0-7).
     * @param hit_duty_pct  HIT duty cycle percentage.
     * @param hold_duty_pct HOLD duty cycle percentage.
     * @param hit_time_ms   HIT time in milliseconds.
     * @return DriverStatus::OK on success, first failing operation's error code.
     */
    max22200::DriverStatus SetupVdrChannel(uint8_t channel, float hit_duty_pct,
                         float hold_duty_pct, float hit_time_ms) noexcept;

    //=========================================================================
    // Channel Control
    //=========================================================================

    /** @brief Enable a channel. */
    max22200::DriverStatus EnableChannel(uint8_t channel) noexcept;

    /** @brief Disable a channel. */
    max22200::DriverStatus DisableChannel(uint8_t channel) noexcept;

    /** @brief Enable all channels. */
    max22200::DriverStatus EnableAllChannels() noexcept;

    /** @brief Disable all channels. */
    max22200::DriverStatus DisableAllChannels() noexcept;

    /** @brief Check if a channel is enabled. */
    bool IsChannelEnabled(uint8_t channel) noexcept;

    /**
     * @brief Set channels enabled by bitmask.
     * @param mask Bitmask of channels to enable (bit 0 = CH0, etc.).
     */
    max22200::DriverStatus SetChannelsMask(uint8_t mask) noexcept;

    //=========================================================================
    // Status & Faults
    //=========================================================================

    /**
     * @brief Read the STATUS register.
     * @param[out] status Status structure to fill.
     * @return DriverStatus::OK on success.
     */
    max22200::DriverStatus GetStatus(max22200::StatusConfig& status) noexcept;

    /**
     * @brief Read fault flags for a channel.
     * @param channel Channel number (0-7).
     * @param[out] faults Fault status structure.
     * @return DriverStatus::OK on success.
     */
    max22200::DriverStatus GetChannelFaults(uint8_t channel, max22200::FaultStatus& faults) noexcept;

    /** @brief Check if any fault is present. */
    bool HasFault() noexcept;

    /** @brief Clear all fault flags. */
    max22200::DriverStatus ClearFaults() noexcept;

    //=========================================================================
    // Device Control
    //=========================================================================

    /**
     * @brief Read fault register into status.
     * @param[out] faults Fault status.
     * @return DriverStatus::OK on success.
     */
    max22200::DriverStatus ReadFaultRegister(max22200::FaultStatus& faults) noexcept;

    //=========================================================================
    // Direct Driver Access
    //=========================================================================

    /**
     * @brief Get the underlying driver for advanced operations.
     * @return Pointer to driver, or nullptr if not initialized.
     */
    [[nodiscard]] DriverType* GetDriver() noexcept;
    [[nodiscard]] const DriverType* GetDriver() const noexcept;

    /** @brief Dump diagnostics to logger. */
    void DumpDiagnostics() noexcept;

private:
    bool EnsureInitializedLocked() noexcept;

    /**
     * @brief Execute a lambda with a locked, initialized driver.
     *
     * Acquires the mutex, ensures initialization, and invokes @p fn(*driver_).
     * Returns a default-constructed R on failure.
     */
    template <typename Fn>
    auto withDriver(Fn&& fn) noexcept {
        using R = std::invoke_result_t<Fn, DriverType&>;
        static_assert(!std::is_void_v<R>, "withDriver requires non-void return");
        MutexLockGuard lock(mutex_);
        if (!EnsureInitializedLocked() || !driver_) {
            if constexpr (std::is_same_v<R, max22200::DriverStatus>)
                return max22200::DriverStatus::INITIALIZATION_ERROR;
            else
                return R{};
        }
        return fn(*driver_);
    }

    bool initialized_{false};
    mutable RtosMutex mutex_;
    std::unique_ptr<HalSpiMax22200Comm> comm_;
    std::unique_ptr<DriverType> driver_;
};

/// @}

#endif // COMPONENT_HANDLER_MAX22200_HANDLER_H_
