---
layout: default
title: Home
nav_order: 1
permalink: /
---

# HF-Core Platform Documentation

Welcome to the **HardFOC Core Platform (hf-core)** documentation. This module provides
the handler layer, RTOS utilities, and driver integration that form the backbone of the
HardFOC motor control system.

## What is hf-core?

hf-core is the **bridge layer** between hardware-agnostic base interfaces (`BaseI2c`,
`BaseSpi`, `BaseGpio`, `BaseAdc`, `BaseTemperature`) and the CRTP-templated device
drivers (`hf-tmc9660-driver`, `hf-bno08x-driver`, `hf-as5047u-driver`, etc.).

It provides:

- **11 Handlers** — Thread-safe facades for AS5047U, BNO08x, PCA9685, PCAL95555, NTC, TMC9660, TMC5160, TLE92466ED, MAX22200, WS2812, and singleton Logger
- **RTOS Wrappers** — FreeRTOS C++ abstractions (mutex, timer, thread, queue, semaphore)
- **General Utilities** — Circular buffers, filters, state machines, interpolation, CRC, and more
- **CANopen Utilities** — Frame construction, SDO/NMT protocol helpers

## Quick Navigation

| Section | Description |
|:--------|:------------|
| [Architecture](architecture.md) | System design, ownership model, layering |
| [Folder Structure](folder_structure.md) | Repository organization |
| [Handlers](handlers/index.md) | Per-handler API reference |
| [Utilities](utils/index.md) | RTOS wrappers, general utils, CANopen |
| [Testing](testing/testing_guide.md) | Test infrastructure, CI pipelines |

## Getting Started

```bash
# Clone with submodules
git clone --recursive https://github.com/N3b3x/hf-hal-vortex-v1.git
cd hf-hal-vortex-v1/lib/core

# Build a specific test app
cd examples/esp32/scripts
./build_app.sh as5047u_handler_test Debug
```

See the [Testing Guide](testing/testing_guide.md) for detailed instructions.
