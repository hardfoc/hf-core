# Quickstart

Add hf-core to an ESP-IDF project, turn on one handler and drive a chip through it.

The example uses the PCA9555 / PCAL9555A I/O expander because it needs only an I2C device. Every handler
follows the same steps.

<div class="hf-steps" markdown>

1. **Add the repository.** hf-core and its submodules (the drivers, the interface wrap, the utilities) go in
   one checkout:

    ```bash
    git submodule add https://github.com/hardfoc/hf-core.git lib/core
    git submodule update --init --recursive lib/core
    ```

    Check it out as a folder named `core`: sources include `core/...` paths, which resolve from the folder above
    it. Any other name works too; the build settings then expose the tree as `core/` through a link in the build
    directory.

2. **Choose the handlers.** In the CMakeLists.txt of the component that builds hf-core, set the switches you
   need, include the build settings and register what they collect:

    ```cmake title="components/hf_core/CMakeLists.txt"
    set(HF_CORE_MCU  "ESP32")
    set(HF_CORE_RTOS "FREERTOS")
    set(HF_CORE_ENABLE_PCAL95555 ON)

    include("${CMAKE_CURRENT_LIST_DIR}/../../lib/core/cmake/hf_core_build_settings.cmake")

    idf_component_register(
        SRCS         ${HF_CORE_SOURCES}
        INCLUDE_DIRS ${HF_CORE_INCLUDE_DIRS}
        REQUIRES     ${HF_CORE_IDF_REQUIRES})
    set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC ${HF_CORE_COMPILE_DEFINITIONS})
    ```

3. **Drive the chip.** The handler takes the expander's `BaseI2c` device, which the board HAL creates (on
   ESP32, an `EspI2cDevice` from an `EspI2cBus`):

    ```cpp title="tests/docs/quickstart.cpp"
    --8<-- "tests/docs/quickstart.cpp:quickstart"
    ```

</div>

## What each step does

| Step | What happens |
| --- | --- |
| `Pcal95555Handler expander(i2c)` | Stores the reference. No bus traffic, no allocation of the driver yet |
| `EnsureInitialized()` | Creates the driver, probes the chip and caches the port registers; later calls return at once |
| `SetDirection()`, `Toggle()` | Each locks the handler's mutex, so two tasks can share one handler |
| `handler_utils::DelayMs()` | `vTaskDelay` on ESP-IDF, the OS abstraction elsewhere, the fake clock in host tests |

The same function runs unchanged in a host test against `hf::host::MockI2c`; the host build does exactly that
(`tests/docs/doc_snippets_test.cpp`).

## If it does not work

| Symptom | Check |
| --- | --- |
| CMake: `Unsupported MCU` | `HF_CORE_MCU` is one of `ESP32`, `STM32`, `NONE` |
| CMake: a driver's `build_settings.cmake` not found | The submodules are not initialised: `git submodule update --init --recursive` |
| Link errors for `Esp*` classes | The interface is optional: set `HF_CORE_ENABLE_UART`, `_CAN`, `_PWM` or `_PIO` |
| `EnsureInitialized()` returns false | Address straps, pull-ups, power; the bus itself must be initialised by the HAL |
| `Driver 'WS2812' requires MCU=ESP32` | The WS2812 handler needs the ESP32 RMT peripheral |

Next: [Integration](integration.md) for every switch and output, or the [handler catalogue](handlers/index.md).
