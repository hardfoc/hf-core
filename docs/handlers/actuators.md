# Solenoid and load drivers

Handlers for the TLE92466ED six-channel and MAX22200 eight-channel solenoid and load drivers.

## Tle92466edHandler

<!-- generated: handler-tle92466ed -->
Unified handler for TLE92466ED six-channel low-side solenoid driver IC.

- Header: [`handlers/tle92466ed/Tle92466edHandler.h`](../../handlers/tle92466ed/Tle92466edHandler.h)
- Switch: `HF_CORE_ENABLE_TLE92466ED` (default OFF)
- Driver: [hf-tle92466ed-driver](https://github.com/hardfoc/hf-tle92466ed-driver)

```cpp
Tle92466edHandler(BaseSpi& spi, BaseGpio& resn, BaseGpio& en, BaseGpio* faultn = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `ConfigureChannel()`, `EnableChannel()`, `DisableChannel()`, `EnableAllChannels()`, `DisableAllChannels()`, `SetChannelCurrent()`, `SetChannelCurrentUa()`, `GetChannelCurrentSetpoint()`, `ConfigurePwmRaw()`, `EnterMissionMode()`, `EnterConfigMode()`, `IsMissionMode()`, `EnableOutputStage()`, `DisableOutputStage()`, `EnableFeedbackUpdates()`, `GetStatus()`, `GetChannelDiagnostics()`, `GetFaultReport()`, `ClearFaults()`, `HasFault()`, `KickWatchdog()`, `GetChipId()`, `GetIcVersion()`, `LastSpiRxWord()`, `LastSpiRxWordFirst()`, `RawSpiChain()`, `WithLock()`, `WithDriver()`, `GetDriver()`, `DumpDiagnostics()` |
<!-- /generated -->

## Max22200Handler

<!-- generated: handler-max22200 -->
Unified handler for MAX22200 octal solenoid and motor driver.

- Header: [`handlers/max22200/Max22200Handler.h`](../../handlers/max22200/Max22200Handler.h)
- Switch: `HF_CORE_ENABLE_MAX22200` (default OFF)
- Driver: [hf-max22200-driver](https://github.com/hardfoc/hf-max22200-driver)

```cpp
Max22200Handler(BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd, BaseGpio* fault = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `Initialize()`, `EnsureInitialized()`, `Deinitialize()`, `IsInitialized()`, `GetLastFaultByte()`, `ConfigureChannel()`, `SetupCdrChannel()`, `SetupVdrChannel()`, `EnableChannel()`, `DisableChannel()`, `EnableAllChannels()`, `DisableAllChannels()`, `IsChannelEnabled()`, `SetChannelsMask()`, `GetStatus()`, `GetChannelFaults()`, `HasFault()`, `ClearFaults()`, `ReadFaultRegister()`, `WithDriver()`, `GetDriver()`, `DumpDiagnostics()` |
<!-- /generated -->

