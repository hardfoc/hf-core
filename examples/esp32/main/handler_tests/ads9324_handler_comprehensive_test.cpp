/**
 * @file ads9324_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Ads9324Handler
 *
 * Tests: CRTP SPI + CONVST/DRDY adapter, construction, initialization
 * (graceful skip without silicon), BaseAdc channel map, PGA/offset/gain,
 * snapshot, visitDriver/GetDriver, diagnostics, and pre-init error handling.
 *
 * @note Requires ADS9324 on SPI with CONVST (DRDY optional). Without hardware,
 *       Initialize fails gracefully and remaining live tests are skipped.
 *       Product images keep HF_CORE_ENABLE_ADS9324 OFF; this app is compiled
 *       because examples/esp32/components/hf_core enables the flag.
 *
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/ads9324/Ads9324Handler.h"

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

static const char* TAG = "ADS9324_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS = true;
static constexpr bool ENABLE_CHANNEL_MAP_TESTS = true;
static constexpr bool ENABLE_PGA_TESTS = true;
static constexpr bool ENABLE_READ_TESTS = true;
static constexpr bool ENABLE_DRIVER_ACCESS_TESTS = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS = true;

static std::unique_ptr<EspGpio> g_convst_gpio;
static std::unique_ptr<EspGpio> g_drdy_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Ads9324Handler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_ADS9324_CS);
    dev_cfg.clock_speed_hz = ADS9324_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_0;
    dev_cfg.queue_size = 1;

    const int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed for ADS9324");
        return false;
    }
    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device init failed for ADS9324");
        return false;
    }

    g_convst_gpio = create_gpio(PIN_ADS9324_CONVST, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_drdy_gpio = create_gpio(PIN_ADS9324_DRDY, hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                              hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);

    if (!g_convst_gpio) {
        ESP_LOGE(TAG, "Failed to create CONVST GPIO");
        return false;
    }

    g_handler = CreateAds9324Handler(*g_spi_device, g_convst_gpio.get(), g_drdy_gpio.get());
    return g_handler != nullptr;
}

static bool test_construction() noexcept {
    if (!g_handler) {
        return false;
    }
    const bool is_init = g_handler->IsInitialized();
    const bool desc_ok = g_handler->GetDescription() != nullptr;
    ESP_LOGI(TAG, "After construction: IsInitialized=%d desc=%s", is_init,
             g_handler->GetDescription());
    return !is_init && desc_ok && g_handler->GetDriver() == nullptr;
}

static bool test_initialize() noexcept {
    if (!g_handler) {
        return false;
    }
    g_hw_present = g_handler->Initialize();
    ESP_LOGI(TAG, "Initialize: %s (hw_present=%d)", g_hw_present ? "OK" : "FAILED", g_hw_present);
    return true;
}

static bool test_is_initialized() noexcept {
    if (!g_handler) {
        return false;
    }
    const bool init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized: %d (expected %d)", init, g_hw_present);
    return init == g_hw_present;
}

static bool test_max_channels() noexcept {
    if (!g_handler) {
        return false;
    }
    const hf_u8_t n = g_handler->GetMaxChannels();
    ESP_LOGI(TAG, "GetMaxChannels: %u (expected 16)", n);
    return n == ads9324::kNumChannels;
}

static bool test_channel_available() noexcept {
    if (!g_handler) {
        return false;
    }
    const bool ch0 = g_handler->IsChannelAvailable(0);
    const bool ch15 = g_handler->IsChannelAvailable(15);
    const bool ch16 = g_handler->IsChannelAvailable(16);
    ESP_LOGI(TAG, "IsChannelAvailable 0=%d 15=%d 16=%d", ch0, ch15, ch16);
    return ch0 && ch15 && !ch16;
}

static bool test_configure_channel() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    ads9324::ChannelConfig cfg{};
    cfg.type = ads9324::InputType::Differential;
    cfg.range = ads9324::InputRange::PlusMinus10V;
    cfg.bandwidth = ads9324::PgaBandwidth::Low;
    const bool ok = g_handler->ConfigureChannel(0, cfg);
    ESP_LOGI(TAG, "ConfigureChannel(0, ±10V diff): %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_set_offset_gain() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const bool ofs = g_handler->SetChannelOffset(0, 0);
    const bool gan = g_handler->SetChannelGain(0, 0);
    ESP_LOGI(TAG, "SetChannelOffset/Gain(0): ofs=%d gan=%d", ofs, gan);
    return ofs && gan;
}

static bool test_read_snapshot() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    ads9324::Snapshot snap{};
    const bool ok = g_handler->ReadSnapshot(snap);
    ESP_LOGI(TAG, "ReadSnapshot: %s valid_mask=0x%04X ch0=%d", ok ? "OK" : "FAILED", snap.valid_mask,
             static_cast<int>(snap.count[0]));
    return ok && snap.ok();
}

static bool test_read_channel_base_adc() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    hf_u32_t count = 0;
    float voltage = 0.0f;
    const hf_adc_err_t err = g_handler->ReadChannel(0, count, voltage, 1, 0);
    ESP_LOGI(TAG, "ReadChannel(0): err=%d count=%lu V=%.4f", static_cast<int>(err),
             static_cast<unsigned long>(count), static_cast<double>(voltage));
    return err == hf_adc_err_t::ADC_SUCCESS;
}

static bool test_invalid_channel() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    hf_u32_t count = 0;
    float voltage = 0.0f;
    const hf_adc_err_t err = g_handler->ReadChannel(16, count, voltage, 1, 0);
    ESP_LOGI(TAG, "ReadChannel(16) out-of-range: err=%d (expect INVALID_CHANNEL)",
             static_cast<int>(err));
    return err == hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
}

static bool test_visit_driver() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const uint16_t id = g_handler->visitDriver([](auto& drv) { return drv.ReadDeviceId(); });
    ESP_LOGI(TAG, "visitDriver ReadDeviceId: 0x%04X (expect 0x%04X)", id, ads9324::kDeviceIdAds9324);
    return id == ads9324::kDeviceIdAds9324;
}

static bool test_get_driver() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    auto* drv = g_handler->GetDriver();
    return drv != nullptr && drv->IsInitialized();
}

static bool test_dump_diagnostics() noexcept {
    if (!g_handler) {
        return false;
    }
    Ads9324Diagnostics diag{};
    const bool ok = g_handler->GetHandlerDiagnostics(diag);
    g_handler->DumpDiagnostics();
    ESP_LOGI(TAG, "DumpDiagnostics: init=%d ready=%d id=0x%04X", diag.initialized, diag.driver_ready,
             diag.device_id);
    return ok;
}

static bool test_missing_convst() noexcept {
    if (!g_spi_device) {
        return true;
    }
    auto uninit = CreateAds9324Handler(*g_spi_device, nullptr, nullptr);
    if (!uninit) {
        return false;
    }
    const bool init_ok = uninit->Initialize();
    ESP_LOGI(TAG, "Initialize without CONVST: %s (expected fail)", init_ok ? "OK" : "FAILED");
    return !init_ok && !uninit->IsInitialized();
}

static bool test_deinitialize() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const bool ok = g_handler->Deinitialize();
    const bool still_init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "Deinitialize: %s IsInitialized=%d", ok ? "OK" : "FAILED", still_init);
    return ok && !still_init;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     ADS9324 HANDLER COMPREHENSIVE TEST SUITE                ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) {
        ESP_LOGE(TAG, "FATAL: Handler creation failed");
        return;
    }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_init", test_is_initialized, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CHANNEL_MAP_TESTS, "CHANNEL MAP",
        RUN_TEST_IN_TASK("max_ch", test_max_channels, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("avail", test_channel_available, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PGA_TESTS, "PGA / CAL",
        RUN_TEST_IN_TASK("cfg_ch", test_configure_channel, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ofs_gan", test_set_offset_gain, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_READ_TESTS, "READ",
        RUN_TEST_IN_TASK("snap", test_read_snapshot, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("read_ch", test_read_channel_base_adc, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("ch_range", test_invalid_channel, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DRIVER_ACCESS_TESTS, "DRIVER ACCESS",
        RUN_TEST_IN_TASK("visit", test_visit_driver, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("drv_ptr", test_get_driver, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("no_convst", test_missing_convst, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinit", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "ADS9324 HANDLER COMPREHENSIVE", TAG);
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
