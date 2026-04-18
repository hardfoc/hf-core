/**
 * @file Ads7952Handler.h
 * @brief Unified handler for ADS7952 12-channel 12-bit SAR ADC with SPI integration.
 *
 * This class provides a modern, unified interface for one or more ADS7952 ADC devices
 * following the same architectural patterns as As5047uHandler and other core handlers:
 * - CRTP bridge pattern for BaseSpi integration (zero-overhead)
 * - Lazy initialization with lightweight construction
 * - Complete exception-free design with noexcept methods
 * - Thread-safe operations with RtosMutex protection
 * - Implements BaseAdc interface for seamless AdcManager integration
 * - Comprehensive error handling, diagnostics, and statistics
 * - Factory method supporting multiple devices on the same SPI bus (different CS)
 *
 * Hardware Notes (Flux V1 board):
 *   - ADS7952 is a 12-channel, 12-bit SAR ADC from TI
 *   - 2.5V external Vref, up to 5.0V VA supply
 *   - Connected via SPI3 (dedicated bus): nCS=GPIO10, MOSI=GPIO11, SCK=GPIO12, MISO=GPIO13
 *   - Multiple devices can share the same SPI bus with different CS pins
 *
 * @author HardFOC Team
 * @version 1.0
 * @date 2026
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_ADS7952_HANDLER_H_
#define COMPONENT_HANDLER_ADS7952_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "core/hf-core-drivers/external/hf-ads7952-driver/inc/ads7952.hpp"
#include "base/BaseSpi.h"
#include "base/BaseAdc.h"
#include "RtosMutex.h"

//======================================================//
// ADS7952 SPI BRIDGE ADAPTER (CRTP — zero virtual overhead)
//======================================================//

/**
 * @brief CRTP adapter connecting BaseSpi interface to ADS7952 SpiInterface.
 *
 * Bridges the HardFOC BaseSpi abstraction to the ADS7952 driver's CRTP
 * SPI interface, enabling the driver to work with any SPI controller that
 * inherits from BaseSpi (EspSpi, StmSpi, etc.).
 *
 * Thread Safety: Thread-safe when the underlying BaseSpi is thread-safe.
 */
class Ads7952SpiAdapter : public ads7952::SpiInterface<Ads7952SpiAdapter> {
public:
    /**
     * @brief Construct SPI adapter with BaseSpi interface.
     * @param spi_interface Reference to BaseSpi implementation (e.g., EspSpiDevice)
     */
    explicit Ads7952SpiAdapter(BaseSpi& spi_interface) noexcept;

    /**
     * @brief Perform full-duplex SPI transfer (CRTP dispatch target).
     * @param tx Transmit buffer (can be nullptr to send zeros)
     * @param rx Receive buffer (can be nullptr to discard received data)
     * @param len Number of bytes to transfer
     */
    void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept;

private:
    BaseSpi& spi_interface_;
};

//======================================================//
// ADS7952 HANDLER CONFIGURATION
//======================================================//

/**
 * @brief Configuration for ADS7952 handler initialization.
 */
struct Ads7952HandlerConfig {
    float vref;                       ///< External reference voltage (e.g. 2.5V)
    float va;                         ///< Analog supply voltage (e.g. 5.0V)
    ads7952::Range range;             ///< Input range (Vref or 2*Vref clamped to VA)
    ads7952::Mode initial_mode;       ///< Initial operating mode (Manual/Auto1/Auto2)
    uint16_t auto1_channel_mask;      ///< Channel mask for Auto-1 mode (bit N = CH N)
    uint8_t auto2_last_channel;       ///< Last channel for Auto-2 mode (0–11)
    uint8_t device_index;             ///< Logical index for multi-device setups (0-based)
};

/**
 * @brief Get the default ADS7952 handler config for the Flux V1 board.
 * @return Default Ads7952HandlerConfig (2.5V Vref, 5.0V VA, Vref range, all 12 channels)
 */
inline Ads7952HandlerConfig GetDefaultAds7952Config() noexcept {
    return Ads7952HandlerConfig{
        .vref                = 2.5f,
        .va                  = 5.0f,
        .range               = ads7952::Range::Vref,
        .initial_mode        = ads7952::Mode::Manual,
        .auto1_channel_mask  = ads7952::kAllChannels,
        .auto2_last_channel  = 11,
        .device_index        = 0
    };
}

//======================================================//
// ADS7952 HANDLER DIAGNOSTICS
//======================================================//

/**
 * @brief Diagnostic information for an ADS7952 handler instance.
 */
struct Ads7952Diagnostics {
    bool initialized;                 ///< Handler initialization state
    bool driver_ready;                ///< Driver instance active and ready
    ads7952::Mode current_mode;       ///< Current operating mode
    ads7952::Range current_range;     ///< Current input range setting
    float vref;                       ///< Configured Vref
    float active_vref;                ///< Effective Vref based on range
    uint32_t total_reads;             ///< Total successful read operations
    uint32_t error_count;             ///< Total failed read operations
    uint8_t device_index;             ///< Logical device index
};

//======================================================//
// ADS7952 HANDLER CLASS — implements BaseAdc
//======================================================//

/**
 * @brief Unified handler for ADS7952 12-channel SAR ADC with BaseAdc integration.
 *
 * Provides a comprehensive interface for the ADS7952 ADC with:
 * - BaseAdc implementation for seamless AdcManager integration
 * - CRTP SPI bridge for zero-overhead bus communication
 * - Lazy initialization with lightweight construction
 * - Thread-safe operations with mutex protection
 * - Support for multiple devices on same SPI bus (different CS pins)
 * - Manual, Auto-1, and Auto-2 channel sequencing modes
 * - Voltage range configuration (Vref or 2*Vref clamped to VA)
 * - Per-channel alarm programming
 * - Comprehensive diagnostics and statistics
 *
 * Architecture follows the same patterns as As5047uHandler:
 * - CRTP bridge adapter (Ads7952SpiAdapter)
 * - unique_ptr ownership of adapter and driver
 * - RtosMutex thread safety
 * - Factory function for instance creation
 */
class Ads7952Handler : public BaseAdc {
public:
    //======================================================//
    // CONSTRUCTION AND LIFECYCLE
    //======================================================//

    /**
     * @brief Construct ADS7952 handler with SPI interface.
     * @param spi_interface Reference to BaseSpi implementation (e.g., EspSpiDevice)
     * @param config Handler configuration (Vref, VA, range, mode, etc.)
     *
     * @note Lightweight constructor following lazy initialization pattern.
     *       Driver objects are created during Initialize().
     */
    explicit Ads7952Handler(BaseSpi& spi_interface,
                            const Ads7952HandlerConfig& config = GetDefaultAds7952Config()) noexcept;

    /** @brief Destructor — automatically cleans up driver resources. */
    ~Ads7952Handler() noexcept override = default;

    // Non-copyable, non-movable
    Ads7952Handler(const Ads7952Handler&) = delete;
    Ads7952Handler& operator=(const Ads7952Handler&) = delete;
    Ads7952Handler(Ads7952Handler&&) = delete;
    Ads7952Handler& operator=(Ads7952Handler&&) = delete;

    //======================================================//
    // BaseAdc INTERFACE IMPLEMENTATION
    //======================================================//

    /**
     * @brief Initialize the ADS7952 ADC (lazy initialization).
     * @return true if successful
     *
     * Creates the CRTP SPI adapter and ADS7952 driver instance, applies
     * configuration (range, mode, channel mask), and verifies communication.
     */
    bool Initialize() noexcept override;

    /**
     * @brief Deinitialize the ADC and release driver resources.
     * @return true if successful
     */
    bool Deinitialize() noexcept override;

    /**
     * @brief Get maximum number of ADC channels (always 12 for ADS7952).
     * @return 12
     */
    hf_u8_t GetMaxChannels() const noexcept override;

    /**
     * @brief Check if a channel is available (0–11).
     * @param channel Channel ID (0-based)
     * @return true if channel is in valid range
     */
    bool IsChannelAvailable(hf_channel_id_t channel) const noexcept override;

    /**
     * @brief Read a channel and return voltage.
     * @param channel Channel ID (0–11)
     * @param voltage Output: converted voltage
     * @param samples Number of samples to average (default 1)
     * @param timeout_ms Timeout in ms (0 = default)
     * @return ADC error code
     */
    hf_adc_err_t ReadChannelV(hf_channel_id_t channel, float& voltage,
                              hf_u8_t samples = 1, hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief Read a channel and return raw count.
     * @param channel Channel ID (0–11)
     * @param count Output: raw 12-bit ADC count (0–4095)
     * @param samples Number of samples to average (default 1)
     * @param timeout_ms Timeout in ms (0 = default)
     * @return ADC error code
     */
    hf_adc_err_t ReadChannelCount(hf_channel_id_t channel, hf_u32_t& count,
                                  hf_u8_t samples = 1, hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief Read a channel and return both raw count and voltage.
     * @param channel Channel ID (0–11)
     * @param count Output: raw 12-bit count
     * @param voltage Output: converted voltage
     * @param samples Number of samples to average (default 1)
     * @param timeout_ms Timeout in ms (0 = default)
     * @return ADC error code
     */
    hf_adc_err_t ReadChannel(hf_channel_id_t channel, hf_u32_t& count, float& voltage,
                             hf_u8_t samples = 1, hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief Read multiple channels in a single batch using Auto-1 mode.
     * @param channels Array of channel IDs to read
     * @param num_channels Number of channels to read
     * @param counts Output array: raw 12-bit counts (may be nullptr)
     * @param voltages Output array: converted voltages (may be nullptr)
     * @return ADC error code
     */
    hf_adc_err_t ReadMultipleChannels(const hf_channel_id_t* channels, hf_u8_t num_channels,
                                      hf_u32_t* counts, float* voltages) noexcept override;

    //======================================================//
    // ADS7952-SPECIFIC METHODS
    //======================================================//

    /**
     * @brief Read all 12 channels using Auto-1 mode.
     * @param readings Output: populated ChannelReadings structure
     * @return true if successful
     */
    bool ReadAllChannels(ads7952::ChannelReadings& readings) noexcept;

    /**
     * @brief Program alarm threshold for a channel (in 12-bit counts).
     * @param channel Channel number (0–11)
     * @param bound High or Low alarm
     * @param threshold_12bit 12-bit threshold value (0–4095)
     * @return true if programming succeeded
     */
    bool ProgramAlarm(uint8_t channel, ads7952::AlarmBound bound, uint16_t threshold_12bit) noexcept;

    /**
     * @brief Program alarm threshold for a channel (in voltage).
     * @param channel Channel number (0–11)
     * @param bound High or Low alarm
     * @param voltage Threshold voltage
     * @return true if programming succeeded
     */
    bool ProgramAlarmVoltage(uint8_t channel, ads7952::AlarmBound bound, float voltage) noexcept;

    /**
     * @brief Set the ADC input range.
     * @param range Vref or 2*Vref (clamped to VA)
     * @return true if successful
     */
    bool SetRange(ads7952::Range range) noexcept;

    /**
     * @brief Get the current effective reference voltage.
     * @return Active Vref (accounting for range setting)
     */
    float GetActiveVref() const noexcept;

    //======================================================//
    // DRIVER ACCESS
    //======================================================//

    /**
     * @brief Get raw pointer to the ADS7952 driver for advanced operations.
     * @return Pointer to driver or nullptr if not initialized
     * @warning Raw pointer — NOT mutex-protected. Prefer visitDriver().
     */
    ads7952::ADS7952<Ads7952SpiAdapter>* GetDriver() noexcept;
    const ads7952::ADS7952<Ads7952SpiAdapter>* GetDriver() const noexcept;

    /**
     * @brief Visit the underlying ADS7952 driver under handler mutex protection.
     * @tparam Fn Callable accepting ADS7952<Ads7952SpiAdapter>&
     * @return Callable result or default-constructed value when driver is unavailable
     */
    template <typename Fn>
    auto visitDriver(Fn&& fn) noexcept -> decltype(fn(std::declval<ads7952::ADS7952<Ads7952SpiAdapter>&>())) {
        using ReturnType = decltype(fn(std::declval<ads7952::ADS7952<Ads7952SpiAdapter>&>()));
        MutexLockGuard lock(handler_mutex_);
        if (!EnsureInitializedLocked() || !adc_driver_) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        return fn(*adc_driver_);
    }

    //======================================================//
    // DIAGNOSTICS
    //======================================================//

    /** @brief Get handler description string. */
    const char* GetDescription() const noexcept;

    /** @brief Get current diagnostics snapshot. */
    bool GetHandlerDiagnostics(Ads7952Diagnostics& diag) const noexcept;

    /** @brief Dump comprehensive diagnostics to log. */
    void DumpDiagnostics() const noexcept;

    /** @brief Get the logical device index. */
    uint8_t GetDeviceIndex() const noexcept { return config_.device_index; }

private:
    //======================================================//
    // PRIVATE MEMBERS
    //======================================================//

    BaseSpi& spi_ref_;                                ///< Reference to SPI interface
    std::unique_ptr<Ads7952SpiAdapter> spi_adapter_;  ///< CRTP SPI adapter
    std::unique_ptr<ads7952::ADS7952<Ads7952SpiAdapter>> adc_driver_;  ///< ADS7952 driver
    Ads7952HandlerConfig config_;                     ///< Handler configuration
    mutable RtosMutex handler_mutex_;                 ///< Thread safety mutex
    char description_[64];                            ///< Description string

    // Statistics
    mutable uint32_t total_reads_{0};                 ///< Successful read count
    mutable uint32_t error_count_{0};                 ///< Failed read count

    //======================================================//
    // PRIVATE HELPERS
    //======================================================//

    /** @brief Ensure initialized while mutex is already held. */
    bool EnsureInitializedLocked() noexcept;

    /** @brief Apply configuration to the driver after init. */
    bool ApplyConfiguration() noexcept;

    /** @brief Perform a single-channel read under lock (no mutex acquire). */
    ads7952::ReadResult ReadChannelLocked(uint8_t channel) noexcept;
};

//======================================================//
// FACTORY METHODS
//======================================================//

/**
 * @brief Create ADS7952 handler instance.
 * @param spi_interface Reference to SPI interface
 * @param config Handler configuration
 * @return Unique pointer to Ads7952Handler
 */
std::unique_ptr<Ads7952Handler> CreateAds7952Handler(
    BaseSpi& spi_interface,
    const Ads7952HandlerConfig& config = GetDefaultAds7952Config()) noexcept;

#endif // COMPONENT_HANDLER_ADS7952_HANDLER_H_
