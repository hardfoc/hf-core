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
| `Initialize()` / `Deinitialize()` | Handler lifecycle management |
| `EnsureInitialized()` | Lazy init helper |
| `GetSensor()` | Direct access to AS5047U driver instance |
| `IsInitialized()` | Initialization state check |
| `DumpDiagnostics()` | Handler and sensor status summary |

### Driver Usage (via `GetSensor()`)

Use the underlying driver unit-enum API for measurements:

```cpp
auto* sensor = handler.GetSensor();
float angle_deg = sensor->GetAngle(as5047u::AngleUnit::Degrees);
float vel_rpm = sensor->GetVelocity(as5047u::VelocityUnit::Rpm);
```

## Thread Safety

All methods are protected by an internal `RtosMutex`.

## Test Coverage

See `examples/esp32/main/handler_tests/as5047u_handler_comprehensive_test.cpp` — 8 test
sections covering initialization, angle reading, velocity, DAEC, zero position,
diagnostics, error handling, and concurrent access.
