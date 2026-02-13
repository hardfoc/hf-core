---
layout: default
title: BNO08x Handler
parent: Handlers
nav_order: 2
---

# Bno08xHandler

9-DOF IMU handler supporting accelerometer, gyroscope, magnetometer, and fused orientation.

## Construction

```cpp
Bno08xHandler(BaseI2c& i2c, BaseGpio* int_pin = nullptr, BaseGpio* rst_pin = nullptr);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize()` | Full init: reset, configure SH2, enable default sensors |
| `EnsureInitialized()` | Lazy init delegation |
| `EnableSensor(sensor, rate_us)` | Enable a sensor report at given interval |
| `DisableSensor(sensor)` | Disable a sensor report |
| `GetAcceleration(x, y, z)` | Read accelerometer data (m/s²) |
| `GetRotationVector(i, j, k, real)` | Read fused rotation quaternion |
| `GetGyroscope(x, y, z)` | Read gyroscope data (rad/s) |
| `HasNewData()` | Freshness gating — true only when new data available |
| `HardwareReset()` | Pulse RST pin and re-initialize |

## Freshness Gating

`HasNewData()` returns true only when the SH2 transport has delivered new sensor
data since the last read. This prevents stale data from being consumed by the
application layer.

## Test Coverage

See `examples/esp32/main/handler_tests/bno08x_handler_comprehensive_test.cpp` — 9 test
sections including sensor enable/disable, freshness gating validation, and hardware reset.
