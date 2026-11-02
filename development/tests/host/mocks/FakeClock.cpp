/**
 * @file FakeClock.cpp
 * @brief FakeClock state and the hf-utils-rtos-wrap clock hooks it backs.
 *
 * @copyright HardFOC
 */

#include "FakeClock.h"

#include <atomic>

namespace {

std::atomic<uint64_t> g_now_us{0};
std::atomic<uint64_t> g_auto_advance_us{hf::host::FakeClock::kDefaultAutoAdvanceUs};
std::atomic<uint64_t> g_total_delay_us{0};
std::atomic<uint64_t> g_delay_count{0};

}  // namespace

namespace hf::host {

uint64_t FakeClock::NowUs() noexcept { return g_now_us.load(); }

void FakeClock::AdvanceUs(uint64_t us) noexcept { g_now_us.fetch_add(us); }

void FakeClock::SetAutoAdvanceUs(uint64_t us) noexcept { g_auto_advance_us.store(us); }

uint64_t FakeClock::AutoAdvanceUs() noexcept { return g_auto_advance_us.load(); }

uint64_t FakeClock::TotalDelayUs() noexcept { return g_total_delay_us.load(); }

uint64_t FakeClock::DelayCount() noexcept { return g_delay_count.load(); }

void FakeClock::Reset() noexcept {
    g_now_us.store(0);
    g_auto_advance_us.store(kDefaultAutoAdvanceUs);
    g_total_delay_us.store(0);
    g_delay_count.store(0);
}

}  // namespace hf::host

// Hooks declared by hf-utils-rtos-wrap (include/os/os_none.h) when
// HF_RTOS_NONE_EXTERNAL_CLOCK is defined.
extern "C" uint64_t hf_os_none_clock_us(void) {
    return g_now_us.fetch_add(g_auto_advance_us.load()) + g_auto_advance_us.load();
}

extern "C" void hf_os_none_delay_us(uint64_t us) {
    g_total_delay_us.fetch_add(us);
    g_delay_count.fetch_add(1);
    g_now_us.fetch_add(us);
}
