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
| `Initialize()` | Full init: reset, configure SH2 |
| `EnsureInitialized()` | Lazy init delegation |
| `Deinitialize()` | Release resources |
| `GetSensor()` / `GetDriver()` | Get underlying `IBno08xDriverOps*` |
| `visitDriver(fn)` | Execute callable with `IBno08xDriverOps&` |
| `Update()` | Service SH-2 transport |
| `SetSensorCallback(cb)` | Register event callback |

Sensor enable/disable/read operations are performed through the underlying
driver object returned by `GetSensor()` / `GetDriver()`.

## Test Coverage

See `examples/esp32/main/handler_tests/bno08x_handler_comprehensive_test.cpp` — 9 test
sections including sensor enable/disable, freshness gating validation, and hardware reset.
