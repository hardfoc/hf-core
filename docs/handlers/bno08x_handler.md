---
layout: default
title: BNO08x Handler
parent: Handlers
nav_order: 2
---

# Bno08xHandler

9-DOF IMU handler supporting accelerometer, gyroscope, magnetometer, and fused
orientation via I2C or SPI. Uses type-erased `IBno08xDriverOps` interface to
hide transport template complexity.

## Construction

```cpp
// I2C mode
explicit Bno08xHandler(BaseI2c& i2c_device,
                       const Bno08xConfig& config = Bno08xConfig{},
                       BaseGpio* reset_gpio = nullptr,
                       BaseGpio* int_gpio = nullptr) noexcept;

// SPI mode
explicit Bno08xHandler(BaseSpi& spi_device,
                       const Bno08xConfig& config = Bno08xConfig{},
                       BaseGpio* reset_gpio = nullptr,
                       BaseGpio* int_gpio = nullptr,
                       BaseGpio* wake_gpio = nullptr) noexcept;
```

### Factories

```cpp
auto handler = CreateBno08xHandlerI2c(i2c, config, rst, intr);
auto handler = CreateBno08xHandlerSpi(spi, config, rst, intr, wake);
```

## Key Methods

### Lifecycle

| Method | Description |
|:-------|:------------|
| `Initialize()` | Reset → Begin → applyConfigLocked. Returns `Bno08xError`. |
| `EnsureInitialized()` | Lazy init helper |
| `Deinitialize()` | Release resources. Returns `Bno08xError`. |
| `IsInitialized()` | Check initialization state |

### Service Loop

| Method | Description |
|:-------|:------------|
| `Update()` | Pump SH-2 transport — call every 5–10 ms |

### Callback Management

| Method | Description |
|:-------|:------------|
| `SetSensorCallback(cb)` | Register event callback (dispatched from `Update()`) |
| `ClearSensorCallback()` | Remove callback |

### Driver Access

| Method | Description |
|:-------|:------------|
| `GetSensor()` / `GetDriver()` | Direct `IBno08xDriverOps*` (nullptr if not init) |
| `visitDriver(fn)` | Execute callable under handler mutex |

### Utility

| Method | Description |
|:-------|:------------|
| `GetInterfaceType()` | `BNO085Interface::I2C` or `BNO085Interface::SPI` |
| `GetLastError()` | Last `Bno08xError` |
| `GetLastDriverError()` | Last SH-2 error code (int) |
| `GetDescription()` | Human-readable string (e.g. `"BNO08x IMU (I2C @0x4A)"`) |
| `GetDefaultConfig()` | Static — returns default `Bno08xConfig` |
| `QuaternionToEuler(quat, euler)` | Static utility |
| `DumpDiagnostics()` | Log comprehensive handler/driver status |

### Sensor Operations (via driver)

Sensor enable/disable/read operations are performed through the underlying
driver object returned by `GetSensor()` / `GetDriver()`:

```cpp
// Direct driver access
auto* drv = handler.GetSensor();
drv->EnableSensor(BNO085Sensor::RotationVector, 10);  // 100 Hz
drv->Update();
auto event = drv->GetLatest(BNO085Sensor::RotationVector);

// Or mutex-protected via visitDriver
handler.visitDriver([](IBno08xDriverOps& drv) {
    drv.EnableSensor(BNO085Sensor::Accelerometer, 20);
});
```

## Thread Safety

All public methods are protected by an internal `RtosMutex` (recursive).
`visitDriver()` additionally holds the mutex for the duration of the callable.

## Test Coverage

See `examples/esp32/main/handler_tests/bno08x_handler_comprehensive_test.cpp` — 9 test
sections including sensor enable/disable, config apply validation, and hardware reset.
