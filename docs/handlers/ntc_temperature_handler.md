---
layout: default
title: NTC Temperature Handler
parent: Handlers
nav_order: 5
---

# NtcTemperatureHandler

NTC thermistor temperature sensor handler implementing the full `BaseTemperature` interface.

## Construction

```cpp
NtcTemperatureHandler(NtcType ntc_type, BaseAdc* adc, const char* name = nullptr);
NtcTemperatureHandler(BaseAdc* adc, const ntc_temp_handler_config_t& config);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize()` / `Deinitialize()` | Lifecycle management |
| `ReadTemperatureCelsius(float*)` | Read temperature via ADC → NTC conversion |
| `SetCalibrationOffset(float)` | Apply constant calibration offset |
| `ResetCalibration()` | Remove calibration offset |
| `SetFiltering(enable, alpha)` | Enable/disable EMA (Exponential Moving Average) filter |
| `SetThresholds(low, high)` | Configure temperature thresholds |
| `EnableThresholdMonitoring(cb, data)` | Enable threshold callbacks |
| `StartContinuousMonitoring(hz, cb, data)` | Periodic sampling via `PeriodicTimer` |
| `StopContinuousMonitoring()` | Stop periodic sampling |
| `EnterSleepMode()` / `ExitSleepMode()` | Power management |
| `SelfTest()` / `CheckHealth()` | Diagnostic checks |
| `GetStatistics()` / `GetDiagnostics()` | Operational metrics |

## Configuration

Use `NTC_TEMP_HANDLER_CONFIG_DEFAULT()` and override fields:

```cpp
ntc_temp_handler_config_t config = NTC_TEMP_HANDLER_CONFIG_DEFAULT();
config.ntc_type = NtcType::NTCG163JFT103FT1S;
config.adc_channel = 0;
config.voltage_divider_series_resistance = 10000.0f;
config.reference_voltage = 3.3f;
config.enable_filtering = true;
config.filter_alpha = 0.1f;
```

## Test Coverage

See `examples/esp32/main/handler_tests/ntc_handler_comprehensive_test.cpp` — tests
temperature reading, calibration, EMA filtering, threshold monitoring, continuous
monitoring with PeriodicTimer, statistics, sleep mode, and self-test.
