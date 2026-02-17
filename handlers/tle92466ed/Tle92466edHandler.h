/**
 * @file Tle92466edHandler.h
 * @brief Unified handler for TLE92466ED six-channel low-side solenoid driver IC.
 *
 * @details
 * Provides HAL-level integration for the TLE92466ED gate driver using BaseSpi and BaseGpio.
 * Features:
 * - CRTP SPI adapter bridging BaseSpi to the TLE92466ED driver
 * - 6-channel solenoid/valve control with current regulation
 * - Comprehensive fault monitoring and diagnostic reporting
 * - SPI watchdog management
 * - Thread-safe operations with RtosMutex
 * - Lazy initialization pattern
 * - Full driver access through GetDriver() for advanced operations
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_TLE92466ED_HANDLER_H_
#define COMPONENT_HANDLER_TLE92466ED_HANDLER_H_

#include <cstdint>
#include <memory>
#include <cstdarg>
#include <span>
#include <type_traits>
#include "core/hf-core-drivers/external/hf-tle92466ed-driver/inc/tle92466ed.hpp"
#include "core/hf-core-drivers/external/hf-tle92466ed-driver/inc/tle92466ed_spi_interface.hpp"
#include "base/BaseSpi.h"
#include "base/BaseGpio.h"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/utils/RtosMutex.h"

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TLE92466ED_HAL_CommAdapter HAL Communication Adapter
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class HalSpiTle92466edComm
 * @brief CRTP SPI communication adapter for TLE92466ED using BaseSpi and BaseGpio.
 *
 * Implements all methods required by tle92466ed::SpiInterface<HalSpiTle92466edComm>.
 */
class HalSpiTle92466edComm : public tle92466ed::SpiInterface<HalSpiTle92466edComm> {
public:
    /**
     * @brief Construct the SPI adapter.
     * @param spi    Reference to pre-configured BaseSpi.
     * @param resn   BaseGpio connected to RESN (reset, active LOW).
     * @param en     BaseGpio connected to EN (enable, active HIGH).
     * @param faultn Optional BaseGpio connected to FAULTN (active LOW input).
     */
    HalSpiTle92466edComm(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                          BaseGpio* faultn = nullptr) noexcept;

    /// @name CRTP-Required Methods
    /// @{

    // Bring base class variadic Log into scope (avoids name hiding)
    using tle92466ed::SpiInterface<HalSpiTle92466edComm>::Log;

    tle92466ed::CommResult<void> Init() noexcept;
    tle92466ed::CommResult<void> Deinit() noexcept;
    tle92466ed::CommResult<uint32_t> Transfer32(uint32_t tx_data) noexcept;
    tle92466ed::CommResult<void> TransferMulti(std::span<const uint32_t> tx_data,
                                                std::span<uint32_t> rx_data) noexcept;
    tle92466ed::CommResult<void> Delay(uint32_t microseconds) noexcept;
    tle92466ed::CommResult<void> Configure(const tle92466ed::SPIConfig& config) noexcept;
    [[nodiscard]] bool IsReady() const noexcept;
    [[nodiscard]] tle92466ed::CommError GetLastError() const noexcept;
    tle92466ed::CommResult<void> ClearErrors() noexcept;
    tle92466ed::CommResult<void> GpioSet(tle92466ed::CtrlPin pin,
                                          tle92466ed::GpioSignal signal) noexcept;
    tle92466ed::CommResult<tle92466ed::GpioSignal> GpioRead(tle92466ed::CtrlPin pin) noexcept;
    void Log(tle92466ed::LogLevel level, const char* tag,
             const char* format, va_list args) noexcept;

    /// @}

private:
    BaseSpi&   spi_;
    BaseGpio&  resn_;
    BaseGpio&  en_;
    BaseGpio*  faultn_;
    bool       initialized_{false};
    tle92466ed::CommError last_error_{tle92466ed::CommError::None};
};

/// @}

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TLE92466ED_Handler Main Handler Class
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Tle92466edHandler
 * @brief Unified handler for TLE92466ED six-channel solenoid driver.
 *
 * Provides thread-safe access to all TLE92466ED driver features:
 * - Channel enable/disable and current control
 * - PWM configuration per channel
 * - Fault monitoring and diagnostics
 * - Watchdog management
 * - Device status and temperature reading
 */
class Tle92466edHandler {
public:
    /// @brief Driver type alias
    using DriverType = tle92466ed::Driver<HalSpiTle92466edComm>;

    static constexpr uint8_t kNumChannels = 6;

    //=========================================================================
    // Construction
    //=========================================================================

    /**
     * @brief Construct TLE92466ED handler.
     * @param spi    Reference to pre-configured BaseSpi.
     * @param resn   BaseGpio for RESN pin (active LOW reset).
     * @param en     BaseGpio for EN pin (active HIGH enable).
     * @param faultn Optional BaseGpio for FAULTN (active LOW fault indicator).
     */
    Tle92466edHandler(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                      BaseGpio* faultn = nullptr) noexcept;

    ~Tle92466edHandler() noexcept;

    // Non-copyable
    Tle92466edHandler(const Tle92466edHandler&) = delete;
    Tle92466edHandler& operator=(const Tle92466edHandler&) = delete;

    // Non-movable
    Tle92466edHandler(Tle92466edHandler&&) = delete;
    Tle92466edHandler& operator=(Tle92466edHandler&&) = delete;

    //=========================================================================
    // Initialization
    //=========================================================================

    /** @brief Initialize driver (hardware reset + SPI init + enter config mode). */
    bool Initialize() noexcept;

    /**
     * @brief Ensure driver is initialized (lazy init entrypoint).
     * @return true if initialized and ready.
     */
    bool EnsureInitialized() noexcept;

    /**
     * @brief Initialize with global configuration.
     * @param config Global device configuration.
     */
    bool Initialize(const tle92466ed::GlobalConfig& config) noexcept;

    /** @brief Deinitialize — disable channels and shut down. */
    bool Deinitialize() noexcept;

    /** @brief Check if initialized. */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    //=========================================================================
    // Channel Control
    //=========================================================================

    /**
     * @brief Configure a channel.
     * @param channel Channel number (0-5).
     * @param config  Channel configuration.
     */
    bool ConfigureChannel(uint8_t channel, const tle92466ed::ChannelConfig& config) noexcept;

    /** @brief Enable a channel. */
    bool EnableChannel(uint8_t channel) noexcept;

    /** @brief Disable a channel. */
    bool DisableChannel(uint8_t channel) noexcept;

    /** @brief Enable all channels. */
    bool EnableAllChannels() noexcept;

    /** @brief Disable all channels. */
    bool DisableAllChannels() noexcept;

    /**
     * @brief Set channel current setpoint.
     * @param channel Channel number (0-5).
     * @param current_ma Current in milliamps.
     */
    bool SetChannelCurrent(uint8_t channel, uint16_t current_ma) noexcept;

    /**
     * @brief Configure raw PWM period for a channel.
     * @param channel Channel number (0-5).
     * @param mantissa PWM period mantissa.
     * @param exponent PWM period exponent.
     * @param low_freq_range Use low frequency range.
     */
    bool ConfigurePwmRaw(uint8_t channel, uint8_t mantissa,
                         uint8_t exponent, bool low_freq_range = false) noexcept;

    //=========================================================================
    // Device Mode
    //=========================================================================

    /** @brief Enter mission mode (enable outputs after configuration). */
    bool EnterMissionMode() noexcept;

    /** @brief Enter config mode (for register writes). */
    bool EnterConfigMode() noexcept;

    /** @brief Check if in mission mode. */
    [[nodiscard]] bool IsMissionMode() noexcept;

    //=========================================================================
    // Status & Diagnostics
    //=========================================================================

    /**
     * @brief Get device status.
     * @param[out] status Device status structure to fill.
     */
    bool GetStatus(tle92466ed::DeviceStatus& status) noexcept;

    /**
     * @brief Get channel diagnostics.
     * @param channel Channel number (0-5).
     * @param[out] diag Diagnostics structure to fill.
     */
    bool GetChannelDiagnostics(uint8_t channel, tle92466ed::ChannelDiagnostics& diag) noexcept;

    /**
     * @brief Get comprehensive fault report.
     * @param[out] report Fault report structure to fill.
     */
    bool GetFaultReport(tle92466ed::FaultReport& report) noexcept;

    /** @brief Clear all fault flags. */
    bool ClearFaults() noexcept;

    /** @brief Check if any fault is present. */
    bool HasFault() noexcept;

    //=========================================================================
    // Watchdog
    //=========================================================================

    /**
     * @brief Reload the SPI watchdog.
     * @param reload_value Watchdog reload value (default 1000).
     */
    bool KickWatchdog(uint16_t reload_value = 1000) noexcept;

    //=========================================================================
    // Device Info
    //=========================================================================

    /** @brief Get chip ID (returns 0 on error). */
    uint32_t GetChipId() noexcept;

    /** @brief Get IC version (returns 0 on error). */
    uint32_t GetIcVersion() noexcept;

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
        if (!EnsureInitializedLocked() || !driver_) return R{};
        return fn(*driver_);
    }

    bool initialized_{false};
    mutable RtosMutex mutex_;
    std::unique_ptr<HalSpiTle92466edComm> comm_;
    std::unique_ptr<DriverType> driver_;
};

/// @}

#endif // COMPONENT_HANDLER_TLE92466ED_HANDLER_H_
