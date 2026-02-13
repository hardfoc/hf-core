---
layout: default
title: TMC9660 Handler
parent: Handlers
nav_order: 6
---

# Tmc9660Handler

Unified handler for the TMC9660 3-phase BLDC motor controller with SPI/UART communication,
GPIO, ADC, and temperature peripheral wrappers.

## Construction

```cpp
// SPI mode
Tmc9660Handler(BaseSpi& spi, BaseGpio& rst, BaseGpio& drv_en,
               BaseGpio& faultn, BaseGpio& wake,
               uint8_t address = 0,
               const tmc9660::BootloaderConfig* bootCfg = &kDefaultBootConfig);

// UART mode
Tmc9660Handler(BaseUart& uart, BaseGpio& rst, BaseGpio& drv_en,
               BaseGpio& faultn, BaseGpio& wake,
               uint8_t address = 0,
               const tmc9660::BootloaderConfig* bootCfg = &kDefaultBootConfig);
```

## Key Methods

### Initialization

| Method | Description |
|:-------|:------------|
| `Initialize(reset, bootInfo, failOnVerify)` | Reset → bootloader config → parameter mode |
| `IsDriverReady()` | Check if driver instance exists and initialized |

### Motor Control

| Method | Description |
|:-------|:------------|
| `SetMotorType(type, polePairs)` | Configure motor type (BLDC, DC, Stepper) |
| `SetCommutationMode(mode)` | Set FOC, block, or open-loop commutation |
| `EnableMotor()` / `DisableMotor()` | Software enable/disable |
| `SetTargetVelocity(int32_t)` | Velocity control target |
| `SetTargetPosition(int32_t)` | Position control target |
| `SetTargetTorque(int16_t)` | Torque (current) control target |

### Telemetry

| Method | Description |
|:-------|:------------|
| `GetSupplyVoltage()` | Motor supply voltage (V) |
| `GetChipTemperature()` | Internal chip temperature (°C) |
| `GetMotorCurrent()` | Phase current (mA) |
| `GetActualVelocity()` | Measured velocity |
| `GetStatusFlags(uint32_t&)` | General status register |
| `GetErrorFlags(uint32_t&)` | Error flags register |

### Peripheral Wrappers

```cpp
handler.gpio(17).SetPinLevel(HF_GPIO_LEVEL_HIGH);  // TMC9660 GPIO17
handler.adc().ReadChannelV(20, voltage);            // Supply voltage ADC
handler.temperature().ReadTemperatureCelsius(&temp); // Chip temperature
```

### Advanced Access

```cpp
handler.visitDriver([](auto& driver) {
    driver.feedbackSense.configureHallSensor(1, 2, 3);
    driver.protection.setOvertemperatureLimit(120.0f);
});
```

## Test Coverage

See `examples/esp32/main/handler_tests/tmc9660_handler_comprehensive_test.cpp` — 10 test
sections covering construction state, initialization, parameters, motor control, telemetry,
GPIO/ADC/Temperature wrappers, driver enable, and error handling.
