/**
 * @file FakeClock.h
 * @brief Controllable time source for hf-core host builds.
 *
 * @details
 * The host build defines HF_RTOS_NONE_EXTERNAL_CLOCK, so every clock read
 * and delay in hf-utils-rtos-wrap (os_time_get, os_delay_msec,
 * os_get_elapsed_time_msec, RtosTime::GetCurrentTimeUs, ...) lands here.
 *
 * - A delay advances the clock by the requested amount and returns at once.
 * - Each clock read advances the clock by AutoAdvanceUs() (default 10 µs),
 *   so code that polls the clock until a timeout expires terminates even
 *   when nothing else moves time forward.
 *
 * Tests call Reset() at the start of each case if they depend on absolute
 * timestamps, and SetAutoAdvanceUs(0) for exact timing checks.
 *
 * @copyright HardFOC
 */

#pragma once

#include <cstdint>

namespace hf::host {

class FakeClock {
public:
    /** @brief Current time in microseconds (does not auto-advance). */
    static uint64_t NowUs() noexcept;

    /** @brief Move time forward by @p us microseconds. */
    static void AdvanceUs(uint64_t us) noexcept;

    /** @brief Move time forward by @p ms milliseconds. */
    static void AdvanceMs(uint64_t ms) noexcept { AdvanceUs(ms * 1000U); }

    /** @brief Time added on every clock read from code under test. */
    static void SetAutoAdvanceUs(uint64_t us) noexcept;
    static uint64_t AutoAdvanceUs() noexcept;

    /** @brief Sum of all delays requested by code under test, in microseconds. */
    static uint64_t TotalDelayUs() noexcept;

    /** @brief Number of delay calls made by code under test. */
    static uint64_t DelayCount() noexcept;

    /** @brief Time back to 0, counters cleared, auto-advance back to default. */
    static void Reset() noexcept;

    static constexpr uint64_t kDefaultAutoAdvanceUs = 10U;
};

}  // namespace hf::host
