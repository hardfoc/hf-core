/**
 * @file Mcp9700TemperatureHandler.h
 * @brief MCP9700 linear active thermistor handler (ADC voltage to °C).
 */

#pragma once

#include "core/hf-core-drivers/external/hf-mcp9700-driver/inc/mcp9700_thermistor.hpp"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseAdc.h"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseTemperature.h"
#include "RtosMutex.h"

#include <cstdint>
#include <memory>

/**
 * @brief Thin adapter: BaseAdc channel read in the shape expected by Mcp9700Thermistor<>.
 */
class Mcp9700AdcAdapter {
public:
    explicit Mcp9700AdcAdapter(BaseAdc* adc) noexcept : adc_(adc) {}

    [[nodiscard]] bool EnsureInitialized() noexcept {
        return adc_ != nullptr && adc_->EnsureInitialized();
    }

    hf_adc_err_t ReadChannelV(uint8_t channel, float* voltage_v) noexcept {
        if (adc_ == nullptr || voltage_v == nullptr) {
            return hf_adc_err_t::ADC_ERR_NULL_POINTER;
        }
        return adc_->ReadChannelV(static_cast<hf_channel_id_t>(channel), *voltage_v);
    }

private:
    BaseAdc* adc_;
};

using Mcp9700ThermistorConcrete = hf::mcp9700::Mcp9700Thermistor<Mcp9700AdcAdapter>;

/**
 * @brief BaseTemperature implementation for an MCP9700 on a multiplexed ADC channel.
 */
class Mcp9700TemperatureHandler : public BaseTemperature {
public:
    Mcp9700TemperatureHandler(BaseAdc* adc_interface, uint8_t adc_channel,
                              const char* sensor_name = "MCP9700") noexcept;

    Mcp9700TemperatureHandler(const Mcp9700TemperatureHandler&) = delete;
    Mcp9700TemperatureHandler& operator=(const Mcp9700TemperatureHandler&) = delete;

    ~Mcp9700TemperatureHandler() noexcept override = default;

protected:
    bool Initialize() noexcept override;
    bool Deinitialize() noexcept override;
    hf_temp_err_t ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept override;
    hf_temp_err_t GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept override;
    [[nodiscard]] hf_u32_t GetCapabilities() const noexcept override;

private:
    mutable RtosMutex mutex_;
    BaseAdc* adc_interface_;
    uint8_t adc_channel_;
    const char* sensor_name_;
    std::unique_ptr<Mcp9700AdcAdapter> adc_adapter_;
    std::unique_ptr<Mcp9700ThermistorConcrete> thermistor_;
};
