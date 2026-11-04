/**
 * @file Delay.h
 * @brief `hf::DelayUs()` and `hf::DelayMs()`: the delays used above the platform bind.
 *
 * standards/threading-and-timing.md: a delay of 1 ms or more blocks the task through rtos-wrap's
 * `os_delay_msec()`, rounded up to whole milliseconds so a datasheet minimum of 2.5 ms never
 * becomes 2 ms; a shorter one busy-waits on `hf_time::MonotonicUs()` and makes no RTOS call.
 * Nothing here tests an MCU macro: the MCU-specific parts are in `hf_time` and rtos-wrap.
 *
 * Busy-waits longer than about 100 us should be avoided in tasks above idle priority.
 */
#pragma once

#include <cstdint>

#include "HfTime.h"
#include "OsUtility.h"

namespace hf {

/// Block the calling task for at least @p ms milliseconds (0 returns at once).
inline void DelayMs(std::uint32_t ms) noexcept {
  while (ms > 0U) {
    const std::uint32_t chunk = ms > 0xFFFFU ? 0xFFFFU : ms;
    os_delay_msec(static_cast<std::uint16_t>(chunk));
    ms -= chunk;
  }
}

/// Wait at least @p us microseconds: busy-wait below 1 ms, `DelayMs()` rounded up from 1 ms.
inline void DelayUs(std::uint32_t us) noexcept {
  if (us >= 1000U) {
    DelayMs(us / 1000U + ((us % 1000U) != 0U ? 1U : 0U));
    return;
  }
  if (us == 0U) {
    return;
  }
  const std::uint64_t deadline_us = hf_time::MonotonicUs() + us;
  while (static_cast<std::int64_t>(hf_time::MonotonicUs() - deadline_us) < 0) {
  }
}

} // namespace hf
