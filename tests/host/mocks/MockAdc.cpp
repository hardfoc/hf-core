/**
 * @file MockAdc.cpp
 * @brief MockAdc implementation.
 *
 * @copyright HardFOC
 */

#include "MockAdc.h"

#include <algorithm>
#include <cmath>

namespace hf::host {

MockAdc::MockAdc(hf_u8_t channel_count, float reference_voltage, uint8_t resolution_bits) noexcept
    : channel_count_(std::min(channel_count, kMaxChannels)),
      reference_voltage_(reference_voltage),
      full_scale_((1U << resolution_bits) - 1U) {}

void MockAdc::SetVoltage(hf_channel_id_t channel, float volts) noexcept {
    if (channel >= kMaxChannels) {
        return;
    }
    channels_[channel].volts = volts;
    const float clamped = std::clamp(volts / reference_voltage_, 0.0f, 1.0f);
    channels_[channel].count = static_cast<hf_u32_t>(std::lround(clamped * static_cast<float>(full_scale_)));
}

void MockAdc::SetCount(hf_channel_id_t channel, hf_u32_t count) noexcept {
    if (channel >= kMaxChannels) {
        return;
    }
    channels_[channel].count = count;
    channels_[channel].volts =
        static_cast<float>(count) * reference_voltage_ / static_cast<float>(full_scale_);
}

void MockAdc::QueueVoltages(hf_channel_id_t channel, const std::vector<float>& volts) {
    if (channel >= kMaxChannels) {
        return;
    }
    for (float v : volts) {
        channels_[channel].queued.push_back(v);
    }
}

void MockAdc::FailChannel(hf_channel_id_t channel, hf_adc_err_t error) noexcept {
    if (channel < kMaxChannels) {
        channels_[channel].error = error;
    }
}

void MockAdc::ClearFailure(hf_channel_id_t channel) noexcept {
    FailChannel(channel, hf_adc_err_t::ADC_SUCCESS);
}

uint32_t MockAdc::ReadCount(hf_channel_id_t channel) const noexcept {
    return channel < kMaxChannels ? channels_[channel].reads : 0U;
}

hf_adc_err_t MockAdc::Sample(hf_channel_id_t channel_id, float& volts, hf_u32_t& count) noexcept {
    if (channel_id >= channel_count_) {
        return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }
    Channel& ch = channels_[channel_id];
    ++ch.reads;
    if (ch.error != hf_adc_err_t::ADC_SUCCESS) {
        return ch.error;
    }
    if (!ch.queued.empty()) {
        volts = ch.queued.front();
        ch.queued.pop_front();
        const float clamped = std::clamp(volts / reference_voltage_, 0.0f, 1.0f);
        count = static_cast<hf_u32_t>(std::lround(clamped * static_cast<float>(full_scale_)));
        return hf_adc_err_t::ADC_SUCCESS;
    }
    volts = ch.volts;
    count = ch.count;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t MockAdc::ReadChannelV(hf_channel_id_t channel_id, float& channel_reading_v,
                                   hf_u8_t /*numOfSamplesToAvg*/, hf_time_t /*timeBetweenSamples*/) noexcept {
    hf_u32_t count = 0;
    return Sample(channel_id, channel_reading_v, count);
}

hf_adc_err_t MockAdc::ReadChannelCount(hf_channel_id_t channel_id, hf_u32_t& channel_reading_count,
                                       hf_u8_t /*numOfSamplesToAvg*/, hf_time_t /*timeBetweenSamples*/) noexcept {
    float volts = 0.0f;
    return Sample(channel_id, volts, channel_reading_count);
}

hf_adc_err_t MockAdc::ReadChannel(hf_channel_id_t channel_id, hf_u32_t& channel_reading_count,
                                  float& channel_reading_v, hf_u8_t /*numOfSamplesToAvg*/,
                                  hf_time_t /*timeBetweenSamples*/) noexcept {
    return Sample(channel_id, channel_reading_v, channel_reading_count);
}

}  // namespace hf::host
