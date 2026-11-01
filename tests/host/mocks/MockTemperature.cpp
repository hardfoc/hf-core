/**
 * @file MockTemperature.cpp
 * @brief MockTemperature implementation.
 *
 * @copyright HardFOC
 */

#include "MockTemperature.h"

namespace hf::host {

hf_temp_err_t MockTemperature::GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept {
    if (info == nullptr) {
        return hf_temp_err_t::TEMP_ERR_NULL_POINTER;
    }
    *info = hf_temp_sensor_info_t{};
    info->sensor_type = HF_TEMP_SENSOR_TYPE_EXTERNAL_ANALOG;
    info->min_temp_celsius = -40.0f;
    info->max_temp_celsius = 125.0f;
    info->resolution_celsius = 0.1f;
    info->accuracy_celsius = 0.5f;
    info->manufacturer = "HardFOC";
    info->model = "MockTemperature";
    info->version = "1";
    return hf_temp_err_t::TEMP_SUCCESS;
}

hf_temp_err_t MockTemperature::ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept {
    ++reads_;
    if (temperature_celsius == nullptr) {
        return hf_temp_err_t::TEMP_ERR_NULL_POINTER;
    }
    if (error_ != hf_temp_err_t::TEMP_SUCCESS) {
        return error_;
    }
    *temperature_celsius = celsius_;
    return hf_temp_err_t::TEMP_SUCCESS;
}

}  // namespace hf::host
