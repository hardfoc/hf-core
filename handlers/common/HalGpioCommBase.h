/**
 * @file HalGpioCommBase.h
 * @brief `hf::HalGpioCommBase`: the control-pin hooks on `BaseGpio` pins.
 *
 * Hooks (standards/driver.md): `GpioSet(pin, signal)` and `GpioRead(pin, signal)` -> `bool`, for a
 * driver's logical pins (`<chip>::CtrlPin`) and levels (`<chip>::GpioSignal::ACTIVE` / `INACTIVE`).
 * The electrical polarity is the `BaseGpio`'s active state, set by the board.
 *
 * The adapter maps each pin; chain this base under a bus base:
 *
 * @code
 * class HalSpiFooComm final
 *     : public hf::HalSpiCommBase<HalSpiFooComm,
 *                                 hf::HalGpioCommBase<HalSpiFooComm,
 * foo::SpiInterface<HalSpiFooComm>>> { public: HalSpiFooComm(BaseSpi& spi, BaseGpio& reset)
 * noexcept : HalSpiCommBase(spi), reset_(reset) {} BaseGpio* GpioFor(foo::CtrlPin pin) noexcept {
 *     return pin == foo::CtrlPin::Reset ? &reset_ : nullptr;  // null: not wired on this board
 *   }
 *
 * private:
 *   BaseGpio& reset_;
 * };
 * @endcode
 *
 * Each hook initialises its pin on first use. A pin the board did not wire (`GpioFor()` returns
 * null) fails the hook with `GPIO_ERR_PIN_NOT_FOUND`: the driver sees the failure and falls back as
 * its documentation says; nothing is skipped silently.
 */
#pragma once

#include <cstdint>

#include "HalCommBase.h"
#include "base/BaseGpio.h"

namespace hf {

/**
 * @brief Control-pin adapter base.
 * @tparam Derived The adapter (CRTP); provides `BaseGpio* GpioFor(Pin) noexcept`.
 * @tparam PortBase The driver's port base, which this class derives from.
 */
template <typename Derived, typename PortBase = NoPort>
class HalGpioCommBase : public PortBase {
public:
  /// Drive logical @p pin to @p signal.
  template <typename Pin, typename Signal>
  bool GpioSet(Pin pin, Signal signal) noexcept {
    BaseGpio* gpio = pinFor(pin);
    if (gpio == nullptr) {
      return false;
    }
    return noteGpio(signal == Signal::ACTIVE ? gpio->SetActive() : gpio->SetInactive());
  }
  /// Read logical @p pin into @p signal.
  template <typename Pin, typename Signal>
  bool GpioRead(Pin pin, Signal& signal) noexcept {
    BaseGpio* gpio = pinFor(pin);
    if (gpio == nullptr) {
      return false;
    }
    bool active = false;
    if (!noteGpio(gpio->IsActive(active))) {
      return false;
    }
    signal = active ? Signal::ACTIVE : Signal::INACTIVE;
    return true;
  }

  /// The code behind the last failed pin hook; `GPIO_SUCCESS` when none failed since the last
  /// clear.
  [[nodiscard]] hf_gpio_err_t LastGpioError() const noexcept {
    return last_gpio_error_;
  }
  /// `LastGpioError()` as a `Result`.
  [[nodiscard]] Result<void> LastGpioResult() const noexcept {
    return {ToStatus(last_gpio_error_), static_cast<std::uint16_t>(last_gpio_error_)};
  }
  /// Forget the last pin failure.
  void ClearLastGpioError() noexcept {
    last_gpio_error_ = hf_gpio_err_t::GPIO_SUCCESS;
  }

protected:
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): built by the next base in the chain
  HalGpioCommBase() noexcept = default;
  ~HalGpioCommBase() = default;

private:
  template <typename Pin>
  BaseGpio* pinFor(Pin pin) noexcept {
    BaseGpio* gpio = static_cast<Derived*>(this)->GpioFor(pin);
    if (gpio == nullptr) {
      noteGpio(hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND);
      return nullptr;
    }
    if (!gpio->EnsureInitialized()) {
      noteGpio(hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED);
      return nullptr;
    }
    return gpio;
  }
  bool noteGpio(hf_gpio_err_t error) noexcept {
    if (error == hf_gpio_err_t::GPIO_SUCCESS) {
      return true;
    }
    last_gpio_error_ = error;
    return false;
  }

  hf_gpio_err_t last_gpio_error_{hf_gpio_err_t::GPIO_SUCCESS};
};

} // namespace hf
