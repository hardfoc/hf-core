# Examples

The ESP32 test applications: one per handler or utility, selected at build time by `APP_TYPE`.

All of them build from [`examples/esp32/`](../examples/esp32/app_config.yml) with the hf-core component in
`examples/esp32/components/hf_core`, which enables every handler they test. CI builds each application marked for
CI, for every ESP-IDF version and build type listed in `app_config.yml`.

<!-- generated: examples -->
| App | What it shows | Source | CI |
| --- | --- | --- | --- |
| `as5047u_handler_test` :material-star:{ title="featured" } | Comprehensive test of As5047uHandler: CRTP SPI adapter, initialization, angle/velocity reading, DAEC configuration, zero position, diagnostics, OTP readback, error handling, and thread safety. | [`handler_tests/as5047u_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/as5047u_handler_comprehensive_test.cpp) | yes |
| `bno08x_handler_test` :material-star:{ title="featured" } | Comprehensive test of Bno08xHandler: I2C/SPI comm adapter, sensor enable/disable, accelerometer/gyroscope/magnetometer reading, rotation vectors, activity detection, freshness gating, callback management, hardware reset, configuration persistence, and error mapping. | [`handler_tests/bno08x_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/bno08x_handler_comprehensive_test.cpp) | yes |
| `pca9685_handler_test` :material-star:{ title="featured" } | Comprehensive test of Pca9685Handler: I2C comm adapter, EnsureInitialized delegation, PWM duty cycle (float + raw), frequency control, sleep/wake, PwmAdapter multi-channel interface, GpioPin wrapper, phase offset, output configuration, and error handling. | [`handler_tests/pca9685_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/pca9685_handler_comprehensive_test.cpp) | yes |
| `pcal95555_handler_test` :material-star:{ title="featured" } | Comprehensive test of Pcal95555Handler: I2C comm adapter, EnsureInitialized, per-pin direction/read/write/toggle, batch mask operations, interrupt management (deferred ISR via atomic flag), chip variant detection (PCA9555 vs PCAL9555A), Agile I/O features, pull mode, drive strength, output mode, and GpioPin wrapper. | [`handler_tests/pcal95555_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/pcal95555_handler_comprehensive_test.cpp) | yes |
| `ntc_handler_test` :material-star:{ title="featured" } | Comprehensive test of NtcTemperatureHandler: ADC reading, temperature conversion (lookup + math), calibration offset, EMA filtering, continuous monitoring via PeriodicTimer, threshold callbacks, statistics/diagnostics, and RtosMutex thread safety. | [`handler_tests/ntc_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/ntc_handler_comprehensive_test.cpp) | yes |
| `tmc9660_handler_test` :material-star:{ title="featured" } | Comprehensive test of Tmc9660Handler: SPI/UART comm adapter, bootloader initialization, TMCL parameter access, motor control (velocity/position/ torque), telemetry readback, GPIO wrapper (GPIO17/GPIO18), ADC wrapper (multi-channel), Temperature wrapper, error flag management, and peripheral accessor null safety. | [`handler_tests/tmc9660_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/tmc9660_handler_comprehensive_test.cpp) | yes |
| `tmc5160_handler_test` :material-star:{ title="featured" } | Comprehensive test of Tmc5160Handler: CRTP SPI/UART comm adapters, initialization with DriverConfig, motor enable/disable, motion control (position/velocity/acceleration), visitDriver() subsystem access, StallGuard readback, diagnostics dump, status checks (overtemp/stall/ standstill), and pre-init error handling. | [`handler_tests/tmc5160_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/tmc5160_handler_comprehensive_test.cpp) | yes |
| `tle92466ed_handler_test` :material-star:{ title="featured" } | Comprehensive test of Tle92466edHandler: CRTP SPI comm adapter, initialization with GlobalConfig, 6-channel enable/disable/current/PWM, fault report, watchdog kick/disable, device ID readback, diagnostics, channel diagnostics, and pre-init error handling. | [`handler_tests/tle92466ed_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/tle92466ed_handler_comprehensive_test.cpp) | yes |
| `max22200_handler_test` :material-star:{ title="featured" } | Comprehensive test of Max22200Handler: CRTP SPI comm adapter with CMD pin two-phase protocol, initialization with BoardConfig, 8-channel enable/disable/mask, CDR/VDR channel setup, fault readback, device ID, soft reset, diagnostics, and pre-init error handling. | [`handler_tests/max22200_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/max22200_handler_comprehensive_test.cpp) | yes |
| `ads9324_handler_test` :material-star:{ title="featured" } | Comprehensive test of Ads9324Handler: CRTP SPI + CONVST/DRDY adapter, BaseAdc 16-channel map, PGA/offset/gain, snapshot, visitDriver/GetDriver, diagnostics, missing-CONVST guard, and pre-init error handling. Compiles in CI; silicon optional at runtime (init fails gracefully). | [`handler_tests/ads9324_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/ads9324_handler_comprehensive_test.cpp) | yes |
| `isf15acp4_handler_test` | Comprehensive test of Isf15acp4Handler: SPI/GPIO adapter with software SS, initialization, DisplayOn/Off, FillScreen/Present, visitDriver/GetDriver, error flags, deinitialize, and pre-init error handling. | [`handler_tests/isf15acp4_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/isf15acp4_handler_comprehensive_test.cpp) | yes |
| `ws2812_handler_test` :material-star:{ title="featured" } | Comprehensive test of Ws2812Handler: RMT-based LED control, pixel set (individual + all), clear, show, brightness, animation effects (rainbow, chase, breathe), tick/step, direct strip/animator access, diagnostics, and pre-init error handling. | [`handler_tests/ws2812_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/ws2812_handler_comprehensive_test.cpp) | yes |
| `se050_handler_test` :material-star:{ title="featured" } | Exercises Se050Handler: BaseI2c→se050 CRTP adapter, EnsureInitialized (GPIO reset, T=1 warm reset, SELECT applet, GetVersion), and live GetRandom via se050::Device (same stack as hf-se050-driver ESP32 examples). | [`handler_tests/se050_handler_comprehensive_test.cpp`](../examples/esp32/main/handler_tests/se050_handler_comprehensive_test.cpp) | yes |
| `general_utils_test` :material-star:{ title="featured" } | Comprehensive test of hf-utils-general on ESP32/FreeRTOS: CircularBuffer, RingBuffer, AveragingFilter, SimpleStateMachine, SlightlyAdvancedStateMachine, StateMachine (allocation-free FSM with Entry/Loop/Exit hooks, transition matrix, visit/dwell counters, stuck-state watchdog), CrcCalculator, ActionTimer, ActionRunLimiter, DynamicArray, EnumArray, BoundedLinearCurve, PiecewiseLinearCurve, LineEstimator, MultiReadings, TimestampedVariable, VariableMonitor, VariableAnomalyMonitor, SoftwareVersion, TestManager, and platform_compat. | [`utils_tests/general_utils_comprehensive_test.cpp`](../examples/esp32/main/utils_tests/general_utils_comprehensive_test.cpp) | yes |
| `rtos_wrap_test` :material-star:{ title="featured" } | Comprehensive test of hf-utils-rtos-wrap on ESP32/FreeRTOS: RtosMutex, MutexLockGuard, PeriodicTimer, BaseThread, OsQueue, OsEventFlags, SignalSemaphore, CriticalGuard, OsUtility (os_delay_msec, os_time_get), OsAbstraction, ErrorSaver, FlagsSaver, EventDrivenDataMultiThread, and Utility (WaitForCondition). | [`utils_tests/rtos_wrap_comprehensive_test.cpp`](../examples/esp32/main/utils_tests/rtos_wrap_comprehensive_test.cpp) | yes |
| `logger_test` | Test of Logger system: log levels, ANSI formatting (font styles, foreground/background colors), configuration, singleton access, level filtering, and output correctness. | [`utils_tests/logger_comprehensive_test.cpp`](../examples/esp32/main/utils_tests/logger_comprehensive_test.cpp) | yes |
| `canopen_utils_test` | Test of hf-utils-canopen: CANopen protocol stack utilities. Requires CAN bus hardware. | [`utils_tests/canopen_utils_comprehensive_test.cpp`](../examples/esp32/main/utils_tests/canopen_utils_comprehensive_test.cpp) | no |
| `full_integration_test` :material-star:{ title="featured" } | End-to-end integration test exercising multiple handlers and utilities together: I2C bus sharing (PCA9685 + PCAL95555 + BNO08x), SPI bus sharing (AS5047U + TMC9660), RTOS threading with handlers, Logger integration, PeriodicTimer-driven monitoring, and multi-handler concurrent access. | [`integration_tests/full_system_integration_test.cpp`](../examples/esp32/main/integration_tests/full_system_integration_test.cpp) | no |

Build and flash one (from `examples/esp32`, ESP-IDF environment loaded):

```bash
./scripts/build_app.sh general_utils_test Release
./scripts/flash_app.sh flash_monitor general_utils_test Release
```

Source of this table: [`examples/esp32/app_config.yml`](../examples/esp32/app_config.yml).
<!-- /generated -->

## One-time setup

The build scripts are the [hf-espidf-project-tools](https://hardfoc.github.io/hf-espidf-project-tools/)
submodule in `examples/esp32/scripts`. They install or locate ESP-IDF and read `app_config.yml`:

```bash
cd examples/esp32
./scripts/build_app.sh list                          # every application
./scripts/build_app.sh as5047u_handler_test Debug    # build one
./scripts/flash_app.sh flash_monitor as5047u_handler_test Debug
```

## What to read in an example

| Part | Where |
| --- | --- |
| Pin and bus assignments for the test board | `main/esp32_test_config.hpp` (override with `-D` flags) |
| Bus creation (`EspSpi`, `EspI2c`, ...) | `main/esp32_bus_setup.hpp` |
| The test harness (`RUN_TEST`, `RUN_TEST_IN_TASK`, summary) | `main/TestFramework.h` |
| The tests themselves | `main/handler_tests/`, `main/utils_tests/`, `main/integration_tests/` |

Each application prints a pass / fail summary on the serial monitor. Applications that need hardware compile in
CI and skip their hardware sections at run time when the chip does not answer.
