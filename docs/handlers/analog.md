# Analog and temperature

Handlers for the ADS7952 and ADS9324 ADCs and for NTC and MCP9700 temperature sensors read through any BaseAdc.

## Ads7952Handler

<!-- generated: handler-ads7952 -->
Unified handler for ADS7952 12-channel 12-bit SAR ADC with SPI integration.

- Header: [`handlers/ads7952/Ads7952Handler.h`](../../handlers/ads7952/Ads7952Handler.h)
- Switch: `HF_CORE_ENABLE_ADS7952` (default OFF)
- Driver: [hf-ads7952-driver](https://github.com/hardfoc/hf-ads7952-driver)

```cpp
Ads7952Handler(
    BaseSpi& spi_interface,
    const Ads7952HandlerConfig& config = GetDefaultAds7952Config());
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `Deinitialize()`, `GetMaxChannels()`, `IsChannelAvailable()`, `ReadChannelV()`, `ReadChannelCount()`, `ReadChannel()`, `ReadMultipleChannels()`, `ReadAllChannels()`, `ReadAuto1Channels()`, `RawManualFrames()`, `ProgramAlarm()`, `ProgramAlarmVoltage()`, `SetRange()`, `GetActiveVref()`, `GetDriver()`, `visitDriver()`, `GetDescription()`, `GetHandlerDiagnostics()`, `DumpDiagnostics()`, `GetDeviceIndex()` |
<!-- /generated -->

## Ads9324Handler

<!-- generated: handler-ads9324 -->
hf-core handler for ADS9324 (16-ch 16-bit simultaneous SAR + PGA AFE).

- Header: [`handlers/ads9324/Ads9324Handler.h`](../../handlers/ads9324/Ads9324Handler.h)
- Switch: `HF_CORE_ENABLE_ADS9324` (default OFF)
- Driver: [hf-ads9324-driver](https://github.com/hardfoc/hf-ads9324-driver)

```cpp
Ads9324Handler(
    BaseSpi& spi,
    BaseGpio* convst,
    BaseGpio* drdy = nullptr,
    const Ads9324HandlerConfig& config = GetDefaultAds9324Config());
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `Deinitialize()`, `GetMaxChannels()`, `IsChannelAvailable()`, `ReadChannelV()`, `ReadChannelCount()`, `ReadChannel()`, `ReadMultipleChannels()`, `ConfigureChannel()`, `SetChannelOffset()`, `SetChannelGain()`, `ReadSnapshot()`, `GetDriver()`, `visitDriver()`, `GetDescription()`, `GetHandlerDiagnostics()`, `DumpDiagnostics()`, `GetDeviceIndex()` |
<!-- /generated -->

## NtcTemperatureHandler

<!-- generated: handler-ntc -->
NTC temperature sensor handler for the HardFOC system.

- Header: [`handlers/ntc/NtcTemperatureHandler.h`](../../handlers/ntc/NtcTemperatureHandler.h)
- Switch: `HF_CORE_ENABLE_NTC_THERMISTOR` (default OFF)
- Driver: [hf-ntc-thermistor-driver](https://github.com/hardfoc/hf-ntc-thermistor-driver)
- Host test: [`tests/host/test_ntc.cpp`](../../tests/host/test_ntc.cpp)

```cpp
NtcTemperatureHandler(NtcType ntc_type, BaseAdc* adc_interface, const char* sensor_name = nullptr);
NtcTemperatureHandler(BaseAdc* adc_interface, const ntc_temp_handler_config_t& config);
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `Deinitialize()`, `ReadTemperatureCelsiusImpl()`, `GetSensorInfo()`, `GetCapabilities()`, `SetRange()`, `GetRange()`, `GetResolution()`, `SetThresholds()`, `GetThresholds()`, `EnableThresholdMonitoring()`, `DisableThresholdMonitoring()`, `StartContinuousMonitoring()`, `StopContinuousMonitoring()`, `IsMonitoringActive()`, `SetCalibrationOffset()`, `GetCalibrationOffset()`, `ResetCalibration()`, `EnterSleepMode()`, `ExitSleepMode()`, `IsSleeping()`, `SelfTest()`, `CheckHealth()`, `GetStatistics()`, `GetDiagnostics()`, `ResetStatistics()`, `ResetDiagnostics()`, `GetNtcThermistor()`, `GetNtcReading()`, `GetNtcConfiguration()`, `SetNtcConfiguration()`, `GetResistance()`, `GetVoltage()`, `GetRawAdcValue()`, `Calibrate()`, `SetConversionMethod()`, `SetVoltageDivider()`, `SetReferenceVoltage()`, `SetBetaValue()`, `SetAdcChannel()`, `SetSamplingParameters()`, `SetFiltering()`, `GetSensorName()`, `GetSensorDescription()`, `GetDriver()` |
<!-- /generated -->

## Mcp9700TemperatureHandler

<!-- generated: handler-mcp9700 -->
MCP9700 linear active thermistor handler (ADC voltage to °C).

- Header: [`handlers/mcp9700/Mcp9700TemperatureHandler.h`](../../handlers/mcp9700/Mcp9700TemperatureHandler.h)
- Switch: `HF_CORE_ENABLE_MCP9700` (default OFF)
- Driver: [hf-mcp9700-driver](https://github.com/hardfoc/hf-mcp9700-driver)

```cpp
Mcp9700TemperatureHandler(
    BaseAdc* adc_interface,
    uint8_t adc_channel,
    const char* sensor_name = "MCP9700");
```

| Group | Public methods |
| --- | --- |
| All | `GetSensorName()` |
<!-- /generated -->

