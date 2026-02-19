---
layout: default
title: "⚙️ CMake Integration"
description: "Configurable CMake build system for hf-core"
nav_order: 5
permalink: /docs/cmake_integration/
---

# hf-core CMake Integration

This document describes the **configurable CMake build system** that allows
consumers (HAL layers, standalone projects, core examples) to select exactly
which drivers, handlers, and utilities are compiled into their build.

---

## Design Goals

| Goal | How It's Achieved |
|------|-------------------|
| **Lean builds** | Each driver/handler is opt-in — only enabled features are compiled |
| **Zero duplication** | HAL CMakeLists.txt no longer re-enumerate core source files |
| **Driver delegation** | Each external driver's `build_settings.cmake` is the single source of truth for its sources and includes |
| **Auto dependencies** | Enabling a driver automatically enables the bus interfaces it needs (e.g., TMC9660 → UART) |
| **Familiar pattern** | Follows the same 3-layer CMake contract used by all HardFOC external drivers |

---

## Architecture

```
┌──────────────────────────────────────────────────────────────┐
│            core/cmake/hf_core_build_settings.cmake           │
│              (Configurable Source/Include Collector)         │
│                                                              │
│  • Feature toggles (HF_CORE_ENABLE_xxx)                      │
│  • Auto-enables bus dependencies                             │
│  • Includes each driver's build_settings.cmake               │
│  • Collects HF_CORE_SOURCES, HF_CORE_INCLUDE_DIRS,           │
│    HF_CORE_IDF_REQUIRES, HF_CORE_COMPILE_DEFINITIONS         │
└────────────┬────────────────────────────┬────────────────────┘
             │                            │
┌────────────▼───────────────┐ ┌──────────▼──────────────────┐
│  Core ESP-IDF Wrapper      │ │  HAL CMakeLists.txt         │
│  (examples/esp32/          │ │  (hf-hal-vortex-v1/ etc.)   │
│   components/hf_core/)     │ │                             │
│                            │ │  • Enables feature subset   │
│  • Enables ALL features    │ │  • Includes build settings  │
│  • Includes build settings │ │  • Adds HAL-specific API +  │
│  • idf_component_register()│ │    managers on top          │
└────────────────────────────┘ └─────────────────────────────┘
```

---

## Usage

### 1. Set Feature Toggles

Before including the build settings, set variables for the features you need:

```cmake
# Enable only what your HAL uses
set(HF_CORE_ENABLE_TMC9660        ON)
set(HF_CORE_ENABLE_PCAL95555      ON)
set(HF_CORE_ENABLE_AS5047U        ON)
set(HF_CORE_ENABLE_BNO08X         ON)
set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
set(HF_CORE_ENABLE_WS2812         ON)
set(HF_CORE_ENABLE_LOGGER         ON)
set(HF_CORE_ENABLE_UTILS_CANOPEN  ON)
set(HF_CORE_ENABLE_CAN            ON)
```

### 2. Include the Build Settings

```cmake
include("${CORE_PATH}/cmake/hf_core_build_settings.cmake")
```

### 3. Use the Collected Variables

```cmake
idf_component_register(
    SRCS
        ${HF_CORE_SOURCES}
        ${MY_HAL_SOURCES}
    INCLUDE_DIRS
        ${HF_CORE_INCLUDE_DIRS}
        ${MY_HAL_INCLUDE_DIRS}
    REQUIRES
        ${HF_CORE_IDF_REQUIRES}
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC ${HF_CORE_COMPILE_DEFINITIONS})
```

---

## Feature Toggles Reference

### External Drivers + Handlers (opt-in, default OFF)

| Toggle | Driver | Bus | Handler |
|--------|--------|-----|---------|
| `HF_CORE_ENABLE_AS5047U` | hf-as5047u-driver | SPI | As5047uHandler |
| `HF_CORE_ENABLE_BNO08X` | hf-bno08x-driver | I2C/SPI | Bno08xHandler |
| `HF_CORE_ENABLE_MAX22200` | hf-max22200-driver | SPI | Max22200Handler |
| `HF_CORE_ENABLE_NTC_THERMISTOR` | hf-ntc-thermistor-driver | ADC | NtcTemperatureHandler |
| `HF_CORE_ENABLE_PCA9685` | hf-pca9685-driver | I2C | Pca9685Handler |
| `HF_CORE_ENABLE_PCAL95555` | hf-pcal95555-driver | I2C | Pcal95555Handler |
| `HF_CORE_ENABLE_TLE92466ED` | hf-tle92466ed-driver | SPI | Tle92466edHandler |
| `HF_CORE_ENABLE_TMC5160` | hf-tmc5160-driver | SPI/UART | Tmc5160Handler |
| `HF_CORE_ENABLE_TMC9660` | hf-tmc9660-driver | SPI/UART | Tmc9660Handler + Tmc9660AdcWrapper |
| `HF_CORE_ENABLE_WS2812` | hf-ws2812-rmt-driver | RMT | Ws2812Handler |
| `HF_CORE_ENABLE_LOGGER` | internal | — | Logger (default ON) |

### Optional Interface Implementations (default OFF)

| Toggle | ESP32 Implementation | Auto-Enabled By |
|--------|---------------------|-----------------|
| `HF_CORE_ENABLE_UART` | EspUart | TMC5160, TMC9660 |
| `HF_CORE_ENABLE_CAN` | EspCan | (manual only) |
| `HF_CORE_ENABLE_PWM` | EspPwm | (manual only) |
| `HF_CORE_ENABLE_PIO` | EspPio | (manual only) |

### Optional Utility Groups (default OFF)

| Toggle | Module |
|--------|--------|
| `HF_CORE_ENABLE_UTILS_CANOPEN` | hf-utils-canopen (CanOpenUtils, CanOpenMotorUtils) |

### Foundation (always included, cannot be disabled)

- **ESP32 Interface Implementations**: EspSpi, EspI2c, EspGpio, EspAdc, EspNvs, EspLogger, EspTemperature, EspPeriodicTimer, AsciiArtGenerator, DigitalOutputGuard
- **RTOS Wrappers**: BaseThread, CriticalSection, FreeRTOSUtils, Mutex, MutexGuard, OsUtility, PeriodicTimer, SignalSemaphore
- **General Utilities**: SoftwareVersion, TimestampedVariable, CrcCalculator, Utility, VariableAnomalyMonitor, VariableMonitor

---

## Auto-Enabled Dependencies

The build settings automatically enable bus interfaces required by selected drivers:

| If You Enable... | Auto-Enabled |
|-----------------|--------------|
| `TMC5160` or `TMC9660` | `HF_CORE_ENABLE_UART` + `esp_driver_uart` IDF component |
| `WS2812` | `HF_CORE_ENABLE_RMT` + `esp_driver_rmt` IDF component |
| `NTC_THERMISTOR` | `HF_CORE_ENABLE_ADC` + `esp_adc` IDF component |

---

## Output Variables

After including `hf_core_build_settings.cmake`, these variables are available:

| Variable | Contents |
|----------|----------|
| `HF_CORE_SOURCES` | All collected `.cpp`/`.c` source files |
| `HF_CORE_INCLUDE_DIRS` | All include directories (core + driver + handler) |
| `HF_CORE_IDF_REQUIRES` | ESP-IDF component requirements list |
| `HF_CORE_COMPILE_DEFINITIONS` | Feature-based preprocessor definitions |
| `HF_CORE_ROOT` | Resolved path to core/ directory |

### Compile Definitions Generated

Each enabled feature adds a preprocessor definition:

```
HARDFOC_AS5047U_SUPPORT=1      (if HF_CORE_ENABLE_AS5047U)
HARDFOC_BNO08X_SUPPORT=1       (if HF_CORE_ENABLE_BNO08X)
HARDFOC_TMC9660_SUPPORT=1      (if HF_CORE_ENABLE_TMC9660)
HARDFOC_LOGGER=1               (if HF_CORE_ENABLE_LOGGER)
HARDFOC_CANOPEN_UTILS=1        (if HF_CORE_ENABLE_UTILS_CANOPEN)
HF_MCU_FAMILY_ESP32=1          (always)
HF_THREAD_SAFE=1               (always)
HARDFOC_RTOS_WRAP=1            (always)
HARDFOC_CORE_UTILS=1           (always)
HARDFOC_CORE_DRIVERS=1         (always)
```

---

## Configuration Summary

During CMake configuration, a summary is printed showing enabled features
and source/include counts:

```
╔══════════════════════════════════════════════════════╗
║           hf-core Build Configuration               ║
╚══════════════════════════════════════════════════════╝
  Features:      TMC9660 PCAL95555 AS5047U BNO08x NTC WS2812 Logger CANopen
  Interfaces:    SPI I2C GPIO ADC NVS Timer UART
  Sources:       42 total (8 handlers, 7 driver)
  Include dirs:  28
  IDF requires:  driver;freertos;nvs_flash;esp_timer;hal;soc;log;...
```

---

## Examples

### Minimal Build (foundation only)

```cmake
# No drivers enabled — just interfaces, RTOS, and general utils
include("${CORE_PATH}/cmake/hf_core_build_settings.cmake")
```

### HAL Build (Vortex subset)

```cmake
set(HF_CORE_ENABLE_TMC9660        ON)
set(HF_CORE_ENABLE_PCAL95555      ON)
set(HF_CORE_ENABLE_AS5047U        ON)
set(HF_CORE_ENABLE_BNO08X         ON)
set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
set(HF_CORE_ENABLE_WS2812         ON)
set(HF_CORE_ENABLE_LOGGER         ON)
set(HF_CORE_ENABLE_UTILS_CANOPEN  ON)
set(HF_CORE_ENABLE_UART           ON)  # Already auto-enabled by TMC9660
set(HF_CORE_ENABLE_CAN            ON)
set(HF_CORE_ENABLE_PWM            ON)
set(HF_CORE_ENABLE_PIO            ON)
include("${CORE_PATH}/cmake/hf_core_build_settings.cmake")
```

### Core Examples (all features)

```cmake
set(HF_CORE_ENABLE_AS5047U        ON)
set(HF_CORE_ENABLE_BNO08X         ON)
set(HF_CORE_ENABLE_MAX22200       ON)
set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
set(HF_CORE_ENABLE_PCA9685        ON)
set(HF_CORE_ENABLE_PCAL95555      ON)
set(HF_CORE_ENABLE_TLE92466ED     ON)
set(HF_CORE_ENABLE_TMC5160        ON)
set(HF_CORE_ENABLE_TMC9660        ON)
set(HF_CORE_ENABLE_WS2812         ON)
set(HF_CORE_ENABLE_LOGGER         ON)
set(HF_CORE_ENABLE_UTILS_CANOPEN  ON)
set(HF_CORE_ENABLE_UART           ON)
set(HF_CORE_ENABLE_CAN            ON)
set(HF_CORE_ENABLE_PWM            ON)
set(HF_CORE_ENABLE_PIO            ON)
include("${CORE_PATH}/cmake/hf_core_build_settings.cmake")
```

---

## Driver Build Settings Styles

The 10 external drivers use two build settings patterns. The core build settings
handles both transparently:

| Style | Drivers | Guard | Root Path | Include Var | Source Var |
|-------|---------|-------|-----------|-------------|-----------|
| **A** | AS5047U, BNO08x, MAX22200, PCA9685, PCAL95555, TLE92466ED, TMC5160 | `include_guard(GLOBAL)` | `CMAKE_CURRENT_LIST_DIR` | `HF_<NAME>_PUBLIC_INCLUDE_DIRS` | `HF_<NAME>_SOURCE_FILES` |
| **B** | NTC Thermistor, TMC9660, WS2812 | Manual boolean | Requires `HF_<NAME>_ROOT` | `HF_<NAME>_INCLUDE_DIRS` | `HF_<NAME>_SOURCES` |

The core build settings sets `HF_<NAME>_ROOT` automatically for Style B drivers
before including them.
