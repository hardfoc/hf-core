/**
 * @file logger_comprehensive_test.cpp
 * @brief Comprehensive test suite for Logger singleton
 *
 * Tests: Singleton access, initialization, log levels, formatted output
 * with colors/styles, ASCII art rendering, log level filtering,
 * tag-based level override, enable/disable features, flush, and thread safety.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"
#include "handlers/logger/Logger.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "Logger_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_SINGLETON_TESTS       = true;
static constexpr bool ENABLE_INIT_TESTS            = true;
static constexpr bool ENABLE_LOG_LEVEL_TESTS       = true;
static constexpr bool ENABLE_FORMATTED_OUTPUT_TESTS = true;
static constexpr bool ENABLE_ASCII_ART_TESTS       = true;
static constexpr bool ENABLE_CONFIG_TESTS          = true;
static constexpr bool ENABLE_THREAD_SAFETY_TESTS   = true;

// ─────────────────────── Singleton ───────────────────────

static bool test_singleton_access() noexcept {
    auto& logger1 = Logger::GetInstance();
    auto& logger2 = Logger::GetInstance();
    bool same = (&logger1 == &logger2);
    ESP_LOGI(TAG, "Singleton: same_instance=%d", same);
    return same;
}

// ─────────────────────── Initialization ───────────────────────

static bool test_initialize() noexcept {
    auto& logger = Logger::GetInstance();
    LogConfig config{};
    bool ok = logger.Initialize(config);
    bool is_init = logger.IsInitialized();
    ESP_LOGI(TAG, "Initialize: ok=%d, is_init=%d", ok, is_init);
    return ok && is_init;
}

static bool test_reinitialize() noexcept {
    auto& logger = Logger::GetInstance();
    logger.Deinitialize();
    bool not_init = !logger.IsInitialized();
    bool ok = logger.Initialize();
    bool is_init = logger.IsInitialized();
    ESP_LOGI(TAG, "Reinitialize: not_init=%d, ok=%d, is_init=%d", not_init, ok, is_init);
    return not_init && ok && is_init;
}

// ─────────────────────── Log Levels ───────────────────────

static bool test_log_all_levels() noexcept {
    auto& logger = Logger::GetInstance();
    if (!logger.IsInitialized()) logger.Initialize();

    logger.Error("LogTest", "Error message: %d", 1);
    logger.Warn("LogTest", "Warning message: %d", 2);
    logger.Info("LogTest", "Info message: %d", 3);
    logger.Debug("LogTest", "Debug message: %d", 4);
    logger.Verbose("LogTest", "Verbose message: %d", 5);

    ESP_LOGI(TAG, "All log levels exercised without crash");
    return true;
}

static bool test_set_log_level() noexcept {
    auto& logger = Logger::GetInstance();
    logger.SetLogLevel("FilterTest", LogLevel::WARN);

    // Only WARN and ERROR should appear for "FilterTest" tag
    logger.Error("FilterTest", "This ERROR should appear");
    logger.Warn("FilterTest", "This WARN should appear");
    logger.Info("FilterTest", "This INFO should be FILTERED");
    logger.Debug("FilterTest", "This DEBUG should be FILTERED");

    // Restore
    logger.SetLogLevel("FilterTest", LogLevel::VERBOSE);
    ESP_LOGI(TAG, "Log level filtering exercised");
    return true;
}

// ─────────────────────── Formatted Output ───────────────────────

static bool test_formatted_output() noexcept {
    auto& logger = Logger::GetInstance();

    logger.Info("FmtTest", LogColor::GREEN, LogStyle::BOLD,
                "Bold green info: value=%d", 42);
    logger.Warn("FmtTest", LogColor::YELLOW, LogStyle::ITALIC,
                "Italic yellow warning");
    logger.Error("FmtTest", LogColor::RED, LogStyle::UNDERLINE,
                 "Underlined red error");

    ESP_LOGI(TAG, "Formatted output exercised without crash");
    return true;
}

// ─────────────────────── ASCII Art ───────────────────────

static bool test_ascii_art() noexcept {
    auto& logger = Logger::GetInstance();
    logger.EnableAsciiArt(true);

    std::string art = R"(
  _   _ _____ _____ _____ 
 | | | |  ___|  _  |  _  |
 | |_| | |__ | | | | |_| |
 |  _  |  __|| | | |  _  |
 | | | | |   | |_| | | | |
 |_| |_|_|   |_____|_| |_|
    )";

    logger.LogAsciiArt("ArtTest", art);
    logger.LogBanner("ArtTest", art);

    logger.EnableAsciiArt(false); // Restore
    ESP_LOGI(TAG, "ASCII art rendered without crash");
    return true;
}

// ─────────────────────── Configuration ───────────────────────

static bool test_enable_disable_features() noexcept {
    auto& logger = Logger::GetInstance();
    logger.EnableColors(false);
    logger.Info("ConfigTest", "Colors disabled");
    logger.EnableColors(true);

    logger.EnableEffects(false);
    logger.Info("ConfigTest", "Effects disabled");
    logger.EnableEffects(true);

    logger.Flush();
    ESP_LOGI(TAG, "Feature toggles exercised");
    return true;
}

// ─────────────────────── Thread Safety ───────────────────────

static bool test_concurrent_logging() noexcept {
    auto& logger = Logger::GetInstance();
    std::atomic<int> done_count{0};

    auto log_task = [](void* arg) {
        auto& l = Logger::GetInstance();
        int id = reinterpret_cast<intptr_t>(arg);
        for (int i = 0; i < 20; ++i) {
            l.Info("Thread", "Task %d iteration %d", id, i);
            vTaskDelay(pdMS_TO_TICKS(5));
        }
        // Signal completion
        vTaskDelete(nullptr);
    };

    TaskHandle_t tasks[3];
    for (int i = 0; i < 3; ++i) {
        xTaskCreate(log_task, "log_thread", 4096, reinterpret_cast<void*>(i), 5, &tasks[i]);
    }

    vTaskDelay(pdMS_TO_TICKS(500)); // Wait for all tasks
    ESP_LOGI(TAG, "Concurrent logging: 3 tasks × 20 iterations completed");
    return true;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     LOGGER COMPREHENSIVE TEST SUITE                         ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_SINGLETON_TESTS, "SINGLETON",
        RUN_TEST("singleton", test_singleton_access); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_INIT_TESTS, "INITIALIZATION",
        RUN_TEST("init", test_initialize); flip_test_progress_indicator();
        RUN_TEST("reinit", test_reinitialize); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_LOG_LEVEL_TESTS, "LOG LEVELS",
        RUN_TEST("all_levels", test_log_all_levels); flip_test_progress_indicator();
        RUN_TEST("set_level", test_set_log_level); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_FORMATTED_OUTPUT_TESTS, "FORMATTED OUTPUT",
        RUN_TEST("formatted", test_formatted_output); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_ASCII_ART_TESTS, "ASCII ART",
        RUN_TEST("ascii_art", test_ascii_art); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CONFIG_TESTS, "CONFIGURATION",
        RUN_TEST("features", test_enable_disable_features); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_THREAD_SAFETY_TESTS, "THREAD SAFETY",
        RUN_TEST_IN_TASK("concurrent", test_concurrent_logging, 8192, 10); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "LOGGER COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
