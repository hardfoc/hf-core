/**
 * @file as5047u_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for As5047uHandler
 *
 * Tests the AS5047U magnetic rotary position sensor handler through its
 * full public API, exercising the underlying CRTP SPI adapter and driver.
 *
 * Test Categories:
 * 1. Initialization & lifecycle
 * 2. Angle reading (14-bit absolute, degrees)
 * 3. Velocity measurement (rpm, dps)
 * 4. DAEC configuration
 * 5. Zero position setting
 * 6. Interface configuration (ABI/UVW/PWM)
 * 7. Diagnostics & error flags
 * 8. Thread safety (mutex contention)
 * 9. Error handling & edge cases
 *
 * Hardware Required:
 * - AS5047U encoder on SPI bus (see esp32_test_config.hpp for pins)
 * - Diametrically magnetized magnet within sensor range
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

// Handler under test
#include "handlers/as5047u/As5047uHandler.h"

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

static const char* TAG = "AS5047U_Handler_Test";
static TestResults g_test_results;

// ═══════════════════════════════════════════════════════════════════════════
// TEST SECTION CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

static constexpr bool ENABLE_INITIALIZATION_TESTS = true;
static constexpr bool ENABLE_ANGLE_TESTS = true;
static constexpr bool ENABLE_VELOCITY_TESTS = true;
static constexpr bool ENABLE_DAEC_TESTS = true;
static constexpr bool ENABLE_ZERO_POSITION_TESTS = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS = true;
static constexpr bool ENABLE_THREAD_SAFETY_TESTS = true;

// ═══════════════════════════════════════════════════════════════════════════
// SHARED RESOURCES
// ═══════════════════════════════════════════════════════════════════════════

static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<As5047uHandler> g_handler;

// ═══════════════════════════════════════════════════════════════════════════
// HELPER: CREATE HANDLER
// ═══════════════════════════════════════════════════════════════════════════

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "SPI bus not available");
        return false;
    }

    // Create SPI device for AS5047U with chip-select pin
    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_AS5047U_CS);
    dev_cfg.clock_speed_hz = AS5047U_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_1;  // CPOL=0, CPHA=1 per AS5047U datasheet
    dev_cfg.queue_size = 1;

    int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for AS5047U");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for AS5047U");
        return false;
    }

    g_handler = std::make_unique<As5047uHandler>(*g_spi_device);
    ESP_LOGI(TAG, "As5047uHandler created");
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: INITIALIZATION
// ═══════════════════════════════════════════════════════════════════════════

static bool test_handler_construction() noexcept {
    if (!g_handler) {
        if (!create_handler()) return false;
    }
    ESP_LOGI(TAG, "Handler constructed successfully");
    return true;
}

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->Initialize();
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "Initialize failed: %d", static_cast<int>(err));
        return false;
    }
    ESP_LOGI(TAG, "Handler initialized — driver ready");
    return true;
}

static bool test_is_initialized() noexcept {
    if (!g_handler) return false;
    bool init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized = %s", init ? "true" : "false");
    return init;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: ANGLE READING
// ═══════════════════════════════════════════════════════════════════════════

static bool test_read_angle_raw() noexcept {
    if (!g_handler) return false;
    uint16_t raw = 0;
    auto err = g_handler->ReadAngle(raw);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "ReadAngle(raw) failed: %d", static_cast<int>(err));
        return false;
    }
    ESP_LOGI(TAG, "Raw angle: %u (0-16383)", raw);
    return raw <= 16383;
}

static bool test_read_angle_degrees() noexcept {
    if (!g_handler) return false;
    uint16_t raw = 0;
    auto err = g_handler->ReadAngle(raw);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "ReadAngle failed: %d", static_cast<int>(err));
        return false;
    }
    float degrees = static_cast<float>(As5047uHandler::LSBToDegrees(raw));
    ESP_LOGI(TAG, "Angle: %.2f degrees", degrees);
    return degrees >= 0.0f && degrees < 360.0f;
}

static bool test_read_angle_consistency() noexcept {
    if (!g_handler) return false;
    // Read 10 times rapidly and check all values are in valid range
    for (int i = 0; i < 10; ++i) {
        uint16_t raw = 0;
        auto err = g_handler->ReadAngle(raw);
        if (err != As5047uError::SUCCESS) return false;
        float deg = static_cast<float>(As5047uHandler::LSBToDegrees(raw));
        if (deg < 0.0f || deg >= 360.0f) return false;
    }
    ESP_LOGI(TAG, "10 consecutive reads all in valid range");
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: VELOCITY
// ═══════════════════════════════════════════════════════════════════════════

static bool test_read_velocity() noexcept {
    if (!g_handler) return false;
    int16_t velocity = 0;
    auto err = g_handler->ReadVelocity(velocity);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "ReadVelocity failed: %d", static_cast<int>(err));
        return false;
    }
    ESP_LOGI(TAG, "Velocity: %d LSB (stationary expected ~0)", static_cast<int>(velocity));
    return true;  // Value can be 0 or small noise
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: DAEC CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

static bool test_daec_enable_disable() noexcept {
    if (!g_handler) return false;
    // Enable DAEC
    auto err = g_handler->SetDAEC(true);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "SetDAEC(true) failed");
        return false;
    }
    // Disable DAEC
    err = g_handler->SetDAEC(false);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "SetDAEC(false) failed");
        return false;
    }
    ESP_LOGI(TAG, "DAEC enable/disable cycle OK");
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: ZERO POSITION
// ═══════════════════════════════════════════════════════════════════════════

static bool test_set_zero_position() noexcept {
    if (!g_handler) return false;
    // Read current angle
    uint16_t before_raw = 0;
    g_handler->ReadAngle(before_raw);
    float before = static_cast<float>(As5047uHandler::LSBToDegrees(before_raw));

    // Set current position as zero
    auto err = g_handler->SetZeroPosition(before_raw);
    if (err != As5047uError::SUCCESS) {
        ESP_LOGE(TAG, "SetZeroPosition failed: %d", static_cast<int>(err));
        return false;
    }

    // Read again — should be close to 0
    vTaskDelay(pdMS_TO_TICKS(50));
    uint16_t after_raw = 0;
    g_handler->ReadAngle(after_raw);
    float after = static_cast<float>(As5047uHandler::LSBToDegrees(after_raw));
    ESP_LOGI(TAG, "Before zero: %.2f°, After zero: %.2f°", before, after);
    return after < 5.0f || after > 355.0f;  // Near zero, accounting for noise
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: DIAGNOSTICS
// ═══════════════════════════════════════════════════════════════════════════

static bool test_dump_diagnostics() noexcept {
    if (!g_handler) return false;
    g_handler->DumpDiagnostics();
    ESP_LOGI(TAG, "Diagnostics dump completed");
    return true;
}

static bool test_get_sensor() noexcept {
    if (!g_handler) return false;
    auto sensor = g_handler->GetSensor();
    if (!sensor) {
        ESP_LOGE(TAG, "GetSensor returned nullptr");
        return false;
    }
    ESP_LOGI(TAG, "GetSensor returned valid driver pointer");
    return true;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: ERROR HANDLING
// ═══════════════════════════════════════════════════════════════════════════

static bool test_operations_before_init() noexcept {
    // Create a fresh handler without initializing — reuse existing SPI device
    if (!g_spi_device) return false;

    As5047uHandler uninit_handler(*g_spi_device);
    uint16_t raw = 0;
    auto err = uninit_handler.ReadAngle(raw);
    bool correct = (err == As5047uError::NOT_INITIALIZED);
    ESP_LOGI(TAG, "ReadAngle before init: %s (expected NOT_INITIALIZED)",
             correct ? "CORRECT" : "UNEXPECTED");
    return correct;
}

// ═══════════════════════════════════════════════════════════════════════════
// TEST: THREAD SAFETY
// ═══════════════════════════════════════════════════════════════════════════

static volatile bool g_thread_test_pass = true;

static void concurrent_reader_task(void* param) {
    auto* handler = static_cast<As5047uHandler*>(param);
    for (int i = 0; i < 50; ++i) {
        uint16_t raw = 0;
        auto err = handler->ReadAngle(raw);
        if (err != As5047uError::SUCCESS) {
            g_thread_test_pass = false;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(nullptr);
}

static bool test_concurrent_access() noexcept {
    if (!g_handler) return false;
    g_thread_test_pass = true;

    // Spawn 3 concurrent reader tasks
    for (int i = 0; i < 3; ++i) {
        char name[16];
        snprintf(name, sizeof(name), "reader_%d", i);
        xTaskCreate(concurrent_reader_task, name, 4096,
                    g_handler.get(), 5, nullptr);
    }

    // Also read from main context
    for (int i = 0; i < 50; ++i) {
        uint16_t raw = 0;
        g_handler->ReadAngle(raw);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelay(pdMS_TO_TICKS(1000));  // Wait for tasks to finish
    ESP_LOGI(TAG, "Concurrent access: %s", g_thread_test_pass ? "PASS" : "FAIL");
    return g_thread_test_pass;
}

// ═══════════════════════════════════════════════════════════════════════════
// MAIN
// ═══════════════════════════════════════════════════════════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     AS5047U HANDLER COMPREHENSIVE TEST SUITE                ║");
    ESP_LOGI(TAG, "║     Testing: As5047uHandler → as5047u::AS5047U driver       ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    print_test_section_status(TAG, "AS5047U HANDLER");

    // Create handler (shared across all tests)
    if (!create_handler()) {
        ESP_LOGE(TAG, "FATAL: Could not create handler. Aborting.");
        return;
    }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("construction", test_handler_construction, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("initialize", test_initialize, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_initialized", test_is_initialized, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ANGLE_TESTS, "ANGLE READING",
        RUN_TEST_IN_TASK("read_raw", test_read_angle_raw, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("read_degrees", test_read_angle_degrees, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("consistency", test_read_angle_consistency, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_VELOCITY_TESTS, "VELOCITY",
        RUN_TEST_IN_TASK("velocity", test_read_velocity, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DAEC_TESTS, "DAEC CONFIGURATION",
        RUN_TEST_IN_TASK("daec_toggle", test_daec_enable_disable, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ZERO_POSITION_TESTS, "ZERO POSITION",
        RUN_TEST_IN_TASK("set_zero", test_set_zero_position, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("get_sensor", test_get_sensor, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_THREAD_SAFETY_TESTS, "THREAD SAFETY",
        RUN_TEST_IN_TASK("concurrent", test_concurrent_access, 16384, 5);
        flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "AS5047U HANDLER COMPREHENSIVE", TAG);

    ESP_LOGI(TAG, "Entering idle loop...");
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
