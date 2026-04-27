/**
 * @file EspLoggerFactory.cpp
 * @brief ESP32-family implementation of `Logger::CreateDefaultBaseLogger()`.
 *
 * @details
 * The `Logger` handler (`handlers/logger/Logger.cpp`) is intentionally
 * MCU-agnostic — it only knows about the abstract `BaseLogger` interface.
 * Each platform contributes a small factory translation unit that knows
 * which concrete backend to instantiate. This keeps `EspLogger.h` /
 * `esp_log.h` (and any future MCU-specific backend headers) out of the
 * shared handler implementation.
 *
 * Linking is handled by `cmake/hf_core_build_settings.cmake`: when
 * `HF_CORE_ENABLE_LOGGER=ON` and the target MCU family is ESP32, this
 * file is added to `HF_CORE_HANDLER_SOURCES`. Other MCU families ship
 * their own `<Mcu>LoggerFactory.cpp` (e.g. `StmLoggerFactory.cpp` for
 * STM32 — STM32 backend is currently a stub).
 *
 * If you want the logger to ride on a **different** ESP32 transport at
 * runtime (e.g. `EspUsbSerialJtag` instead of UART0 `EspLogger`), don't
 * edit this file — call the explicit injection overload from the HAL
 * instead:
 *
 * @code
 * Logger::GetInstance().Initialize(
 *     LogConfig{},
 *     std::make_unique<MyUsbSerialJtagBackedLogger>(...)
 * );
 * @endcode
 *
 * @author HardFOC Team
 * @date 2026
 * @copyright HardFOC
 */

#include "../Logger.h"

#include "../../../hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseLogger.h"
#include "../../../hf-core-drivers/internal/hf-internal-interface-wrap/inc/mcu/esp32/EspLogger.h"

#include <memory>

std::unique_ptr<BaseLogger> Logger::CreateDefaultBaseLogger() noexcept {
    return std::make_unique<EspLogger>();
}
