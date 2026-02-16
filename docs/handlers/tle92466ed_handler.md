---
layout: default
title: TLE92466ED Handler
parent: Handlers
nav_order: 8
---

# Tle92466edHandler

Six-channel solenoid driver handler for the Infineon TLE92466ED.

## Construction

```cpp
Tle92466edHandler(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                  BaseGpio* faultn = nullptr);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize()` | Hardware reset + SPI init + enter config mode |
| `Initialize(config)` | Initialize with `GlobalConfig` |
| `EnsureInitialized()` | Lazy init entrypoint |
| `Deinitialize()` | Disable channels and shut down |
| `ConfigureChannel(ch, config)` | Configure a channel (0–5) |
| `EnableChannel(ch)` / `DisableChannel(ch)` | Per-channel control |
| `EnableAllChannels()` / `DisableAllChannels()` | Batch control |
| `SetChannelCurrent(ch, mA)` | Set current setpoint in milliamps |
| `ConfigurePwmRaw(ch, mantissa, exp, low_freq)` | Raw PWM period config |
| `EnterMissionMode()` / `EnterConfigMode()` | Device mode switching |
| `GetStatus(status)` | Read device status |
| `GetChannelDiagnostics(ch, diag)` | Per-channel diagnostics |
| `GetFaultReport(report)` | Comprehensive fault report |
| `ClearFaults()` / `HasFault()` | Fault management |
| `KickWatchdog(reload)` | Reload SPI watchdog |
| `GetChipId()` / `GetIcVersion()` | Device identification |
| `DumpDiagnostics()` | Print diagnostic info to Logger |

## Thread Safety

All methods are protected by an internal `RtosMutex`. The `withDriver()` pattern
acquires the lock, ensures initialization, and invokes the callable atomically.

## Test Coverage

See `examples/esp32/main/handler_tests/tle92466ed_handler_comprehensive_test.cpp`.
