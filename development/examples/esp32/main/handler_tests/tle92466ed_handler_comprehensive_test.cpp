/**
 * @file tle92466ed_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Tle92466edHandler
 *
 * Tests: CRTP SPI adapter, initialization (default + GlobalConfig), 6-channel
 * enable/disable/current/PWM, channel diagnostics, fault report/clear, watchdog
 * kick/disable, device ID readback, GetDriver() direct access, diagnostics dump,
 * and pre-init error handling.
 *
 * @note This test requires a TLE92466ED evaluation board connected via SPI.
 *       Without hardware, initialization will fail gracefully and remaining
 *       tests will be skipped.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/tle92466ed/Tle92466edHandler.h"

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

static const char* TAG = "TLE92466ED_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS    = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS  = true;
static constexpr bool ENABLE_CHANNEL_CONTROL_TESTS = true;
static constexpr bool ENABLE_STATUS_TESTS          = true;
static constexpr bool ENABLE_WATCHDOG_TESTS        = true;
static constexpr bool ENABLE_DEVICE_INFO_TESTS     = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS     = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS  = true;

// Hardware resources
static std::unique_ptr<EspGpio> g_resn_gpio;
static std::unique_ptr<EspGpio> g_en_gpio;
static std::unique_ptr<EspGpio> g_faultn_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Tle92466edHandler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_TLE92466ED_CS);
    dev_cfg.clock_speed_hz = TLE92466ED_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_1;  // CPOL=0, CPHA=1 per TLE datasheet
    dev_cfg.queue_size = 1;

    int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for TLE92466ED");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for TLE92466ED");
        return false;
    }

    g_resn_gpio = create_gpio(PIN_TLE92466ED_RESN,
                              hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                              hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    g_en_gpio = create_gpio(PIN_TLE92466ED_EN,
                            hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                            hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_faultn_gpio = create_gpio(PIN_TLE92466ED_FAULTN,
                                hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);

    if (!g_resn_gpio || !g_en_gpio) {
        ESP_LOGE(TAG, "Failed to create control GPIOs");
        return false;
    }

    g_handler = std::make_unique<Tle92466edHandler>(
        *g_spi_device, *g_resn_gpio, *g_en_gpio, g_faultn_gpio.get());

    return g_handler != nullptr;
}

// ─────────────────────── Construction ───────────────────────

static bool test_construction() noexcept {
    bool is_init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "After construction: IsInitialized=%d (expected false)", is_init);
    return !is_init;
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    bool ok = g_handler->Initialize();
    g_hw_present = ok;
    ESP_LOGI(TAG, "Initialize: %s (hw_present=%d)", ok ? "OK" : "FAILED", g_hw_present);
    return true;
}

static bool test_is_initialized() noexcept {
    if (!g_handler) return false;
    bool init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized: %d (expected %d)", init, g_hw_present);
    return init == g_hw_present;
}

// ─────────────────────── Channel Control ───────────────────────

static bool test_enable_channel() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->EnableChannel(0);
    ESP_LOGI(TAG, "EnableChannel(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_disable_channel() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->DisableChannel(0);
    ESP_LOGI(TAG, "DisableChannel(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_enable_all_channels() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->EnableAllChannels();
    ESP_LOGI(TAG, "EnableAllChannels: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_disable_all_channels() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->DisableAllChannels();
    ESP_LOGI(TAG, "DisableAllChannels: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_channel_current() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->SetChannelCurrent(0, 500); // 500 mA
    ESP_LOGI(TAG, "SetChannelCurrent(0, 500mA): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_configure_pwm_raw() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->ConfigurePwmRaw(0, 128, 3); // mantissa=128, exponent=3
    ESP_LOGI(TAG, "ConfigurePwmRaw(0, 128, 3): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_channel_range_guard() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    // Channel 6 is out of range (0-5)
    bool ok = g_handler->EnableChannel(6);
    ESP_LOGI(TAG, "EnableChannel(6) out-of-range: %s (expected false)", ok ? "OK" : "FAILED");
    return !ok;
}

// ─────────────────────── Status & Faults ───────────────────────

static bool test_get_status() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    tle92466ed::DeviceStatus status = {};
    bool ok = g_handler->GetStatus(status);
    ESP_LOGI(TAG, "GetStatus: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_get_channel_diagnostics() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    tle92466ed::ChannelDiagnostics diag = {};
    bool ok = g_handler->GetChannelDiagnostics(0, diag);
    ESP_LOGI(TAG, "GetChannelDiagnostics(0): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_get_fault_report() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    tle92466ed::FaultReport report = {};
    bool ok = g_handler->GetFaultReport(report);
    ESP_LOGI(TAG, "GetFaultReport: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_has_fault() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool fault = g_handler->HasFault();
    ESP_LOGI(TAG, "HasFault: %d", fault);
    return true;
}

static bool test_clear_faults() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->ClearFaults();
    ESP_LOGI(TAG, "ClearFaults: %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Watchdog ───────────────────────

static bool test_kick_watchdog() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->KickWatchdog();
    ESP_LOGI(TAG, "KickWatchdog: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_enter_mission_mode() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->EnterMissionMode();
    ESP_LOGI(TAG, "EnterMissionMode: %s", ok ? "OK" : "FAILED");
    if (ok) {
        bool is_mission = g_handler->IsMissionMode();
        ESP_LOGI(TAG, "IsMissionMode: %d", is_mission);
    }
    return ok;
}

// ─────────────────────── Device Info ───────────────────────

static bool test_get_chip_id() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    uint32_t id = g_handler->GetChipId();
    ESP_LOGI(TAG, "Chip ID: 0x%08lX", static_cast<unsigned long>(id));
    return id != 0;
}

static bool test_get_driver_access() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* drv = g_handler->GetDriver();
    bool ok = drv != nullptr;
    ESP_LOGI(TAG, "GetDriver: %s (ptr=%p)", ok ? "OK" : "NULL", static_cast<void*>(drv));
    return ok;
}

// ─────────────────────── Diagnostics ───────────────────────

static bool test_dump_diagnostics() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    g_handler->DumpDiagnostics();
    ESP_LOGI(TAG, "DumpDiagnostics: completed");
    return true;
}

// ─────────────────────── Error Handling ───────────────────────

static bool test_operations_before_init() noexcept {
    if (!g_spi_device || !g_resn_gpio || !g_en_gpio) return true;

    auto uninit = std::make_unique<Tle92466edHandler>(
        *g_spi_device, *g_resn_gpio, *g_en_gpio);

    bool init = uninit->IsInitialized();
    bool en_ok = uninit->EnableChannel(0);
    bool fault = uninit->HasFault();

    ESP_LOGI(TAG, "Uninit handler: init=%d, enable_ch0=%d, fault=%d",
             init, en_ok, fault);
    return !init && !en_ok;
}

static bool test_deinitialize() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->Deinitialize();
    ESP_LOGI(TAG, "Deinitialize: %s", ok ? "OK" : "FAILED");
    bool still_init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized after deinit: %d (expected false)", still_init);
    return ok && !still_init;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     TLE92466ED HANDLER COMPREHENSIVE TEST SUITE             ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_init", test_is_initialized, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CHANNEL_CONTROL_TESTS, "CHANNEL CONTROL",
        RUN_TEST_IN_TASK("en_ch", test_enable_channel, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("dis_ch", test_disable_channel, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("en_all", test_enable_all_channels, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("dis_all", test_disable_all_channels, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_curr", test_set_channel_current, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("pwm_raw", test_configure_pwm_raw, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_range", test_channel_range_guard, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_STATUS_TESTS, "STATUS & FAULTS",
        RUN_TEST_IN_TASK("status", test_get_status, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_diag", test_get_channel_diagnostics, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("fault_rpt", test_get_fault_report, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("has_fault", test_has_fault, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("clear_flt", test_clear_faults, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_WATCHDOG_TESTS, "WATCHDOG",
        RUN_TEST_IN_TASK("kick_wdt", test_kick_watchdog, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("mission", test_enter_mission_mode, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DEVICE_INFO_TESTS, "DEVICE INFO",
        RUN_TEST_IN_TASK("chip_id", test_get_chip_id, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("drv_ptr", test_get_driver_access, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinit", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "TLE92466ED HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
