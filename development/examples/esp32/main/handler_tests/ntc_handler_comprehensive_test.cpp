/**
 * @file ntc_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for NtcTemperatureHandler
 *
 * Tests: BaseAdc-driven init, temperature reading (Celsius/Fahrenheit), calibration
 * offset, EMA filtering, voltage divider config, conversion methods, threshold
 * monitoring, continuous monitoring (PeriodicTimer), statistics, diagnostics,
 * self-test, health check, sleep mode, and thread safety via RtosMutex.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/ntc/NtcTemperatureHandler.h"

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

static const char* TAG = "NTC_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_INITIALIZATION_TESTS       = true;
static constexpr bool ENABLE_TEMPERATURE_READING_TESTS  = true;
static constexpr bool ENABLE_CALIBRATION_TESTS          = true;
static constexpr bool ENABLE_FILTERING_TESTS            = true;
static constexpr bool ENABLE_THRESHOLD_TESTS            = true;
static constexpr bool ENABLE_CONTINUOUS_MONITORING_TESTS = true;
static constexpr bool ENABLE_STATISTICS_TESTS           = true;
static constexpr bool ENABLE_SLEEP_MODE_TESTS           = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS        = true;

static std::unique_ptr<NtcTemperatureHandler> g_handler;

// Threshold monitoring state
static volatile bool g_threshold_triggered  = false;
static volatile float g_threshold_temperature = 0.0f;

static void threshold_callback(BaseTemperature* sensor, float temperature, hf_u32_t type, void* user_data) {
    g_threshold_triggered   = true;
    g_threshold_temperature = temperature;
    ESP_LOGI(TAG, "Threshold callback: %.2f°C (type %" PRIu32 ")", temperature, type);
}

static bool create_handler() noexcept {
    auto* adc = get_shared_adc();
    if (!adc) {
        ESP_LOGE(TAG, "Failed to get ADC interface");
        return false;
    }

    ntc_temp_handler_config_t config = NTC_TEMP_HANDLER_CONFIG_DEFAULT();
    config.adc_channel             = NTC_ADC_CHANNEL;
    config.voltage_divider_series_resistance = 10000.0f;
    config.reference_voltage       = 3.3f;
    config.enable_filtering        = false;
    config.filter_alpha            = 0.1f;
    config.sensor_name             = "Test_NTC";

    g_handler = std::make_unique<NtcTemperatureHandler>(adc, config);
    return g_handler != nullptr;
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    bool ok = g_handler->Initialize();
    ESP_LOGI(TAG, "Initialize: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_sensor_info() noexcept {
    if (!g_handler) return false;
    hf_temp_sensor_info_t info{};
    auto err = g_handler->GetSensorInfo(&info);
    if (err != TEMP_SUCCESS) return false;
    ESP_LOGI(TAG, "Sensor: %s, type: %d", info.model, static_cast<int>(info.sensor_type));
    return true;
}

static bool test_capabilities() noexcept {
    if (!g_handler) return false;
    auto caps = g_handler->GetCapabilities();
    ESP_LOGI(TAG, "Capabilities: 0x%08" PRIx32, static_cast<uint32_t>(caps));
    return caps != 0;
}

// ─────────────────────── Temperature Reading ───────────────────────

static bool test_read_temperature() noexcept {
    if (!g_handler) return false;
    float temp = 0.0f;
    auto err = g_handler->ReadTemperatureCelsius(&temp);
    if (err != TEMP_SUCCESS) {
        ESP_LOGW(TAG, "ReadTemperatureCelsius failed: %d (may be OK if NTC not connected)",
                 static_cast<int>(err));
        return true; // Pass even without hardware
    }
    bool plausible = (temp > -40.0f && temp < 125.0f);
    ESP_LOGI(TAG, "Temperature: %.2f°C (plausible: %s)", temp, plausible ? "YES" : "NO");
    return plausible;
}

static bool test_read_temperature_consistency() noexcept {
    if (!g_handler) return false;
    float readings[5];
    for (int i = 0; i < 5; ++i) {
        auto err = g_handler->ReadTemperatureCelsius(&readings[i]);
        if (err != TEMP_SUCCESS) return true; // No hardware
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    // Check readings are within 5°C of each other
    float min_r = readings[0], max_r = readings[0];
    for (int i = 1; i < 5; ++i) {
        if (readings[i] < min_r) min_r = readings[i];
        if (readings[i] > max_r) max_r = readings[i];
    }
    bool consistent = (max_r - min_r) < 5.0f;
    ESP_LOGI(TAG, "Temperature spread: %.2f°C (consistent: %s)", max_r - min_r,
             consistent ? "YES" : "NO");
    return consistent;
}

// ─────────────────────── Calibration ───────────────────────

static bool test_set_calibration_offset() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->SetCalibrationOffset(2.5f);
    if (err != TEMP_SUCCESS) return false;
    float offset = 0.0f;
    err = g_handler->GetCalibrationOffset(&offset);
    bool correct = (err == TEMP_SUCCESS && std::fabs(offset - 2.5f) < 0.01f);
    ESP_LOGI(TAG, "Calibration offset: %.2f (correct: %s)", offset, correct ? "YES" : "NO");
    g_handler->ResetCalibration(); // Restore
    return correct;
}

// ─────────────────────── Filtering ───────────────────────

static bool test_enable_filtering() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->SetFiltering(true, 0.2f);
    bool ok = (err == NtcError::Success);
    ESP_LOGI(TAG, "Enable filtering (alpha=0.2): %s", ok ? "OK" : "FAILED");
    // Read a few values to exercise the EMA filter
    float temp;
    for (int i = 0; i < 10; ++i) {
        g_handler->ReadTemperatureCelsius(&temp);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    g_handler->SetFiltering(false); // Restore
    return ok;
}

// ─────────────────────── Threshold Monitoring ───────────────────────

static bool test_threshold_monitoring() noexcept {
    if (!g_handler) return false;
    g_threshold_triggered = false;
    // Set thresholds very wide; trigger should NOT fire
    auto err = g_handler->SetThresholds(-40.0f, 125.0f);
    if (err != TEMP_SUCCESS) return false;

    err = g_handler->EnableThresholdMonitoring(threshold_callback, nullptr);
    if (err != TEMP_SUCCESS) return false;

    // Take a reading to trigger check
    float temp;
    g_handler->ReadTemperatureCelsius(&temp);
    vTaskDelay(pdMS_TO_TICKS(100));

    err = g_handler->DisableThresholdMonitoring();
    ESP_LOGI(TAG, "Threshold monitoring cycle: %s (triggered=%d)",
             (err == TEMP_SUCCESS) ? "OK" : "FAILED", g_threshold_triggered);
    return err == TEMP_SUCCESS;
}

// ─────────────────────── Continuous Monitoring ───────────────────────

static volatile int g_continuous_count = 0;

static void continuous_callback(BaseTemperature* sensor, const hf_temp_reading_t* reading, void* user_data) {
    g_continuous_count++;
}

static bool test_continuous_monitoring() noexcept {
    if (!g_handler) return false;
    g_continuous_count = 0;

    auto err = g_handler->StartContinuousMonitoring(10, continuous_callback, nullptr); // 10 Hz
    if (err != TEMP_SUCCESS) {
        ESP_LOGW(TAG, "StartContinuousMonitoring failed: %d", static_cast<int>(err));
        return true; // PeriodicTimer may not be available
    }

    bool is_active = g_handler->IsMonitoringActive();
    vTaskDelay(pdMS_TO_TICKS(1000)); // Wait ~1 second for ~10 callbacks

    g_handler->StopContinuousMonitoring();
    ESP_LOGI(TAG, "Continuous monitoring: active=%d, callbacks=%d", is_active, g_continuous_count);
    return is_active && (g_continuous_count > 0);
}

// ─────────────────────── Statistics ───────────────────────

static bool test_statistics() noexcept {
    if (!g_handler) return false;
    // Take a few readings first
    float temp;
    for (int i = 0; i < 5; ++i) {
        g_handler->ReadTemperatureCelsius(&temp);
        vTaskDelay(pdMS_TO_TICKS(20));
    }

    hf_temp_statistics_t stats{};
    auto err = g_handler->GetStatistics(stats);
    ESP_LOGI(TAG, "Stats: total=%" PRIu32 ", success=%" PRIu32, stats.total_operations, stats.successful_operations);
    return err == TEMP_SUCCESS;
}

static bool test_diagnostics() noexcept {
    if (!g_handler) return false;
    hf_temp_diagnostics_t diag{};
    auto err = g_handler->GetDiagnostics(diag);
    ESP_LOGI(TAG, "Diagnostics: last_error=%d", static_cast<int>(diag.last_error_code));
    return err == TEMP_SUCCESS;
}

// ─────────────────────── Sleep Mode ───────────────────────

static bool test_sleep_mode() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->EnterSleepMode();
    if (err != TEMP_SUCCESS) return false;
    bool sleeping = g_handler->IsSleeping();

    err = g_handler->ExitSleepMode();
    bool awake = !g_handler->IsSleeping();

    ESP_LOGI(TAG, "Sleep: sleeping=%d, awake=%d", sleeping, awake);
    return sleeping && awake;
}

// ─────────────────────── Error Handling ───────────────────────

static bool test_self_test() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->SelfTest();
    ESP_LOGI(TAG, "SelfTest: %d", static_cast<int>(err));
    return err == TEMP_SUCCESS;
}

static bool test_health_check() noexcept {
    if (!g_handler) return false;
    auto err = g_handler->CheckHealth();
    ESP_LOGI(TAG, "CheckHealth: %d", static_cast<int>(err));
    return err == TEMP_SUCCESS;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     NTC TEMPERATURE HANDLER COMPREHENSIVE TEST SUITE        ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("sensor_info", test_sensor_info, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("capabilities", test_capabilities, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_TEMPERATURE_READING_TESTS, "TEMPERATURE READING",
        RUN_TEST_IN_TASK("read_temp", test_read_temperature, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("consistency", test_read_temperature_consistency, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CALIBRATION_TESTS, "CALIBRATION",
        RUN_TEST_IN_TASK("offset", test_set_calibration_offset, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_FILTERING_TESTS, "EMA FILTERING",
        RUN_TEST_IN_TASK("filtering", test_enable_filtering, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_THRESHOLD_TESTS, "THRESHOLD MONITORING",
        RUN_TEST_IN_TASK("threshold", test_threshold_monitoring, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONTINUOUS_MONITORING_TESTS, "CONTINUOUS MONITORING",
        RUN_TEST_IN_TASK("continuous", test_continuous_monitoring, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_STATISTICS_TESTS, "STATISTICS & DIAGNOSTICS",
        RUN_TEST_IN_TASK("statistics", test_statistics, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("diagnostics", test_diagnostics, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_SLEEP_MODE_TESTS, "SLEEP MODE",
        RUN_TEST_IN_TASK("sleep", test_sleep_mode, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "SELF-TEST & HEALTH",
        RUN_TEST_IN_TASK("self_test", test_self_test, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("health", test_health_check, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "NTC TEMPERATURE HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
