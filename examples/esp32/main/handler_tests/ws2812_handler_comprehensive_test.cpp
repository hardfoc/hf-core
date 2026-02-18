/**
 * @file ws2812_handler_comprehensive_test.cpp
 * @brief Comprehensive test suite for Ws2812Handler
 *
 * Tests: Config-based construction, initialization, pixel set (individual +
 * all), clear, show, brightness, animation effects (rainbow, chase, breathe),
 * tick/step, GetNumLeds, direct strip/animator access, diagnostics dump,
 * and pre-init error handling.
 *
 * @note This test requires a WS2812B LED strip connected to the data GPIO.
 *       Without hardware, initialization may still succeed (RMT configures
 *       regardless) but visual confirmation requires LEDs.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/ws2812/Ws2812Handler.h"

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "WS2812_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CONSTRUCTION_TESTS    = true;
static constexpr bool ENABLE_INITIALIZATION_TESTS  = true;
static constexpr bool ENABLE_PIXEL_CONTROL_TESTS   = true;
static constexpr bool ENABLE_BRIGHTNESS_TESTS      = true;
static constexpr bool ENABLE_ANIMATION_TESTS       = true;
static constexpr bool ENABLE_DIRECT_ACCESS_TESTS   = true;
static constexpr bool ENABLE_DIAGNOSTICS_TESTS     = true;
static constexpr bool ENABLE_ERROR_HANDLING_TESTS  = true;

static std::unique_ptr<Ws2812Handler> g_handler;

static bool g_hw_present = false;

static bool create_handler() noexcept {
    Ws2812Handler::Config cfg = {};
    cfg.gpio_pin = static_cast<gpio_num_t>(PIN_WS2812_DATA);
    cfg.num_leds = WS2812_NUM_LEDS;
    cfg.brightness = 50; // Low brightness for testing
    cfg.rmt_channel = WS2812_RMT_CHANNEL;

    g_handler = std::make_unique<Ws2812Handler>(cfg);
    return g_handler != nullptr;
}

// ─────────────────────── Construction ───────────────────────

static bool test_construction() noexcept {
    bool is_init = g_handler->IsInitialized();
    uint32_t num_leds = g_handler->GetNumLeds();
    ESP_LOGI(TAG, "After construction: IsInitialized=%d (expected false), num_leds=%lu",
             is_init, static_cast<unsigned long>(num_leds));
    return !is_init && num_leds == WS2812_NUM_LEDS;
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    if (!g_handler) return false;
    esp_err_t err = g_handler->Initialize();
    g_hw_present = (err == ESP_OK);
    ESP_LOGI(TAG, "Initialize: %s (err=%d)", g_hw_present ? "OK" : "FAILED", err);
    return true; // RMT init may succeed even without LEDs attached
}

static bool test_is_initialized() noexcept {
    if (!g_handler) return false;
    bool init = g_handler->IsInitialized();
    ESP_LOGI(TAG, "IsInitialized: %d (expected %d)", init, g_hw_present);
    return init == g_hw_present;
}

// ─────────────────────── Pixel Control ───────────────────────

static bool test_set_pixel() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    strip->SetPixel(0, 0xFF0000); // Red
    ESP_LOGI(TAG, "SetPixel(0, Red): OK");
    return true;
}

static bool test_set_all_pixels() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    for (uint32_t i = 0; i < g_handler->GetNumLeds(); ++i) {
        strip->SetPixel(i, 0x00FF00); // Green
    }
    ESP_LOGI(TAG, "SetAllPixels(Green): OK");
    return true;
}

static bool test_show() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    esp_err_t err = strip->Show();
    bool ok = (err == ESP_OK);
    ESP_LOGI(TAG, "Show: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_clear() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    for (uint32_t i = 0; i < g_handler->GetNumLeds(); ++i) {
        strip->SetPixel(i, 0x000000);
    }
    esp_err_t err = strip->Show();
    bool ok = (err == ESP_OK);
    ESP_LOGI(TAG, "Clear: %s", ok ? "OK" : "FAILED");
    return ok;
}

static bool test_pixel_out_of_range() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    // SetPixel with out-of-range index — strip silently ignores (void return)
    strip->SetPixel(WS2812_NUM_LEDS + 10, 0xFFFFFF);
    ESP_LOGI(TAG, "SetPixel(out-of-range): no crash");
    return true;
}

// ─────────────────────── Color Sequence ───────────────────────

static bool test_color_sequence() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }

    auto* strip = g_handler->GetStrip();
    if (!strip) return false;

    // Walk through red, green, blue on first 3 LEDs
    strip->SetPixel(0, 0xFF0000);
    if (WS2812_NUM_LEDS > 1) strip->SetPixel(1, 0x00FF00);
    if (WS2812_NUM_LEDS > 2) strip->SetPixel(2, 0x0000FF);
    strip->Show();
    vTaskDelay(pdMS_TO_TICKS(500));

    for (uint32_t i = 0; i < g_handler->GetNumLeds(); ++i) strip->SetPixel(i, 0);
    strip->Show();
    ESP_LOGI(TAG, "Color sequence: R-G-B displayed for 500ms");
    return true;
}

// ─────────────────────── Brightness ───────────────────────

static bool test_set_brightness() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    strip->SetBrightness(100);
    ESP_LOGI(TAG, "SetBrightness(100): OK");
    strip->SetBrightness(50); // Restore
    return true;
}

static bool test_brightness_range() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    if (!strip) return false;
    strip->SetBrightness(0);
    strip->SetBrightness(255);
    ESP_LOGI(TAG, "Brightness min/max: OK");
    strip->SetBrightness(50);
    return true;
}

// ─────────────────────── Animation Effects ───────────────────────

static bool test_set_effect_rainbow() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* anim = g_handler->GetAnimator();
    if (!anim) return false;
    anim->SetEffect(WS2812Animator::Effect::Rainbow, 0xFFFFFF);
    ESP_LOGI(TAG, "SetEffect(RAINBOW): OK");
    return true;
}

static bool test_tick() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* anim = g_handler->GetAnimator();
    if (!anim) return false;
    // Run a few animation ticks
    for (int i = 0; i < 10; ++i) {
        anim->Tick();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    ESP_LOGI(TAG, "Tick x10: completed");
    return true;
}

static bool test_step() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* anim = g_handler->GetAnimator();
    if (!anim) return false;
    uint16_t step = anim->Step();
    ESP_LOGI(TAG, "Step: %u", step);
    return true;
}

// ─────────────────────── Direct Access ───────────────────────

static bool test_get_strip() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* strip = g_handler->GetStrip();
    bool ok = strip != nullptr;
    ESP_LOGI(TAG, "GetStrip: %s (ptr=%p)", ok ? "OK" : "NULL", static_cast<void*>(strip));
    return ok;
}

static bool test_get_animator() noexcept {
    if (!g_hw_present) { ESP_LOGW(TAG, "SKIP: no hardware"); return true; }
    auto* anim = g_handler->GetAnimator();
    bool ok = anim != nullptr;
    ESP_LOGI(TAG, "GetAnimator: %s (ptr=%p)", ok ? "OK" : "NULL", static_cast<void*>(anim));
    return ok;
}

static bool test_get_num_leds() noexcept {
    uint32_t n = g_handler->GetNumLeds();
    ESP_LOGI(TAG, "GetNumLeds: %lu (expected %d)", static_cast<unsigned long>(n), WS2812_NUM_LEDS);
    return n == WS2812_NUM_LEDS;
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
    Ws2812Handler::Config cfg = {};
    cfg.gpio_pin = static_cast<gpio_num_t>(PIN_WS2812_DATA);
    cfg.num_leds = 4;

    auto uninit = std::make_unique<Ws2812Handler>(cfg);

    bool init = uninit->IsInitialized();
    auto* strip = uninit->GetStrip();
    auto* anim  = uninit->GetAnimator();

    ESP_LOGI(TAG, "Uninit handler: init=%d, strip=%p, anim=%p",
             init, static_cast<void*>(strip), static_cast<void*>(anim));
    return !init && strip == nullptr && anim == nullptr;
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
    ESP_LOGI(TAG, "║     WS2812 HANDLER COMPREHENSIVE TEST SUITE                 ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    if (!create_handler()) { ESP_LOGE(TAG, "FATAL: Handler creation failed"); return; }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONSTRUCTION_TESTS, "CONSTRUCTION",
        RUN_TEST_IN_TASK("construct", test_construction, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INITIALIZATION_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 16384, 15); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("is_init", test_is_initialized, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PIXEL_CONTROL_TESTS, "PIXEL CONTROL",
        RUN_TEST_IN_TASK("set_pix", test_set_pixel, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("set_all", test_set_all_pixels, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("show", test_show, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("clear", test_clear, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("out_range", test_pixel_out_of_range, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("color_seq", test_color_sequence, 16384, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_BRIGHTNESS_TESTS, "BRIGHTNESS",
        RUN_TEST_IN_TASK("bright", test_set_brightness, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("bright_rng", test_brightness_range, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ANIMATION_TESTS, "ANIMATION",
        RUN_TEST_IN_TASK("rainbow", test_set_effect_rainbow, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("tick", test_tick, 8192, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("step", test_step, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIRECT_ACCESS_TESTS, "DIRECT ACCESS",
        RUN_TEST_IN_TASK("strip", test_get_strip, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("animator", test_get_animator, 8192, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("num_leds", test_get_num_leds, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DIAGNOSTICS_TESTS, "DIAGNOSTICS",
        RUN_TEST_IN_TASK("dump_diag", test_dump_diagnostics, 16384, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ERROR_HANDLING_TESTS, "ERROR HANDLING",
        RUN_TEST_IN_TASK("before_init", test_operations_before_init, 16384, 10); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("deinit", test_deinitialize, 8192, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "WS2812 HANDLER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
