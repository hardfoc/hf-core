/**
 * @file test_pf1550.cpp
 * @brief HalPf1550Comm, the PF1550 bus adapter: its DelayUs() hook, which the driver requires
 *        (hf-pf1550-driver) for the profile delays and the 2 ms OTP read wait, waits at least
 *        the time asked, for any uint32_t microsecond value.
 */

#include "HfCoreMocks.h"
#include "HostTest.h"
#include "Pf1550Handler.h"

using hf::host::FakeClock;
using hf::host::MockI2c;

namespace {

uint64_t DelayOf(uint32_t us) {
    MockI2c i2c;
    HalPf1550Comm comm(i2c, nullptr, nullptr, nullptr);
    FakeClock::Reset();
    FakeClock::SetAutoAdvanceUs(0);
    const uint64_t before = FakeClock::TotalDelayUs();
    comm.DelayUs(us);
    return FakeClock::TotalDelayUs() - before;
}

}  // namespace

HF_TEST(Pf1550Comm_DelayUsWaitsTheOtpReadTime) {
    CHECK(DelayOf(2000U) >= 2000U);  // ReadOtpByte(): 2 ms between FMRADDR and FMRDATA
}

HF_TEST(Pf1550Comm_DelayUsWaitsAShortProfileDelay) {
    CHECK(DelayOf(1500U) >= 1500U);  // rounded up to whole milliseconds, never down
}

HF_TEST(Pf1550Comm_DelayUsDoesNotWrapLongDelays) {
    // 70 s does not fit a uint16_t of milliseconds; the old adapter waited 70000 % 65536 = 4464 ms.
    CHECK(DelayOf(70'000'000U) >= 70'000'000U);
    CHECK(DelayOf(0xFFFFFFFFU) >= 0xFFFFFFFFULL);
}
