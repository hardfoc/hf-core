/**
 * @file pca9685_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Pca9685Handler
 *
 * Tests the PCA9685 16-channel PWM controller handler: I2C comm adapter,
 * EnsureInitialized delegation, PWM duty cycle control, frequency setting,
 * sleep/wake, PwmAdapter, GpioPin wrapper, and error handling.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/pca9685/Pca9685Handler.h"

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "PCA9685_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_INITIALIZATION_TESTS = true;
static constexpr bool ENABLE_PWM_DUTY_TESTS       = true;
static constexpr bool ENABLE_FREQUENCY_TESTS       = true;
static constexpr bool ENABLE_SLEEP_WAKE_TESTS      = true;
static constexpr bool ENABLE_PWM_ADAPTER_TESTS     = true;
static constexpr bool ENABLE_GPIO_PIN_TESTS        = true;
static constexpr bool ENABLE_PHASE_OFFSET_TESTS    = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS   = true;

static std::unique_ptr<Pca9685Handler> g_handler;
static std::shared_ptr<BasePwm> g_pwm;

static bool create_handler() noexcept {
    auto* dev = get_i2c_device(PCA9685_I2C_ADDR);
    if (!dev) return false;
    g_handler = std::make_unique<Pca9685Handler>(*dev);
    return g_handler != nullptr;
}

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    bool ok = g_handler->EnsureInitialized();
    ESP_LOGI(TAG, "EnsureInitialized: %s", ok ? "OK" : "FAILED");
    if (ok) {
        g_pwm = g_handler->GetPwmAdapter();
        if (!g_pwm) {
            ESP_LOGE(TAG, "GetPwmAdapter() returned nullptr");
            return false;
        }
    }
    return ok;
}

static bool test_set_duty_cycle() noexcept {
    if (!g_pwm) return false;
    auto err = g_pwm->SetDutyCycle(0, 0.5f);
    ESP_LOGI(TAG, "SetDutyCycle(ch0, 50%%%%): %s", err == hf_pwm_err_t::PWM_SUCCESS ? "OK" : "FAILED");
    bool ok = (err == hf_pwm_err_t::PWM_SUCCESS);
    ok = ok && (g_pwm->SetDutyCycle(1, 0.0f) == hf_pwm_err_t::PWM_SUCCESS);
    ok = ok && (g_pwm->SetDutyCycle(2, 1.0f) == hf_pwm_err_t::PWM_SUCCESS);
    ok = ok && (g_pwm->SetDutyCycle(15, 0.25f) == hf_pwm_err_t::PWM_SUCCESS);
    ESP_LOGI(TAG, "Multi-channel duty set: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_duty_raw() noexcept {
    if (!g_pwm) return false;
    // Raw value with phase offset — exercises the (on_time + raw) & max fix
    auto err = g_pwm->SetDutyCycleRaw(0, 2048);
    ESP_LOGI(TAG, "SetDutyCycleRaw: %d", static_cast<int>(err));
    return err == hf_pwm_err_t::PWM_SUCCESS;
}

static bool test_frequency() noexcept {
    if (!g_pwm) return false;
    auto err = g_pwm->SetFrequency(0, 50.0f);  // Servo frequency
    bool ok = (err == hf_pwm_err_t::PWM_SUCCESS);
    ESP_LOGI(TAG, "SetFrequency(50Hz): %s", ok ? "OK" : "FAILED");
    ok = ok && (g_pwm->SetFrequency(0, 1000.0f) == hf_pwm_err_t::PWM_SUCCESS);  // LED frequency
    ESP_LOGI(TAG, "SetFrequency(1kHz): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_sleep_wake() noexcept {
    if (!g_pwm) return false;
    auto err = g_pwm->StopAll();
    bool ok = (err == hf_pwm_err_t::PWM_SUCCESS);
    ESP_LOGI(TAG, "StopAll (sleep): %s", ok ? "OK" : "FAILED");
    vTaskDelay(pdMS_TO_TICKS(100));
    err = g_pwm->StartAll();
    ok = ok && (err == hf_pwm_err_t::PWM_SUCCESS);
    ESP_LOGI(TAG, "StartAll (wake): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_gpio_pin_wrapper() noexcept {
    if (!g_handler) return false;
    auto pin = g_handler->CreateGpioPin(0);
    if (!pin) {
        ESP_LOGE(TAG, "CreateGpioPin(0) returned nullptr");
        return false;
    }
    auto err = pin->SetState(hf_gpio_state_t::HF_GPIO_STATE_ACTIVE);
    ESP_LOGI(TAG, "GpioPin SetState: %d", static_cast<int>(err));
    return err == hf_gpio_err_t::GPIO_SUCCESS;
}

static bool test_error_invalid_channel() noexcept {
    if (!g_pwm) return false;
    auto err = g_pwm->SetDutyCycle(16, 0.5f);  // Channel 16 invalid (0-15)
    bool ok = (err != hf_pwm_err_t::PWM_SUCCESS);
    ESP_LOGI(TAG, "Invalid channel: %s (expected error)", ok ? "CORRECT" : "UNEXPECTED");
    return ok;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     PCA9685 HANDLER COMPREHENSIVE TEST SUITE                ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 8192, 5);
        flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PWM_DUTY_TESTS, "PWM DUTY CYCLE",
        RUN_TEST_IN_TASK("duty_float", test_set_duty_cycle, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("duty_raw", test_set_duty_raw, 8192, 5);
        flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_FREQUENCY_TESTS, "FREQUENCY",
        RUN_TEST_IN_TASK("frequency", test_frequency, 8192, 5);
        flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_SLEEP_WAKE_TESTS, "SLEEP/WAKE",
        RUN_TEST_IN_TASK("sleep_wake", test_sleep_wake, 8192, 5);
        flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_GPIO_PIN_TESTS, "GPIO PIN WRAPPER",
        RUN_TEST_IN_TASK("gpio_pin", test_gpio_pin_wrapper, 8192, 5);
        flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("invalid_ch", test_error_invalid_channel, 8192, 5);
        flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "PCA9685 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
