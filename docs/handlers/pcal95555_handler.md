---
layout: default
title: PCAL95555 Handler
parent: Handlers
nav_order: 4
---

# Pcal95555Handler

16-pin I²C GPIO expander handler with interrupt support and Agile I/O features.

## Construction

```cpp
Pcal95555Handler(BaseI2c& i2c, BaseGpio* int_pin = nullptr);
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `EnsureInitialized()` | Lazy init, delegates to driver |
| `SetPinDirection(pin, dir)` | Configure individual pin as input/output |
| `ReadPin(pin, level)` | Read a single pin level |
| `WritePin(pin, level)` | Write a single pin level |
| `TogglePin(pin)` | Toggle a pin output |
| `SetMultipleDirections(mask, dir)` | Batch direction config (16-bit mask) |
| `WritePins(mask, value)` | Batch write (16-bit mask + value) |
| `GetChipVariant()` | Detect PCA9555 vs PCAL9555A |
| `HasAgileIO()` | Check for extended (PCAL) features |
| `SupportsInterrupts()` | Check interrupt capability |
| `DrainPendingInterrupts()` | Deferred ISR processing via atomic flag |
| `GetPin(pin)` | Get a `GpioPin` wrapper for BaseGpio integration |

## Chip Variant Detection

The handler auto-detects the chip variant at initialization:

- **PCA9555** — Basic 16-pin GPIO expander
- **PCAL9555A** — Extended with Agile I/O, interrupt masking, output drive strength

## Interrupt Handling

Interrupt handling uses a **deferred ISR** pattern:
1. ISR sets an `std::atomic<bool>` flag
2. Application calls `DrainPendingInterrupts()` from task context
3. Handler reads the interrupt status register and processes

This avoids I²C communication in ISR context.

## Test Coverage

See `examples/esp32/main/handler_tests/pcal95555_handler_comprehensive_test.cpp`.
