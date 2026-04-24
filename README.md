---
layout: default
title: "HardFOC Core Platform"
description: "Shared platform layer for HardFOC boards — handlers, RTOS utilities, driver integration, and CANopen for motor control systems"
nav_order: 1
---

# HF-Core Platform

**Shared platform layer for HardFOC boards — handlers, RTOS utilities, driver integration, and CANopen for motor control systems**

[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20.html)
[![License](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![ESP-IDF Build](https://github.com/hardfoc/hf-core/actions/workflows/esp32-examples-build-ci.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/esp32-examples-build-ci.yml)
[![C++ Analysis](https://github.com/hardfoc/hf-core/actions/workflows/ci-cpp-analysis.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-cpp-analysis.yml)
[![C++ Lint](https://github.com/hardfoc/hf-core/actions/workflows/ci-cpp-lint.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-cpp-lint.yml)
[![Docs](https://github.com/hardfoc/hf-core/actions/workflows/ci-docs-publish.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-docs-publish.yml)
[![Markdown Lint](https://github.com/hardfoc/hf-core/actions/workflows/ci-markdown-lint.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-markdown-lint.yml)
[![YAML Lint](https://github.com/hardfoc/hf-core/actions/workflows/ci-yaml-lint.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-yaml-lint.yml)
[![Link Check](https://github.com/hardfoc/hf-core/actions/workflows/ci-docs-linkcheck.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/ci-docs-linkcheck.yml)
[![Release](https://github.com/hardfoc/hf-core/actions/workflows/release.yml/badge.svg)](https://github.com/hardfoc/hf-core/actions/workflows/release.yml)

## 📚 Table of Contents

1. [Overview](#-overview)
2. [Features](#-features)
3. [Architecture](#-architecture)
4. [Quick Start](#-quick-start)
5. [Handlers](#-handlers)
6. [Utilities](#-utilities)
7. [Repository Structure](#-repository-structure)
8. [Documentation](#-documentation)
9. [Contributing](#-contributing)
10. [License](#-license)

## 📦 Overview

> **📖 [📚🌐 Live Complete Documentation](https://hardfoc.github.io/hf-core/)** —
> Architecture guides, per-handler API reference, and testing instructions

**hf-core** is the shared platform layer for all HardFOC boards. It provides
the handler layer, RTOS utilities, and driver integration that form the backbone
of the HardFOC motor control system.

hf-core is the **bridge layer** between hardware-agnostic base interfaces
(`BaseI2c`, `BaseSpi`, `BaseGpio`, `BaseAdc`, `BaseTemperature`) and CRTP-templated
device drivers (`hf-tmc9660-driver`, `hf-bno08x-driver`, `hf-as5047u-driver`, etc.).
No board-specific configuration lives here — board HALs (e.g. `hf-hal-vortex-v1`)
depend on this repo and add managers, API, and pin mapping.

## ✨ Features

- ✅ **12 Device Handlers** — AS5047U, BNO08x, PCA9685, PCAL95555, NTC, TMC9660, TMC5160, TLE92466ED, MAX22200, WS2812, SE050, Logger
- ✅ **30+ General Utilities** — Buffers, filters, timers, CRC, interpolation, linked
  lists, flag sets, physical units
- ✅ **Full RTOS Abstraction** — Mutex, semaphore, queue, event flags, threads, timers
- ✅ **CANopen Utilities** — CAN frame, SDO protocol, NMT commands
- ✅ **ESP32 Test Suite** — 16 test applications covering every handler, utility, and
  integration scenario
- ✅ **8 CI Pipelines** — Build, lint, analysis, documentation, release
- ✅ **Thread-Safe** — All handlers use `RtosMutex` for safe concurrent access
- ✅ **Modern C++20** — Type-safe APIs with CRTP-based design and zero-overhead abstractions

## 🏗️ Architecture

```text
┌─────────────────────────────────────────────────┐
│                Board HAL (Vortex)                │  Managers + API
├─────────────────────────────────────────────────┤
│              hf-core (this repo)                │
│  ┌──────────┐  ┌──────────────┐  ┌───────────┐ │
│  │ Handlers │  │ hf-core-utils│  │  Logger   │ │
│  │ (adapt)  │  │ (general,    │  │(singleton)│ │
│  │          │  │  rtos, can)  │  │           │ │
│  └────┬─────┘  └──────────────┘  └───────────┘ │
│       │                                         │
│  ┌────▼──────────────────────────────────────┐  │
│  │         hf-core-drivers (submodule)       │  │
│  │  BaseI2c, BaseSpi, BaseGpio, BaseAdc      │  │
│  │  AS5047U, BNO08x, PCA9685, SE050, TMC9660 ... │  │
│  └───────────────────────────────────────────┘  │
├─────────────────────────────────────────────────┤
│          MCU Platform (ESP32, etc.)             │
└─────────────────────────────────────────────────┘
```

Handlers **own** their driver instances and translate base-interface calls into
high-level operations. They never expose the raw driver — all access goes through
handler methods.

## 🚀 Quick Start

### Clone

```bash
git clone --recurse-submodules https://github.com/hardfoc/hf-core.git
```

Or after a shallow clone:

```bash
git submodule update --init --recursive
```

### Build and Flash a Test

```bash
cd examples/esp32/scripts

# Build a utility test (no hardware needed)
./build_app.sh general_utils_test Debug

# Flash and monitor
cd .. && idf.py -p /dev/ttyUSB0 flash monitor
```

### Build All CI-Enabled Tests

Use the CI pipeline or iterate over `app_config.yml` entries:

```bash
./build_app.sh list  # See all available apps
```

## 📖 Handlers

| Handler | Device | Bus | Key Capabilities |
|:--------|:-------|:----|:-----------------|
| `As5047uHandler` | AS5047U | SPI | Angle, velocity, DAEC, diagnostics, zero-position |
| `Bno08xHandler` | BNO08x | I2C | Sensor enable/disable, data read, freshness gating |
| `Pca9685Handler` | PCA9685 | I2C | PWM duty/frequency, phase offset, sleep/wake |
| `Pcal95555Handler` | PCAL95555 | I2C | GPIO read/write, toggle, batch, interrupt drain |
| `NtcTemperatureHandler` | NTC | ADC | Temperature, calibration, EMA filter, thresholds |
| `Tmc9660Handler` | TMC9660 | SPI/UART | Motor control, telemetry, GPIO/ADC/temp wrappers |
| `Tmc5160Handler` | TMC5160 | SPI/UART | Stepper motor, ramp generator, StallGuard, visitDriver |
| `Tle92466edHandler` | TLE92466ED | SPI | 6-ch solenoid driver, PWM, diagnostics, watchdog |
| `Max22200Handler` | MAX22200 | SPI | 8-ch solenoid/motor, CDR/VDR, HIT/HOLD, DPM |
| `Ws2812Handler` | WS2812 | RMT | Addressable LED strip, pixel control, animations |
| `Se050Handler` | SE050 / SE050A | I2C | Secure element, T=1 over I²C, `GetDevice()` for `se050::Device` ([details](docs/handlers/se050_handler.md)) |
| `Logger` | — | — | Singleton, log levels, per-tag filter, formatted output, MCU-agnostic via injectable `BaseLogger` backend ([details](handlers/logger/README.md)) |

## 🔧 Utilities

| Library | Contents |
|:--------|:---------|
| `hf-utils-general` | CircularBuffer, RingBuffer, AveragingFilter, CRC, ActionTimer, IntervalAction, SoftwareVersion, LinearInterpolation, SimpleLinkedList, FlagSet, PhysicalUnit, and more |
| `hf-utils-rtos-wrap` | RtosMutex, MutexLockGuard, PeriodicTimer, BaseThread, OsQueue, OsEventFlags, OsSemaphore, os_delay_msec |
| `hf-utils-canopen` | CanFrame, SDO helpers, NMT commands |

## 📂 Repository Structure

```text
hf-core/
├── handlers/           # Device handler implementations
│   ├── as5047u/        #   Magnetic encoder (SPI)
│   ├── bno08x/         #   9-axis IMU (I2C)
│   ├── common/         #   Shared handler utilities (HandlerCommon.h)
│   ├── logger/         #   Singleton logging
│   ├── max22200/       #   8-ch solenoid/motor driver (SPI)
│   ├── ntc/            #   NTC thermistor (ADC)
│   ├── pca9685/        #   16-ch PWM controller (I2C)
│   ├── pcal95555/      #   16-bit GPIO expander (I2C)
│   ├── se050/          #   NXP SE050 secure element (I2C)
│   ├── tle92466ed/     #   6-ch solenoid driver (SPI)
│   ├── tmc5160/        #   Stepper motor driver (SPI/UART)
│   ├── tmc9660/        #   Motor controller (SPI/UART)
│   └── ws2812/         #   Addressable LED strip (RMT)
├── hf-core-drivers/    # [submodule] Base interfaces + external drivers
├── hf-core-utils/      # [submodule] General, RTOS, CANopen utilities
├── examples/
│   └── esp32/          # ESP-IDF test applications
│       ├── app_config.yml      # Test app registry (16 apps)
│       ├── main/
│       │   ├── handler_tests/  # 11 handler test apps
│       │   ├── utils_tests/    # 4 utility test apps
│       │   └── integration_tests/  # 1 integration test
│       └── components/
│           └── hf_core/        # ESP-IDF component wrapper
├── .github/workflows/  # 8 CI pipelines
├── _config/            # clang-format, clang-tidy, Jekyll, Doxygen, linters
├── docs/               # Jekyll documentation site
└── README.md
```

## 📚 Documentation

Full documentation is available at the [GitHub Pages site](https://hardfoc.github.io/hf-core/):

- [Architecture](docs/architecture.md) — Layered design, handler pattern, ownership
- [Folder Structure](docs/folder_structure.md) — Complete tree with descriptions
- [Handler Reference](docs/handlers/index.md) — Per-handler API documentation
- [Utilities Reference](docs/utils/index.md) — General, RTOS, CANopen utilities
- [Testing Guide](docs/testing/testing_guide.md) — How to build and run tests
- [CI Pipelines](docs/testing/ci_pipelines.md) — Workflow descriptions

## 🤝 Contributing

1. Create a feature branch from `develop`
2. Follow `.clang-format` and `.clang-tidy` (configs in `_config/`)
3. Add or update tests for any handler/utility changes
4. Ensure all CI checks pass
5. Open a pull request targeting `develop`

## 📄 License

This project is licensed under the **GNU General Public License v3.0**.
See the [GPL v3 license](https://www.gnu.org/licenses/gpl-3.0) for details.
Individual submodules may carry their own licenses — see per-submodule LICENSE files.
