/**
 * @file HandlerCommon.h
 * @brief Shared utility functions for handler CRTP communication adapters.
 *
 * Provides common implementations for debug logging routing and delay
 * operations, eliminating code duplication across handler communication
 * adapters (TLE92466ED, TMC5160, TMC9660, MAX22200, etc.).
 *
 * All functions are header-only (inline) so no additional .cpp is needed.
 *
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdio>

#include "Logger.h"

#if defined(ESP_PLATFORM)
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#elif defined(HF_RTOS_FREERTOS)
#include "OsUtility.h"
#elif defined(USE_HAL_DRIVER)
extern "C" void HAL_Delay(uint32_t Delay);
#elif defined(HF_RTOS_THREADX) || defined(HF_RTOS_NONE)
#include "OsUtility.h"
#endif

namespace handler_utils {

/**
 * @brief Route a driver log message to the Logger singleton.
 *
 * Maps integer log levels to Logger methods:
 * - 0 → Error
 * - 1 → Warn
 * - 2 → Info
 * - 3+ → Debug
 *
 * @param level Integer log level from the driver callback.
 * @param tag   Logging tag (null-terminated string).
 * @param format printf-style format string.
 * @param args  Variadic argument list matching @p format.
 */
inline void RouteLogToLogger(int level, const char* tag,
                             const char* format, va_list args) noexcept {
    char buf[256];
    vsnprintf(buf, sizeof(buf), format, args);
    auto& log = Logger::GetInstance();
    switch (level) {
        case 0: log.Error(tag, "%s", buf); break;
        case 1: log.Warn(tag, "%s", buf);  break;
        case 2: log.Info(tag, "%s", buf);  break;
        default: log.Debug(tag, "%s", buf); break;
    }
}

/**
 * @brief RTOS-aware millisecond delay.
 *
 * On ESP-IDF / FreeRTOS, yields via the scheduler so TLE/MAX reset settle
 * times (datasheet ≥10 ms) are real wall time — not a short `volatile`
 * busy-wait that was ~3×–10× too short at 200–240 MHz.
 *
 * A non-zero @p ms always sleeps at least one tick, the rule
 * `os_convert_msec_to_delay_ticks()` in hf-utils-rtos-wrap follows: below
 * 1 kHz `pdMS_TO_TICKS()` rounds a short delay down to 0 ticks, which
 * would only yield. HardFOC runs FreeRTOS at 1 kHz, where 1 ms is 1 tick.
 *
 * @param ms Delay duration in milliseconds.
 */
inline void DelayMs(uint32_t ms) noexcept {
#if defined(ESP_PLATFORM)
    if (ms == 0U) {
        return;
    }
    TickType_t ticks = pdMS_TO_TICKS(ms);
    if (ticks == 0U) {
        ticks = 1U;
    }
    vTaskDelay(ticks);
#elif defined(HF_RTOS_FREERTOS) || (!defined(USE_HAL_DRIVER) && (defined(HF_RTOS_THREADX) || defined(HF_RTOS_NONE)))
    while (ms > 0U) {
        const uint16_t chunk =
            (ms > 60000U) ? static_cast<uint16_t>(60000U)
                          : static_cast<uint16_t>(ms);
        os_delay_msec(chunk);
        ms -= chunk;
    }
#elif defined(USE_HAL_DRIVER)
    HAL_Delay(ms);
#else
    /* Last-resort busy-wait (~1 ms @ ~200 MHz Cortex-M). Prefer RTOS/HAL. */
    for (uint32_t m = 0; m < ms; ++m) {
        for (uint32_t spin = 40000U; spin > 0U; --spin) {
            __asm__ volatile("" ::: "memory");
        }
    }
#endif
}

/**
 * @brief Microsecond delay with automatic fallback to RTOS delay for large values.
 *
 * For delays >= 1 ms, delegates to @ref DelayMs (scheduler / HAL), rounded
 * up to whole milliseconds so a datasheet minimum such as 2.5 ms is not cut
 * to 2 ms. Shorter delays use `esp_rom_delay_us` on ESP-IDF or a denser
 * busy-wait elsewhere so SPI CS setup / inter-frame gaps are not no-ops.
 *
 * @param us Delay duration in microseconds.
 */
inline void DelayUs(uint32_t us) noexcept {
    if (us >= 1000U) {
        DelayMs((us + 999U) / 1000U);
        return;
    }
#if defined(ESP_PLATFORM)
    esp_rom_delay_us(us);
#else
    /* ~1 µs/iteration @ Cortex-M ~200–240 MHz. */
    for (uint32_t i = 0; i < us; ++i) {
        for (uint32_t spin = 40U; spin > 0U; --spin) {
            __asm__ volatile("" ::: "memory");
        }
    }
#endif
}

} // namespace handler_utils
