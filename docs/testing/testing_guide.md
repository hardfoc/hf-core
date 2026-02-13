---
layout: default
title: Testing Guide
parent: Testing
nav_order: 1
---

# Testing Guide

## Overview

hf-core uses an ESP32-based testing infrastructure that mirrors the pattern used by
all external HardFOC drivers. Each test is a standalone ESP-IDF application selected
at build time via the `APP_TYPE` mechanism.

## Test Applications

### Handler Tests

| APP_TYPE | Source File | Hardware Required |
|:---------|:-----------|:-----------------|
| `as5047u_handler_test` | `handler_tests/as5047u_handler_comprehensive_test.cpp` | AS5047U + SPI |
| `bno08x_handler_test` | `handler_tests/bno08x_handler_comprehensive_test.cpp` | BNO08x + I2C |
| `pca9685_handler_test` | `handler_tests/pca9685_handler_comprehensive_test.cpp` | PCA9685 + I2C |
| `pcal95555_handler_test` | `handler_tests/pcal95555_handler_comprehensive_test.cpp` | PCAL95555 + I2C |
| `ntc_handler_test` | `handler_tests/ntc_handler_comprehensive_test.cpp` | NTC + ADC |
| `tmc9660_handler_test` | `handler_tests/tmc9660_handler_comprehensive_test.cpp` | TMC9660 + SPI |

### Utility Tests

| APP_TYPE | Source File | Hardware Required |
|:---------|:-----------|:-----------------|
| `general_utils_test` | `utils_tests/general_utils_comprehensive_test.cpp` | None |
| `rtos_wrap_test` | `utils_tests/rtos_wrap_comprehensive_test.cpp` | None (FreeRTOS only) |
| `logger_test` | `utils_tests/logger_comprehensive_test.cpp` | None |
| `canopen_utils_test` | `utils_tests/canopen_utils_comprehensive_test.cpp` | None |

### Integration Tests

| APP_TYPE | Source File | Hardware Required |
|:---------|:-----------|:-----------------|
| `full_integration_test` | `integration_tests/full_system_integration_test.cpp` | All peripherals |

## Building a Test

### Using build_app.sh

```bash
cd examples/esp32/scripts
./build_app.sh --app as5047u_handler_test --build-type debug
```

### Using idf.py directly

```bash
cd examples/esp32
export APP_TYPE=general_utils_test
export BUILD_TYPE=debug
idf.py build
```

### Using VS Code ESP-IDF Extension

1. Set `APP_TYPE` environment variable in your workspace settings
2. Use the ESP-IDF build command

## Flashing and Monitoring

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

## Pin Configuration

Pin assignments are defined in `esp32_test_config.hpp` and can be overridden with
compile-time `-D` flags. See the file for the complete pin map.

## Test Framework

All tests use `TestFramework.h` — a shared header providing:

- `RUN_TEST(name, fn)` — Run a test function, track pass/fail
- `RUN_TEST_IN_TASK(name, fn, stack, timeout)` — Run in a FreeRTOS task with timeout
- `RUN_TEST_SECTION_IF_ENABLED(flag, name, ...)` — Conditionally run test groups
- `flip_test_progress_indicator()` — Toggle GPIO14 for visual progress
- `print_test_summary(results, name, tag)` — Print final pass/fail report

## Writing New Tests

Follow the existing pattern:

```cpp
#include "TestFramework.h"
static TestResults g_test_results;

static bool test_my_feature() noexcept {
    // ... test logic ...
    return success;
}

extern "C" void app_main(void) {
    RUN_TEST("my_feature", test_my_feature);
    print_test_summary(g_test_results, "MY TEST SUITE", "MyTag");
    while (true) { vTaskDelay(pdMS_TO_TICKS(10000)); }
}
```

Then add the app to `app_config.yml` and update `main/CMakeLists.txt`.
