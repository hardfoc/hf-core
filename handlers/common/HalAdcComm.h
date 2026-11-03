/**
 * @file HalAdcComm.h
 * @brief `hf::HalAdcComm`: the standard ADC port hooks on a `BaseAdc`.
 *
 * Hooks: `ReadChannelV(channel, volts)`, `ReadChannelCount(channel, count)` -> `bool`,
 * `IsChannelAvailable(channel)`, `IsInitialized()`, `EnsureInitialized()`, `DelayUs()`, `DelayMs()`.
 * A `false` keeps the `hf_adc_err_t` behind it in `LastBusError()`.
 *
 * Each read is one conversion with no averaging and no delay between samples; a driver that wants
 * averaging asks for it. An adapter can declare `static constexpr hf_u8_t kSamplesPerRead` and
 * `static constexpr hf_time_t kSampleIntervalMs` to change that.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstdint>

#include "HalCommBase.h"
#include "IiwStatus.h"
#include "base/BaseAdc.h"

namespace hf {

/**
 * @brief ADC adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from (default: none).
 */
template <typename Derived, typename PortBase = NoPort>
class HalAdcComm : public HalDelayHooks<PortBase> {
public:
    /// Adapter on @p adc; the object must outlive the adapter.
    explicit HalAdcComm(BaseAdc& adc) noexcept : adc_(adc) {}

    /// Read @p channel in volts.
    bool ReadChannelV(std::uint8_t channel, float& volts) noexcept {
        return Note(adc_.ReadChannelV(channel, volts, SamplesPerRead(), SampleIntervalMs()));
    }
    /// Read @p channel as a raw count.
    bool ReadChannelCount(std::uint8_t channel, std::uint32_t& count) noexcept {
        hf_u32_t raw = 0;
        const bool ok = Note(adc_.ReadChannelCount(channel, raw, SamplesPerRead(), SampleIntervalMs()));
        count = ok ? raw : 0U;
        return ok;
    }
    /// True when @p channel exists on this ADC.
    [[nodiscard]] bool IsChannelAvailable(std::uint8_t channel) const noexcept {
        return adc_.IsChannelAvailable(channel);
    }
    /// True once the ADC is initialised.
    [[nodiscard]] bool IsInitialized() const noexcept {
        return adc_.IsInitialized();
    }
    /// Bring the ADC up (idempotent).
    bool EnsureInitialized() noexcept {
        return adc_.EnsureInitialized() || Note(hf_adc_err_t::ADC_ERR_NOT_INITIALIZED);
    }

    /// The code behind the last failed hook, `ADC_SUCCESS` if none failed since `ClearLastBusError()`.
    [[nodiscard]] hf_adc_err_t LastBusError() const noexcept {
        return last_error_;
    }
    /// `LastBusError()` as a `Result`.
    [[nodiscard]] Result<void> LastBusResult() const noexcept {
        return ToResult(last_error_);
    }
    /// Forget the last failure.
    void ClearLastBusError() noexcept {
        last_error_ = hf_adc_err_t::ADC_SUCCESS;
    }
    /// The underlying ADC.
    [[nodiscard]] BaseAdc& Adc() noexcept {
        return adc_;
    }

protected:
    ~HalAdcComm() = default;

    /// Keep @p error if it is a failure; true on success.
    bool Note(hf_adc_err_t error) noexcept {
        if (error == hf_adc_err_t::ADC_SUCCESS) {
            return true;
        }
        last_error_ = error;
        return false;
    }

private:
    static constexpr hf_u8_t SamplesPerRead() noexcept {
        if constexpr (requires { Derived::kSamplesPerRead; }) {
            return Derived::kSamplesPerRead;
        } else {
            return 1U;
        }
    }
    static constexpr hf_time_t SampleIntervalMs() noexcept {
        if constexpr (requires { Derived::kSampleIntervalMs; }) {
            return Derived::kSampleIntervalMs;
        } else {
            return 0U;
        }
    }

    BaseAdc& adc_;
    hf_adc_err_t last_error_{hf_adc_err_t::ADC_SUCCESS};
};

} // namespace hf
