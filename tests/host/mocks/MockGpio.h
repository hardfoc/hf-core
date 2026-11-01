/**
 * @file MockGpio.h
 * @brief Scriptable BaseGpio fake for host tests.
 *
 * @details
 * Levels are electrical (HIGH/LOW); BaseGpio applies the active-high /
 * active-low mapping on top, exactly as on target. An output pin reads back
 * the level last driven; an input pin reads the level set with
 * SetInputLevel(). Every driven level is appended to LevelHistory(), so a
 * test can check reset pulses and chip-select sequences. FireInterrupt()
 * invokes the callback registered through ConfigureInterrupt().
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseGpio.h"

#include <cstdint>
#include <vector>

namespace hf::host {

class MockGpio : public BaseGpio {
public:
    explicit MockGpio(hf_pin_num_t pin = 0,
                      hf_gpio_direction_t direction = hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                      hf_gpio_active_state_t active_state = hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH) noexcept;
    ~MockGpio() noexcept override = default;

    // ── BaseGpio ──────────────────────────────────────────────────────────
    bool Initialize() noexcept override;
    [[nodiscard]] bool IsPinAvailable() const noexcept override { return true; }
    [[nodiscard]] hf_u8_t GetMaxPins() const noexcept override { return 64; }
    [[nodiscard]] const char* GetDescription() const noexcept override { return "MockGpio"; }
    [[nodiscard]] hf_gpio_err_t SupportsInterrupts() const noexcept override {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }
    hf_gpio_err_t ConfigureInterrupt(hf_gpio_interrupt_trigger_t trigger,
                                     InterruptCallback callback = nullptr,
                                     void* user_data = nullptr) noexcept override;
    hf_gpio_err_t EnableInterrupt() noexcept override;
    hf_gpio_err_t DisableInterrupt() noexcept override;

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetInputLevel(hf_gpio_level_t level) noexcept { input_level_ = level; }
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    /** Make level reads (true) or writes (false) fail with GPIO_ERR_HARDWARE_FAULT. */
    void FailReads(bool fail = true) noexcept { fail_reads_ = fail; }
    void FailWrites(bool fail = true) noexcept { fail_writes_ = fail; }
    /** Call the registered interrupt callback, if interrupts are enabled. Returns true if called. */
    bool FireInterrupt() noexcept;

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] hf_gpio_level_t DrivenLevel() const noexcept { return driven_level_; }
    [[nodiscard]] const std::vector<hf_gpio_level_t>& LevelHistory() const noexcept { return history_; }
    void ClearHistory() noexcept { history_.clear(); }
    [[nodiscard]] hf_gpio_pull_mode_t PullMode() const noexcept { return pull_mode_hw_; }
    [[nodiscard]] hf_gpio_interrupt_trigger_t InterruptTrigger() const noexcept { return trigger_; }
    [[nodiscard]] bool InterruptEnabled() const noexcept { return irq_enabled_; }

protected:
    hf_gpio_err_t SetDirectionImpl(hf_gpio_direction_t direction) noexcept override;
    hf_gpio_err_t GetDirectionImpl(hf_gpio_direction_t& direction) const noexcept override;
    hf_gpio_err_t SetOutputModeImpl(hf_gpio_output_mode_t mode) noexcept override;
    hf_gpio_err_t GetOutputModeImpl(hf_gpio_output_mode_t& mode) const noexcept override;
    hf_gpio_err_t SetPullModeImpl(hf_gpio_pull_mode_t mode) noexcept override;
    hf_gpio_pull_mode_t GetPullModeImpl() const noexcept override { return pull_mode_hw_; }
    hf_gpio_err_t SetPinLevelImpl(hf_gpio_level_t level) noexcept override;
    hf_gpio_err_t GetPinLevelImpl(hf_gpio_level_t& level) noexcept override;

private:
    bool init_result_{true};
    bool fail_reads_{false};
    bool fail_writes_{false};
    hf_gpio_direction_t direction_hw_;
    hf_gpio_output_mode_t output_mode_hw_{hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL};
    hf_gpio_pull_mode_t pull_mode_hw_{hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING};
    hf_gpio_level_t driven_level_{hf_gpio_level_t::HF_GPIO_LEVEL_LOW};
    hf_gpio_level_t input_level_{hf_gpio_level_t::HF_GPIO_LEVEL_LOW};
    std::vector<hf_gpio_level_t> history_;
    hf_gpio_interrupt_trigger_t trigger_{hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_NONE};
    InterruptCallback callback_{nullptr};
    void* callback_user_data_{nullptr};
    bool irq_enabled_{false};
};

}  // namespace hf::host
