/**
 * @file test_rtos_host.cpp
 * @brief hf-utils-rtos-wrap on the host: fake clock, delays, mutexes, BaseThread.
 */

#include "BaseThread.h"
#include "FakeClock.h"
#include "HostTest.h"
#include "OsUtility.h"
#include "RtosMutex.h"

using hf::host::FakeClock;

HF_TEST(Clock_DelaysAdvanceTime) {
    FakeClock::SetAutoAdvanceUs(0);
    const uint32_t t0 = os_get_elapsed_time_msec();
    os_delay_msec(25);
    CHECK_EQ(os_get_elapsed_time_msec() - t0, 25u);
    CHECK_EQ(FakeClock::TotalDelayUs(), 25000u);

    os_delay_time(3, TIME_UNIT_S);
    CHECK_EQ(os_get_elapsed_time_msec() - t0, 3025u);
    os_delay_time(500, TIME_UNIT_US);
    CHECK_EQ(FakeClock::NowUs(), 3025500u);
    CHECK_EQ(FakeClock::DelayCount(), 3u);
}

HF_TEST(Clock_RtosTimeFollowsFakeClock) {
    FakeClock::SetAutoAdvanceUs(0);
    FakeClock::AdvanceMs(7);
    CHECK_EQ(RtosTime::GetCurrentTimeUs(), 7000u);
    CHECK_EQ(static_cast<uint32_t>(os_time_get()), 7u);  // 1 tick == 1 ms on this backend
}

HF_TEST(Clock_PollingLoopTerminatesWithAutoAdvance) {
    // A loop that waits for a timeout by polling the clock, as driver code does,
    // finishes because each read moves time forward.
    const uint64_t start = RtosTime::GetCurrentTimeUs();
    uint32_t polls = 0;
    while (RtosTime::GetCurrentTimeUs() - start < 200000U) {
        ++polls;
    }
    CHECK(polls > 0u);
    CHECK(polls <= 200000u / FakeClock::kDefaultAutoAdvanceUs);
}

HF_TEST(RtosMutex_IsRecursiveAndLockable) {
    RtosMutex m;
    CHECK(m.lock());
    CHECK(m.lock());
    m.unlock();
    m.unlock();
    {
        MutexLockGuard guard(m);
        CHECK(guard.IsLocked());
    }
}

namespace {

class CountingThread : public BaseThread {
public:
    CountingThread() : BaseThread("counting") {}
    int setups{0};
    int steps{0};
    int cleanups{0};

    bool Setup() noexcept override {
        ++setups;
        return true;
    }
    uint32_t Step() noexcept override {
        ++steps;
        return 10;
    }
    bool Cleanup() noexcept override {
        ++cleanups;
        return true;
    }
    bool Initialize() noexcept override { return true; }
    bool ResetVariables() noexcept override {
        steps = 0;
        return true;
    }
};

}  // namespace

HF_TEST(BaseThread_HostStartRunsSetupOnceAndCallerDrivesStep) {
    CountingThread t;
    CHECK(!t.IsThreadRunning());
    CHECK(t.StartThreadAndWaitToVerify(100));
    CHECK(t.IsThreadRunning());
    CHECK(t.IsSetupComplete());
    CHECK_EQ(t.setups, 1);

    // No scheduler: the test pumps Step() and applies the returned delay itself.
    for (int i = 0; i < 3; ++i) {
        const uint32_t wait_ms = t.Step();
        os_delay_msec(static_cast<uint16_t>(wait_ms));
    }
    CHECK_EQ(t.steps, 3);

    CHECK(t.StartThreadAndWaitToVerify(100));
    CHECK_EQ(t.setups, 1);  // setup is not repeated while complete

    CHECK(t.StopThreadAndWaitToVerify(100));
    CHECK(t.IsThreadStopped());
    CHECK(t.IsThreadStopRequested());
    CHECK_EQ(t.GetStackHighWaterMark(), 0u);
}
