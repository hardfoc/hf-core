/**
 * @file MockAdc.h
 * @brief Scriptable BaseAdc fake for host tests.
 *
 * @details
 * Each channel holds a voltage and a raw count, set independently. When only
 * a voltage is set, the count is derived from the reference voltage and the
 * resolution given at construction (and the other way round). A queued
 * sequence (QueueVoltages) is returned one value per read before falling back
 * to the fixed value. Reads of a channel at or above GetMaxChannels() return
 * ADC_ERR_INVALID_CHANNEL; FailChannel() makes a channel return an error.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseAdc.h"

#include <array>
#include <cstdint>
#include <deque>
#include <vector>

namespace hf::host {

class MockAdc : public BaseAdc {
public:
    static constexpr hf_u8_t kMaxChannels = 32;

    explicit MockAdc(hf_u8_t channel_count = 16, float reference_voltage = 3.3f,
                     uint8_t resolution_bits = 12) noexcept;
    ~MockAdc() noexcept override = default;

    // ── BaseAdc ───────────────────────────────────────────────────────────
    bool Initialize() noexcept override { return init_result_; }
    bool Deinitialize() noexcept override { return true; }
    [[nodiscard]] hf_u8_t GetMaxChannels() const noexcept override { return channel_count_; }
    [[nodiscard]] bool IsChannelAvailable(hf_channel_id_t channel_id) const noexcept override {
        return channel_id < channel_count_;
    }
    hf_adc_err_t ReadChannelV(hf_channel_id_t channel_id, float& channel_reading_v,
                              hf_u8_t numOfSamplesToAvg = 1,
                              hf_time_t timeBetweenSamples = 0) noexcept override;
    hf_adc_err_t ReadChannelCount(hf_channel_id_t channel_id, hf_u32_t& channel_reading_count,
                                  hf_u8_t numOfSamplesToAvg = 1,
                                  hf_time_t timeBetweenSamples = 0) noexcept override;
    hf_adc_err_t ReadChannel(hf_channel_id_t channel_id, hf_u32_t& channel_reading_count,
                             float& channel_reading_v, hf_u8_t numOfSamplesToAvg = 1,
                             hf_time_t timeBetweenSamples = 0) noexcept override;

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    void SetVoltage(hf_channel_id_t channel, float volts) noexcept;
    void SetCount(hf_channel_id_t channel, hf_u32_t count) noexcept;
    void QueueVoltages(hf_channel_id_t channel, const std::vector<float>& volts);
    void FailChannel(hf_channel_id_t channel, hf_adc_err_t error = hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR) noexcept;
    void ClearFailure(hf_channel_id_t channel) noexcept;

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] uint32_t ReadCount(hf_channel_id_t channel) const noexcept;
    [[nodiscard]] float ReferenceVoltage() const noexcept { return reference_voltage_; }
    [[nodiscard]] hf_u32_t FullScaleCount() const noexcept { return full_scale_; }

private:
    struct Channel {
        float volts{0.0f};
        hf_u32_t count{0};
        std::deque<float> queued;
        hf_adc_err_t error{hf_adc_err_t::ADC_SUCCESS};
        uint32_t reads{0};
    };
    hf_adc_err_t Sample(hf_channel_id_t channel_id, float& volts, hf_u32_t& count) noexcept;

    bool init_result_{true};
    hf_u8_t channel_count_;
    float reference_voltage_;
    hf_u32_t full_scale_;
    std::array<Channel, kMaxChannels> channels_{};
};

}  // namespace hf::host
