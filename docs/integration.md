# Integration

How a project builds hf-core: platform selection, feature switches, the variables the build settings return and
how a HAL or an ESP-IDF component consumes them.

## One file for every consumer

`cmake/hf_core_build_settings.cmake` is the only build entry point on target. It does not create a target: it
fills variables, and the consumer registers them in whatever its build system wants (an ESP-IDF component, a
plain CMake library, an STM32 project). Each external driver's own `build_settings.cmake` stays the source of
truth for that driver's files; hf-core includes it when the driver's switch is on.

## Platform selection

Set both before the `include()`.

| Variable | Values | Default | Effect |
| --- | --- | --- | --- |
| `HF_CORE_MCU` | `ESP32`, `STM32`, `NONE` | `ESP32` | Which `inc/mcu/<mcu>/` implementations of the `Base*` interfaces are compiled; `NONE` compiles none |
| `HF_CORE_RTOS` | `FREERTOS`, `THREADX`, `NONE` | `FREERTOS` | Which RTOS sources of hf-utils-rtos-wrap are compiled and whether `HF_THREAD_SAFE=1` is defined |
| `HF_CORE_CANOPENNODE_ROOT` | path | `${HF_HAL_ROOT}/third_party/CANopenNode` | Upstream CANopenNode, needed only by `HF_CORE_ENABLE_CANOPENNODE` |

The foundation is always compiled for the selected MCU: SPI, I2C, GPIO, ADC, NVS, logger, temperature and
periodic-timer implementations, the RTOS wrapper and hf-utils-general.

## Feature switches

Every driver, handler, optional interface and utility group has a switch. The [feature switch
reference](feature-switches.md) lists all of them with defaults and the definitions they add. Three rules:

- Everything is **off** by default except `HF_CORE_ENABLE_LOGGER`.
- A driver **turns on what it needs**: TMC5160, TMC9660, Alicat BASIS-2, FDO2 and Dynamixel turn on UART; WS2812
  turns on RMT; NTC and MCP9700 turn on ADC.
- A driver tied to one MCU's peripheral **fails at configure time** on another MCU (WS2812 needs the ESP32 RMT).

## Outputs

| Variable | Contents |
| --- | --- |
| `HF_CORE_SOURCES` | Every `.c` / `.cpp` file to compile: interfaces, RTOS, utilities, CANopen, handlers, drivers |
| `HF_CORE_INCLUDE_DIRS` | Include directories for all of them |
| `HF_CORE_IDF_REQUIRES` | ESP-IDF components (`driver`, `freertos`, `esp_driver_spi`, ...; `esp_driver_uart`, `esp_driver_twai`, `esp_adc`, `esp_driver_rmt` when needed); empty for other MCUs |
| `HF_CORE_COMPILE_DEFINITIONS` | `HF_MCU_FAMILY_<MCU>=1`, `HF_RTOS_<RTOS>=1`, `HF_THREAD_SAFE=1` with an RTOS, `HARDFOC_RTOS_WRAP=1`, `HARDFOC_CORE_UTILS=1`, `HARDFOC_CORE_DRIVERS=1` and one `HARDFOC_<CHIP>_SUPPORT=1` per enabled driver |
| `HF_CORE_ROOT` | The hf-core folder |

Configuration prints a summary of the enabled features, interfaces and source counts.

## In an ESP-IDF component

[`examples/esp32/components/hf_core/CMakeLists.txt`](../examples/esp32/components/hf_core/CMakeLists.txt) is
the reference: it enables every handler the examples test, includes the build settings and registers the
outputs with `idf_component_register()`. It also sets the per-target MCU definitions (`HF_TARGET_MCU_ESP32S3=1`
and so on) and the WS2812 Kconfig values.

## In a board HAL

A HAL sets its subset and includes the same file. The Vortex V1 HAL, for example:

```cmake
set(HF_CORE_MCU  "ESP32")
set(HF_CORE_RTOS "FREERTOS")
set(HF_CORE_ENABLE_TMC9660        ON)
set(HF_CORE_ENABLE_AS5047U        ON)
set(HF_CORE_ENABLE_BNO08X         ON)
set(HF_CORE_ENABLE_PCAL95555      ON)
set(HF_CORE_ENABLE_PCA9685        ON)
set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
set(HF_CORE_ENABLE_WS2812         ON)
set(HF_CORE_ENABLE_CAN            ON)
set(HF_CORE_ENABLE_UTILS_CANOPEN  ON)
set(HF_CORE_ENABLE_VORTEX_DRIVER  ON)
include("${CMAKE_CURRENT_LIST_DIR}/lib/core/cmake/hf_core_build_settings.cmake")
```

## On the host

The top-level `CMakeLists.txt` is the host build only (it refuses to cross-compile). It sets
`HF_CORE_MCU=NONE`, `HF_CORE_RTOS=NONE`, turns on every handler that does not need an MCU peripheral or a
third-party checkout, and creates the `hf_core`, `hf_core_host_mocks` and `hf_core_host_test_main` libraries. A
HAL's host tests use it with `add_subdirectory()`. See [Host build and mocks](host-build.md).

Next: [Feature switches](feature-switches.md).
