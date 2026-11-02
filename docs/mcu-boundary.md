# MCU boundary

Which files may include a vendor SDK, how the host build enforces it, and where ESP32, STM32 and CAN FD support
stand.

## The rule

Everything in hf-core above the `Base*` interfaces is MCU-agnostic. It may include only `Base*` headers,
`OsAbstraction.h` / `OsUtility.h` and `HandlerCommon.h` for platform services. The host build compiles and links
all of it with `HF_CORE_MCU=NONE` and `HF_CORE_RTOS=NONE`; a vendor include anywhere else breaks that build in CI.

| May include ESP-IDF, STM32 HAL, FreeRTOS or other vendor headers | Must not |
| --- | --- |
| `inc/mcu/<mcu>/` and `src/mcu/<mcu>/` in hf-internal-interface-wrap: the `Esp*` and `Stm*` implementations | Every other file in `handlers/` |
| `handlers/logger/factory/<Mcu>LoggerFactory.cpp`: the default log backend per MCU | The device drivers in `hf-core-drivers/external/` |
| `os/os_<rtos>.h` and the RTOS sources of hf-utils-rtos-wrap selected by `HF_CORE_RTOS` | hf-utils-general and hf-utils-canopen |
| MCU-gated handlers in the gate table of the build settings (WS2812 on the ESP32 RMT) and `handlers/common/canopen/CO_driver_*` | |

Where a handler needs an MCU-specific path, it guards it on the platform macro (`ESP_PLATFORM`,
`HF_MCU_FAMILY_STM32`) and keeps a portable fallback. `handler_utils::DelayMs()` is the pattern: `vTaskDelay` on
ESP-IDF, the OS abstraction with an RTOS, `HAL_Delay` with the STM32 HAL, a busy-wait otherwise.

Known exceptions, kept for now and not compiled on the host: `Pcal95555Handler` and `Se050Handler` contain
board-specific blocks guarded by `PW_HAL_STM32H7` that include `StmI2c.h`. `HalSpiMax22200Comm::Transfer` stages
the caller's buffers with aligned 32-bit accesses for one MCU's memory map; it is compiled everywhere and touches
up to three bytes either side of the buffers, which an address sanitizer would flag.

## Where each MCU stands

| MCU | `HF_CORE_MCU` | Interfaces | Status |
| --- | --- | --- | --- |
| ESP32 family (ESP32, -S3, -C6, ...) | `ESP32` | `Esp*`: SPI, I2C, UART, CAN (TWAI), GPIO, ADC, PWM, PIO (RMT), NVS, logger, temperature, timers, USB Serial/JTAG | Production: both board HALs; every example builds in CI |
| STM32 | `STM32` | `Stm*`: SPI, I2C, UART, CAN, GPIO, ADC, PWM, NVS, logger, temperature, timers, over handles from STM32CubeMX | Selectable, not built or tested in CI. `StmCan` does not implement the BaseCan v2 bus-status calls yet |
| Host | `NONE` | none: the [mocks](host-build.md#the-mocks) | CI on every pull request |

[`STM32H7_PORTING.md`](../STM32H7_PORTING.md) records the changes one downstream STM32H7 integration needed;
they are the starting list for making the STM32 path a tested target.

## CAN and CAN FD

`BaseCan` messages are FD-sized (`hf_can_message_t`: 64-byte `data`, `is_canfd`, `is_brs`), so an FD controller
fits the same interface. The ESP32 TWAI controller is classic CAN, and the CANopen bridge sends classic frames
(`is_canfd = false`, at most 8 bytes). An FD implementation goes in the interface wrap (an `Stm*` class over
FDCAN, for example); handlers and the CANopen helpers do not change.

## Adding an MCU

1. Implement the `Base*` interfaces under `inc/mcu/<mcu>/` and `src/mcu/<mcu>/` in hf-internal-interface-wrap
   ([porting guide](https://hardfoc.github.io/hf-internal-interface-wrap/)).
2. Add the MCU to `_HF_SUPPORTED_MCUS` and its source list to the interface section of
   `cmake/hf_core_build_settings.cmake`, with the build-system requirements it needs.
3. Add a logger factory in `handlers/logger/factory/`.
4. Add the MCU's peripheral-gated drivers to the gate table, if any.
5. Build a HAL example for it in CI.

Next: [Handlers](handlers/index.md).
