/**
 * @file MockTemperature.h
 * @brief Scriptable BaseTemperature fake for host tests.
 *
 * @details
 * Returns the temperature set with SetCelsius() through the normal
 * BaseTemperature read path (ReadTemperatureCelsius, unit conversion,
 * lazy initialization). Fail() makes reads return the given error.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseTemperature.h"

namespace hf::host {

class MockTemperature : public BaseTemperature {
public:
    MockTemperature() noexcept = default;
    ~MockTemperature() noexcept override = default;

    void SetCelsius(float celsius) noexcept { celsius_ = celsius; }
    void Fail(hf_temp_err_t error = hf_temp_err_t::TEMP_ERR_READ_FAILED) noexcept { error_ = error; }
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    [[nodiscard]] int ReadCalls() const noexcept { return reads_; }

    hf_temp_err_t GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept override;
    [[nodiscard]] hf_u32_t GetCapabilities() const noexcept override { return HF_TEMP_CAP_NONE; }

protected:
    bool Initialize() noexcept override { return init_result_; }
    bool Deinitialize() noexcept override { return true; }
    hf_temp_err_t ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept override;

private:
    float celsius_{25.0f};
    hf_temp_err_t error_{hf_temp_err_t::TEMP_SUCCESS};
    bool init_result_{true};
    int reads_{0};
};

}  // namespace hf::host
