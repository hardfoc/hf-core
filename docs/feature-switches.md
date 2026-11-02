# Feature switches

Every `HF_CORE_ENABLE_*` switch, its default, what turns it on, what it compiles and the definition it adds.

Set a switch before including `cmake/hf_core_build_settings.cmake`; a switch you leave unset takes the default.
"Host build" is the value the top-level `CMakeLists.txt` uses for the host tests. "Turned on by" lists the
switches that set it automatically.

<div class="hf-registers" markdown>

<!-- generated: feature-switches -->
| Switch | Default | Host build | Turned on by | Compiles | Defines |
| --- | --- | --- | --- | --- | --- |
| **Utility groups** | | | | | |
| `HF_CORE_ENABLE_UTILS_CANOPEN` | OFF | ON |  | [hf-utils-canopen](https://github.com/hardfoc/hf-utils-canopen) | `HARDFOC_CANOPEN_UTILS` |
| `HF_CORE_ENABLE_CANOPENNODE` | OFF | OFF |  |  | `HARDFOC_CANOPENNODE_SLAVE` |
| **MCU interfaces** | | | | | |
| `HF_CORE_ENABLE_UART` | OFF | ON | `TMC5160`, `TMC9660`, `ALICAT_BASIS2`, `FDO2`, `DYNAMIXEL` | `EspUart` / `StmUart` |  |
| `HF_CORE_ENABLE_USB_SERIAL_JTAG` | OFF | OFF |  | `EspUsbSerialJtag` | `HARDFOC_USB_SERIAL_JTAG_SUPPORT` |
| `HF_CORE_ENABLE_CAN` | OFF | ON |  | `EspCan` / `StmCan` |  |
| `HF_CORE_ENABLE_PWM` | OFF | OFF |  | `EspPwm` / `StmPwm` |  |
| `HF_CORE_ENABLE_PIO` | OFF | OFF |  | `EspPio` |  |
| **Drivers and handlers** | | | | | |
| `HF_CORE_ENABLE_AS5047U` | OFF | ON |  | [`handlers/as5047u/`](handlers/position.md) + [hf-as5047u-driver](https://github.com/hardfoc/hf-as5047u-driver) | `HARDFOC_AS5047U_SUPPORT` |
| `HF_CORE_ENABLE_ADS7952` | OFF | ON |  | [`handlers/ads7952/`](handlers/analog.md) + [hf-ads7952-driver](https://github.com/hardfoc/hf-ads7952-driver) | `HARDFOC_ADS7952_SUPPORT` |
| `HF_CORE_ENABLE_ADS9324` | OFF | ON |  | [`handlers/ads9324/`](handlers/analog.md) + [hf-ads9324-driver](https://github.com/hardfoc/hf-ads9324-driver) | `HARDFOC_ADS9324_SUPPORT` |
| `HF_CORE_ENABLE_AT25040` | OFF | ON |  | [`handlers/at25040/`](handlers/power-security.md) + [hf-at25040-driver](https://github.com/hardfoc/hf-at25040-driver) | `HARDFOC_AT25040_SUPPORT` |
| `HF_CORE_ENABLE_BNO08X` | OFF | ON |  | [`handlers/bno08x/`](handlers/position.md) + [hf-bno08x-driver](https://github.com/hardfoc/hf-bno08x-driver) | `HARDFOC_BNO08X_SUPPORT` |
| `HF_CORE_ENABLE_MAX22200` | OFF | ON |  | [`handlers/max22200/`](handlers/actuators.md) + [hf-max22200-driver](https://github.com/hardfoc/hf-max22200-driver) | `HARDFOC_MAX22200_SUPPORT` |
| `HF_CORE_ENABLE_NTC_THERMISTOR` | OFF | ON |  | [`handlers/ntc/`](handlers/analog.md) + [hf-ntc-thermistor-driver](https://github.com/hardfoc/hf-ntc-thermistor-driver) | `HARDFOC_NTC_THERMISTOR_SUPPORT` |
| `HF_CORE_ENABLE_MCP9700` | OFF | ON |  | [`handlers/mcp9700/`](handlers/analog.md) + [hf-mcp9700-driver](https://github.com/hardfoc/hf-mcp9700-driver) | `HARDFOC_MCP9700_SUPPORT` |
| `HF_CORE_ENABLE_ALICAT_BASIS2` | OFF | ON |  | [`handlers/alicat_basis2/`](handlers/environment.md) + [hf-alicat-basis2-driver](https://github.com/hardfoc/hf-alicat-basis2-driver) | `HARDFOC_ALICAT_BASIS2_SUPPORT` |
| `HF_CORE_ENABLE_FDO2` | OFF | ON |  | [`handlers/fdo2/`](handlers/environment.md) + [hf-fdo2-driver](https://github.com/hardfoc/hf-fdo2-driver) | `HARDFOC_FDO2_SUPPORT` |
| `HF_CORE_ENABLE_DYNAMIXEL` | OFF | OFF |  | [`handlers/dynamixel/`](handlers/motion.md) + [hf-dynamixel-driver](https://github.com/hardfoc/hf-dynamixel-driver) | `HARDFOC_DYNAMIXEL_SUPPORT` |
| `HF_CORE_ENABLE_SFM` | OFF | ON |  | [`handlers/sfm/`](handlers/environment.md) + [hf-sfm-flow-meter-driver](https://github.com/hardfoc/hf-sfm-flow-meter-driver) | `HARDFOC_SFM_SUPPORT` |
| `HF_CORE_ENABLE_BMP581` | OFF | ON |  | [`handlers/bmp581/`](handlers/environment.md) + [hf-bmp581-driver](https://github.com/hardfoc/hf-bmp581-driver) | `HARDFOC_BMP581_SUPPORT` |
| `HF_CORE_ENABLE_SE050` | OFF | ON |  | [`handlers/se050/`](handlers/power-security.md) + [hf-se050-driver](https://github.com/hardfoc/hf-se050-driver) |  |
| `HF_CORE_ENABLE_PCA9685` | OFF | ON |  | [`handlers/pca9685/`](handlers/io.md) + [hf-pca9685-driver](https://github.com/hardfoc/hf-pca9685-driver) | `HARDFOC_PCA9685_SUPPORT` |
| `HF_CORE_ENABLE_PF1550` | OFF | ON |  | [`handlers/pf1550/`](handlers/power-security.md) + [hf-pf1550-driver](https://github.com/hardfoc/hf-pf1550-driver) | `HARDFOC_PF1550_SUPPORT` |
| `HF_CORE_ENABLE_PCAL95555` | OFF | ON |  | [`handlers/pcal95555/`](handlers/io.md) + [hf-pcal95555-driver](https://github.com/hardfoc/hf-pcal95555-driver) | `HARDFOC_PCAL95555_SUPPORT` |
| `HF_CORE_ENABLE_TLE92466ED` | OFF | ON |  | [`handlers/tle92466ed/`](handlers/actuators.md) + [hf-tle92466ed-driver](https://github.com/hardfoc/hf-tle92466ed-driver) | `HARDFOC_TLE92466_SUPPORT` |
| `HF_CORE_ENABLE_TMC5160` | OFF | ON |  | [`handlers/tmc5160/`](handlers/motion.md) + [hf-tmc5160-driver](https://github.com/hardfoc/hf-tmc5160-driver) | `HARDFOC_TMC5160_SUPPORT` |
| `HF_CORE_ENABLE_TMC9660` | OFF | ON |  | [`handlers/tmc9660/`](handlers/motion.md) + [hf-tmc9660-driver](https://github.com/hardfoc/hf-tmc9660-driver) | `HARDFOC_TMC9660_SUPPORT` |
| `HF_CORE_ENABLE_WS2812` | OFF | OFF |  | [`handlers/ws2812/`](handlers/io.md) + [hf-ws2812-rmt-driver](https://github.com/hardfoc/hf-ws2812-rmt-driver) (needs ESP32: RMT peripheral) | `HARDFOC_WS2812_SUPPORT` |
| `HF_CORE_ENABLE_ISF15ACP4` | OFF | ON |  | [`handlers/isf15acp4/`](handlers/io.md) + [hf-isf15acp4-driver](https://github.com/hardfoc/hf-isf15acp4-driver) | `HARDFOC_ISF15ACP4_SUPPORT` |
| `HF_CORE_ENABLE_VORTEX_DRIVER` | OFF | ON |  | [hf-vortex-driver](https://github.com/hardfoc/hf-vortex-driver) | `HARDFOC_VORTEX_DRIVER` |
| **Logger** | | | | | |
| `HF_CORE_ENABLE_LOGGER` | ON | ON |  | [`handlers/logger/`](handlers/logger.md) | `HARDFOC_LOGGER` |
| **Peripherals a driver turns on** | | | | | |
| `HF_CORE_ENABLE_RMT` | OFF | OFF | `WS2812` |  |  |
| `HF_CORE_ENABLE_ADC` | OFF | ON | `NTC_THERMISTOR`, `MCP9700` |  |  |

Read from [`cmake/hf_core_build_settings.cmake`](../cmake/hf_core_build_settings.cmake) and [`CMakeLists.txt`](../CMakeLists.txt).
<!-- /generated -->

</div>

Optional interfaces add their `Esp*` (or `Stm*`) source and ESP-IDF component: `UART` adds `EspUart` and
`esp_driver_uart`, `CAN` adds `EspCan` and `esp_driver_twai` (ESP-IDF 5.5 and later), `USB_SERIAL_JTAG` adds
`EspUsbSerialJtag`, `PWM` adds `EspPwm`, `PIO` adds `EspPio`. `HF_CORE_ENABLE_CANOPENNODE` needs
`HF_CORE_ENABLE_UTILS_CANOPEN` and an upstream CANopenNode checkout at `HF_CORE_CANOPENNODE_ROOT`.
