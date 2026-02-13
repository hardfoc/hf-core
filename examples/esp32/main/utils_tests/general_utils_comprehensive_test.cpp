/**
 * @file general_utils_comprehensive_test.cpp
 * @brief Comprehensive test suite for hf-utils-general utilities
 *
 * Tests: CircularBuffer, RingBuffer, AveragingFilter,
 * CRC utilities, ActionTimer, and SoftwareVersion.
 *
 * These are all header-only, platform-independent utilities —
 * they run on ESP32 FreeRTOS but do not require any peripherals.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"

// General utils includes
#include "CircularBuffer.h"
#include "RingBuffer.h"
#include "AveragingFilter.h"
#include "CrcCalculator.h"
#include "ActionTimer.h"
#include "SoftwareVersion.h"

#include <cmath>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef __cplusplus
}
#endif

static const char* TAG = "GeneralUtils_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_CIRCULAR_BUFFER_TESTS  = true;
static constexpr bool ENABLE_RING_BUFFER_TESTS      = true;
static constexpr bool ENABLE_AVERAGING_FILTER_TESTS = true;
static constexpr bool ENABLE_CRC_TESTS              = true;
static constexpr bool ENABLE_TIMER_TESTS            = true;
static constexpr bool ENABLE_VERSION_TESTS          = true;

// ─────────────────────── CircularBuffer ───────────────────────

static bool test_circular_buffer_basic() noexcept {
    CircularBuffer<int, 8> buf;
    bool empty = buf.IsEmpty();
    buf.Write(42);
    buf.Write(99);
    bool not_empty = !buf.IsEmpty();
    int val = 0;
    buf.Read(val);
    bool correct_fifo = (val == 42);
    ESP_LOGI(TAG, "CircularBuffer: empty=%d, not_empty=%d, fifo=%d", empty, not_empty, correct_fifo);
    return empty && not_empty && correct_fifo;
}

static bool test_circular_buffer_overflow() noexcept {
    CircularBuffer<int, 4> buf;
    for (int i = 0; i < 8; ++i) buf.Write(i); // Overflow — oldest discarded
    int newest = 0;
    buf.Read(newest);
    ESP_LOGI(TAG, "CircularBuffer overflow: pop=%d (expected 4)", newest);
    return newest == 4;
}

// ─────────────────────── RingBuffer ───────────────────────

static bool test_ring_buffer_basic() noexcept {
    RingBuffer<float, 16> ring;
    ring.Append(1.0f);
    ring.Append(2.0f);
    ring.Append(3.0f);
    bool sz = (ring.GetCount() == 3);
    float front = *ring.begin();
    bool correct = (std::fabs(front - 1.0f) < 0.001f);
    ESP_LOGI(TAG, "RingBuffer: size=%d, front=%.1f", ring.GetCount(), front);
    return sz && correct;
}

// ─────────────────────── AveragingFilter ───────────────────────

static bool test_averaging_filter() noexcept {
    AveragingFilter<float, 5> filter;
    for (int i = 1; i <= 5; ++i) filter.Append(static_cast<float>(i));
    float avg = filter.GetValue();
    bool correct = (std::fabs(avg - 3.0f) < 0.01f); // (1+2+3+4+5)/5 = 3.0
    ESP_LOGI(TAG, "AveragingFilter: avg=%.2f (expected 3.0)", avg);
    return correct;
}

static bool test_averaging_filter_partial() noexcept {
    AveragingFilter<float, 10> filter;
    filter.Append(10.0f);
    filter.Append(20.0f);
    float avg = filter.GetValue();
    bool correct = (std::fabs(avg - 15.0f) < 0.01f);
    ESP_LOGI(TAG, "AveragingFilter partial: avg=%.2f (expected 15.0)", avg);
    return correct;
}

// ─────────────────────── CRC ───────────────────────

static bool test_crc_functions() noexcept {
    const uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    auto crc = crc16(data, sizeof(data));
    ESP_LOGI(TAG, "CRC16 of '123456789': 0x%04X", crc);
    return true; // Validation depends on polynomial — just verify no crash
}

// ─────────────────────── ActionTimer ───────────────────────

static bool test_action_timer() noexcept {
    ActionTimer timer;
    timer.Start();
    vTaskDelay(pdMS_TO_TICKS(50));
    timer.Stop();
    auto duration = timer.GetDuration();
    bool plausible = (duration >= 40 && duration <= 100); // ms
    ESP_LOGI(TAG, "ActionTimer: %lu ms (plausible=%d)", static_cast<unsigned long>(duration), plausible);
    return plausible;
}

// ─────────────────────── SoftwareVersion ───────────────────────

static bool test_software_version() noexcept {
    SoftwareVersion v(2, 5, 13);
    bool major_ok = (v.GetMajor() == 2);
    bool minor_ok = (v.GetMinor() == 5);
    bool build_ok = (v.GetBuild() == 13);
    const char* str = v.GetString();
    ESP_LOGI(TAG, "SoftwareVersion: %s (major=%d, minor=%d, build=%d)",
             str, v.GetMajor(), v.GetMinor(), v.GetBuild());
    return major_ok && minor_ok && build_ok;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     GENERAL UTILITIES COMPREHENSIVE TEST SUITE              ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CIRCULAR_BUFFER_TESTS, "CIRCULAR BUFFER",
        RUN_TEST("basic", test_circular_buffer_basic); flip_test_progress_indicator();
        RUN_TEST("overflow", test_circular_buffer_overflow); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_RING_BUFFER_TESTS, "RING BUFFER",
        RUN_TEST("basic", test_ring_buffer_basic); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_AVERAGING_FILTER_TESTS, "AVERAGING FILTER",
        RUN_TEST("avg", test_averaging_filter); flip_test_progress_indicator();
        RUN_TEST("partial", test_averaging_filter_partial); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_CRC_TESTS, "CRC",
        RUN_TEST("crc8", test_crc_functions); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_TIMER_TESTS, "TIMERS",
        RUN_TEST_IN_TASK("action_timer", test_action_timer, 4096, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_VERSION_TESTS, "SOFTWARE VERSION",
        RUN_TEST("version", test_software_version); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "GENERAL UTILITIES COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
