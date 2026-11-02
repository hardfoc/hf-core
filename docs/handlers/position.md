# Position and inertial sensing

Handlers for the AS5047U magnetic angle sensor and the BNO08x IMU family.

## As5047uHandler

<!-- generated: handler-as5047u -->
Unified handler for AS5047U magnetic rotary position sensor with SPI integration.

- Header: [`handlers/as5047u/As5047uHandler.h`](../../handlers/as5047u/As5047uHandler.h)
- Switch: `HF_CORE_ENABLE_AS5047U` (default OFF)
- Driver: [hf-as5047u-driver](https://github.com/hardfoc/hf-as5047u-driver)
- Host test: [`tests/host/test_as5047u.cpp`](../../tests/host/test_as5047u.cpp)

```cpp
As5047uHandler(BaseSpi& spi_interface, const As5047uConfig& config = GetDefaultConfig());
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `IsSensorReady()`, `GetSensor()`, `GetDriver()`, `visitDriver()`, `GetDefaultConfig()`, `GetDescription()`, `GetLastError()`, `GetDiagnostics()`, `RefreshDiagnostics()`, `DumpDiagnostics()` |
<!-- /generated -->

## Bno08xHandler

<!-- generated: handler-bno08x -->
Unified handler for BNO08x IMU sensor family with multi-interface support.

- Header: [`handlers/bno08x/Bno08xHandler.h`](../../handlers/bno08x/Bno08xHandler.h)
- Switch: `HF_CORE_ENABLE_BNO08X` (default OFF)
- Driver: [hf-bno08x-driver](https://github.com/hardfoc/hf-bno08x-driver)
- Host test: [`tests/host/test_bno08x.cpp`](../../tests/host/test_bno08x.cpp)

```cpp
Bno08xHandler(
    BaseI2c& i2c_device,
    const Bno08xConfig& config = Bno08xConfig{},
    BaseGpio* reset_gpio = nullptr,
    BaseGpio* int_gpio = nullptr);
Bno08xHandler(
    BaseSpi& spi_device,
    const Bno08xConfig& config = Bno08xConfig{},
    BaseGpio* reset_gpio = nullptr,
    BaseGpio* int_gpio = nullptr,
    BaseGpio* wake_gpio = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `Update()`, `SetSensorCallback()`, `ClearSensorCallback()`, `QuaternionToEuler()`, `GetInterfaceType()`, `GetSensor()`, `GetDriver()`, `visitDriver()`, `GetLastError()`, `GetLastDriverError()`, `GetDescription()`, `GetDefaultConfig()`, `DumpDiagnostics()` |
<!-- /generated -->

