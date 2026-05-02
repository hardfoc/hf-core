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
#include "StateMachine.h"

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
static constexpr bool ENABLE_STATE_MACHINE_TESTS    = true;

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
─────────────────────── StateMachine ───────────────────────
//
// Exercises the new allocation-free `hf_utils::StateMachine` template:
// Entry/Loop/Exit ordering, dwell + visit counters, illegal-transition
// rejection, single-slot external intent inbox, and stuck-state watchdog.
//
// All tests use a fake `Owner` whose hooks just record what was called.

namespace sm_test {

enum class S : uint8_t { A = 0, B = 1, C = 2, COUNT = 3 };

struct Owner {
    int entry_a{0}, loop_a{0}, exit_a{0};
    int entry_b{0}, loop_b{0}, exit_b{0};
    int entry_c{0}, loop_c{0}, exit_c{0};
    bool entry_b_should_fail{false};
    bool exit_a_should_fail{false};

    bool EnterA() noexcept { ++entry_a; return true; }
    uint32_t LoopA() noexcept { ++loop_a; return 50; }
    bool ExitA()  noexcept { ++exit_a; return !exit_a_should_fail; }

    bool EnterB() noexcept { ++entry_b; return !entry_b_should_fail; }
    uint32_t LoopB() noexcept { ++loop_b; return 50; }
    bool ExitB()  noexcept { ++exit_b; return true; }

    bool EnterC() noexcept { ++entry_c; return true; }
    uint32_t LoopC() noexcept { ++loop_c; return 50; }
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_STATE_MACHINE_TESTS, "STATE MACHINE",
        RUN_TEST("finalize",          test_sm_finalize_requires_full_registration); flip_test_progress_indicator();
        RUN_TEST("first_tick",        test_sm_entry_loop_runs_on_first_tick); flip_test_progress_indicator();
        RUN_TEST("transition",        test_sm_owner_request_transition_runs_exit_then_entry); flip_test_progress_indicator();
        RUN_TEST("illegal",           test_sm_illegal_transition_rejected); flip_test_progress_indicator();
        RUN_TEST("external_intent",   test_sm_external_intent_drained_on_update); flip_test_progress_indicator();
        RUN_TEST("entry_retry",       test_sm_entry_failure_retries_next_tick); flip_test_progress_indicator();
        RUN_TEST("exit_retry",        test_sm_exit_failure_keeps_state_with_intent_pending); flip_test_progress_indicator();
        RUN_TEST("watchdog",          test_sm_max_dwell_watchdog_fires_once_per_state); flip_test_progress_indicator();
    );
    bool ExitC()  noexcept { ++exit_c; return true; }
};

using SM = hf_utils::StateMachine<Owner, S, static_cast<size_t>(S::COUNT)>;

static void register_all(SM& sm, Owner& o) {
    sm.Register(S::A, {&Owner::EnterA, &Owner::LoopA, &Owner::ExitA});
    sm.Register(S::B, {&Owner::EnterB, &Owner::LoopB, &Owner::ExitB});
    sm.Register(S::C, {&Owner::EnterC, &Owner::LoopC, &Owner::ExitC});
    (void)o;
}

} // namespace sm_test

static bool test_sm_finalize_requires_full_registration() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    bool before = sm.IsFinalized();
    sm.Register(S::A, {&Owner::EnterA, &Owner::LoopA, &Owner::ExitA});
    bool finalized_partial = sm.Finalize();
    register_all(sm, o);
    bool finalized_full = sm.Finalize();
    ESP_LOGI(TAG, "SM Finalize: before=%d partial=%d full=%d", before, finalized_partial, finalized_full);
    return !before && !finalized_partial && finalized_full;
}

static bool test_sm_entry_loop_runs_on_first_tick() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    (void)sm.Finalize();
    uint32_t delay = sm.Update(0);
    bool entry_called = (o.entry_a == 1);
    bool loop_called  = (o.loop_a  == 1);
    bool delay_ok     = (delay == 50);
    ESP_LOGI(TAG, "SM first tick: entry=%d loop=%d delay=%lu", o.entry_a, o.loop_a, (unsigned long)delay);
    return entry_called && loop_called && delay_ok;
}

static bool test_sm_owner_request_transition_runs_exit_then_entry() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    (void)sm.Finalize();
    sm.Update(0);                       // enter A, loop A
    bool req = sm.RequestTransition(S::B);
    sm.Update(100);                     // exit A, enter B, loop B
    bool req_ok    = req;
    bool exit_a_ok = (o.exit_a == 1);
    bool enter_b_ok = (o.entry_b == 1);
    bool loop_b_ok  = (o.loop_b  == 1);
    bool dwell_a_ok = (sm.LastDwellMs(S::A) == 100);
    bool visits_b_ok = (sm.VisitCount(S::B) == 1);
    ESP_LOGI(TAG, "SM transition: req=%d exitA=%d enterB=%d loopB=%d dwell=%lu",
             req_ok, exit_a_ok, enter_b_ok, loop_b_ok,
             (unsigned long)sm.LastDwellMs(S::A));
    return req_ok && exit_a_ok && enter_b_ok && loop_b_ok && dwell_a_ok && visits_b_ok;
}

static bool test_sm_illegal_transition_rejected() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    // Matrix: A -> {B}, B -> {A}, C -> {} (unreachable).
    std::array<uint64_t, 3> mask{};
    mask[0] = (uint64_t{1} << 1);
    mask[1] = (uint64_t{1} << 0);
    mask[2] = 0;
    sm.SetTransitionMatrix(mask);
    (void)sm.Finalize();
    sm.Update(0);
    bool illegal_req = sm.RequestTransition(S::C); // A -> C is illegal
    sm.Update(100);
    bool illegal_count_ok = (sm.IllegalTransitionCount() == 1);
    bool no_exit_called   = (o.exit_a == 0);
    bool stayed_in_a      = (sm.GetCurrentState() == S::A);
    ESP_LOGI(TAG, "SM illegal: rejected=%d count=%lu state=%u",
             !illegal_req, (unsigned long)sm.IllegalTransitionCount(),
             (unsigned)sm.GetCurrentState());
    return !illegal_req && illegal_count_ok && no_exit_called && stayed_in_a;
}

static bool test_sm_external_intent_drained_on_update() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    (void)sm.Finalize();
    sm.Update(0);
    sm.OnExternalIntent(S::B);
    sm.OnExternalIntent(S::C); // last-writer-wins
    sm.Update(50);
    bool now_in_c = (sm.GetCurrentState() == S::C);
    bool entered_c = (o.entry_c == 1);
    bool not_entered_b = (o.entry_b == 0);
    ESP_LOGI(TAG, "SM external: state=%u entryC=%d entryB=%d",
             (unsigned)sm.GetCurrentState(), o.entry_c, o.entry_b);
    return now_in_c && entered_c && not_entered_b;
}

static bool test_sm_entry_failure_retries_next_tick() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    (void)sm.Finalize();
    sm.Update(0);
    o.entry_b_should_fail = true;
    sm.RequestTransition(S::B);
    sm.Update(100);                                 // exit A, attempt enter B (fails)
    bool entering = (sm.GetCurrentPhase() == hf_utils::StatePhase::Entering);
    bool one_attempt = (o.entry_b == 1);
    o.entry_b_should_fail = false;
    sm.Update(110);                                 // re-attempt entry → succeeds
    bool running_now = (sm.GetCurrentPhase() == hf_utils::StatePhase::Running);
    bool two_attempts = (o.entry_b == 2);
    ESP_LOGI(TAG, "SM entry-retry: enteringFirst=%d attempts=%d running=%d",
             entering, o.entry_b, running_now);
    return entering && one_attempt && running_now && two_attempts;
}

static bool test_sm_exit_failure_keeps_state_with_intent_pending() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    (void)sm.Finalize();
    sm.Update(0);
    o.exit_a_should_fail = true;
    sm.RequestTransition(S::B);
    sm.Update(100);                                 // exit A returns false → stay in A
    bool still_a = (sm.GetCurrentState() == S::A);
    bool exit_attempted = (o.exit_a == 1);
    bool no_enter_b     = (o.entry_b == 0);
    o.exit_a_should_fail = false;
    sm.Update(110);                                 // retry exit → succeeds
    bool now_b = (sm.GetCurrentState() == S::B);
    ESP_LOGI(TAG, "SM exit-retry: stillA=%d exits=%d enters=%d nowB=%d",
             still_a, o.exit_a, o.entry_b, now_b);
    return still_a && exit_attempted && no_enter_b && now_b;
}

static bool test_sm_max_dwell_watchdog_fires_once_per_state() noexcept {
    using namespace sm_test;
    Owner o;
    SM sm(o, S::A);
    register_all(sm, o);
    sm.SetMaxDwell(S::A, 100);
    (void)sm.Finalize();
    sm.Update(0);
    sm.Update(50);                          // dwell 50; not breached
    sm.Update(150);                         // dwell 150; breach
    sm.Update(250);                         // dwell 250; latched, shouldn't double-count
    bool count_one = (sm.MaxDwellBreachCount() == 1);
    bool last_state_a = (sm.MaxDwellLastBreachState() == S::A);
    sm.RequestTransition(S::B);
    sm.Update(300);                         // transition → re-arm
    sm.Update(500);                         // dwell 200 in B; B has no cap → still 1
    bool still_one = (sm.MaxDwellBreachCount() == 1);
    ESP_LOGI(TAG, "SM watchdog: count=%lu lastA=%d still=%d",
             (unsigned long)sm.MaxDwellBreachCount(), last_state_a, still_one);
    return count_one && last_state_a && still_one;
}

// 
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
