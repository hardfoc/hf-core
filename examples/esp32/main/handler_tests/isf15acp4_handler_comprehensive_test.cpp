/**
 * @file isf15acp4_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Isf15acp4Handler (NKK SmartDisplay OLED).
 */
#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/isf15acp4/Isf15acp4Handler.h"
#include "isf15acp4_button_ui.hpp"

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

static const char* TAG = "ISF15ACP4_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS = true;
static constexpr bool ENABLE_DISPLAY_TESTS = true;
static constexpr bool ENABLE_GRAPHICS_TESTS = true;
static constexpr bool ENABLE_BUTTON_TESTS = true;
static constexpr bool ENABLE_DRIVER_ACCESS_TESTS = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS = true;

static std::unique_ptr<EspGpio> g_cs_gpio;
static std::unique_ptr<EspGpio> g_dc_gpio;
static std::unique_ptr<EspGpio> g_res_gpio;
static std::unique_ptr<EspGpio> g_vcc_gpio;
static std::unique_ptr<EspGpio> g_switch_gpio;
static BaseSpi* g_spi_device = nullptr;
static std::unique_ptr<Isf15acp4Handler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    auto* spi_bus = get_shared_spi_bus();
    if (!spi_bus) {
        ESP_LOGE(TAG, "Failed to get SPI bus");
        return false;
    }

    hf_spi_device_config_t dev_cfg = {};
    dev_cfg.cs_pin = static_cast<hf_pin_num_t>(PIN_ISF15ACP4_SPI_DUMMY_CS);
    dev_cfg.clock_speed_hz = ISF15ACP4_SPI_CLOCK_HZ;
    dev_cfg.mode = hf_spi_mode_t::HF_SPI_MODE_0;
    dev_cfg.queue_size = 4;

    const int dev_idx = spi_bus->CreateDevice(dev_cfg);
    if (dev_idx < 0) {
        ESP_LOGE(TAG, "SPI device creation failed");
        return false;
    }

    g_spi_device = spi_bus->GetDevice(dev_idx);
    if (!g_spi_device) {
        ESP_LOGE(TAG, "SPI device unavailable");
        return false;
    }

    g_cs_gpio = create_gpio(PIN_ISF15ACP4_CS, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                            hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    g_dc_gpio = create_gpio(PIN_ISF15ACP4_DC, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                            hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_res_gpio = create_gpio(PIN_ISF15ACP4_RES, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    g_vcc_gpio = create_gpio(PIN_ISF15ACP4_VCC_EN, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT,
                             hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    g_switch_gpio = create_gpio(PIN_ISF15ACP4_SWITCH, hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
                                hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);

    if (!g_cs_gpio || !g_dc_gpio || !g_res_gpio || !g_vcc_gpio) {
        ESP_LOGE(TAG, "GPIO setup failed");
        return false;
    }

    Isf15acp4HandlerConfig cfg{
        .variant = isf15acp4::ProductVariant::Isf15acp4,
        .graphics = isf15acp4::GraphicsBackend::BuiltinCanvas,
        .enable_vcc_on_init = true,
    };

    g_handler = std::make_unique<Isf15acp4Handler>(*g_spi_device, *g_cs_gpio, *g_dc_gpio, *g_res_gpio,
                                                   cfg, g_vcc_gpio.get(), g_switch_gpio.get(), true);
    return g_handler != nullptr;
}

static bool test_construction() noexcept {
    const bool ok = g_handler != nullptr && !g_handler->IsInitialized();
    ESP_LOGI(TAG, "Construction: handler=%p init=%d", g_handler.get(), g_handler->IsInitialized());
    return ok;
}

static bool test_initialize() noexcept {
    if (!g_handler) {
        return false;
    }
    g_hw_present = g_handler->EnsureInitialized();
    ESP_LOGI(TAG, "EnsureInitialized: %s", g_hw_present ? "OK" : "FAILED (no hardware?)");
    return g_hw_present;
}

static bool test_is_initialized() noexcept {
    if (!g_hw_present) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    return g_handler->IsInitialized();
}

static bool test_display_on_off() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const bool off_ok = g_handler->DisplayOff();
    vTaskDelay(pdMS_TO_TICKS(100));
    const bool on_ok = g_handler->DisplayOn();
    ESP_LOGI(TAG, "DisplayOff=%d DisplayOn=%d", off_ok, on_ok);
    return off_ok && on_ok;
}

static bool test_fill_and_present() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    bool ok = g_handler->FillScreen(isf15acp4::colors::Blue);
    vTaskDelay(pdMS_TO_TICKS(200));
    ok = ok && g_handler->FillScreen(isf15acp4::colors::Black);
    return ok;
}

static bool test_graphics_context() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    auto gfx = g_handler->CreateGraphicsContext();
    gfx.Clear(isf15acp4::colors::Black);
    gfx.DrawString(4, 4, "HF-CORE", isf15acp4::colors::White);
    gfx.DrawString(4, 16, "ISF15ACP4", isf15acp4::colors::Cyan);
    return g_handler->Present(gfx);
}

static bool test_button_ui() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    isf15acp4::ButtonUI button;
    const bool pressed = g_handler->IsPressed();
    (void)button.Update(pressed, 50);
    auto gfx = g_handler->CreateGraphicsContext();
    button.DrawStatus(gfx, "TEST", pressed);
    return g_handler->Present(gfx);
}

static bool test_visit_driver() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    return g_handler->visitDriver([](isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>& drv) {
        return drv.IsInitialized();
    });
}

static bool test_get_driver() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    auto* drv = g_handler->GetDriver();
    return drv != nullptr && drv->IsInitialized();
}

static bool test_error_flags() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const uint16_t flags = g_handler->GetErrorFlags();
    g_handler->ClearErrorFlags();
    ESP_LOGI(TAG, "ErrorFlags before clear: 0x%04X", flags);
    return true;
}

static bool test_dump_diagnostics() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    g_handler->DumpDiagnostics();
    return true;
}

static bool test_operations_before_init() noexcept {
    if (!g_spi_device || !g_cs_gpio || !g_dc_gpio || !g_res_gpio) {
        return true;
    }
    Isf15acp4Handler uninit(*g_spi_device, *g_cs_gpio, *g_dc_gpio, *g_res_gpio);
    const bool init = uninit.IsInitialized();
    const bool fill = uninit.FillScreen(isf15acp4::colors::Red);
    ESP_LOGI(TAG, "Uninit: init=%d fill=%d", init, fill);
    return !init && !fill;
}

static bool test_deinitialize() noexcept {
    if (!g_hw_present || !g_handler) {
        ESP_LOGW(TAG, "SKIP: no hardware");
        return true;
    }
    const bool ok = g_handler->Deinitialize();
    const bool still_init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "Deinitialize=%d still_init=%d", ok, still_init);
    g_hw_present = false;
    return ok && !still_init;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "ISF15ACP4 handler comprehensive test");

    if (!create_handler()) {
        ESP_LOGE(TAG, "FATAL: handler creation failed");
        return;
    }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("initialize", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_initialized", test_is_initialized, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DISPLAY_TESTS, "DISPLAY",
        RUN_TEST_IN_TASK("display_on_off", test_display_on_off, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("fill_screen", test_fill_and_present, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_GRAPHICS_TESTS, "GRAPHICS",
        RUN_TEST_IN_TASK("graphics_context", test_graphics_context, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_BUTTON_TESTS, "BUTTON",
        RUN_TEST_IN_TASK("button_ui", test_button_ui, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DRIVER_ACCESS_TESTS, "DRIVER ACCESS",
        RUN_TEST_IN_TASK("visit_driver", test_visit_driver, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("get_driver", test_get_driver, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("error_flags", test_error_flags, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinitialize", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "ISF15ACP4 HANDLER", TAG);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
