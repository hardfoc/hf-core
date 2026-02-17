---
layout: default
title: TMC5160 Handler
parent: Handlers
nav_order: 7
---

# Tmc5160Handler

Stepper motor driver handler for TMC5160/TMC5130 with integrated motion controller.

## Construction

```cpp
// SPI mode
Tmc5160Handler(BaseSpi& spi, BaseGpio& enable,
               BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
               uint8_t daisy_chain_position = 0,
               const tmc51x0::PinActiveLevels& active_levels = {});

// UART mode
Tmc5160Handler(BaseUart& uart, BaseGpio& enable,
               BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
               uint8_t uart_node_address = 0,
               const tmc51x0::PinActiveLevels& active_levels = {});
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize(config, verbose)` | Full driver initialization with `DriverConfig` |
| `EnsureInitialized()` | Lazy init entrypoint |
| `Deinitialize()` | Disable motor and release resources |
| `EnableMotor()` / `DisableMotor()` | Drive enable control (DRV_ENN) |
| `SetTargetPosition(pos)` | Set target position in microsteps |
| `SetTargetVelocity(vel)` | Set target velocity |
| `SetMaxSpeed(speed, unit)` | Configure ramp generator max speed |
| `SetAcceleration(accel, unit)` | Configure ramp generator acceleration |
| `Stop()` | Immediate motor stop |
| `GetCurrentPosition()` | Read current position in microsteps |
| `GetCurrentVelocity()` | Read current velocity |
| `IsTargetReached()` | Check if positioning is complete |
| `SetCurrent(irun, ihold)` | Set run and hold current (0–31) |
| `IsStallDetected()` | StallGuard detection flag |
| `GetStallGuardResult()` | Raw StallGuard value |
| `DumpDiagnostics()` | Print diagnostic info to Logger |

## Direct Driver Access

When you know the comm mode (you always do — you chose it at construction):

```cpp
// SPI mode — direct subsystem access
auto* drv = handler.driverViaSpi();
drv->motorControl.Enable();
drv->rampControl.SetTargetPosition(51200);

// UART mode equivalent
auto* drv = handler.driverViaUart();
drv->rampControl.SetMaxSpeed(50000);
```

For generic code that works with either comm mode (rare), use `visitDriver()`:

```cpp
handler.visitDriver([](auto& drv) {
    drv.motorControl.Enable();
    drv.rampControl.SetTargetPosition(51200);
});

auto pos = handler.visitDriver([](auto& drv) -> int32_t {
    auto result = drv.rampControl.GetCurrentPosition();
    return result ? result.Value() : 0;
});

// Or fetch active driver pointer explicitly without visitor:
auto active = handler.GetDriver();
// std::variant<std::monostate, SpiDriver*, UartDriver*>
```

## Thread Safety

All methods are protected by an internal `RtosMutex`.

## Test Coverage

See `examples/esp32/main/handler_tests/tmc5160_handler_comprehensive_test.cpp`.
