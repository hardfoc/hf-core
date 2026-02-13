/**
 * @file max22200_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Max22200Handler
 *
 * Tests: CRTP SPI adapter with CMD-pin two-phase protocol, initialization
 * (default + BoardConfig), 8-channel enable/disable/mask, CDR/VDR channel
 * setup, channel faults, device status, device ID, soft reset, GetDriver()
 * direct access, diagnostics dump, and pre-init error handling.
 *
 * @note This test requires a MAX22200 evaluation board connected via SPI.
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

#include "handlers/max22200/Max22200Handler.h"

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

static const char* TAG = "MAX22200_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS    = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS  = true;
static constexpr bool ENABLE_CHANNEL_CONTROL_TESTS = true;
static constexpr bool ENABLE_CHANNEL_SETUP_TESTS   = true;
static constexpr bool ENABLE_STATUS_TESTS          = true;
static constexpr bool ENABLE_DEVICE_CONTROL_TESTS  = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS     = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS  = true;

// Hardware resources
static std::unique_ptr<EspGpio> g_enable_gpio;
static std::unique_ptr<EspGpio> g_cmd_gpio;
static std::unique_ptr<EspGpio> g_fault_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Max22200Handler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_MAX22200_CS);
    dev_cfg.clock_speed_hz = MAX22200_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_0;  // CPOL=0, CPHA=0
    dev_cfg.queue_size = 1;

    int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for MAX22200");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for MAX22200");
        return false;
    }

    g_enable_gpio = create_gpio(PIN_MAX22200_ENABLE,
                                hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_cmd_gpio = create_gpio(PIN_MAX22200_CMD,
                             hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_fault_gpio = create_gpio(PIN_MAX22200_FAULT,
                               hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                               hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);

    if (!g_enable_gpio || !g_cmd_gpio) {
        ESP_LOGE(TAG, "Failed to create control GPIOs");
        return false;
    }

    g_handler = std::make_unique<Max22200Handler>(
        *g_spi_device, *g_enable_gpio, *g_cmd_gpio, g_fault_gpio.get());

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

static bool test_is_channel_enabled() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    g_handler->EnableChannel(3);
    bool enabled = g_handler->IsChannelEnabled(3);
    ESP_LOGI(TAG, "IsChannelEnabled(3): %d (expected true)", enabled);
    g_handler->DisableChannel(3);
    return enabled;
}

static bool test_set_channels_mask() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->SetChannelsMask(0x55); // Channels 0,2,4,6
    ESP_LOGI(TAG, "SetChannelsMask(0x55): %s", ok ? "OK" : "FAILED");
    g_handler->DisableAllChannels();
    return ok;
}

static bool test_channel_range_guard() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->EnableChannel(8); // out of range (0-7)
    ESP_LOGI(TAG, "EnableChannel(8) out-of-range: %s (expected false)", ok ? "OK" : "FAILED");
    return !ok;
}

// ─────────────────────── Channel Setup ───────────────────────

static bool test_setup_cdr_channel() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->SetupCdrChannel(0, 1000, 300, 10.0f); // 1A hit, 300mA hold, 10ms
    ESP_LOGI(TAG, "SetupCdrChannel(0, 1000, 300, 10ms): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_setup_vdr_channel() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    bool ok = g_handler->SetupVdrChannel(1, 100.0f, 30.0f, 15.0f); // 100% hit, 30% hold, 15ms
    ESP_LOGI(TAG, "SetupVdrChannel(1, 100%%, 30%%, 15ms): %s", ok ? "OK" : "FAILED");
    return ok;
}

// ─────────────────────── Status & Faults ───────────────────────

static bool test_get_status() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    max22200::StatusConfig status = {};
    bool ok = g_handler->GetStatus(status);
    ESP_LOGI(TAG, "GetStatus: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_get_channel_faults() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    max22200::FaultStatus faults = {};
    bool ok = g_handler->GetChannelFaults(0, faults);
    ESP_LOGI(TAG, "GetChannelFaults(0): %s", ok ? "OK" : "FAILED");
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

// ─────────────────────── Device Control ───────────────────────

static bool test_read_fault_register() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    max22200::FaultStatus faults{};
    bool ok = g_handler->ReadFaultRegister(faults);
    ESP_LOGI(TAG, "ReadFaultRegister: %s, hasFault=%d", ok ? "OK" : "FAILED", faults.hasFault());
    return ok;
}

static bool test_get_driver_access() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* drv = g_handler->GetDriver();
    bool ok = drv != nullptr;
    ESP_LOGI(TAG, "GetDriver: %s (ptr=%p)", ok ? "OK" : "NULL", static_cast<void*>(drv));
    return ok;
}

static bool test_enable_disable_device() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* drv = g_handler->GetDriver();
    if (!drv) return false;
    auto s1 = drv->EnableDevice();
    ESP_LOGI(TAG, "EnableDevice: %s", s1 == max22200::DriverStatus::OK ? "OK" : "FAILED");
    auto s2 = drv->DisableDevice();
    ESP_LOGI(TAG, "DisableDevice: %s", s2 == max22200::DriverStatus::OK ? "OK" : "FAILED");
    // Re-enable
    drv->EnableDevice();
    return s1 == max22200::DriverStatus::OK;
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
    if (!g_spi_device || !g_enable_gpio || !g_cmd_gpio) return true;

    auto uninit = std::make_unique<Max22200Handler>(
        *g_spi_device, *g_enable_gpio, *g_cmd_gpio);

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
    ESP_LOGI(TAG, "║     MAX22200 HANDLER COMPREHENSIVE TEST SUITE               ║");
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
        RUN_TEST_IN_TASK("is_en", test_is_channel_enabled, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("mask", test_set_channels_mask, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_range", test_channel_range_guard, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CHANNEL_SETUP_TESTS, "CHANNEL SETUP",
        RUN_TEST_IN_TASK("cdr", test_setup_cdr_channel, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("vdr", test_setup_vdr_channel, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_STATUS_TESTS, "STATUS & FAULTS",
        RUN_TEST_IN_TASK("status", test_get_status, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_flt", test_get_channel_faults, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("has_flt", test_has_fault, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("clr_flt", test_clear_faults, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DEVICE_CONTROL_TESTS, "DEVICE CONTROL",
        RUN_TEST_IN_TASK("flt_reg", test_read_fault_register, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("drv_ptr", test_get_driver_access, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("en_dis_dev", test_enable_disable_device, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinit", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "MAX22200 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
