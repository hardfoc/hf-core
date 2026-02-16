---
layout: default
title: Architecture
nav_order: 2
---

# Architecture Overview

## Layered Design

The HardFOC platform follows a strict layered architecture:

```
┌──────────────────────────────────────────────────────┐
│                  Application / API                    │
│              (Vortex.h, managers/*.h)                 │
├──────────────────────────────────────────────────────┤
│                   HANDLERS (hf-core)                  │
│  As5047uHandler  Bno08xHandler  Pca9685Handler       │
│  Pcal95555Handler  NtcTemperatureHandler              │
│  Tmc9660Handler  Tmc5160Handler  Tle92466edHandler    │
│  Max22200Handler  Ws2812Handler  Logger               │
├──────────────────────────────────────────────────────┤
│              BASE INTERFACES (internal)               │
│  BaseSpi  BaseI2c  BaseGpio  BaseAdc                 │
│  BaseTemperature  BaseEncoder  BaseImu               │
├──────────────────────────────────────────────────────┤
│            CRTP DEVICE DRIVERS (external)             │
│  hf-tmc9660-driver  hf-bno08x-driver                │
│  hf-as5047u-driver  hf-pca9685-driver               │
│  hf-pcal95555-driver  hf-ntc-thermistor-driver      │
│  hf-tmc5160-driver  hf-tle92466ed-driver            │
│  hf-max22200-driver  hf-ws2812-rmt-driver           │
├──────────────────────────────────────────────────────┤
│           PLATFORM IMPLEMENTATIONS                    │
│  EspI2cBus/Device  EspSpi  EspGpio  EspAdc          │
│  (hf-internal-interface-wrap/inc/mcu/esp32/)         │
└──────────────────────────────────────────────────────┘
```

## Handler Pattern

Each handler follows a consistent pattern:

1. **Construction** — Accepts base interface references (not owned)
2. **Lazy initialization** — `Initialize()` or `EnsureInitialized()` creates the CRTP driver
3. **Thread safety** — `RtosMutex` protects all public methods
4. **Error propagation** — Maps driver errors to interface error codes
5. **Zero overhead** — CRTP dispatch means no virtual function overhead in the driver layer

## Ownership Model

```
Manager (owns) → Handler (owns) → CRTP Driver Instance
                      │
                      └── References (not owned) → BaseI2c/BaseSpi/BaseGpio
                                                        │
                                                        └── Platform creates → EspI2c/EspSpi/EspGpio
```

**Key rule**: Handlers do NOT own their communication interfaces. The platform layer
(or test setup) must ensure interfaces outlive the handler.

## RTOS Integration

- `RtosMutex` — Wrapper around `xSemaphoreCreateMutex`
- `MutexLockGuard` — RAII lock guard
- `PeriodicTimer` — Wrapper around FreeRTOS software timers
- `BaseThread` — Abstract base for FreeRTOS tasks
- `OsQueue`, `OsEventFlags`, `OsSemaphore` — Standard RTOS primitives

All handlers use `RtosMutex` internally. The `MutexLockGuard` RAII pattern ensures
deadlock-free operation even when exceptions or early returns occur.

## Communication Adapters (TMC9660 Example)

The TMC9660 is the most complex handler due to its multi-subsystem architecture:

```
Tmc9660Handler
├── HalSpiTmc9660Comm (CRTP adapter: BaseSpi → SpiCommInterface)
│   └── Tmc9660CtrlPins (RST, DRV_EN, FAULTN, WAKE)
├── SpiDriver (tmc9660::TMC9660<HalSpiTmc9660Comm>)
├── Gpio wrappers (GPIO17, GPIO18 → BaseGpio)
├── Adc wrapper (multi-channel → BaseAdc)
└── Temperature wrapper (chip temp → BaseTemperature)
```

The `visitDriver()` template pattern routes calls through a type-erased facade,
keeping the public API non-templated while preserving zero-overhead dispatch internally.
The `withDriver()` pattern (used by TLE92466ED and MAX22200) is a simplified variant
that acquires the mutex, ensures initialization, and invokes a callable on the driver
in a single atomic step.
