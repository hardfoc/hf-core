---
layout: default
title: TMC5160 Handler
parent: Handlers
nav_order: 7
---

# Tmc5160Handler

Stepper motor driver handler for TMC5160/TMC5130 with SPI/UART support.
All motor control, ramp, chopper, and StallGuard operations are accessed
through the typed driver pointers or `visitDriver()` — the handler itself
provides only lifecycle management and driver routing.

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

### Lifecycle

| Method | Description |
|:-------|:------------|
| `Initialize(config, verbose)` | Full driver initialization with `DriverConfig`. Returns `bool`. |
| `EnsureInitialized()` | Lazy init entrypoint |
| `Deinitialize()` | Disable motor and release resources |
| `IsInitialized()` | Check initialization state |
| `IsSpi()` | Check if SPI mode is active |
| `GetDriverConfig()` | Get the `DriverConfig` snapshot used at init |

### Driver Access

| Method | Description |
|:-------|:------------|
| `driverViaSpi()` | Typed `SpiDriver*` (nullptr if UART or not init) |
| `driverViaUart()` | Typed `UartDriver*` (nullptr if SPI or not init) |
| `GetDriver()` | `std::variant<monostate, SpiDriver*, UartDriver*>` |
| `visitDriver(fn)` | Execute callable on active driver under mutex |

### Diagnostics

| Method | Description |
|:-------|:------------|
| `DumpDiagnostics()` | Log status, registers, and driver info |

## Driver Subsystem Access

The TMC5160 driver exposes 15 subsystems. You access them through the
typed driver pointer or `visitDriver()` — **not** through handler-level
convenience wrappers:

| Subsystem | Purpose |
|:----------|:--------|
| `rampControl` | Motion profile: position, velocity, acceleration |
| `motorControl` | Enable/disable, current setting, chopper config |
| `stallGuard` | StallGuard2 load detection |
| `encoder` | ABN encoder interface |
| `homing` | Sensorless/switch/encoder homing routines |
| `thresholds` | Velocity threshold configuration |
| `switches` | Reference switch control |
| `tuning` | Auto-tuning routines |
| `status` | Status register and diagnostics |
| `powerStage` | Over-current / short protection |
| `communication` | Raw register read/write |
| `io` | Pin / mode helpers |
| `events` | XCompare / ramp events |
| `printer` | Debug printing |
| `uartConfig` | UART node addressing |

### Typed Pointer Access (recommended)

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

### Generic Access via `visitDriver()`

For code that must work with either comm mode:

```cpp
handler.visitDriver([](auto& drv) {
    drv.motorControl.Enable();
    drv.rampControl.SetTargetPosition(51200);
});

auto pos = handler.visitDriver([](auto& drv) -> int32_t {
    auto result = drv.rampControl.GetCurrentPosition();
    return result ? result.Value() : 0;
});
```

### Variant Access via `GetDriver()`

```cpp
auto active = handler.GetDriver();
// std::variant<std::monostate, SpiDriver*, UartDriver*>
```

## Thread Safety

All public methods are protected by an internal `RtosMutex`.
`visitDriver()` holds the mutex for the duration of the callable.

## Test Coverage

See `examples/esp32/main/handler_tests/tmc5160_handler_comprehensive_test.cpp`.
