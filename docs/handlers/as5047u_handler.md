---
layout: default
title: AS5047U Handler
parent: Handlers
nav_order: 1
---

# As5047uHandler

High-precision 14-bit magnetic rotary encoder handler with SPI integration.

## Construction

```cpp
explicit As5047uHandler(BaseSpi& spi_interface,
                        const As5047uConfig& config = GetDefaultConfig()) noexcept;
```

### Factory

```cpp
auto handler = CreateAs5047uHandler(spi_interface);   // returns std::unique_ptr<As5047uHandler>
```

## Key Methods

### Lifecycle

| Method | Description |
|:-------|:------------|
| `Initialize()` | Create driver instance, apply config. Returns `bool`. |
| `EnsureInitialized()` | Lazy init helper — calls `Initialize()` if needed |
| `Deinitialize()` | Release sensor resources. Returns `bool`. |
| `IsInitialized()` | Check initialization state |
| `IsSensorReady()` | Check if driver instance exists and is ready |

### Driver Access

| Method | Description |
|:-------|:------------|
| `GetSensor()` / `GetDriver()` | Direct pointer to `AS5047U<As5047uSpiAdapter>*` (nullptr if not init) |
| `visitDriver(fn)` | Execute callable under handler mutex; returns default if driver unavailable |

### Utility

| Method | Description |
|:-------|:------------|
| `GetDefaultConfig()` | Static — returns default `As5047uConfig` |
| `GetDescription()` | Human-readable handler description string |
| `GetLastError()` | Last `AS5047U_Error` from the driver |
| `DumpDiagnostics()` | Log sensor status, comm health, measurement stats |

### Driver Usage (via `GetSensor()` / `GetDriver()`)

Use the underlying driver's unit-enum API for measurements:

```cpp
auto* sensor = handler.GetSensor();
float angle_deg = sensor->GetAngle(as5047u::AngleUnit::Degrees);
float vel_rpm  = sensor->GetVelocity(as5047u::VelocityUnit::Rpm);
```

### Mutex-Protected Access (via `visitDriver()`)

```cpp
float angle = handler.visitDriver([](auto& drv) {
    return drv.GetAngle(as5047u::AngleUnit::Degrees);
});
```

## Thread Safety

All public methods are protected by an internal `RtosMutex`.
`visitDriver()` additionally holds the mutex for the duration of the callable.

## Test Coverage

See `examples/esp32/main/handler_tests/as5047u_handler_comprehensive_test.cpp` — 8 test
sections covering initialization, angle reading, velocity, DAEC, zero position,
diagnostics, error handling, and concurrent access.
