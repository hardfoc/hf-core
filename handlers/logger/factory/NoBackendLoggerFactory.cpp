/**
 * @file NoBackendLoggerFactory.cpp
 * @brief `Logger::CreateDefaultBaseLogger()` for builds with no MCU (HF_CORE_MCU=NONE).
 *
 * @details
 * A host build has no default log transport, so the factory returns nullptr
 * and `Logger::Initialize(config)` returns false. Host code that wants log
 * output injects a backend with the two-argument overload:
 *
 * @code
 * Logger::GetInstance().Initialize(LogConfig{}, std::make_unique<MyHostLogger>());
 * @endcode
 *
 * Logging calls made before a backend is injected are dropped, as on target.
 * The host build (top-level CMakeLists.txt) links this file; on-target
 * builds link their MCU factory instead (EspLoggerFactory.cpp).
 *
 * @copyright HardFOC
 */

#include "../Logger.h"

#include "base/BaseLogger.h"

#include <memory>

std::unique_ptr<BaseLogger> Logger::CreateDefaultBaseLogger() noexcept {
    return nullptr;
}
