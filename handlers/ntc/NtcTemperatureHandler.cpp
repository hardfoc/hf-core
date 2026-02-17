/**
 * @file NtcTemperatureHandler.cpp
 * @brief Implementation of NTC temperature sensor handler for HardFOC system.
 *
 * This file implements the NtcTemperatureHandler class, which provides a BaseTemperature
 * interface for NTC thermistors. It wraps the NtcThermistor library and uses a BaseAdc
 * pointer for underlying ADC operations.
 *
 * @author Nebiyu Tadesse
 * @date 2025
 * @copyright HardFOC
 */

#include "NtcTemperatureHandler.h"
#include "handlers/logger/Logger.h"
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

static const char* TAG = "NtcTempHandler";

RtosMutex NtcTemperatureHandler::callback_registry_mutex_{};
std::array<NtcTemperatureHandler*, NtcTemperatureHandler::kMonitoringContextSlots>
    NtcTemperatureHandler::callback_registry_ = {};

//--------------------------------------
//  NtcTemperatureHandler Implementation
//--------------------------------------

NtcTemperatureHandler::NtcTemperatureHandler(BaseAdc* adc_interface, 
                                             const ntc_temp_handler_config_t& config) noexcept
    : BaseTemperature()
    , mutex_()
    , ntc_adc_adapter_(nullptr)
    , ntc_thermistor_(nullptr)
    , adc_interface_(adc_interface)
    , config_(config)
    , current_state_{}
    , base_config_{}
    , low_threshold_celsius_(0.0f)
    , high_threshold_celsius_(0.0f)
    , threshold_callback_(nullptr)
    , threshold_user_data_(nullptr)
    , sample_rate_hz_(0)
    , continuous_callback_(nullptr)
    , continuous_user_data_(nullptr)
    , monitoring_timer_()
    , monitoring_context_id_(0)
    , calibration_offset_(0.0f)
    , statistics_({})
    , diagnostics_({})
    , initialized_(false)
    , threshold_monitoring_enabled_(false)
    , monitoring_active_(false) {
    
    // Initialize statistics
    statistics_.total_operations = 0;
    statistics_.successful_operations = 0;
    statistics_.failed_operations = 0;
    statistics_.temperature_readings = 0;
    statistics_.calibration_count = 0;
    statistics_.threshold_violations = 0;
    statistics_.average_operation_time_us = 0;
    statistics_.max_operation_time_us = 0;
    statistics_.min_operation_time_us = UINT32_MAX;
    statistics_.min_temperature_celsius = FLT_MAX;
    statistics_.max_temperature_celsius = -FLT_MAX;
    statistics_.avg_temperature_celsius = 0.0f;
    
    // Initialize diagnostics
    diagnostics_.sensor_healthy = true;
    diagnostics_.last_error_code = TEMP_SUCCESS;
    diagnostics_.last_error_timestamp = 0;
    diagnostics_.consecutive_errors = 0;
    diagnostics_.sensor_available = (adc_interface_ != nullptr);
    diagnostics_.threshold_monitoring_supported = true;
    diagnostics_.threshold_monitoring_enabled = false;
    diagnostics_.continuous_monitoring_active = false;
    diagnostics_.current_temperature_raw = 0;
    diagnostics_.calibration_valid = false;
}

NtcTemperatureHandler::NtcTemperatureHandler(NtcType ntc_type, BaseAdc* adc_interface,
                                             const char* sensor_name) noexcept
    : BaseTemperature()
    , mutex_()
    , ntc_adc_adapter_(nullptr)
    , ntc_thermistor_(nullptr)
    , adc_interface_(adc_interface)
    , config_({})
    , current_state_{}
    , base_config_{}
    , low_threshold_celsius_(0.0f)
    , high_threshold_celsius_(0.0f)
    , threshold_callback_(nullptr)
    , threshold_user_data_(nullptr)
    , sample_rate_hz_(0)
    , continuous_callback_(nullptr)
    , continuous_user_data_(nullptr)
    , monitoring_timer_()
    , monitoring_context_id_(0)
    , calibration_offset_(0.0f)
    , statistics_({})
    , diagnostics_({})
    , initialized_(false)
    , threshold_monitoring_enabled_(false)
    , monitoring_active_(false) {
    
    // Apply default configuration and override NTC type / sensor name
    ntc_temp_handler_config_t default_config = NTC_TEMP_HANDLER_CONFIG_DEFAULT();
    config_ = default_config;
    config_.ntc_type = ntc_type;
    if (sensor_name != nullptr) {
        config_.sensor_name = sensor_name;
    }
    
    // Initialize statistics
    statistics_.total_operations = 0;
    statistics_.successful_operations = 0;
    statistics_.failed_operations = 0;
    statistics_.temperature_readings = 0;
    statistics_.calibration_count = 0;
    statistics_.threshold_violations = 0;
    statistics_.average_operation_time_us = 0;
    statistics_.max_operation_time_us = 0;
    statistics_.min_operation_time_us = UINT32_MAX;
    statistics_.min_temperature_celsius = FLT_MAX;
    statistics_.max_temperature_celsius = -FLT_MAX;
    statistics_.avg_temperature_celsius = 0.0f;
    
    // Initialize diagnostics
    diagnostics_.sensor_healthy = true;
    diagnostics_.last_error_code = TEMP_SUCCESS;
    diagnostics_.last_error_timestamp = 0;
    diagnostics_.consecutive_errors = 0;
    diagnostics_.sensor_available = (adc_interface_ != nullptr);
    diagnostics_.threshold_monitoring_supported = true;
    diagnostics_.threshold_monitoring_enabled = false;
    diagnostics_.continuous_monitoring_active = false;
    diagnostics_.current_temperature_raw = 0;
    diagnostics_.calibration_valid = false;
}

NtcTemperatureHandler::~NtcTemperatureHandler() noexcept {
    // Stop monitoring if active
    if (monitoring_active_) {
        StopContinuousMonitoring();
    }
    
    // Clean up timer (PeriodicTimer handles its own cleanup in destructor)
    monitoring_timer_.Destroy();
    UnregisterMonitoringContext(monitoring_context_id_);
    monitoring_context_id_ = 0;
    
    // Clean up thermistor
    if (ntc_thermistor_) {
        ntc_thermistor_->Deinitialize();
        ntc_thermistor_.reset();
    }
}

bool NtcTemperatureHandler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return true;
    }
    
    if (adc_interface_ == nullptr) {
        Logger::GetInstance().Error(TAG, "ADC interface is null");
        SetLastError(TEMP_ERR_NULL_POINTER);
        return false;
    }
    
    // Ensure ADC interface is initialized
    if (!adc_interface_->EnsureInitialized()) {
        Logger::GetInstance().Error(TAG, "Failed to initialize ADC interface");
        SetLastError(TEMP_ERR_RESOURCE_UNAVAILABLE);
        return false;
    }
    
    // Create ADC adapter bridging BaseAdc to ntc::AdcInterface
    ntc_adc_adapter_ = std::make_unique<NtcAdcAdapter>(
        adc_interface_, config_.reference_voltage);
    if (!ntc_adc_adapter_) {
        Logger::GetInstance().Error(TAG, "Failed to create NTC ADC adapter");
        SetLastError(TEMP_ERR_OUT_OF_MEMORY);
        return false;
    }
    
    // Create NTC thermistor instance
    ntc_thermistor_ = std::make_unique<NtcThermistorConcrete>(
        config_.ntc_type, ntc_adc_adapter_.get());
    if (!ntc_thermistor_) {
        Logger::GetInstance().Error(TAG, "Failed to create NTC thermistor");
        SetLastError(TEMP_ERR_OUT_OF_MEMORY);
        return false;
    }
    
    // Initialize NTC thermistor
    if (!ntc_thermistor_->Initialize()) {
        Logger::GetInstance().Error(TAG, "Failed to initialize NTC thermistor");
        SetLastError(TEMP_ERR_FAILURE);
        return false;
    }
    
    // Apply configuration
    if (config_.conversion_method != NtcConversionMethod::Auto) {
        ntc_thermistor_->SetConversionMethod(config_.conversion_method);
    }
    
    if (config_.voltage_divider_series_resistance > 0) {
        ntc_thermistor_->SetVoltageDivider(config_.voltage_divider_series_resistance);
    }
    
    if (config_.reference_voltage > 0) {
        ntc_thermistor_->SetReferenceVoltage(config_.reference_voltage);
    }
    
    if (config_.beta_value > 0) {
        ntc_thermistor_->SetBetaValue(config_.beta_value);
    }
    
    // Apply calibration offset if set
    if (calibration_offset_ != 0.0f) {
        ntc_thermistor_->SetCalibrationOffset(calibration_offset_);
        diagnostics_.calibration_valid = true;
    }
    
    // Set thresholds if enabled
    if (config_.enable_threshold_monitoring) {
        SetThresholds(config_.low_threshold_celsius, config_.high_threshold_celsius);
        EnableThresholdMonitoring(config_.threshold_callback, config_.threshold_user_data);
    }
    
    initialized_ = true;
    current_state_ = HF_TEMP_STATE_INITIALIZED;
    
    Logger::GetInstance().Info(TAG, "NTC temperature handler initialized successfully");
    return true;
}

bool NtcTemperatureHandler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!initialized_) {
        return true;
    }
    
    // Stop continuous monitoring
    if (monitoring_active_) {
        StopContinuousMonitoring();
    }
    
    // Clean up timer
    monitoring_timer_.Destroy();
    UnregisterMonitoringContext(monitoring_context_id_);
    monitoring_context_id_ = 0;
    
    // Deinitialize thermistor (must happen before adapter reset)
    if (ntc_thermistor_) {
        ntc_thermistor_->Deinitialize();
        ntc_thermistor_.reset();
    }
    ntc_adc_adapter_.reset();
    
    // Reset callbacks
    threshold_callback_ = nullptr;
    threshold_user_data_ = nullptr;
    continuous_callback_ = nullptr;
    continuous_user_data_ = nullptr;
    
    initialized_ = false;
    current_state_ = HF_TEMP_STATE_UNINITIALIZED;
    
    Logger::GetInstance().Info(TAG, "NTC temperature handler deinitialized");
    return true;
}

hf_temp_err_t NtcTemperatureHandler::ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept {
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    const auto start_time = os_time_get();
    
    NtcError result = ntc_thermistor_->ReadTemperatureCelsius(temperature_celsius);
    
    const auto end_time = os_time_get();
    const auto operation_time = static_cast<hf_u32_t>(end_time - start_time);
    
    if (result == NtcError::Success) {
        UpdateStatistics(true, operation_time);
        UpdateDiagnostics(TEMP_SUCCESS);
        
        // Update diagnostics with raw reading
        uint32_t raw_value = 0;
        if (ntc_thermistor_->GetRawAdcValue(&raw_value) == NtcError::Success) {
            diagnostics_.current_temperature_raw = static_cast<hf_u32_t>(raw_value);
        }
        
        // Check thresholds
        CheckThresholds(*temperature_celsius);
        
    } else {
        UpdateStatistics(false, operation_time);
        hf_temp_err_t temp_error = ConvertNtcError(result);
        UpdateDiagnostics(temp_error);
        return temp_error;
    }
    
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept {
    if (info == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (!self->EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    // Fill in NTC-specific information
    info->sensor_type = HF_TEMP_SENSOR_TYPE_THERMISTOR;
    info->min_temp_celsius = -40.0f;  // Typical NTC range
    info->max_temp_celsius = 125.0f;  // Typical NTC range
    info->resolution_celsius = 0.1f;  // Typical resolution
    info->accuracy_celsius = 1.0f;    // Typical accuracy
    info->response_time_ms = 100;     // Typical response time
    info->capabilities = HF_TEMP_CAP_THRESHOLD_MONITORING | 
                        HF_TEMP_CAP_CONTINUOUS_READING | 
                        HF_TEMP_CAP_CALIBRATION |
                        HF_TEMP_CAP_HIGH_PRECISION;
    info->manufacturer = "Generic";
    info->model = "NTC Thermistor";
    info->version = "1.0";
    
    return TEMP_SUCCESS;
}

hf_u32_t NtcTemperatureHandler::GetCapabilities() const noexcept {
    return HF_TEMP_CAP_THRESHOLD_MONITORING | 
           HF_TEMP_CAP_CONTINUOUS_READING | 
           HF_TEMP_CAP_CALIBRATION |
           HF_TEMP_CAP_HIGH_PRECISION;
}

hf_temp_err_t NtcTemperatureHandler::SetThresholds(float low_threshold_celsius, float high_threshold_celsius) noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    if (low_threshold_celsius >= high_threshold_celsius) {
        return TEMP_ERR_INVALID_THRESHOLD;
    }
    
    config_.low_threshold_celsius = low_threshold_celsius;
    config_.high_threshold_celsius = high_threshold_celsius;
    
    Logger::GetInstance().Info(TAG, "Thresholds set: low=%.2f°C, high=%.2f°C", low_threshold_celsius, high_threshold_celsius);
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetThresholds(float* low_threshold_celsius, float* high_threshold_celsius) const noexcept {
    if (low_threshold_celsius == nullptr || high_threshold_celsius == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (!self->EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    *low_threshold_celsius = config_.low_threshold_celsius;
    *high_threshold_celsius = config_.high_threshold_celsius;
    
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::EnableThresholdMonitoring(hf_temp_threshold_callback_t callback, void* user_data) noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    threshold_callback_ = callback;
    threshold_user_data_ = user_data;
    diagnostics_.threshold_monitoring_enabled = (callback != nullptr);
    
    Logger::GetInstance().Info(TAG, "Threshold monitoring %s", callback ? "enabled" : "disabled");
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::DisableThresholdMonitoring() noexcept {
    MutexLockGuard lock(mutex_);
    
    threshold_callback_ = nullptr;
    threshold_user_data_ = nullptr;
    diagnostics_.threshold_monitoring_enabled = false;
    
    Logger::GetInstance().Info(TAG, "Threshold monitoring disabled");
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::StartContinuousMonitoring(hf_u32_t sample_rate_hz, 
                                                              hf_temp_reading_callback_t callback, 
                                                              void* user_data) noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    if (callback == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    
    if (sample_rate_hz == 0) {
        return TEMP_ERR_INVALID_PARAMETER;
    }
    
    // Stop existing monitoring if active
    if (monitoring_active_) {
        StopContinuousMonitoring();
    }
    
    // Calculate period in milliseconds (clamp to 1ms minimum).
    const uint32_t period_ms = (sample_rate_hz >= 1000U)
                                   ? 1U
                                   : (1000U / sample_rate_hz);

    monitoring_context_id_ = RegisterMonitoringContext(this);
    if (monitoring_context_id_ == 0) {
        Logger::GetInstance().Error(TAG, "No callback context slot available");
        return TEMP_ERR_RESOURCE_UNAVAILABLE;
    }
    
    // Create hardware-agnostic periodic timer
    if (!monitoring_timer_.Create("ntc_monitor", ContinuousMonitoringCallback,
                                  monitoring_context_id_, period_ms, true)) {
        UnregisterMonitoringContext(monitoring_context_id_);
        monitoring_context_id_ = 0;
        Logger::GetInstance().Error(TAG, "Failed to create monitoring timer");
        return TEMP_ERR_RESOURCE_UNAVAILABLE;
    }
    
    continuous_callback_ = callback;
    continuous_user_data_ = user_data;
    monitoring_active_ = true;
    diagnostics_.continuous_monitoring_active = true;
    
    Logger::GetInstance().Info(TAG, "Continuous monitoring started at %u Hz", sample_rate_hz);
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::StopContinuousMonitoring() noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!monitoring_active_) {
        return TEMP_SUCCESS;
    }
    
    // Stop and destroy timer
    monitoring_timer_.Stop();
    UnregisterMonitoringContext(monitoring_context_id_);
    monitoring_context_id_ = 0;
    monitoring_timer_.Destroy();
    
    continuous_callback_ = nullptr;
    continuous_user_data_ = nullptr;
    monitoring_active_ = false;
    diagnostics_.continuous_monitoring_active = false;
    
    Logger::GetInstance().Info(TAG, "Continuous monitoring stopped");
    return TEMP_SUCCESS;
}

bool NtcTemperatureHandler::IsMonitoringActive() const noexcept {
    MutexLockGuard lock(mutex_);
    return monitoring_active_;
}

hf_temp_err_t NtcTemperatureHandler::Calibrate(float reference_temperature_celsius) noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    // Read current temperature
    float current_temperature;
    NtcError result = ntc_thermistor_->ReadTemperatureCelsius(&current_temperature);
    if (result != NtcError::Success) {
        return ConvertNtcError(result);
    }
    
    // Calculate offset
    calibration_offset_ = reference_temperature_celsius - current_temperature;
    
    // Apply offset to thermistor
    ntc_thermistor_->SetCalibrationOffset(calibration_offset_);
    
    diagnostics_.calibration_valid = true;
    statistics_.calibration_count++;
    
    Logger::GetInstance().Info(TAG, "Calibration completed: offset=%.2f°C", calibration_offset_);
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::SetCalibrationOffset(float offset_celsius) noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    calibration_offset_ = offset_celsius;
    ntc_thermistor_->SetCalibrationOffset(offset_celsius);
    diagnostics_.calibration_valid = true;
    
    Logger::GetInstance().Info(TAG, "Calibration offset set: %.2f°C", offset_celsius);
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetCalibrationOffset(float* offset_celsius) const noexcept {
    if (offset_celsius == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (!self->EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    *offset_celsius = calibration_offset_;
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::ResetCalibration() noexcept {
    MutexLockGuard lock(mutex_);
    
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    
    calibration_offset_ = 0.0f;
    ntc_thermistor_->SetCalibrationOffset(0.0f);
    diagnostics_.calibration_valid = false;
    
    Logger::GetInstance().Info(TAG, "Calibration reset");
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetStatistics(hf_temp_statistics_t& statistics) noexcept {
    MutexLockGuard lock(mutex_);
    statistics = statistics_;
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetDiagnostics(hf_temp_diagnostics_t& diagnostics) noexcept {
    MutexLockGuard lock(mutex_);
    diagnostics = diagnostics_;
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::ResetStatistics() noexcept {
    MutexLockGuard lock(mutex_);
    
    statistics_ = {};
    statistics_.min_temperature_celsius = FLT_MAX;
    statistics_.max_temperature_celsius = -FLT_MAX;
    statistics_.min_operation_time_us = UINT32_MAX;
    
    Logger::GetInstance().Info(TAG, "Statistics reset");
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::ResetDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    
    diagnostics_.last_error_code = TEMP_SUCCESS;
    diagnostics_.last_error_timestamp = 0;
    diagnostics_.consecutive_errors = 0;
    diagnostics_.sensor_healthy = true;
    
    Logger::GetInstance().Info(TAG, "Diagnostics reset");
    return TEMP_SUCCESS;
}

//--------------------------------------
//  NTC-Specific Methods
//--------------------------------------

void* NtcTemperatureHandler::GetNtcThermistor() noexcept {
    if (!EnsureInitialized()) {
        return nullptr;
    }
    return ntc_thermistor_.get();
}

const void* NtcTemperatureHandler::GetNtcThermistor() const noexcept {
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    return self->GetNtcThermistor();
}

NtcError NtcTemperatureHandler::GetNtcConfiguration(ntc_config_t* config) const noexcept {
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (config == nullptr) {
        return NtcError::NullPointer;
    }
    if (!self->EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    
    return ntc_thermistor_->GetConfiguration(config);
}

NtcError NtcTemperatureHandler::GetNtcReading(ntc_reading_t* reading) noexcept {
    if (reading == nullptr) {
        return NtcError::NullPointer;
    }
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    
    return ntc_thermistor_->ReadTemperature(reading);
}

// NOTE: GetNtcStatistics and GetNtcDiagnostics removed — the NtcThermistor
// driver does not provide ntc_statistics_t / ntc_diagnostics_t types or
// corresponding accessor methods.  Use the BaseTemperature-level
// GetStatistics() / GetDiagnostics() instead.

//--------------------------------------
//  BaseTemperature Override Stubs
//--------------------------------------

hf_temp_err_t NtcTemperatureHandler::SetRange(float min_celsius, float max_celsius) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    if (min_celsius >= max_celsius) {
        return TEMP_ERR_INVALID_RANGE;
    }
    config_.min_temperature = min_celsius;
    config_.max_temperature = max_celsius;
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetRange(float* min_celsius, float* max_celsius) const noexcept {
    if (min_celsius == nullptr || max_celsius == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (!self->EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    *min_celsius = config_.min_temperature;
    *max_celsius = config_.max_temperature;
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::GetResolution(float* resolution_celsius) const noexcept {
    if (resolution_celsius == nullptr) {
        return TEMP_ERR_NULL_POINTER;
    }
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    if (!self->EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    *resolution_celsius = 0.1f;  // Typical NTC resolution
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::EnterSleepMode() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    // NTC thermistors are passive — no hardware sleep mode, but we track state
    current_state_ = HF_TEMP_STATE_SLEEPING;
    Logger::GetInstance().Info(TAG, "Entered sleep mode (passive sensor)");
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::ExitSleepMode() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    current_state_ = HF_TEMP_STATE_INITIALIZED;
    Logger::GetInstance().Info(TAG, "Exited sleep mode");
    return TEMP_SUCCESS;
}

bool NtcTemperatureHandler::IsSleeping() const noexcept {
    return current_state_ == HF_TEMP_STATE_SLEEPING;
}

hf_temp_err_t NtcTemperatureHandler::SelfTest() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    // Basic self-test: attempt a temperature reading and validate range
    float temperature = 0.0f;
    NtcError result = ntc_thermistor_->ReadTemperatureCelsius(&temperature);
    if (result != NtcError::Success) {
        diagnostics_.sensor_healthy = false;
        return ConvertNtcError(result);
    }
    if (temperature < config_.min_temperature || temperature > config_.max_temperature) {
        diagnostics_.sensor_healthy = false;
        return TEMP_ERR_OUT_OF_RANGE;
    }
    diagnostics_.sensor_healthy = true;
    Logger::GetInstance().Info(TAG, "Self-test passed: %.2f°C", temperature);
    return TEMP_SUCCESS;
}

hf_temp_err_t NtcTemperatureHandler::CheckHealth() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized()) {
        return TEMP_ERR_NOT_INITIALIZED;
    }
    if (!diagnostics_.sensor_healthy) {
        return TEMP_ERR_HARDWARE_FAULT;
    }
    if (diagnostics_.consecutive_errors > 5) {
        return TEMP_ERR_SENSOR_NOT_AVAILABLE;
    }
    return TEMP_SUCCESS;
}

//--------------------------------------
//  NTC-Specific Method Implementations
//--------------------------------------

NtcError NtcTemperatureHandler::SetNtcConfiguration(const ntc_config_t& config) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetConfiguration(config);
}

NtcError NtcTemperatureHandler::GetResistance(float* resistance_ohms) noexcept {
    MutexLockGuard lock(mutex_);
    if (resistance_ohms == nullptr) {
        return NtcError::NullPointer;
    }
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->GetResistance(resistance_ohms);
}

NtcError NtcTemperatureHandler::GetVoltage(float* voltage_volts) noexcept {
    MutexLockGuard lock(mutex_);
    if (voltage_volts == nullptr) {
        return NtcError::NullPointer;
    }
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->GetVoltage(voltage_volts);
}

NtcError NtcTemperatureHandler::GetRawAdcValue(uint32_t* adc_value) noexcept {
    MutexLockGuard lock(mutex_);
    if (adc_value == nullptr) {
        return NtcError::NullPointer;
    }
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->GetRawAdcValue(adc_value);
}

NtcError NtcTemperatureHandler::SetConversionMethod(NtcConversionMethod method) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetConversionMethod(method);
}

NtcError NtcTemperatureHandler::SetVoltageDivider(float series_resistance) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetVoltageDivider(series_resistance);
}

NtcError NtcTemperatureHandler::SetReferenceVoltage(float reference_voltage) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetReferenceVoltage(reference_voltage);
}

NtcError NtcTemperatureHandler::SetBetaValue(float beta_value) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetBetaValue(beta_value);
}

NtcError NtcTemperatureHandler::SetAdcChannel(uint8_t adc_channel) noexcept {
    MutexLockGuard lock(mutex_);
    config_.adc_channel = adc_channel;
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetAdcChannel(adc_channel);
}

NtcError NtcTemperatureHandler::SetSamplingParameters(uint32_t sample_count,
                                                      uint32_t sample_delay_ms) noexcept {
    MutexLockGuard lock(mutex_);
    config_.sample_count = sample_count;
    config_.sample_delay_ms = sample_delay_ms;
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetSamplingParameters(sample_count, sample_delay_ms);
}

NtcError NtcTemperatureHandler::SetFiltering(bool enable, float alpha) noexcept {
    MutexLockGuard lock(mutex_);
    config_.enable_filtering = enable;
    config_.filter_alpha = alpha;
    if (!EnsureInitialized() || ntc_thermistor_ == nullptr) {
        return NtcError::NotInitialized;
    }
    return ntc_thermistor_->SetFiltering(enable, alpha);
}

const char* NtcTemperatureHandler::GetSensorName() const noexcept {
    return config_.sensor_name;
}

const char* NtcTemperatureHandler::GetSensorDescription() const noexcept {
    return config_.sensor_description;
}

NtcThermistorConcrete* NtcTemperatureHandler::GetDriver() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return nullptr;
    return ntc_thermistor_.get();
}

const NtcThermistorConcrete* NtcTemperatureHandler::GetDriver() const noexcept {
    auto* self = const_cast<NtcTemperatureHandler*>(this);
    return self->GetDriver();
}

//--------------------------------------
//  Private Helper Methods
//--------------------------------------

void NtcTemperatureHandler::SetLastError(hf_temp_err_t error) noexcept {
    diagnostics_.last_error_code = error;
            diagnostics_.last_error_timestamp = static_cast<hf_u32_t>(os_time_get() / 1000); // Convert to ms
    diagnostics_.consecutive_errors++;
    
    if (diagnostics_.consecutive_errors > 5) {
        diagnostics_.sensor_healthy = false;
    }
}

void NtcTemperatureHandler::UpdateStatistics(bool operation_successful, hf_u32_t operation_time_us) noexcept {
    statistics_.total_operations++;
    
    if (operation_successful) {
        statistics_.successful_operations++;
        diagnostics_.consecutive_errors = 0;
        diagnostics_.sensor_healthy = true;
    } else {
        statistics_.failed_operations++;
    }
    
    // Update operation time statistics
    if (operation_time_us > 0) {
        if (operation_time_us > statistics_.max_operation_time_us) {
            statistics_.max_operation_time_us = operation_time_us;
        }
        if (operation_time_us < statistics_.min_operation_time_us) {
            statistics_.min_operation_time_us = operation_time_us;
        }
        
        // Update average (simple moving average)
        const uint32_t total_ops = statistics_.successful_operations + statistics_.failed_operations;
        if (total_ops > 0) {
            statistics_.average_operation_time_us = 
                (statistics_.average_operation_time_us * (total_ops - 1) + operation_time_us) / total_ops;
        }
    }
}

void NtcTemperatureHandler::UpdateDiagnostics(hf_temp_err_t error) noexcept {
    if (error != TEMP_SUCCESS) {
        SetLastError(error);
    } else {
        diagnostics_.consecutive_errors = 0;
        diagnostics_.sensor_healthy = true;
    }
}

void NtcTemperatureHandler::CheckThresholds(float temperature_celsius) noexcept {
    if (!diagnostics_.threshold_monitoring_enabled || threshold_callback_ == nullptr) {
        return;
    }
    
    if (temperature_celsius < config_.low_threshold_celsius) {
        statistics_.threshold_violations++;
        threshold_callback_(this, temperature_celsius, 0, threshold_user_data_); // 0 = low threshold
    } else if (temperature_celsius > config_.high_threshold_celsius) {
        statistics_.threshold_violations++;
        threshold_callback_(this, temperature_celsius, 1, threshold_user_data_); // 1 = high threshold
    }
}

hf_temp_err_t NtcTemperatureHandler::ConvertNtcError(NtcError ntc_error) const noexcept {
    switch (ntc_error) {
        case NtcError::Success:
            return TEMP_SUCCESS;
        case NtcError::NullPointer:
            return TEMP_ERR_NULL_POINTER;
        case NtcError::NotInitialized:
            return TEMP_ERR_NOT_INITIALIZED;
        case NtcError::AlreadyInitialized:
            return TEMP_ERR_ALREADY_INITIALIZED;
        case NtcError::InvalidParameter:
            return TEMP_ERR_INVALID_PARAMETER;
        case NtcError::OutOfMemory:
            return TEMP_ERR_OUT_OF_MEMORY;
        case NtcError::AdcReadFailed:
            return TEMP_ERR_READ_FAILED;
        case NtcError::InvalidResistance:
            return TEMP_ERR_INVALID_READING;
        case NtcError::TemperatureOutOfRange:
            return TEMP_ERR_OUT_OF_RANGE;
        case NtcError::Timeout:
            return TEMP_ERR_TIMEOUT;
        case NtcError::CalibrationFailed:
            return TEMP_ERR_CALIBRATION_FAILED;
        case NtcError::HardwareFault:
            return TEMP_ERR_HARDWARE_FAULT;
        case NtcError::ConversionFailed:
            return TEMP_ERR_CONVERSION_FAILED;
        case NtcError::LookupTableError:
        case NtcError::UnsupportedOperation:
        case NtcError::Failure:
        default:
            return TEMP_ERR_FAILURE;
    }
}

//--------------------------------------
//  Static Callback Functions
//--------------------------------------

void NtcTemperatureHandler::ContinuousMonitoringCallback(uint32_t arg) {
    auto* handler = ResolveMonitoringContext(arg);
    if (handler == nullptr) {
        return;
    }
    
    // Read temperature
    hf_temp_reading_t reading = {};
    hf_temp_err_t error = handler->ReadTemperature(&reading);
    (void)error;
    
    // Call user callback if provided
    if (handler->continuous_callback_ != nullptr) {
        handler->continuous_callback_(handler, &reading, handler->continuous_user_data_);
    }
}

hf_u32_t NtcTemperatureHandler::RegisterMonitoringContext(NtcTemperatureHandler* handler) noexcept {
    if (handler == nullptr) {
        return 0;
    }

    MutexLockGuard lock(callback_registry_mutex_);
    if (!lock.IsLocked()) {
        return 0;
    }

    for (hf_u32_t idx = 0; idx < kMonitoringContextSlots; ++idx) {
        if (callback_registry_[idx] == nullptr) {
            callback_registry_[idx] = handler;
            return idx + 1;  // Reserve 0 as invalid ID.
        }
    }
    return 0;
}

void NtcTemperatureHandler::UnregisterMonitoringContext(hf_u32_t context_id) noexcept {
    if (context_id == 0 || context_id > kMonitoringContextSlots) {
        return;
    }

    MutexLockGuard lock(callback_registry_mutex_);
    if (!lock.IsLocked()) {
        return;
    }

    callback_registry_[context_id - 1] = nullptr;
}

NtcTemperatureHandler* NtcTemperatureHandler::ResolveMonitoringContext(hf_u32_t context_id) noexcept {
    if (context_id == 0 || context_id > kMonitoringContextSlots) {
        return nullptr;
    }

    MutexLockGuard lock(callback_registry_mutex_);
    if (!lock.IsLocked()) {
        return nullptr;
    }

    return callback_registry_[context_id - 1];
}
