/**
 * @file MockGpio.cpp
 * @brief MockGpio implementation.
 *
 * @copyright HardFOC
 */

#include "MockGpio.h"

namespace hf::host {

MockGpio::MockGpio(hf_pin_num_t pin, hf_gpio_direction_t direction,
                   hf_gpio_active_state_t active_state) noexcept
    : BaseGpio(pin, direction, active_state), direction_hw_(direction) {}

bool MockGpio::Initialize() noexcept { return init_result_; }

hf_gpio_err_t MockGpio::ConfigureInterrupt(hf_gpio_interrupt_trigger_t trigger,
                                           InterruptCallback callback, void* user_data) noexcept {
    trigger_ = trigger;
    callback_ = callback;
    callback_user_data_ = user_data;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::EnableInterrupt() noexcept {
    irq_enabled_ = true;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::DisableInterrupt() noexcept {
    irq_enabled_ = false;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

bool MockGpio::FireInterrupt() noexcept {
    if (!irq_enabled_ || callback_ == nullptr) {
        return false;
    }
    callback_(this, trigger_, callback_user_data_);
    return true;
}

hf_gpio_err_t MockGpio::SetDirectionImpl(hf_gpio_direction_t direction) noexcept {
    direction_hw_ = direction;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::GetDirectionImpl(hf_gpio_direction_t& direction) const noexcept {
    direction = direction_hw_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::SetOutputModeImpl(hf_gpio_output_mode_t mode) noexcept {
    output_mode_hw_ = mode;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::GetOutputModeImpl(hf_gpio_output_mode_t& mode) const noexcept {
    mode = output_mode_hw_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::SetPullModeImpl(hf_gpio_pull_mode_t mode) noexcept {
    pull_mode_hw_ = mode;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::SetPinLevelImpl(hf_gpio_level_t level) noexcept {
    if (fail_writes_) {
        return hf_gpio_err_t::GPIO_ERR_HARDWARE_FAULT;
    }
    driven_level_ = level;
    history_.push_back(level);
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t MockGpio::GetPinLevelImpl(hf_gpio_level_t& level) noexcept {
    if (fail_reads_) {
        return hf_gpio_err_t::GPIO_ERR_HARDWARE_FAULT;
    }
    level = (direction_hw_ == hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) ? driven_level_
                                                                             : input_level_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

}  // namespace hf::host
