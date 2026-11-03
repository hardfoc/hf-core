/**
 * @file HalGpioComm.h
 * @brief `hf::HalGpioComm`: the standard control-pin hooks on `BaseGpio` pins.
 *
 * Hooks: `GpioSet(pin, signal)` and `GpioRead(pin, signal)` -> `bool`, for a driver's logical pins
 * (`<chip>::CtrlPin`) and logical levels (`<chip>::GpioSignal::ACTIVE` / `INACTIVE`). The physical
 * polarity is the `BaseGpio`'s active state, set by the board; the driver never sees HIGH or LOW.
 *
 * The adapter maps each logical pin to a `BaseGpio`:
 *
 * @code
 * class HalSpiFooComm final
 *     : public hf::HalSpiComm<HalSpiFooComm, hf::HalGpioComm<HalSpiFooComm, foo::SpiInterface<HalSpiFooComm>>> {
 * public:
 *     HalSpiFooComm(BaseSpi& spi, BaseGpio& reset, BaseGpio* wake) noexcept
 *         : HalSpiComm(spi), reset_(reset), wake_(wake) {}
 *     BaseGpio* GpioFor(foo::CtrlPin pin) noexcept {
 *         switch (pin) {
 *         case foo::CtrlPin::RESET: return &reset_;
 *         case foo::CtrlPin::WAKE:  return wake_;  // may be null: not wired on this board
 *         }
 *         return nullptr;
 *     }
 * private:
 *     BaseGpio& reset_;
 *     BaseGpio* wake_;
 * };
 * @endcode
 *
 * Each hook initialises its pin on first use (`BaseGpio::EnsureInitialized()`). A pin the board did not wire
 * (`GpioFor()` returns null) fails the hook with `GPIO_ERR_PIN_NOT_FOUND`: the driver sees the failure and decides;
 * nothing is skipped silently.
 *
 * This base has no delay hooks; chain it under a bus base (`HalSpiComm<D, HalGpioComm<D, Port>>`).
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstdint>

#include "HalCommBase.h"
#include "IiwStatus.h"
#include "base/BaseGpio.h"

namespace hf {

/**
 * @brief Control-pin adapter base.
 * @tparam Derived The adapter (CRTP); provides `BaseGpio* GpioFor(Pin) noexcept` for each pin type.
 * @tparam PortBase The driver's port base, which this class derives from (default: none).
 */
template <typename Derived, typename PortBase = NoPort>
class HalGpioComm : public PortBase {
public:
    /// Drive logical @p pin to @p signal (`Signal::ACTIVE` or `Signal::INACTIVE`).
    template <typename Pin, typename Signal>
    bool GpioSet(Pin pin, Signal signal) noexcept {
        BaseGpio* gpio = static_cast<Derived*>(this)->GpioFor(pin);
        if (gpio == nullptr) {
            return NoteGpio(hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND);
        }
        if (!gpio->EnsureInitialized()) {
            return NoteGpio(hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED);
        }
        return NoteGpio(signal == Signal::ACTIVE ? gpio->SetActive() : gpio->SetInactive());
    }

    /// Read logical @p pin into @p signal.
    template <typename Pin, typename Signal>
    bool GpioRead(Pin pin, Signal& signal) noexcept {
        BaseGpio* gpio = static_cast<Derived*>(this)->GpioFor(pin);
        if (gpio == nullptr) {
            return NoteGpio(hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND);
        }
        if (!gpio->EnsureInitialized()) {
            return NoteGpio(hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED);
        }
        bool active = false;
        if (!NoteGpio(gpio->IsActive(active))) {
            return false;
        }
        signal = active ? Signal::ACTIVE : Signal::INACTIVE;
        return true;
    }

    /// The code behind the last failed pin hook, `GPIO_SUCCESS` if none failed since `ClearLastGpioError()`.
    [[nodiscard]] hf_gpio_err_t LastGpioError() const noexcept {
        return last_gpio_error_;
    }
    /// `LastGpioError()` as a `Result`.
    [[nodiscard]] Result<void> LastGpioResult() const noexcept {
        return ToResult(last_gpio_error_);
    }
    /// Forget the last pin failure.
    void ClearLastGpioError() noexcept {
        last_gpio_error_ = hf_gpio_err_t::GPIO_SUCCESS;
    }

protected:
    HalGpioComm() noexcept = default;
    ~HalGpioComm() = default;

    /// Keep @p error if it is a failure; true on success.
    bool NoteGpio(hf_gpio_err_t error) noexcept {
        if (error == hf_gpio_err_t::GPIO_SUCCESS) {
            return true;
        }
        last_gpio_error_ = error;
        return false;
    }

private:
    hf_gpio_err_t last_gpio_error_{hf_gpio_err_t::GPIO_SUCCESS};
};

} // namespace hf
