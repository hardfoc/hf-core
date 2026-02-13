---
layout: default
title: Home
nav_order: 1
---

# HardFOC Platform Core (hf-core)

[![ESP-IDF Build](https://github.com/N3b3x/hf-core/actions/workflows/esp32-examples-build-ci.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/esp32-examples-build-ci.yml)
[![C++ Analysis](https://github.com/N3b3x/hf-core/actions/workflows/ci-cpp-analysis.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-cpp-analysis.yml)
[![C++ Lint](https://github.com/N3b3x/hf-core/actions/workflows/ci-cpp-lint.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-cpp-lint.yml)
[![Docs](https://github.com/N3b3x/hf-core/actions/workflows/ci-docs-publish.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-docs-publish.yml)
[![Markdown Lint](https://github.com/N3b3x/hf-core/actions/workflows/ci-markdown-lint.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-markdown-lint.yml)
[![YAML Lint](https://github.com/N3b3x/hf-core/actions/workflows/ci-yaml-lint.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-yaml-lint.yml)
[![Link Check](https://github.com/N3b3x/hf-core/actions/workflows/ci-docs-linkcheck.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/ci-docs-linkcheck.yml)
[![Release](https://github.com/N3b3x/hf-core/actions/workflows/release.yml/badge.svg)](https://github.com/N3b3x/hf-core/actions/workflows/release.yml)

Shared platform layer for all HardFOC boards: base interfaces, drivers, utilities,
and handlers. No board-specific configuration — board HALs (e.g. `hf-hal-vortex-v1`)
depend on this repo and add managers, API, and pin mapping.

## Features

- **7 Device Handlers** — AS5047U, BNO08x, PCA9685, PCAL95555, NTC, TMC9660, Logger
- **30+ General Utilities** — Buffers, filters, timers, CRC, interpolation, linked
  lists, flag sets, physical units
- **Full RTOS Abstraction** — Mutex, semaphore, queue, event flags, threads, timers
- **CANopen Utilities** — CAN frame, SDO protocol, NMT commands
- **ESP32 Test Suite** — 11 test applications covering every handler, utility, and
  integration scenario
- **8 CI Pipelines** — Build, lint, analysis, documentation, release

## Architecture

```
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
│  │  AS5047U, BNO08x, PCA9685, TMC9660 ...   │  │
│  └───────────────────────────────────────────┘  │
├─────────────────────────────────────────────────┤
│          MCU Platform (ESP32, etc.)             │
└─────────────────────────────────────────────────┘
```

Handlers **own** their driver instances and translate base-interface calls into
high-level operations. They never expose the raw driver — all access goes through
handler methods.

## Repository Structure

```
hf-core/
├── handlers/           # Device handler implementations
│   ├── as5047u/        #   Magnetic encoder (SPI)
│   ├── bno08x/         #   9-axis IMU (I2C)
│   ├── pca9685/        #   16-ch PWM controller (I2C)
│   ├── pcal95555/      #   16-bit GPIO expander (I2C)
│   ├── ntc/            #   NTC thermistor (ADC)
│   ├── tmc9660/        #   Motor controller (SPI/UART)
│   └── logger/         #   Singleton logging
├── hf-core-drivers/    # [submodule] Base interfaces + external drivers
├── hf-core-utils/      # [submodule] General, RTOS, CANopen utilities
├── examples/
│   └── esp32/          # ESP-IDF test applications
│       ├── app_config.yml      # Test app registry (11 apps)
│       ├── main/
│       │   ├── handler_tests/  # 6 handler test apps
│       │   ├── utils_tests/    # 4 utility test apps
│       │   └── integration_tests/  # 1 integration test
│       └── components/
│           └── hf_core/        # ESP-IDF component wrapper
├── .github/workflows/  # 8 CI pipelines
├── _config/            # clang-format, clang-tidy, Jekyll, Doxygen, linters
├── docs/               # Jekyll documentation site
└── README.md
```

## Handlers

| Handler | Device | Bus | Key Capabilities |
|:--------|:-------|:----|:-----------------|
| `As5047uHandler` | AS5047U | SPI | Angle, velocity, DAEC, diagnostics, zero-position |
| `Bno08xHandler` | BNO08x | I2C | Sensor enable/disable, data read, freshness gating |
| `Pca9685Handler` | PCA9685 | I2C | PWM duty/frequency, phase offset, sleep/wake |
| `Pcal95555Handler` | PCAL95555 | I2C | GPIO read/write, toggle, batch, interrupt drain |
| `NtcTemperatureHandler` | NTC | ADC | Temperature, calibration, EMA filter, thresholds |
| `Tmc9660Handler` | TMC9660 | SPI/UART | Motor control, telemetry, GPIO/ADC/temp wrappers |
| `Logger` | — | — | Singleton, log levels, per-tag filter, formatted output |

## Utilities

| Library | Contents |
|:--------|:---------|
| `hf-utils-general` | CircularBuffer, RingBuffer, AveragingFilter, CRC, ActionTimer, IntervalAction, SoftwareVersion, LinearInterpolation, SimpleLinkedList, FlagSet, PhysicalUnit, and more |
| `hf-utils-rtos-wrap` | RtosMutex, MutexLockGuard, PeriodicTimer, BaseThread, OsQueue, OsEventFlags, OsSemaphore, OsRecursiveMutex, os_delay_msec |
| `hf-utils-canopen` | CanFrame, SDO helpers, NMT commands |

## Quick Start

### Clone

```bash
git clone --recurse-submodules https://github.com/N3b3x/hf-core.git
```

Or after a shallow clone:

```bash
git submodule update --init --recursive
```

### Build and Flash a Test

```bash
cd examples/esp32/scripts

# Build a utility test (no hardware needed)
./build_app.sh --app general_utils_test --build-type debug

# Flash and monitor
cd .. && idf.py -p /dev/ttyUSB0 flash monitor
```

### Build All CI-Enabled Tests

```bash
./build_app.sh --all --build-type debug
```

## Documentation

Full documentation is available at the [GitHub Pages site](https://n3b3x.github.io/hf-core/):

- [Architecture](docs/architecture.md) — Layered design, handler pattern, ownership
- [Folder Structure](docs/folder_structure.md) — Complete tree with descriptions
- [Handler Reference](docs/handlers/index.md) — Per-handler API documentation
- [Utilities Reference](docs/utils/index.md) — General, RTOS, CANopen utilities
- [Testing Guide](docs/testing/testing_guide.md) — How to build and run tests
- [CI Pipelines](docs/testing/ci_pipelines.md) — Workflow descriptions

## Contributing

1. Create a feature branch from `develop`
2. Follow `.clang-format` and `.clang-tidy` (configs in `_config/`)
3. Add or update tests for any handler/utility changes
4. Ensure all CI checks pass
5. Open a pull request targeting `develop`

## License

See LICENSE and per-submodule licenses.
