# Motor and motion control

Handlers for the TMC9660 motor controller, the TMC5160 stepper driver and Dynamixel Protocol 2.0 servos.

## Tmc9660Handler

<!-- generated: handler-tmc9660 -->
Unified handler for TMC9660 motor controller with GPIO, ADC, and temperature integration.

- Header: [`handlers/tmc9660/Tmc9660Handler.h`](../../handlers/tmc9660/Tmc9660Handler.h)
- Switch: `HF_CORE_ENABLE_TMC9660` (default OFF)
- Driver: [hf-tmc9660-driver](https://github.com/hardfoc/hf-tmc9660-driver)
- Host test: [`tests/host/test_tmc9660.cpp`](../../tests/host/test_tmc9660.cpp)

```cpp
Tmc9660Handler(
    BaseSpi& spi,
    BaseGpio& rst,
    BaseGpio& drv_en,
    BaseGpio& faultn,
    BaseGpio& wake,
    uint8_t address = 0,
    const tmc9660::BootloaderConfig* bootCfg = &kDefaultBootConfig);
Tmc9660Handler(
    BaseUart& uart,
    BaseGpio& rst,
    BaseGpio& drv_en,
    BaseGpio& faultn,
    BaseGpio& wake,
    uint8_t address = 0,
    const tmc9660::BootloaderConfig* bootCfg = &kDefaultBootConfig);
```

| Group | Public methods |
| --- | --- |
| Initialization | `Initialize()`, `EnsureInitialized()`, `IsDriverReady()`, `SetTmclReplyTimeoutMs()` |
| Peripheral Accessors | `gpio()`, `adc()`, `temperature()` |
| Communication Info | `GetCommMode()`, `bootConfig()` |
| Diagnostics | `DumpDiagnostics()`, `GetDescription()` |
| Typed Driver Access | `driverViaSpi()`, `driverViaUart()`, `GetDriver()` |
| Driver Visitor Pattern | `visitDriver()` |
<!-- /generated -->

`visitDriver()` locks the handler and initialises it before calling you; `driverViaSpi()` and
`driverViaUart()` return the typed driver without locking, so synchronise yourself when several tasks use
them. `gpio()`, `adc()` and `temperature()` present the chip's GPIOs, ADC and temperature sensor as `BaseGpio`,
`BaseAdc` and `BaseTemperature`. Like the typed driver accessors they run the lazy bring-up and return `nullptr`
when it fails; `gpio()` also returns `nullptr` for any pin other than 17 or 18. `bootConfig()` returns `nullptr`
when the handler was built without a bootloader configuration. The driver itself is documented on the
[TMC9660 driver site](https://hardfoc.github.io/hf-tmc9660-driver/).

## Tmc5160Handler

<!-- generated: handler-tmc5160 -->
Unified handler for TMC5160/TMC5130 stepper motor driver with SPI/UART integration.

- Header: [`handlers/tmc5160/Tmc5160Handler.h`](../../handlers/tmc5160/Tmc5160Handler.h)
- Switch: `HF_CORE_ENABLE_TMC5160` (default OFF)
- Driver: [hf-tmc5160-driver](https://github.com/hardfoc/hf-tmc5160-driver)

```cpp
Tmc5160Handler(
    BaseSpi& spi,
    BaseGpio& enable,
    BaseGpio* diag0 = nullptr,
    BaseGpio* diag1 = nullptr,
    uint8_t daisy_chain_position = 0,
    const tmc51x0::PinActiveLevels& active_levels = {});
Tmc5160Handler(
    BaseUart& uart,
    BaseGpio& enable,
    BaseGpio* diag0 = nullptr,
    BaseGpio* diag1 = nullptr,
    uint8_t uart_node_address = 0,
    const tmc51x0::PinActiveLevels& active_levels = {});
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `IsSpi()`, `visitDriver()`, `driverViaSpi()`, `driverViaUart()`, `GetDriver()`, `DumpDiagnostics()`, `GetDescription()`, `GetDefaultConfig()`, `GetDriverConfig()` |
<!-- /generated -->

## DynamixelHandler

<!-- generated: handler-dynamixel -->
HAL-level handler for Protocol 2.0 Dynamixel servos on BaseUart.

- Header: [`handlers/dynamixel/DynamixelHandler.h`](../../handlers/dynamixel/DynamixelHandler.h)
- Switch: `HF_CORE_ENABLE_DYNAMIXEL` (default OFF)
- Driver: [hf-dynamixel-driver](https://github.com/hardfoc/hf-dynamixel-driver)

```cpp
DynamixelHandler(
    BaseUart& uart,
    const DynamixelHandlerConfig& config = DynamixelHandlerConfig{},
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `IsPresent()`, `Identity()`, `Model()`, `BindModel()`, `BindXSeriesFallback()`, `Ping()`, `Scan()`, `SetTorqueEnabled()`, `SetOperatingMode()`, `SetMotionProfile()`, `SetGoalPosition()`, `SetGoalVelocity()`, `ReadPosition()`, `ReadTelemetry()`, `bus()`, `device()`, `visitDevice()` |
<!-- /generated -->

