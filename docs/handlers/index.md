---
layout: default
title: Handlers
nav_order: 4
has_children: true
---

# Handler Reference

Handlers are the bridge layer between platform-agnostic base interfaces and
CRTP-templated device drivers. Each handler:

- Accepts `BaseXxx&` references (I2C, SPI, GPIO, ADC) at construction
- Performs lazy initialization of the underlying driver
- Provides a thread-safe public API via `RtosMutex`
- Maps driver-specific errors to interface error codes

## Handler Summary

| Handler | Driver | Interface | Key Features |
|:--------|:-------|:----------|:-------------|
| [As5047uHandler](as5047u_handler.md) | hf-as5047u-driver | BaseSpi | 14-bit angle, velocity, DAEC, zero position |
| [Bno08xHandler](bno08x_handler.md) | hf-bno08x-driver | BaseI2c | 9-DOF IMU, sensor enable/disable, freshness gating |
| [Pca9685Handler](pca9685_handler.md) | hf-pca9685-driver | BaseI2c | 16-ch PWM, duty + phase, sleep/wake, PwmAdapter |
| [Pcal95555Handler](pcal95555_handler.md) | hf-pcal95555-driver | BaseI2c | 16-pin GPIO expander, interrupts, Agile I/O, batch ops |
| [NtcTemperatureHandler](ntc_temperature_handler.md) | hf-ntc-thermistor-driver | BaseAdc | Temperature sensing, EMA filter, thresholds, monitoring |
| [Tmc9660Handler](tmc9660_handler.md) | hf-tmc9660-driver | BaseSpi | Motor control, GPIO/ADC/Temp wrappers, bootloader |
| [Logger](logger.md) | — | — | Singleton, colors, ASCII art, per-tag filtering |
