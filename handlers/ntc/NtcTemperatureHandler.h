/**
 * @file NtcTemperatureHandler.h
 * @brief NTC temperature sensor handler for the HardFOC system.
 *
 * This handler provides a BaseTemperature interface for NTC thermistor temperature
 * sensors using a BaseAdc pointer for voltage measurements. It wraps the NtcThermistor
 * library to provide a unified temperature sensor interface.
 *
 * @author HardFOC Development Team
 * @date 2025
 * @copyright HardFOC
 */

#pragma once

#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseTemperature.h"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseAdc.h"
#include "core/hf-core-drivers/external/hf-ntc-thermistor-driver/inc/ntc_thermistor.hpp"
#include "utils/RtosMutex.h"
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/PeriodicTimer.h"

#include <memory>
#include <cfloat>

//--------------------------------------
//  NTC ADC Adapter (BaseAdc → ntc::AdcInterface bridge)
//--------------------------------------

/**
 * @brief Adapter that bridges BaseAdc to the ntc::AdcInterface CRTP interface
 *        required by the NtcThermistor template class.
 *
 * NtcThermistor<AdcType> requires AdcType to inherit from ntc::AdcInterface<AdcType>.
 * BaseAdc uses a separate virtual interface. This adapter translates between them.
 */
class NtcAdcAdapter : public ntc::AdcInterface<NtcAdcAdapter> {
public:
    /**
     * @brief Construct adapter wrapping a BaseAdc pointer.
     * @param adc Pointer to BaseAdc (must outlive this adapter).
     * @param ref_voltage ADC reference voltage in volts.
     * @param res_bits ADC resolution in bits.
     */
    explicit NtcAdcAdapter(BaseAdc* adc, float ref_voltage = 3.3f, uint8_t res_bits = 12) noexcept
        : adc_(adc), reference_voltage_(ref_voltage), resolution_bits_(res_bits) {}

    [[nodiscard]] bool IsInitialized() const { return adc_ != nullptr && adc_->IsInitialized(); }
    bool EnsureInitialized() { return adc_ != nullptr && adc_->EnsureInitialized(); }
    [[nodiscard]] bool IsChannelAvailable(uint8_t channel) const {
        return adc_ != nullptr && adc_->IsChannelAvailable(static_cast<hf_channel_id_t>(channel));
    }
    ntc::AdcError ReadChannelCount(uint8_t channel, uint32_t* count) {
        if (adc_ == nullptr || count == nullptr) return ntc::AdcError::ReadFailed;
        hf_u32_t raw = 0;
        auto err = adc_->ReadChannelCount(static_cast<hf_channel_id_t>(channel), raw);
        if (err != hf_adc_err_t::ADC_SUCCESS) return ntc::AdcError::ReadFailed;
        *count = raw;
        return ntc::AdcError::Success;
    }
    ntc::AdcError ReadChannelV(uint8_t channel, float* voltage_v) {
        if (adc_ == nullptr || voltage_v == nullptr) return ntc::AdcError::ReadFailed;
        float v = 0.0f;
        auto err = adc_->ReadChannelV(static_cast<hf_channel_id_t>(channel), v);
        if (err != hf_adc_err_t::ADC_SUCCESS) return ntc::AdcError::ReadFailed;
        *voltage_v = v;
        return ntc::AdcError::Success;
    }
    [[nodiscard]] float GetReferenceVoltage() const { return reference_voltage_; }
    [[nodiscard]] uint8_t GetResolutionBits() const { return resolution_bits_; }

private:
    BaseAdc* adc_;
    float reference_voltage_;
    uint8_t resolution_bits_;
};

/// Convenience alias for the concrete NtcThermistor type used by this handler.
using NtcThermistorConcrete = NtcThermistor<NtcAdcAdapter>;

//--------------------------------------
//  NTC Temperature Handler Configuration
//--------------------------------------

/**
 * @brief NTC temperature handler configuration structure
 */
typedef struct {
    NtcType ntc_type;                       ///< NTC thermistor type (driver enum)
    uint8_t adc_channel;                    ///< ADC channel for voltage measurement
    float voltage_divider_series_resistance; ///< Series resistance in voltage divider (ohms)
    float voltage_divider_parallel_resistance; ///< Parallel resistance (ohms, 0 if none)
    float reference_voltage;                ///< Reference voltage (V)
    float calibration_offset;               ///< Calibration offset (°C)
    float beta_value;                       ///< Beta value override (0 = use default)
    NtcConversionMethod conversion_method;  ///< Temperature conversion method (driver enum)
    uint32_t sample_count;                  ///< Number of samples to average
    uint32_t sample_delay_ms;               ///< Delay between samples (ms)
    float min_temperature;                  ///< Minimum temperature (°C)
    float max_temperature;                  ///< Maximum temperature (°C)
    bool enable_filtering;                  ///< Enable temperature filtering
    float filter_alpha;                     ///< Filter alpha value (0.0-1.0)
    bool enable_threshold_monitoring;       ///< Enable threshold monitoring at init
    float low_threshold_celsius;            ///< Low temperature threshold (°C)
    float high_threshold_celsius;           ///< High temperature threshold (°C)
    hf_temp_threshold_callback_t threshold_callback; ///< Threshold callback (optional)
    void* threshold_user_data;              ///< Threshold callback user data
    const char* sensor_name;                ///< Sensor name/identifier
    const char* sensor_description;         ///< Sensor description
} ntc_temp_handler_config_t;

/**
 * @brief Default NTC temperature handler configuration
 */
#define NTC_TEMP_HANDLER_CONFIG_DEFAULT() { \
    .ntc_type = NtcType::NtcG163Jft103Ft1S, \
    .adc_channel = 0, \
    .voltage_divider_series_resistance = 10000.0f, \
    .voltage_divider_parallel_resistance = 0.0f, \
    .reference_voltage = 3.3f, \
    .calibration_offset = 0.0f, \
    .beta_value = 0.0f, \
    .conversion_method = NtcConversionMethod::Auto, \
    .sample_count = 1, \
    .sample_delay_ms = 0, \
    .min_temperature = -40.0f, \
    .max_temperature = 125.0f, \
    .enable_filtering = false, \
    .filter_alpha = 0.1f, \
    .enable_threshold_monitoring = false, \
    .low_threshold_celsius = -40.0f, \
    .high_threshold_celsius = 125.0f, \
    .threshold_callback = nullptr, \
    .threshold_user_data = nullptr, \
    .sensor_name = "NTC_Temperature_Sensor", \
    .sensor_description = "NTC Thermistor Temperature Sensor" \
}

//--------------------------------------
//  NtcTemperatureHandler Class
//--------------------------------------

/**
 * @class NtcTemperatureHandler
 * @brief NTC temperature sensor handler implementing BaseTemperature interface
 * 
 * This class provides a complete implementation of the BaseTemperature interface
 * for NTC thermistor temperature sensors. It uses a BaseAdc pointer for voltage
 * measurements and wraps the NtcThermistor library to provide comprehensive
 * temperature sensing capabilities.
 * 
 * Key features:
 * - BaseTemperature interface compliance
 * - Hardware-agnostic design using BaseAdc
 * - Support for multiple NTC types
 * - Dual conversion methods (lookup table and mathematical)
 * - Built-in calibration and filtering
 * - Comprehensive error handling
 * - Thread-safe operations
 * - Statistics and diagnostics
 * - Threshold monitoring support
 * - Continuous monitoring support
 * 
 * @note This class is thread-safe and can be used in multi-threaded applications
 * @note The handler requires a BaseAdc interface for voltage measurements
 * @note Supports all BaseTemperature features including threshold and continuous monitoring
 */
class NtcTemperatureHandler : public BaseTemperature {
public:
    //==============================================================//
    // CONSTRUCTORS AND DESTRUCTOR
    //==============================================================//
    
    /**
     * @brief Constructor with NTC type and ADC interface
     * @param ntc_type NTC thermistor type
     * @param adc_interface Pointer to BaseAdc interface
     * @param sensor_name Optional sensor name
     */
    NtcTemperatureHandler(NtcType ntc_type, BaseAdc* adc_interface, const char* sensor_name = nullptr) noexcept;
    
    /**
     * @brief Constructor with ADC interface and configuration
     * @param adc_interface Pointer to BaseAdc interface
     * @param config NTC temperature handler configuration
     */
    NtcTemperatureHandler(BaseAdc* adc_interface, const ntc_temp_handler_config_t& config) noexcept;
    
    /**
     * @brief Copy constructor is deleted
     */
    NtcTemperatureHandler(const NtcTemperatureHandler&) = delete;
    
    /**
     * @brief Assignment operator is deleted
     */
    NtcTemperatureHandler& operator=(const NtcTemperatureHandler&) = delete;
    
    /**
     * @brief Move constructor
     */
    NtcTemperatureHandler(NtcTemperatureHandler&&) noexcept = default;
    
    /**
     * @brief Move assignment operator
     */
    NtcTemperatureHandler& operator=(NtcTemperatureHandler&&) noexcept = default;
    
    /**
     * @brief Virtual destructor - cleans up timer and thermistor resources
     */
    virtual ~NtcTemperatureHandler() noexcept;
    
    //==============================================================//
    // BASE TEMPERATURE INTERFACE IMPLEMENTATION
    //==============================================================//
    
    // Pure virtual implementations
    bool Initialize() noexcept override;
    bool Deinitialize() noexcept override;
    hf_temp_err_t ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept override;
    
    // Information interface
    hf_temp_err_t GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept override;
    hf_u32_t GetCapabilities() const noexcept override;
    
    // Advanced features (supported)
    hf_temp_err_t SetRange(float min_celsius, float max_celsius) noexcept override;
    hf_temp_err_t GetRange(float* min_celsius, float* max_celsius) const noexcept override;
    hf_temp_err_t GetResolution(float* resolution_celsius) const noexcept override;
    hf_temp_err_t SetThresholds(float low_threshold_celsius, float high_threshold_celsius) noexcept override;
    hf_temp_err_t GetThresholds(float* low_threshold_celsius, float* high_threshold_celsius) const noexcept override;
    hf_temp_err_t EnableThresholdMonitoring(hf_temp_threshold_callback_t callback, void* user_data) noexcept override;
    hf_temp_err_t DisableThresholdMonitoring() noexcept override;
    hf_temp_err_t StartContinuousMonitoring(hf_u32_t sample_rate_hz, hf_temp_reading_callback_t callback, void* user_data) noexcept override;
    hf_temp_err_t StopContinuousMonitoring() noexcept override;
    bool IsMonitoringActive() const noexcept override;
    hf_temp_err_t SetCalibrationOffset(float offset_celsius) noexcept override;
    hf_temp_err_t GetCalibrationOffset(float* offset_celsius) const noexcept override;
    hf_temp_err_t ResetCalibration() noexcept override;
    hf_temp_err_t EnterSleepMode() noexcept override;
    hf_temp_err_t ExitSleepMode() noexcept override;
    bool IsSleeping() const noexcept override;
    hf_temp_err_t SelfTest() noexcept override;
    hf_temp_err_t CheckHealth() noexcept override;
    hf_temp_err_t GetStatistics(hf_temp_statistics_t& statistics) noexcept override;
    hf_temp_err_t GetDiagnostics(hf_temp_diagnostics_t& diagnostics) noexcept override;
    hf_temp_err_t ResetStatistics() noexcept override;
    hf_temp_err_t ResetDiagnostics() noexcept override;
    
    //==============================================================//
    // NTC-SPECIFIC METHODS
    //==============================================================//
    
    /**
     * @brief Get NTC thermistor instance
     * @return Pointer to NtcThermistor instance (opaque)
     * @note Returns raw pointer for observation only; handler retains ownership.
     */
    void* GetNtcThermistor() noexcept;
    
    /**
     * @brief Get NTC thermistor instance (const)
     * @return Pointer to const NtcThermistor instance
     * @note Returns raw pointer for observation only; handler retains ownership.
     */
    const void* GetNtcThermistor() const noexcept;
    
    /**
     * @brief Get NTC reading
     * @param reading Pointer to store NTC reading
     * @return NTC error code
     */
    NtcError GetNtcReading(ntc_reading_t* reading) noexcept;
    
    /**
     * @brief Get NTC configuration
     * @param config Pointer to store configuration
     * @return Error code
     */
    NtcError GetNtcConfiguration(ntc_config_t* config) const noexcept;
    
    /**
     * @brief Set NTC configuration
     * @param config New configuration
     * @return Error code
     */
    NtcError SetNtcConfiguration(const ntc_config_t& config) noexcept;
    
    /**
     * @brief Get thermistor resistance
     * @param resistance_ohms Pointer to store resistance
     * @return Error code
     */
    NtcError GetResistance(float* resistance_ohms) noexcept;
    
    /**
     * @brief Get voltage across thermistor
     * @param voltage_volts Pointer to store voltage
     * @return Error code
     */
    NtcError GetVoltage(float* voltage_volts) noexcept;
    
    /**
     * @brief Get raw ADC value
     * @param adc_value Pointer to store ADC value
     * @return Error code
     */
    NtcError GetRawAdcValue(uint32_t* adc_value) noexcept;
    
    /**
     * @brief Calibrate the thermistor
     * @param reference_temperature_celsius Known reference temperature
     * @return Error code
     */
    hf_temp_err_t Calibrate(float reference_temperature_celsius) noexcept;
    
    /**
     * @brief Set conversion method
     * @param method Conversion method
     * @return Error code
     */
    NtcError SetConversionMethod(NtcConversionMethod method) noexcept;
    
    /**
     * @brief Set voltage divider parameters
     * @param series_resistance Series resistance (ohms)
     * @return Error code
     */
    NtcError SetVoltageDivider(float series_resistance) noexcept;
    
    /**
     * @brief Set reference voltage
     * @param reference_voltage Reference voltage (V)
     * @return Error code
     */
    NtcError SetReferenceVoltage(float reference_voltage) noexcept;
    
    /**
     * @brief Set beta value
     * @param beta_value Beta value (K)
     * @return Error code
     */
    NtcError SetBetaValue(float beta_value) noexcept;
    
    /**
     * @brief Set ADC channel
     * @param adc_channel ADC channel number
     * @return Error code
     */
    NtcError SetAdcChannel(uint8_t adc_channel) noexcept;
    
    /**
     * @brief Set sampling parameters
     * @param sample_count Number of samples
     * @param sample_delay_ms Delay between samples (ms)
     * @return Error code
     */
    NtcError SetSamplingParameters(uint32_t sample_count, uint32_t sample_delay_ms) noexcept;
    
    /**
     * @brief Enable/disable filtering
     * @param enable Enable filtering
     * @param alpha Filter alpha value (0.0-1.0)
     * @return Error code
     */
    NtcError SetFiltering(bool enable, float alpha = 0.1f) noexcept;
    
    /**
     * @brief Get sensor name
     * @return Sensor name string
     */
    const char* GetSensorName() const noexcept;
    
    /**
     * @brief Get sensor description
     * @return Sensor description string
     */
    const char* GetSensorDescription() const noexcept;

private:
    //==============================================================//
    // PRIVATE MEMBER VARIABLES
    //==============================================================//
    
    mutable RtosMutex mutex_;              ///< Thread safety mutex (hardware-agnostic)
    std::unique_ptr<NtcAdcAdapter> ntc_adc_adapter_;       ///< ADC adapter (BaseAdc→ntc::AdcInterface)
    std::unique_ptr<NtcThermistorConcrete> ntc_thermistor_; ///< NTC thermistor driver instance
    BaseAdc* adc_interface_;                ///< ADC interface pointer
    ntc_temp_handler_config_t config_;      ///< Handler configuration
    
    // BaseTemperature state
    bool initialized_;                      ///< Initialization status
    hf_temp_state_t current_state_;         ///< Current state
    hf_temp_config_t base_config_;          ///< Base configuration
    
    // Threshold monitoring
    float low_threshold_celsius_;           ///< Low temperature threshold
    float high_threshold_celsius_;          ///< High temperature threshold
    bool threshold_monitoring_enabled_;     ///< Threshold monitoring status
    hf_temp_threshold_callback_t threshold_callback_; ///< Threshold callback
    void* threshold_user_data_;             ///< Threshold callback user data
    
    // Continuous monitoring (using hardware-agnostic PeriodicTimer)
    bool monitoring_active_;               ///< Continuous monitoring status
    hf_u32_t sample_rate_hz_;               ///< Sample rate for continuous monitoring
    hf_temp_reading_callback_t continuous_callback_; ///< Continuous monitoring callback
    void* continuous_user_data_;            ///< Continuous monitoring callback user data
    PeriodicTimer monitoring_timer_;        ///< Hardware-agnostic periodic timer
    float calibration_offset_;             ///< Current calibration offset
    
    // Statistics and diagnostics
    hf_temp_statistics_t statistics_;       ///< BaseTemperature statistics
    hf_temp_diagnostics_t diagnostics_;     ///< BaseTemperature diagnostics
    
    //==============================================================//
    // PRIVATE HELPER METHODS
    //==============================================================//
    
    /**
     * @brief Set last error in diagnostics
     * @param error Error code to record
     */
    void SetLastError(hf_temp_err_t error) noexcept;
    
    /**
     * @brief Update operation statistics
     * @param operation_successful Whether operation was successful
     * @param operation_time_us Operation time in microseconds
     */
    void UpdateStatistics(bool operation_successful, hf_u32_t operation_time_us) noexcept;
    
    /**
     * @brief Update diagnostics with error code
     * @param error Error code
     */
    void UpdateDiagnostics(hf_temp_err_t error) noexcept;
    
    /**
     * @brief Initialize NTC thermistor with current configuration
     * @return true if successful, false otherwise
     */
    bool InitializeNtcThermistor() noexcept;
    
    /**
     * @brief Convert NTC error to BaseTemperature error
     * @param ntc_error NTC error code
     * @return BaseTemperature error code
     */
    hf_temp_err_t ConvertNtcError(NtcError ntc_error) const noexcept;
    
    /**
     * @brief Check thresholds and trigger callback if needed
     * @param temperature_celsius Current temperature reading
     */
    void CheckThresholds(float temperature_celsius) noexcept;
    
    /**
     * @brief Static callback for continuous monitoring timer
     * @param arg Timer callback argument (pointer to handler instance)
     */
    static void ContinuousMonitoringCallback(uint32_t arg);
    
    /**
     * @brief Update BaseTemperature diagnostics
     * @param error Error code
     */
    void UpdateBaseDiagnostics(hf_temp_err_t error) noexcept;
    
    /**
     * @brief Validate configuration
     * @param config Configuration to validate
     * @return Error code
     */
    hf_temp_err_t ValidateConfiguration(const ntc_temp_handler_config_t& config) const noexcept;
    
    /**
     * @brief Get current timestamp in microseconds
     * @return Current timestamp
     */
    static hf_u64_t GetCurrentTimeUs() noexcept;
}; 