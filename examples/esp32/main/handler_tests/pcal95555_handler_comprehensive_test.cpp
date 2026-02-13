/**
 * @file pcal95555_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Pcal95555Handler
 *
 * Tests: I2C comm, EnsureInitialized, per-pin direction/read/write/toggle,
 * batch 16-bit operations, interrupt management (deferred ISR), chip variant
 * detection (PCA9555 vs PCAL9555A), Agile I/O features, and GpioPin wrapper.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/pcal95555/Pcal95555Handler.h"

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

static const char* TAG = "PCAL95555_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_INITIALIZATION_TESTS   = true;
static constexpr bool ENABLE_PIN_DIRECTION_TESTS     = true;
static constexpr bool ENABLE_PIN_READ_WRITE_TESTS    = true;
static constexpr bool ENABLE_BATCH_OPERATIONS_TESTS  = true;
static constexpr bool ENABLE_INTERRUPT_TESTS         = true;
static constexpr bool ENABLE_CHIP_VARIANT_TESTS      = true;
static constexpr bool ENABLE_AGILE_IO_TESTS          = true;
static constexpr bool ENABLE_GPIO_PIN_WRAPPER_TESTS  = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS     = true;

static std::unique_ptr<EspGpio> g_int_gpio;
static std::unique_ptr<Pcal95555Handler> g_handler;

static bool create_handler() noexcept {
    auto* dev = get_i2c_device(PCAL95555_I2C_ADDR);
    if (!dev) return false;

    g_int_gpio = create_gpio(PIN_PCAL95555_INT,
                             hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);

    g_handler = std::make_unique<Pcal95555Handler>(*dev, g_int_gpio.get());
    return g_handler != nullptr;
}

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    bool ok = g_handler->EnsureInitialized();
    ESP_LOGI(TAG, "EnsureInitialized: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_chip_variant() noexcept {
    if (!g_handler) return false;
    auto variant = g_handler->GetChipVariant();
    bool has_agile = g_handler->HasAgileIO();
    ESP_LOGI(TAG, "Chip variant: %d, HasAgileIO: %s",
             static_cast<int>(variant), has_agile ? "YES" : "NO");
    return true;  // Both PCA9555 and PCAL9555A are valid
}

static bool test_pin_direction() noexcept {
    if (!g_handler) return false;
    // Set pin 0 as output
    auto err = g_handler->SetDirection(0, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) return false;
    // Set pin 1 as input
    err = g_handler->SetDirection(1, hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT);
    ESP_LOGI(TAG, "Pin direction set: %d", static_cast<int>(err));
    return err == hf_gpio_err_t::GPIO_SUCCESS;
}

static bool test_pin_write_read() noexcept {
    if (!g_handler) return false;
    // Write HIGH to pin 0 (configured as output above)
    auto err = g_handler->SetOutput(0, true);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) return false;
    vTaskDelay(pdMS_TO_TICKS(10));
    // Read pin 1
    bool active = false;
    err = g_handler->ReadInput(1, active);
    ESP_LOGI(TAG, "Pin1 level: %d", static_cast<int>(active));
    return err == hf_gpio_err_t::GPIO_SUCCESS;
}

static bool test_toggle_pin() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->Toggle(0);
    ESP_LOGI(TAG, "Toggle(0): %d", static_cast<int>(err));
    return err == hf_gpio_err_t::GPIO_SUCCESS;
}

static bool test_batch_write() noexcept {
    if (!g_handler) return false;
    // Set pins 0-3 as outputs, write 0x0F
    g_handler->SetDirections(0x000F, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    auto err = g_handler->SetOutputs(0x000F, true);  // mask=low nibble, all high
    ESP_LOGI(TAG, "Batch write: %d", static_cast<int>(err));
    return err == hf_gpio_err_t::GPIO_SUCCESS;
}

static bool test_supports_interrupts() noexcept {
    if (!g_handler) return false;
    auto pin = g_handler->GetGpioPin(0);
    if (!pin) return false;
    auto err = pin->SupportsInterrupts();
    bool expected_agile = g_handler->HasAgileIO();
    bool correct = (expected_agile && err == hf_gpio_err_t::GPIO_SUCCESS) ||
                   (!expected_agile && err == hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION);
    ESP_LOGI(TAG, "SupportsInterrupts: %d (HasAgileIO=%d, correct=%d)",
             static_cast<int>(err), expected_agile, correct);
    return correct;
}

static bool test_drain_pending_interrupts() noexcept {
    if (!g_handler) return false;
    // DrainPendingInterrupts should be safe to call even without pending interrupts
    g_handler->DrainPendingInterrupts();
    ESP_LOGI(TAG, "DrainPendingInterrupts completed without crash");
    return true;
}

static bool test_gpio_pin_wrapper() noexcept {
    if (!g_handler) return false;
    auto pin = g_handler->GetGpioPin(5);
    if (!pin) return false;
    bool ok = pin->Initialize();
    ESP_LOGI(TAG, "GpioPin(5) init: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_error_invalid_pin() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->SetOutput(16, true);
    bool correct = (err != hf_gpio_err_t::GPIO_SUCCESS);
    ESP_LOGI(TAG, "Invalid pin 16: %s", correct ? "CORRECT" : "UNEXPECTED");
    return correct;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     PCAL95555 HANDLER COMPREHENSIVE TEST SUITE              ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CHIP_VARIANT_TESTS, "CHIP VARIANT",
        RUN_TEST_IN_TASK("variant", test_chip_variant, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PIN_DIRECTION_TESTS, "PIN DIRECTION",
        RUN_TEST_IN_TASK("direction", test_pin_direction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PIN_READ_WRITE_TESTS, "PIN READ/WRITE",
        RUN_TEST_IN_TASK("write_read", test_pin_write_read, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("toggle", test_toggle_pin, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_BATCH_OPERATIONS_TESTS, "BATCH OPERATIONS",
        RUN_TEST_IN_TASK("batch_write", test_batch_write, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INTERRUPT_TESTS, "INTERRUPT MANAGEMENT",
        RUN_TEST_IN_TASK("supports_int", test_supports_interrupts, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("drain_int", test_drain_pending_interrupts, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_GPIO_PIN_WRAPPER_TESTS, "GPIO PIN WRAPPER",
        RUN_TEST_IN_TASK("pin_wrap", test_gpio_pin_wrapper, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("invalid_pin", test_error_invalid_pin, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "PCAL95555 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
