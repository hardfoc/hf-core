---
layout: default
title: AS5047U Handler
parent: Handlers
nav_order: 1
---

# As5047uHandler

High-precision 14-bit magnetic rotary encoder handler.

## Construction

```cpp
As5047uHandler(BaseSpi& spi, uint32_t spi_clock_hz = 1000000);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `EnsureInitialized()` | Lazy init — creates CRTP driver on first call |
| `GetAngleRaw()` | 14-bit raw angle (0–16383) |
| `GetAngleDegrees()` | Angle in degrees (0.0–360.0) |
| `GetVelocity()` | Angular velocity from DAEC |
| `GetDaecValue()` | Dynamic Angle Error Compensation value |
| `SetZeroPosition(uint16_t)` | Program zero position offset |
| `GetDiagnostics()` | AGC, CORDIC magnitude, error flags |

## Thread Safety

All methods are protected by an internal `RtosMutex`.

## Test Coverage

See `examples/esp32/main/handler_tests/as5047u_handler_comprehensive_test.cpp` — 8 test
sections covering initialization, angle reading, velocity, DAEC, zero position,
diagnostics, error handling, and concurrent access.
