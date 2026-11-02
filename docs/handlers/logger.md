# Logger

The Logger singleton: levels, per-tag filtering, colours and ASCII-art banners over a per-MCU backend.

## Logger

<!-- generated: handler-logger -->
Advanced logging system with formatting capabilities.

- Header: [`handlers/logger/Logger.h`](../../handlers/logger/Logger.h)
- Switch: `HF_CORE_ENABLE_LOGGER` (default ON)

| Group | Public methods |
| --- | --- |
| All | `GetInstance()`, `Initialize()`, `Deinitialize()`, `IsInitialized()`, `SetLogLevel()`, `GetLogLevel()`, `SetTagLevel()`, `Error()`, `Warn()`, `Info()`, `Debug()`, `Verbose()`, `LogAsciiArt()`, `LogBanner()`, `SetConfig()`, `GetConfig()`, `EnableColors()`, `EnableEffects()`, `EnableAsciiArt()`, `Flush()` |
<!-- /generated -->

