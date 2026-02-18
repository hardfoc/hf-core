---
layout: default
title: Handlers
nav_order: 4
has_children: true
---

# Handler Reference

Handlers are the bridge layer between platform-agnostic base interfaces and
CRTP-templated device drivers. Each handler:

- Accepts `BaseXxx&` references (I2C, SPI, UART, GPIO, ADC) at construction
- Performs lazy initialization of the underlying driver
- Provides a thread-safe public API (most handlers use `RtosMutex`)
- Exposes `GetDriver()` / `GetSensor()` for direct driver access
- Optionally provides `visitDriver(fn)` for mutex-protected callable dispatch
- Reports errors via handler-specific return types or driver-native codes

## Handler Summary

| Handler | Driver | Interface | Key Features |
|:--------|:-------|:----------|:-------------|
| [As5047uHandler](as5047u_handler.md) | hf-as5047u-driver | BaseSpi | 14-bit angle, velocity, DAEC, GetDriver/visitDriver |
| [Bno08xHandler](bno08x_handler.md) | hf-bno08x-driver | BaseI2c / BaseSpi | 9-DOF IMU, type-erased IBno08xDriverOps, GetDriver/visitDriver |
| [Pca9685Handler](pca9685_handler.md) | hf-pca9685-driver | BaseI2c | 16-ch PWM, duty + phase, sleep/wake, PwmAdapter |
| [Pcal95555Handler](pcal95555_handler.md) | hf-pcal95555-driver | BaseI2c | 16-pin GPIO expander, interrupts, Agile I/O, batch ops |
| [NtcTemperatureHandler](ntc_temperature_handler.md) | hf-ntc-thermistor-driver | BaseAdc | Temperature sensing, EMA filter, thresholds, monitoring |
| [Tmc9660Handler](tmc9660_handler.md) | hf-tmc9660-driver | BaseSpi / BaseUart | BLDC motor, GPIO/ADC/Temp wrappers, GetDriver/visitDriver |
| [Tmc5160Handler](tmc5160_handler.md) | hf-tmc5160-driver | BaseSpi / BaseUart | Stepper motor, 15 subsystems, GetDriver/visitDriver |
| [Tle92466edHandler](tle92466ed_handler.md) | hf-tle92466ed-driver | BaseSpi | 6-ch solenoid driver, PWM, diagnostics, watchdog |
| [Max22200Handler](max22200_handler.md) | hf-max22200-driver | BaseSpi | 8-ch solenoid/motor, CDR/VDR, HIT/HOLD, DPM |
| [Ws2812Handler](ws2812_handler.md) | hf-ws2812-rmt-driver | RMT | Addressable LED strip, GetDriver/visitDriver/visitAnimator |
| [Logger](logger.md) | — | — | Singleton, colors, ASCII art, per-tag filtering |
