---
layout: default
title: TMC9660 Handler
parent: Handlers
nav_order: 6
---

# Tmc9660Handler

Unified handler for the TMC9660 3-phase BLDC motor controller with SPI/UART
communication and GPIO, ADC, and temperature peripheral wrappers.

All motor control, telemetry, and feedback operations are accessed through the
typed driver pointers or `visitDriver()`. The handler provides lifecycle
management, peripheral wrapping (GPIO/ADC/Temperature via inner classes), and
driver routing.

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

### Lifecycle

| Method | Description |
|:-------|:------------|
| `Initialize(reset, bootInfo, failOnVerify)` | Reset → bootloader config → parameter mode. Returns `bool`. |
| `EnsureInitialized()` | Lazy init helper |
| `IsDriverReady()` | Check if driver instance exists and is initialized |

### Communication Info

| Method | Description |
|:-------|:------------|
| `GetCommMode()` | `tmc9660::CommMode::SPI` or `::UART` |
| `bootConfig()` | Const reference to the active `BootloaderConfig` |

### Driver Access

| Method | Description |
|:-------|:------------|
| `driverViaSpi()` | Typed `SpiDriver*` (nullptr if UART or not init); raw pointer — not mutex-protected |
| `driverViaUart()` | Typed `UartDriver*` (nullptr if SPI or not init); raw pointer — not mutex-protected |
| `GetDriver()` | `std::variant<monostate, SpiDriver*, UartDriver*>` |
| `visitDriver(fn)` | Execute callable on active driver (mutex held; see Thread safety) |

### Peripheral Wrappers

The handler owns three inner-class adapters that implement HardFOC base
interfaces, enabling manager-level integration:

```cpp
handler.gpio(17).SetPinLevel(HF_GPIO_LEVEL_HIGH);  // TMC9660 GPIO17
handler.adc().ReadChannelV(20, voltage);            // Supply voltage ADC
handler.temperature().ReadTemperatureCelsius(&temp); // Chip temperature
```

| Accessor | Type | Description |
|:---------|:-----|:------------|
| `gpio(uint8_t)` | `Gpio&` (BaseGpio) | TMC9660 internal GPIO (17, 18) |
| `adc()` | `Adc&` (BaseAdc) | Multi-channel: AIN, current, voltage, temp, motor |
| `temperature()` | `Temperature&` (BaseTemperature) | Chip temperature sensor |

### Diagnostics

| Method | Description |
|:-------|:------------|
| `DumpDiagnostics()` | Log comm mode, voltage, temperature, flags, GPIO/ADC status |

## Motor Control & Telemetry via Driver Subsystems

Motor control and telemetry are accessed through the driver's subsystems,
not through handler-level wrapper methods:

### Typed Pointer Access (recommended)

```cpp
// SPI mode — direct subsystem access
auto* drv = handler.driverViaSpi();
drv->motorConfig.setType(tmcl::MotorType::BLDC_MOTOR, 7);
drv->commutation.setMode(tmcl::CommutationMode::FOC);
drv->motorControl.enable();
drv->velocityControl.setTargetVelocity(1000);

// Telemetry
float supply_v = drv->telemetry.getSupplyVoltage();
float chip_t   = drv->telemetry.getChipTemperature();
```

### Generic Access via `visitDriver()`

```cpp
handler.visitDriver([](auto& driver) {
    driver.feedbackSense.configureHallSensor(1, 2, 3);
    driver.protection.setOvertemperatureLimit(120.0f);
});
```

### Variant Access via `GetDriver()`

```cpp
auto active = handler.GetDriver();
// std::variant<std::monostate, SpiDriver*, UartDriver*>
```

## UART TMCL expectations

UART mode uses the same TMCL opcodes as SPI. The underlying `hf-tmc9660-driver` expects the platform `UartCommInterface` to deliver a **complete 9-byte** TMCL reply for every transaction. Partial reads or leftover RX bytes typically surface as checksum or TMCL status errors. See the driver’s [Platform Integration](https://github.com/N3b3x/hf-tmc9660-driver/blob/main/docs/platform_integration.md) guide (`uartReceiveTMCL` contract).

## Thread safety

`visitDriver()`, `EnsureInitialized()`, and accessors such as `gpio()` / `adc()` / `temperature()` take the handler’s recursive mutex (directly or via `EnsureInitialized()`). `Initialize()` does **not** take that mutex; avoid calling it concurrently with `visitDriver()` or wrapper methods from another thread until initialization is stable, or restrict init to a single thread.

`driverViaSpi()` and `driverViaUart()` return raw pointers without locking — use them only when no other thread can race initialization or driver access, or prefer `visitDriver()`.

The `Adc` and `Temperature` inner classes use an additional mutex for internal statistics.

## Test Coverage

See `examples/esp32/main/handler_tests/tmc9660_handler_comprehensive_test.cpp` — 10 test
sections covering construction state, initialization, parameters, motor control, telemetry,
GPIO/ADC/Temperature wrappers, driver enable, and error handling.
