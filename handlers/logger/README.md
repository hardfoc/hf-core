# `Logger` Handler

> 🪵 **Singleton, MCU-agnostic logging facade.** Wraps a per-MCU
> [`BaseLogger`](../../hf-core-drivers/internal/hf-internal-interface-wrap/inc/base/BaseLogger.h)
> backend behind a stable, namespace-aware API.

The `Logger` handler is the project's only sanctioned way to emit log output.
Application code only ever sees the MCU-agnostic facade — it never knows (and
must not know) whether the underlying backend is `EspLogger`, `StmLogger`, a
host-side mock, or something routed through the USB Serial/JTAG controller.

---

## At a glance

| Concern                          | Where it lives                                                      |
|----------------------------------|---------------------------------------------------------------------|
| Public API                       | [`Logger.h`](Logger.h)                                              |
| Facade implementation            | [`Logger.cpp`](Logger.cpp) — **no MCU includes**                    |
| Default-backend factory (ESP32)  | [`factory/EspLoggerFactory.cpp`](factory/EspLoggerFactory.cpp)      |
| Default-backend factory (STM32)  | _planned — `factory/StmLoggerFactory.cpp`_                          |
| Backend interface                | `BaseLogger` (in `hf-internal-interface-wrap/inc/base/BaseLogger.h`)|
| Concrete ESP32 backend           | `EspLogger` (in `hf-internal-interface-wrap/inc/mcu/esp32/`)        |

---

## Why a factory + injection?

Earlier revisions of `Logger.cpp` directly included `EspLogger.h` (and even
`esp_log.h`, for `esp_log_level_set`). That violated the handler boundary —
the same handler was supposed to compile cleanly on ST or any other target —
and made it impossible to substitute the backend (e.g. for unit testing or
for routing logs over the USB Serial/JTAG console instead of UART0).

The current design has two entry points and one factory:

```mermaid
flowchart LR
    A[Application] -->|"Initialize()"| B(Logger facade)
    A -->|"Initialize(cfg, std::move(backend))"| B
    B -->|"first overload only"| F[Logger::CreateDefaultBaseLogger\n_per-MCU translation unit_]
    F -->|ESP32 build| E[std::make_unique&lt;EspLogger&gt;()]
    F -.->|STM32 build (planned)| S[std::make_unique&lt;StmLogger&gt;()]
    B --> X[BaseLogger backend\n(owned by Logger)]
```

- **`Logger::Initialize(const LogConfig&)`** — convenience overload. Calls the
  static factory `Logger::CreateDefaultBaseLogger()` and adopts whatever
  backend it returns. This is the path 99% of application code uses.
- **`Logger::Initialize(const LogConfig&, std::unique_ptr<BaseLogger>)`** —
  injection overload. The HAL bring-up layer (or a unit test) supplies its
  own backend; `Logger` adopts ownership and never instantiates one itself.
- **`static std::unique_ptr<BaseLogger> Logger::CreateDefaultBaseLogger()`**
  — declared in `Logger.h`, **defined per-MCU** in a dedicated translation
  unit (e.g. `factory/EspLoggerFactory.cpp`). This is the **only** place a
  concrete `BaseLogger` subclass appears in the build for the default path.

The CMake gate `HF_CORE_ENABLE_LOGGER` controls whether any of the logger
sources compile at all. When ON and `HF_CORE_MCU` is `ESP32`, the build
adds `factory/EspLoggerFactory.cpp` automatically (see
`cmake/hf_core_build_settings.cmake`).

---

## Public API surface

```cpp
class Logger {
 public:
    static Logger& GetInstance() noexcept;

    // Default backend (calls CreateDefaultBaseLogger() internally)
    bool Initialize(const LogConfig& config = LogConfig{}) noexcept;

    // Injected backend (advanced / test path)
    bool Initialize(const LogConfig& config,
                    std::unique_ptr<BaseLogger> backend) noexcept;

    bool Deinitialize() noexcept;

    // Per-tag log-level override — delegates to backend->SetLogLevel(...)
    bool SetTagLevel(const char* tag, LogLevel level) noexcept;

    void LogError  (const char* tag, const char* fmt, ...) noexcept;
    void LogWarn   (const char* tag, const char* fmt, ...) noexcept;
    void LogInfo   (const char* tag, const char* fmt, ...) noexcept;
    void LogDebug  (const char* tag, const char* fmt, ...) noexcept;
    void LogVerbose(const char* tag, const char* fmt, ...) noexcept;

    // ...statistics, diagnostics, formatting helpers...

 private:
    // Per-MCU factory — defined in Esp/Stm/...LoggerFactory.cpp
    static std::unique_ptr<BaseLogger> CreateDefaultBaseLogger() noexcept;
};
```

---

## Usage patterns

### Default — let the facade pick the backend

```cpp
#include "handlers/logger/Logger.h"

void boot() {
    auto& log = Logger::GetInstance();
    if (!log.Initialize()) {
        // pre-init failure (very rare — usually OOM)
        return;
    }
    log.LogInfo("BOOT", "logger online");
}
```

### Inject a backend (HAL bring-up, tests, alternative transport)

```cpp
#include "handlers/logger/Logger.h"
#include "mcu/esp32/EspLogger.h"   // HAL-side include is fine

void hal_bringup() {
    auto backend = std::make_unique<EspLogger>(/*…tweaked cfg…*/);
    Logger::GetInstance().Initialize(LogConfig{}, std::move(backend));
}
```

This is also the hook that lets a future build route logs over the
[USB Serial/JTAG controller](../../hf-core-drivers/internal/hf-internal-interface-wrap/docs/api/BaseUsbSerialJtag.md)
instead of UART0 — the HAL just hands `Logger` a `BaseLogger` whose
backend writes to `EspUsbSerialJtag` instead of `esp_log_write`.

### Per-tag filtering

```cpp
Logger::GetInstance().SetTagLevel("MOTOR", LogLevel::Debug);
```

Internally this delegates to `BaseLogger::SetLogLevel(tag, level)`, which
the ESP32 backend implements by calling `esp_log_level_set` — no
ESP-IDF-specific calls are required at the handler layer.

---

## Adding a new MCU backend

1. Implement `XxxLogger : public BaseLogger` in
   `hf-internal-interface-wrap/inc/mcu/<mcu>/` and
   `src/mcu/<mcu>/`. Override `Initialize`, `Deinitialize`, `Log`,
   `SetLogLevel`, etc.
2. Create `factory/XxxLoggerFactory.cpp` next to
   `EspLoggerFactory.cpp`, defining
   `std::unique_ptr<BaseLogger> Logger::CreateDefaultBaseLogger() noexcept`
   to return `std::make_unique<XxxLogger>()`.
3. Wire it into `cmake/hf_core_build_settings.cmake` by extending the
   `HF_CORE_MCU` block that already gates `EspLoggerFactory.cpp`.

That's it — no changes to `Logger.cpp`.

---

## See also

- [`BaseLogger`](../../hf-core-drivers/internal/hf-internal-interface-wrap/docs/api/BaseLogger.md)
  — abstract interface.
- [`EspLogger`](../../hf-core-drivers/internal/hf-internal-interface-wrap/docs/esp_api/EspLogger.md)
  — concrete ESP32 backend.
- [`BaseUsbSerialJtag`](../../hf-core-drivers/internal/hf-internal-interface-wrap/docs/api/BaseUsbSerialJtag.md)
  — peer interface for the native-USB console transport, useful when you
  want to inject a backend that writes there instead of UART0.
