---
layout: default
title: MAX22200 Handler
parent: Handlers
nav_order: 9
---

# Max22200Handler

Octal solenoid/motor driver handler for the Analog Devices MAX22200.

## Construction

```cpp
Max22200Handler(BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
                BaseGpio* fault = nullptr);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize()` | ENABLE HIGH, read/clear STATUS, set ACTIVE |
| `Initialize(board_config)` | Initialize with `BoardConfig` (IFS + safety limits) |
| `EnsureInitialized()` | Lazy init entrypoint |
| `Deinitialize()` | Disable all channels, ACTIVE=0, ENABLE LOW |
| `ConfigureChannel(ch, config)` | Full channel configuration (0–7) |
| `SetupCdrChannel(ch, hit_mA, hold_mA, hit_ms)` | Quick CDR setup with milliamp values |
| `SetupVdrChannel(ch, hit_pct, hold_pct, hit_ms)` | Quick VDR setup with duty percentages |
| `EnableChannel(ch)` / `DisableChannel(ch)` | Per-channel control |
| `EnableAllChannels()` / `DisableAllChannels()` | Batch control |
| `IsChannelEnabled(ch)` | Check channel enable state |
| `SetChannelsMask(mask)` | Enable/disable by bitmask |
| `GetStatus(status)` | Read STATUS register |
| `GetChannelFaults(ch, faults)` | Per-channel fault flags |
| `HasFault()` / `ClearFaults()` | Fault management |
| `DumpDiagnostics()` | Print diagnostic info to Logger |

## Thread Safety

All methods are protected by an internal `RtosMutex`. The `withDriver()` pattern
acquires the lock, ensures initialization, and invokes the callable atomically.

## Test Coverage

See `examples/esp32/main/handler_tests/max22200_handler_comprehensive_test.cpp`.
