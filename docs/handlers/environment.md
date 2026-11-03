# Pressure, flow and gas

Handlers for the BMP581 barometer, Sensirion SF06 / SFx6000 flow meters, the Alicat BASIS-2 mass-flow
instrument and the PyroScience FDO2-G2 oxygen probe.

## Bmp581Handler

<!-- generated: handler-bmp581 -->
Handler for the Bosch BMP581 barometric pressure sensor on a `BaseI2c`.

- Header: [`handlers/bmp581/Bmp581Handler.h`](../../handlers/bmp581/Bmp581Handler.h)
- Switch: `HF_CORE_ENABLE_BMP581` (default OFF)
- Driver: [hf-bmp581-driver](https://github.com/hardfoc/hf-bmp581-driver)

```cpp
Bmp581Handler(
    BaseI2c& i2c_device,
    const Bmp581HandlerConfig& config = Bmp581HandlerConfig{},
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `IsPresent()`, `ChipId()`, `EffectiveOsr()`, `DataReady()`, `ReadSample()`, `Configure()`, `Reset()`, `GetCounters()` |
<!-- /generated -->

## SfmHandler

<!-- generated: handler-sfm -->
Handler for Sensirion gas mass flow meters on a `BaseI2c`: SF06 (SFM4300, …) and SFx6000 (SFM6000D).

- Header: [`handlers/sfm/SfmHandler.h`](../../handlers/sfm/SfmHandler.h)
- Switch: `HF_CORE_ENABLE_SFM` (default OFF)
- Driver: [hf-sfm-flow-meter-driver](https://github.com/hardfoc/hf-sfm-flow-meter-driver)

```cpp
SfmHandler(
    BaseI2c& i2c_device,
    const SfmHandlerConfig& config = SfmHandlerConfig{},
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `Invalidate()`, `IsPresent()`, `Identity()`, `SupportsGas()`, `FullScaleSlm()`, `Start()`, `Stop()`, `ReadMeasurement()`, `ConfigureAveraging()`, `UpdateConcentration()`, `ReadTemperature()`, `DecodeFrame()`, `Family()`, `Address()`, `ActiveGas()`, `ActiveScaling()`, `Measuring()`, `GetCounters()` |
<!-- /generated -->

## AlicatBasis2Handler

<!-- generated: handler-alicat-basis2 -->
HAL-level handler for the Alicat BASIS-2 mass-flow instrument.

- Header: [`handlers/alicat_basis2/AlicatBasis2Handler.h`](../../handlers/alicat_basis2/AlicatBasis2Handler.h)
- Switch: `HF_CORE_ENABLE_ALICAT_BASIS2` (default OFF)
- Driver: [hf-alicat-basis2-driver](https://github.com/hardfoc/hf-alicat-basis2-driver)

```cpp
AlicatBasis2Handler(
    BaseUart& uart,
    const AlicatBasis2HandlerConfig& config,
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `IsInitialized()`, `Address()`, `DeviceIndex()`, `Identity()`, `FlowDecimals()`, `ReadInstantaneous()`, `RereadIdentity()`, `Tare()`, `SetGas()`, `SetSetpoint()`, `SetSetpointSource()`, `SetCommWatchdogMs()`, `SetMaxSetpointRamp()`, `SetAutotareEnabled()`, `ResetTotalizer()`, `SetTotalizerLimitMode()`, `SetTotalizerBatch()`, `SetFlowAveragingMs()`, `SetReferenceTemperatureC()`, `ConfigureMeasurementTrigger()`, `StartMeasurementSamples()`, `ReadMeasurement()`, `SetModbusAddress()`, `SetAsciiUnitId()`, `SetBaudRate()`, `FactoryRestore()`, `Discover()`, `DiscoverAcrossBauds()`, `NormalizeBusBaud()`, `GetDriver()` |
<!-- /generated -->

## Fdo2Handler

<!-- generated: handler-fdo2 -->
HAL-level handler for the PyroScience FDO2-G2 oxygen probe.

- Header: [`handlers/fdo2/Fdo2Handler.h`](../../handlers/fdo2/Fdo2Handler.h)
- Switch: `HF_CORE_ENABLE_FDO2` (default OFF)
- Driver: [hf-fdo2-driver](https://github.com/hardfoc/hf-fdo2-driver)
- Host test: [`tests/host/test_fdo2.cpp`](../../tests/host/test_fdo2.cpp)

```cpp
Fdo2Handler(
    BaseUart& uart,
    const Fdo2HandlerConfig& config = Fdo2HandlerConfig{},
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `IsPresent()`, `Identity()`, `MeasureMoxy()`, `MeasureMraw()`, `ReadVersion()`, `ReadUniqueId()` |
<!-- /generated -->

