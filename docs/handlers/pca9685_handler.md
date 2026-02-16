---
layout: default
title: PCA9685 Handler
parent: Handlers
nav_order: 3
---

# Pca9685Handler

16-channel, 12-bit PWM driver handler with phase offset support.

## Construction

```cpp
Pca9685Handler(BaseI2c& i2c);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `EnsureInitialized()` | Lazy init — delegates to driver's `EnsureInitialized()` |
| `SetDuty(channel, duty_float)` | Set PWM duty cycle (0.0–1.0) |
| `SetRawDuty(channel, on_tick, off_tick)` | Set raw 12-bit on/off ticks with phase |
| `SetFrequency(freq_hz)` | Set PWM frequency (24–1526 Hz) |
| `Sleep()` / `Wake()` | Enter/exit low-power mode |
| `GetPwmAdapter(channel)` | Returns a `PwmAdapter` for BaseMotor integration |
| `GetGpioPinWrapper(channel)` | Returns a `GpioPin` wrapper for digital I/O |

## Phase Offset

The `SetRawDuty(channel, on_tick, off_tick)` method supports phase-shifted PWM by
allowing the ON tick to be non-zero. This reduces current surge when driving multiple
channels simultaneously (e.g., LED arrays, servo banks).

## Direct Driver Access

```cpp
auto* drv = handler.GetDriver();
// Access any PCA9685 register-level API directly
```

## Test Coverage

See `examples/esp32/main/handler_tests/pca9685_handler_comprehensive_test.cpp` — 8 test
sections covering initialization, float/raw duty, frequency, sleep/wake, adapters, and
phase offset validation.
