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
 * On ESP-IDF, yields to the RTOS scheduler via vTaskDelay.
 * On other platforms, falls back to a busy-wait loop.
 *
 * @param ms Delay duration in milliseconds.
 */
inline void DelayMs(uint32_t ms) noexcept {
#if defined(ESP_PLATFORM)
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    volatile uint32_t count = ms * 10000;
    while (count--) { __asm__ volatile(""); }
#endif
}

/**
 * @brief Microsecond delay with automatic fallback to RTOS delay for large values.
 *
 * For delays >= 1 ms, delegates to an RTOS task delay to avoid blocking
 * the CPU. For shorter delays, uses a hardware microsecond delay on
 * ESP-IDF or a busy-wait loop on other platforms.
 *
 * @param us Delay duration in microseconds.
 */
inline void DelayUs(uint32_t us) noexcept {
#if defined(ESP_PLATFORM)
    if (us >= 1000) {
        vTaskDelay(pdMS_TO_TICKS(us / 1000));
    } else {
        esp_rom_delay_us(us);
    }
#else
    volatile uint32_t count = us * 10;
    while (count--) { __asm__ volatile(""); }
#endif
}

} // namespace handler_utils
