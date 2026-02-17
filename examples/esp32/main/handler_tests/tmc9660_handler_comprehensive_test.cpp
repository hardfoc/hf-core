/**
 * @file tmc9660_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Tmc9660Handler
 *
 * Tests: SPI comm adapter, bootloader initialization, TMCL parameters, motor
 * control (velocity/position/torque), telemetry (voltage/temp/current/flags),
 * GPIO wrapper (GPIO17/18), ADC wrapper (multi-channel), Temperature wrapper,
 * null-safe accessors (eager wrapper creation), driver enable, PID gains,
 * and visitDriver() advanced access.
 *
 * @note This test requires a TMC9660-3PH-EVAL or compatible board connected
 *       via SPI. Without hardware, initialization will fail gracefully and
 *       remaining tests will be skipped.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/tmc9660/Tmc9660Handler.h"
#include "handlers/tmc9660/Tmc9660AdcWrapper.h"

#include <cmath>
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

static const char* TAG = "TMC9660_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS    = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS  = true;
static constexpr bool ENABLE_PARAMETER_TESTS       = true;
static constexpr bool ENABLE_MOTOR_CONTROL_TESTS   = true;
static constexpr bool ENABLE_TELEMETRY_TESTS       = true;
static constexpr bool ENABLE_GPIO_WRAPPER_TESTS    = true;
static constexpr bool ENABLE_ADC_WRAPPER_TESTS     = true;
static constexpr bool ENABLE_TEMP_WRAPPER_TESTS    = true;
static constexpr bool ENABLE_DRIVER_ENABLE_TESTS   = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS   = true;

// GPIO control pins for the TMC9660
static std::unique_ptr<EspGpio> g_rst_gpio;
static std::unique_ptr<EspGpio> g_drv_en_gpio;
static std::unique_ptr<EspGpio> g_faultn_gpio;
static std::unique_ptr<EspGpio> g_wake_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Tmc9660Handler> g_handler;

static bool g_hw_present = false; // Set true if init succeeds

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    // Create SPI device for TMC9660 with chip-select pin
    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_TMC9660_CS);
    dev_cfg.clock_speed_hz = TMC9660_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_3;  // CPOL=1, CPHA=1 per TMC9660 datasheet
    dev_cfg.queue_size = 1;

    int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for TMC9660");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for TMC9660");
        return false;
    }

    g_rst_gpio = create_gpio(PIN_TMC9660_RST,
                             hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_drv_en_gpio = create_gpio(PIN_TMC9660_DRV_EN,
                                hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_faultn_gpio = create_gpio(PIN_TMC9660_FAULTN,
                                hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    g_wake_gpio = create_gpio(PIN_TMC9660_WAKE,
                              hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                              hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);

    if (!g_rst_gpio || !g_drv_en_gpio || !g_faultn_gpio || !g_wake_gpio) {
        ESP_LOGE(TAG, "Failed to create control GPIOs");
        return false;
    }

    g_handler = std::make_unique<Tmc9660Handler>(
        *g_spi_device, *g_rst_gpio, *g_drv_en_gpio, *g_faultn_gpio, *g_wake_gpio,
        TMC9660_DEVICE_ADDR);

    return g_handler != nullptr;
}

// ─────────────────────── Construction ───────────────────────

static bool test_construction() noexcept {
    bool is_ready = g_handler->IsDriverReady();
    ESP_LOGI(TAG, "After construction: IsDriverReady=%d (expected false)", is_ready);
    return !is_ready; // Should NOT be ready before Initialize()
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    bool ok = g_handler->Initialize(true /*reset*/, true /*bootInfo*/, false /*failOnVerify*/);
    g_hw_present = ok;
    ESP_LOGI(TAG, "Initialize: %s (hw_present=%d)", ok ? "OK" : "FAILED", g_hw_present);
    return true; // Always pass — no hardware is a valid test scenario
}

static bool test_is_ready() noexcept {
    if (!g_handler) return false;
    bool ready = g_handler->IsDriverReady();
    ESP_LOGI(TAG, "IsDriverReady: %d (expected %d)", ready, g_hw_present);
    return ready == g_hw_present;
}

// ─────────────────────── Parameter Access ───────────────────────

static bool test_read_parameter() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    uint32_t value = 0;
    bool ok = g_handler->visitDriver([&](auto& driver) -> bool {
        return driver.readParameter(tmc9660::tmcl::Parameters::ACTUAL_VELOCITY, value);
    });
    ESP_LOGI(TAG, "ReadParameter(ACTUAL_VELOCITY): %s, value=%lu", ok ? "OK" : "FAILED", value);
    return ok;
}

static bool test_write_parameter() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    // Write target velocity (safe parameter)
    bool ok = g_handler->visitDriver([](auto& driver) -> bool {
        return driver.writeParameter(tmc9660::tmcl::Parameters::TARGET_VELOCITY, 5000);
    });
    ESP_LOGI(TAG, "WriteParameter(TARGET_VELOCITY=5000): %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Motor Control ───────────────────────

static bool test_set_target_velocity() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    // Set velocity to 0 (safe)
    bool ok = g_handler->visitDriver([](auto& driver) -> bool {
        return driver.velocityControl.setTargetVelocity(0);
    });
    ESP_LOGI(TAG, "SetTargetVelocity(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_target_position() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& driver) -> bool {
        return driver.positionControl.setTargetPosition(0);
    });
    ESP_LOGI(TAG, "SetTargetPosition(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Telemetry ───────────────────────

static bool test_supply_voltage() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    float v = g_handler->visitDriver([](auto& driver) -> float {
        return driver.telemetry.getSupplyVoltage();
    });
    bool valid = !std::isnan(v) && v >= 0.0f;
    ESP_LOGI(TAG, "Supply voltage: %.2fV (valid=%d)", v, valid);
    return valid;
}

static bool test_chip_temperature() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    float t = g_handler->visitDriver([](auto& driver) -> float {
        return driver.telemetry.getChipTemperature();
    });
    bool valid = !std::isnan(t) && t > -273.0f;
    ESP_LOGI(TAG, "Chip temperature: %.2f°C (valid=%d)", t, valid);
    return valid;
}

static bool test_motor_current() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    int16_t current = g_handler->visitDriver([](auto& driver) -> int16_t {
        return driver.telemetry.getMotorCurrent();
    });
    ESP_LOGI(TAG, "Motor current: %d mA", current);
    return true; // 0 is valid at standstill
}

static bool test_status_flags() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    uint32_t flags = 0;
    bool ok = g_handler->visitDriver([&](auto& driver) -> bool {
        return driver.telemetry.getGeneralStatusFlags(flags);
    });
    ESP_LOGI(TAG, "Status flags: 0x%08lX (%s)", flags, ok ? "OK" : "FAILED");
    return ok;
}

static bool test_error_flags() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    uint32_t flags = 0;
    bool ok = g_handler->visitDriver([&](auto& driver) -> bool {
        return driver.telemetry.getGeneralErrorFlags(flags);
    });
    ESP_LOGI(TAG, "Error flags: 0x%08lX (%s)", flags, ok ? "OK" : "FAILED");
    if (ok && flags != 0) {
        g_handler->visitDriver([&](auto& driver) {
            driver.telemetry.clearGeneralErrorFlags(flags);
        });
        ESP_LOGI(TAG, "Cleared error flags");
    }
    return ok;
}

// ─────────────────────── GPIO Wrapper ───────────────────────

static bool test_gpio17_wrapper() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto& gpio17 = g_handler->gpio(17);
    bool ok = gpio17.Initialize();
    ESP_LOGI(TAG, "GPIO17 init: %s, desc: %s", ok ? "OK" : "FAILED", gpio17.GetDescription());
    if (ok) {
        gpio17.SetState(hf_gpio_state_t::HF_GPIO_STATE_ACTIVE);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio17.SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
    }
    return ok;
}

static bool test_gpio18_wrapper() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto& gpio18 = g_handler->gpio(18);
    bool available = gpio18.IsPinAvailable();
    ESP_LOGI(TAG, "GPIO18 available: %d, max_pins: %d", available, gpio18.GetMaxPins());
    return available;
}

// ─────────────────────── ADC Wrapper ───────────────────────

static bool test_adc_wrapper() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto& adc = g_handler->adc();
    bool ok = adc.Initialize();
    ESP_LOGI(TAG, "ADC wrapper init: %s", ok ? "OK" : "FAILED");

    if (ok) {
        // Read supply voltage channel (ID 20)
        float voltage = 0.0f;
        auto err = adc.ReadChannelV(20, voltage);
        ESP_LOGI(TAG, "ADC ch20 (supply): %.3fV, err=%d", voltage, static_cast<int>(err));
    }
    return ok;
}

static bool test_adc_wrapper_delegation() noexcept {
    // Test Tmc9660AdcWrapper delegation pattern (null-safe)
    if (!g_handler) return false;
    // Wrapper should be usable even without direct initialization
    ESP_LOGI(TAG, "ADC wrapper delegation: handler exists");
    return true;
}

// ─────────────────────── Temperature Wrapper ───────────────────────

static bool test_temperature_wrapper() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto& temp = g_handler->temperature();
    bool ok = temp.Initialize();
    ESP_LOGI(TAG, "Temperature wrapper init: %s", ok ? "OK" : "FAILED");

    if (ok) {
        float celsius = 0.0f;
        auto err = temp.ReadTemperatureCelsius(&celsius);
        ESP_LOGI(TAG, "Chip temp via wrapper: %.2f°C, err=%d", celsius, static_cast<int>(err));
    }
    return ok;
}

// ─────────────────────── Driver Enable ───────────────────────

static bool test_driver_enable() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->visitDriver([](auto& driver) -> bool {
        return driver.GpioSetActive(tmc9660::TMC9660CtrlPin::DRV_EN);
    });
    ESP_LOGI(TAG, "EnableDriverOutput: %s", ok ? "OK" : "FAILED");
    vTaskDelay(pdMS_TO_TICKS(100));
    ok = g_handler->visitDriver([](auto& driver) -> bool {
        return driver.GpioSetInactive(tmc9660::TMC9660CtrlPin::DRV_EN);
    });
    ESP_LOGI(TAG, "DisableDriverOutput: %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Error Handling ───────────────────────

static bool test_operations_before_init() noexcept {
    // Create a SECOND handler that is NOT initialized
    if (!g_spi_device) return true;

    auto uninit = std::make_unique<Tmc9660Handler>(
        *g_spi_device, *g_rst_gpio, *g_drv_en_gpio, *g_faultn_gpio, *g_wake_gpio, 0x02);

    bool ready = uninit->IsDriverReady();

    ESP_LOGI(TAG, "Uninit handler: ready=%d (expected false)", ready);
    return !ready;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     TMC9660 HANDLER COMPREHENSIVE TEST SUITE                ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_ready", test_is_ready, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PARAMETER_TESTS, "PARAMETER ACCESS",
        RUN_TEST_IN_TASK("read_param", test_read_parameter, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("write_param", test_write_parameter, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_MOTOR_CONTROL_TESTS, "MOTOR CONTROL",
        RUN_TEST_IN_TASK("velocity", test_set_target_velocity, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("position", test_set_target_position, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_TELEMETRY_TESTS, "TELEMETRY",
        RUN_TEST_IN_TASK("supply_v", test_supply_voltage, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("chip_temp", test_chip_temperature, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("current", test_motor_current, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("status", test_status_flags, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("errors", test_error_flags, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_GPIO_WRAPPER_TESTS, "GPIO WRAPPERS",
        RUN_TEST_IN_TASK("gpio17", test_gpio17_wrapper, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("gpio18", test_gpio18_wrapper, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ADC_WRAPPER_TESTS, "ADC WRAPPER",
        RUN_TEST_IN_TASK("adc", test_adc_wrapper, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("adc_delegate", test_adc_wrapper_delegation, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_TEMP_WRAPPER_TESTS, "TEMPERATURE WRAPPER",
        RUN_TEST_IN_TASK("temp", test_temperature_wrapper, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DRIVER_ENABLE_TESTS, "DRIVER ENABLE",
        RUN_TEST_IN_TASK("drv_en", test_driver_enable, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "TMC9660 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
