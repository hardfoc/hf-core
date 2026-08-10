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
 * @param ms Delay duration in milliseconds.
 */
inline void DelayMs(uint32_t ms) noexcept {
#if defined(ESP_PLATFORM)
    vTaskDelay(pdMS_TO_TICKS(ms));
#elif defined(HF_RTOS_FREERTOS)
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
        volatile uint32_t spin = 40000U;
        while (spin--) {
            __asm__ volatile("");
        }
    }
#endif
}

/**
 * @brief Microsecond delay with automatic fallback to RTOS delay for large values.
 *
 * For delays >= 1 ms, delegates to @ref DelayMs (scheduler / HAL). Shorter
 * delays use `esp_rom_delay_us` on ESP-IDF or a denser busy-wait on STM32
 * so SPI CS setup / inter-frame gaps are not no-ops.
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
    if (us >= 1000U) {
        DelayMs((us + 999U) / 1000U);
        return;
    }
    /* ~1 µs/iteration @ Cortex-M ~200–240 MHz. */
    for (uint32_t i = 0; i < us; ++i) {
        volatile uint32_t spin = 40U;
        while (spin--) {
            __asm__ volatile("");
        }
    }
#endif
}

} // namespace handler_utils
