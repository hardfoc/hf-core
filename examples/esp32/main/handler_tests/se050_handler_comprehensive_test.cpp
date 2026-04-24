/**
 * @file se050_handler_comprehensive_test.cpp
 * @brief Exercises `Se050Handler` on ESP32 (same usage pattern as `hf-se050-driver` examples).
 *
 * @details Verifies the `HalI2cSe050Comm` bridge (`BaseI2c` → `se050::I2cTransceiveInterface`),
 *          mutex-serialized `EnsureInitialized()` (GPIO reset, T=1 warm reset, `SELECT`, `GetVersion`),
 *          and a few live applet calls through `GetDevice()`.
 *
 * Hardware required:
 * - NXP SE050 on I²C at @ref SE050_I2C_ADDR (default `0x48`)
 * - Optional `SE_RESET` GPIO (pass to handler ctor when wiring is available)
 *
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "esp32_bus_setup.hpp"
#include "esp32_test_config.hpp"

#include "handlers/se050/Se050Handler.h"

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

static const char* TAG = "SE050_Handler_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_INIT_TESTS    = true;
static constexpr bool ENABLE_APPLET_TESTS  = true;

static std::unique_ptr<Se050Handler> g_handler;

static bool create_handler() noexcept {
    BaseI2c* dev = get_i2c_device(SE050_I2C_ADDR);
    if (dev == nullptr) {
        ESP_LOGE(TAG, "No BaseI2c device at 0x%02X (check esp32_bus_setup)", SE050_I2C_ADDR);
        return false;
    }
    g_handler = std::make_unique<Se050Handler>(*dev, Se050HandlerConfig{}, nullptr, nullptr);
    return g_handler != nullptr;
}

static bool test_initialize() noexcept {
    if (!g_handler) {
        return false;
    }
    const bool ok = g_handler->EnsureInitialized();
    ESP_LOGI(TAG, "EnsureInitialized: %s", ok ? "OK" : "FAILED");
    return ok && g_handler->IsPresent();
}

static bool test_get_version() noexcept {
    if (!g_handler || !g_handler->IsPresent()) {
        return false;
    }
    se050::cmd::VersionInfo v{};
    const se050::Error e = g_handler->GetDevice().GetVersion(&v, g_handler->Config().apdu_timeout_ms);
    ESP_LOGI(TAG, "GetVersion: se050::Error=%u", static_cast<unsigned>(e));
    return e == se050::Error::Ok;
}

static bool test_get_random() noexcept {
    if (!g_handler || !g_handler->IsPresent()) {
        return false;
    }
    std::uint8_t buf[16]{};
    std::size_t n = 0;
    const se050::Error e =
        g_handler->GetDevice().GetRandom(16U, buf, sizeof(buf), &n, g_handler->Config().apdu_timeout_ms);
    ESP_LOGI(TAG, "GetRandom: err=%u len=%u", static_cast<unsigned>(e), static_cast<unsigned>(n));
    return e == se050::Error::Ok && n == 16U;
}

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "======== SE050 HANDLER COMPREHENSIVE TEST ========");

    if (!create_handler()) {
        ESP_LOGE(TAG, "FATAL: handler creation failed");
        return;
    }

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INIT_TESTS, "INITIALIZATION",
        RUN_TEST_IN_TASK("init", test_initialize, 8192, 5);
        flip_test_progress_indicator();
    );

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_APPLET_TESTS, "APPLET API",
        RUN_TEST_IN_TASK("get_version", test_get_version, 8192, 5);
        flip_test_progress_indicator();
        RUN_TEST_IN_TASK("get_random", test_get_random, 8192, 5);
        flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "SE050 HANDLER COMPREHENSIVE", TAG);
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
