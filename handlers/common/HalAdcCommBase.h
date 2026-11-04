/**
 * @file HalAdcCommBase.h
 * @brief `hf::HalAdcCommBase`: the standard ADC port hooks on a `BaseAdc`.
 *
 * Hooks (standards/driver.md): `ReadVoltageMv(channel, voltage_mv)` -> `bool`,
 * `EnsureInitialized()`, `DelayUs()` and `DelayMs()`. For drivers that still read volts or counts:
 * `ReadChannelV()`, `ReadChannelCount()`, `IsChannelAvailable()` and `IsInitialized()`.
 *
 * Each read is one conversion, with no averaging. An adapter can declare
 * `static constexpr hf_u8_t kSamplesPerRead` and `static constexpr hf_time_t kSampleIntervalMs`.
 */
#pragma once

#include <cmath>
#include <cstdint>

#include "HalCommBase.h"
#include "base/BaseAdc.h"

namespace hf {

/**
 * @brief ADC adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from.
 */
template <typename Derived, typename PortBase = NoPort>
class HalAdcCommBase : public HalDelayHooks<PortBase> {
public:
  /// Adapter on @p adc, which must outlive it. Public so an adapter can inherit it.
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): inherited by `using` in the adapter
  explicit HalAdcCommBase(BaseAdc& adc) noexcept : adc_(adc) {}

  /// Read @p channel in millivolts (rounded to the nearest; negative readings give 0).
  bool ReadVoltageMv(std::uint8_t channel, std::uint32_t& voltage_mv) noexcept {
    float volts = 0.0F;
    if (!ReadChannelV(channel, volts)) {
      voltage_mv = 0U;
      return false;
    }
    voltage_mv = volts <= 0.0F ? 0U : static_cast<std::uint32_t>(std::lround(volts * 1000.0F));
    return true;
  }
  /// Read @p channel in volts.
  bool ReadChannelV(std::uint8_t channel, float& volts) noexcept {
    return note(adc_.ReadChannelV(channel, volts, samplesPerRead(), sampleIntervalMs()));
  }
  /// Read @p channel as a raw count.
  bool ReadChannelCount(std::uint8_t channel, std::uint32_t& count) noexcept {
    hf_u32_t raw = 0;
    const bool ok = note(adc_.ReadChannelCount(channel, raw, samplesPerRead(), sampleIntervalMs()));
    count = ok ? raw : 0U;
    return ok;
  }
  /// True when @p channel exists on this ADC (no I/O).
  [[nodiscard]] bool IsChannelAvailable(std::uint8_t channel) const noexcept {
    return adc_.IsChannelAvailable(channel);
  }
  /// True once the ADC is initialised (no I/O).
  [[nodiscard]] bool IsInitialized() const noexcept {
    return adc_.IsInitialized();
  }
  /// Bring the ADC up on first use (idempotent).
  bool EnsureInitialized() noexcept {
    return adc_.EnsureInitialized() || note(hf_adc_err_t::ADC_ERR_NOT_INITIALIZED);
  }

  /// The code behind the last failed hook; `ADC_SUCCESS` when none failed since the last clear.
  [[nodiscard]] hf_adc_err_t LastBusError() const noexcept {
    return last_error_;
  }
  /// `LastBusError()` as a `Result`.
  [[nodiscard]] Result<void> LastBusResult() const noexcept {
    return {ToStatus(last_error_), static_cast<std::uint16_t>(last_error_)};
  }
  /// Forget the last failure.
  void ClearLastBusError() noexcept {
    last_error_ = hf_adc_err_t::ADC_SUCCESS;
  }

protected:
  ~HalAdcCommBase() = default;

private:
  bool note(hf_adc_err_t error) noexcept {
    if (error == hf_adc_err_t::ADC_SUCCESS) {
      return true;
    }
    last_error_ = error;
    return false;
  }
  static constexpr hf_u8_t samplesPerRead() noexcept {
    if constexpr (requires { Derived::kSamplesPerRead; }) {
      return Derived::kSamplesPerRead;
    } else {
      return 1U;
    }
  }
  static constexpr hf_time_t sampleIntervalMs() noexcept {
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
