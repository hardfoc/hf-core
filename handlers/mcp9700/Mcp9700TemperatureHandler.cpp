/**
 * @file Mcp9700TemperatureHandler.cpp
 * @brief MCP9700 thermistor handler implementation.
 */

#include "Mcp9700TemperatureHandler.h"

#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/utils/memory_utils.h"

Mcp9700TemperatureHandler::Mcp9700TemperatureHandler(BaseAdc* adc_interface, uint8_t adc_channel,
                                                     const char* sensor_name) noexcept
    : BaseTemperature()
    , adc_interface_(adc_interface)
    , adc_channel_(adc_channel)
    , sensor_name_(sensor_name != nullptr ? sensor_name : "MCP9700") {}

bool Mcp9700TemperatureHandler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (adc_interface_ == nullptr) {
        return false;
    }
    adc_adapter_ = hf::utils::make_unique_nothrow<Mcp9700AdcAdapter>(adc_interface_);
    if (!adc_adapter_) {
        return false;
    }
    thermistor_ = hf::utils::make_unique_nothrow<Mcp9700ThermistorConcrete>(adc_adapter_.get(), adc_channel_);
    if (!thermistor_) {
        adc_adapter_.reset();
        return false;
    }
    if (!thermistor_->Initialize()) {
        thermistor_.reset();
        adc_adapter_.reset();
        return false;
    }
    return true;
}

bool Mcp9700TemperatureHandler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (thermistor_) {
        thermistor_->Deinitialize();
        thermistor_.reset();
    }
    adc_adapter_.reset();
    return true;
}

hf_temp_err_t Mcp9700TemperatureHandler::ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept {
    if (temperature_celsius == nullptr) {
        return hf_temp_err_t::TEMP_ERR_NULL_POINTER;
    }
    MutexLockGuard lock(mutex_);
    if (!thermistor_) {
        return hf_temp_err_t::TEMP_ERR_NOT_INITIALIZED;
    }
    if (!thermistor_->ReadTemperatureCelsius(temperature_celsius)) {
        return hf_temp_err_t::TEMP_ERR_READ_FAILED;
    }
    return hf_temp_err_t::TEMP_SUCCESS;
}

hf_temp_err_t Mcp9700TemperatureHandler::GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept {
    if (info == nullptr) {
        return hf_temp_err_t::TEMP_ERR_NULL_POINTER;
    }
    info->sensor_type = HF_TEMP_SENSOR_TYPE_EXTERNAL_ANALOG;
    info->min_temp_celsius = -40.0f;
    info->max_temp_celsius = 125.0f;
    info->resolution_celsius = 0.1f;
    info->accuracy_celsius = 1.0f;
    info->response_time_ms = 10;
    info->capabilities = HF_TEMP_CAP_NONE;
    info->manufacturer = "Microchip";
    info->model = "MCP9700";
    info->version = "1";
    return hf_temp_err_t::TEMP_SUCCESS;
}

hf_u32_t Mcp9700TemperatureHandler::GetCapabilities() const noexcept {
    return HF_TEMP_CAP_NONE;
}
