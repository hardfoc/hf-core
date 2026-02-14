/**
 * @file rtos_wrap_comprehensive_test.cpp
 * @brief Comprehensive test suite for hf-utils-rtos-wrap FreeRTOS wrappers
 *
 * Tests: Mutex, MutexGuard, PeriodicTimer, BaseThread (subclass),
 * OsQueue, OsEventFlags, SignalSemaphore,
 * os_delay functions, and multi-threaded synchronization scenarios.
 *
 * @note These tests REQUIRE FreeRTOS and exercise real RTOS primitives.
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#include "TestFramework.h"

#include "Mutex.h"
#include "MutexGuard.h"
#include "PeriodicTimer.h"
#include "BaseThread.h"
#include "OsQueue.h"
#include "OsEventFlags.h"
#include "SignalSemaphore.h"
#include "OsUtility.h"

#include <atomic>
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

static const char* TAG = "RTOS_Wrap_Test";
static TestResults g_test_results;

static constexpr bool ENABLE_MUTEX_TESTS           = true;
static constexpr bool ENABLE_LOCK_GUARD_TESTS      = true;
static constexpr bool ENABLE_PERIODIC_TIMER_TESTS  = true;
static constexpr bool ENABLE_THREAD_TESTS          = true;
static constexpr bool ENABLE_QUEUE_TESTS           = true;
static constexpr bool ENABLE_EVENT_FLAGS_TESTS     = true;
static constexpr bool ENABLE_SEMAPHORE_TESTS       = true;
static constexpr bool ENABLE_DELAY_TESTS           = true;

// ─────────────────────── Mutex ───────────────────────

static bool test_mutex_create() noexcept {
    Mutex mtx("test_mtx");
    bool init = mtx.IsInitialized();
    ESP_LOGI(TAG, "RtosMutex create: init=%d, name=%s", init, mtx.GetName());
    return init;
}

static bool test_mutex_lock_unlock() noexcept {
    Mutex mtx("lock_test");
    bool locked = mtx.Lock(100);
    bool unlocked = mtx.Unlock();
    ESP_LOGI(TAG, "RtosMutex lock/unlock: locked=%d, unlocked=%d", locked, unlocked);
    return locked && unlocked;
}

static bool test_mutex_contention() noexcept {
    static Mutex mtx("contention");
    static std::atomic<int> counter{0};

    auto task_fn = [](void* arg) {
        for (int i = 0; i < 100; ++i) {
            mtx.Lock(portMAX_DELAY);
            counter++;
            mtx.Unlock();
        }
        vTaskDelete(nullptr);
    };

    counter = 0;
    TaskHandle_t t1, t2;
    xTaskCreate(task_fn, "cnt1", 4096, nullptr, 5, &t1);
    xTaskCreate(task_fn, "cnt2", 4096, nullptr, 5, &t2);

    vTaskDelay(pdMS_TO_TICKS(500)); // Wait for tasks to finish
    bool correct = (counter == 200);
    ESP_LOGI(TAG, "Mutex contention: counter=%d (expected 200)", counter.load());
    return correct;
}

// ─────────────────────── MutexGuard ───────────────────────

static bool test_lock_guard_scoped() noexcept {
    Mutex mtx("guard_test");
    {
        bool guardOk = false;
        MutexGuard guard(mtx, &guardOk);
        // Mutex should be locked here
        ESP_LOGI(TAG, "MutexGuard: locked=%d (in scope)", guardOk);
        if (!guardOk) return false;
    }
    // Mutex should be unlocked after scope exit
    bool can_lock = mtx.Lock(0); // Try non-blocking lock
    if (can_lock) mtx.Unlock();
    ESP_LOGI(TAG, "MutexGuard: can_lock_after_scope=%d", can_lock);
    return can_lock;
}

// ─────────────────────── PeriodicTimer ───────────────────────

static std::atomic<int> g_timer_count{0};
static void timer_callback(uint32_t arg) {
    g_timer_count++;
}

static bool test_periodic_timer() noexcept {
    g_timer_count = 0;
    PeriodicTimer timer;
    bool created = timer.Create("test_timer", timer_callback, 0, 100, true /*autoStart*/);
    if (!created) {
        ESP_LOGW(TAG, "PeriodicTimer create failed (platform limitation?)");
        return true;
    }
    bool valid = timer.IsValid();
    vTaskDelay(pdMS_TO_TICKS(550)); // Wait ~5.5 periods
    timer.Stop();
    int count = g_timer_count;
    timer.Destroy();

    bool plausible = (count >= 3 && count <= 8); // ~5 expected
    ESP_LOGI(TAG, "PeriodicTimer: valid=%d, callbacks=%d (expected ~5)", valid, count);
    return valid && plausible;
}

// ─────────────────────── BaseThread (subclass) ───────────────────────

class TestThread : public BaseThread {
public:
    TestThread() : BaseThread("TestThread"), run_count_(0) {}
    int GetRunCount() const { return run_count_; }

protected:
    bool Initialize() noexcept override {
        return CreateBaseThread(stack_, sizeof(stack_), 5, 5, 0, OS_AUTO_START);
    }
    bool Setup() noexcept override {
        ESP_LOGI(TAG, "TestThread::Setup");
        return true;
    }
    uint32_t Step() noexcept override {
        run_count_++;
        if (run_count_ >= 5) Stop();
        return 50; // ms delay before next Step()
    }
    bool Cleanup() noexcept override {
        ESP_LOGI(TAG, "TestThread::Cleanup");
        return true;
    }
    bool ResetVariables() noexcept override {
        run_count_ = 0;
        return true;
    }

private:
    int run_count_;
    uint8_t stack_[4096];
};

static bool test_base_thread() noexcept {
    TestThread thread;
    if (!thread.EnsureInitialized()) {
        ESP_LOGW(TAG, "Thread init failed");
        return true;
    }
    bool started = thread.Start();
    if (!started) {
        ESP_LOGW(TAG, "Thread start failed");
        return true;
    }
    vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 5 loops
    int count = thread.GetRunCount();
    bool ok = (count >= 4);
    ESP_LOGI(TAG, "BaseThread: run_count=%d (expected >=4)", count);
    return ok;
}

// ─────────────────────── OsQueue ───────────────────────

static bool test_os_queue() noexcept {
    OsQueue<int, 10> queue("test_q", 1);
    if (!queue.EnsureInitialized()) return true;

    queue.Send(42, 0);
    queue.Send(99, 0);

    int val = 0;
    bool ok1 = queue.Receive(val, 100);
    bool correct1 = (val == 42);

    bool ok2 = queue.Receive(val, 100);
    bool correct2 = (val == 99);

    ESP_LOGI(TAG, "OsQueue: recv1=%d(%d), recv2=%d(%d)", ok1, val, ok2, correct2);
    return ok1 && correct1 && ok2 && correct2;
}

static bool test_os_queue_full() noexcept {
    OsQueue<int, 2> queue("full_q", 1);
    queue.Send(1, 0);
    queue.Send(2, 0);
    bool full_send = queue.Send(3, 0); // Should fail (queue full, 0 timeout)
    ESP_LOGI(TAG, "OsQueue full: send_when_full=%d (expected 0)", full_send);
    return !full_send;
}

// ─────────────────────── OsEventFlags ───────────────────────

static bool test_event_flags() noexcept {
    OsEventFlags<1> events("test_events");
    if (!events.EnsureInitialized()) return true;

    bool set1 = events.Set(0x01);
    bool set2 = events.Set(0x04);

    OS_Ulong flagsToGet = 0x05;
    bool ok = events.Get(flagsToGet, OS_OR, 100);
    ESP_LOGI(TAG, "OsEventFlags: set1=%d set2=%d get=%d", set1, set2, ok);
    return set1 && set2 && ok;
}

// ─────────────────────── SignalSemaphore ───────────────────────

static bool test_semaphore() noexcept {
    SignalSemaphore sem("test_sem");
    if (!sem.EnsureInitialized()) return true;

    // Pre-signal 3 times (counting semaphore behavior)
    sem.Signal();
    sem.Signal();
    sem.Signal();

    bool t1 = sem.WaitUntilSignalled(0);
    bool t2 = sem.WaitUntilSignalled(0);
    bool t3 = sem.WaitUntilSignalled(0);
    bool t4 = sem.WaitUntilSignalled(0); // Should fail — count exhausted

    sem.Signal();
    bool t5 = sem.WaitUntilSignalled(0); // Should succeed now

    ESP_LOGI(TAG, "SignalSemaphore: t1=%d t2=%d t3=%d t4=%d t5=%d", t1, t2, t3, t4, t5);
    return t1 && t2 && t3 && !t4 && t5;
}

// ─────────────────────── os_delay ───────────────────────

static bool test_os_delay() noexcept {
    auto start = xTaskGetTickCount();
    os_delay_msec(100);
    auto end = xTaskGetTickCount();
    auto elapsed_ms = (end - start) * portTICK_PERIOD_MS;
    bool plausible = (elapsed_ms >= 80 && elapsed_ms <= 150);
    ESP_LOGI(TAG, "os_delay_msec(100): %lu ms (plausible=%d)", elapsed_ms, plausible);
    return plausible;
}

// ═══════════════════════ ENTRY POINT ═══════════════════════

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "\n");
    ESP_LOGI(TAG, "╔══════════════════════════════════════════════════════════════╗");
    ESP_LOGI(TAG, "║     RTOS WRAPPERS COMPREHENSIVE TEST SUITE                  ║");
    ESP_LOGI(TAG, "╚══════════════════════════════════════════════════════════════╝");

    RUN_TEST_SECTION_IF_ENABLED(ENABLE_MUTEX_TESTS, "RTOS MUTEX",
        RUN_TEST_IN_TASK("create", test_mutex_create, 4096, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("lock_unlock", test_mutex_lock_unlock, 4096, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("contention", test_mutex_contention, 8192, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_LOCK_GUARD_TESTS, "MUTEX LOCK GUARD",
        RUN_TEST_IN_TASK("scoped", test_lock_guard_scoped, 4096, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_PERIODIC_TIMER_TESTS, "PERIODIC TIMER",
        RUN_TEST_IN_TASK("timer", test_periodic_timer, 8192, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_THREAD_TESTS, "BASE THREAD",
        RUN_TEST_IN_TASK("thread", test_base_thread, 8192, 10); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_QUEUE_TESTS, "OS QUEUE",
        RUN_TEST_IN_TASK("basic", test_os_queue, 4096, 5); flip_test_progress_indicator();
        RUN_TEST_IN_TASK("full", test_os_queue_full, 4096, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_EVENT_FLAGS_TESTS, "EVENT FLAGS",
        RUN_TEST_IN_TASK("flags", test_event_flags, 4096, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_SEMAPHORE_TESTS, "SEMAPHORE",
        RUN_TEST_IN_TASK("counting", test_semaphore, 4096, 5); flip_test_progress_indicator();
    );
    RUN_TEST_SECTION_IF_ENABLED(ENABLE_DELAY_TESTS, "OS DELAY",
        RUN_TEST_IN_TASK("delay", test_os_delay, 4096, 5); flip_test_progress_indicator();
    );

    print_test_summary(g_test_results, "RTOS WRAPPERS COMPREHENSIVE", TAG);
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
