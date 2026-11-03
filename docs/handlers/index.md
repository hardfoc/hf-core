# Handlers

Every handler in hf-core: the chip it wraps, the interfaces it takes, the switch that compiles it and its host
test.

Each handler follows the [handler model](../handler-model.md): construct it with `Base*` references, let it
initialise on first use, call it from any task. The family pages list each handler's constructors and public
methods, read from its header; the [API reference](../api-reference.md) has every signature.

<!-- generated: handler-catalogue -->
| Handler | Family | Driver | Takes | Switch | Host test |
| --- | --- | --- | --- | --- | --- |
| [`DynamixelHandler`](motion.md#dynamixelhandler) | Motor and motion control | [hf-dynamixel-driver](https://github.com/hardfoc/hf-dynamixel-driver) | `BaseUart` | `HF_CORE_ENABLE_DYNAMIXEL` |  |
| [`Tmc5160Handler`](motion.md#tmc5160handler) | Motor and motion control | [hf-tmc5160-driver](https://github.com/hardfoc/hf-tmc5160-driver) | `BaseGpio`, `BaseSpi`, `BaseUart` | `HF_CORE_ENABLE_TMC5160` |  |
| [`Tmc9660Handler`](motion.md#tmc9660handler) | Motor and motion control | [hf-tmc9660-driver](https://github.com/hardfoc/hf-tmc9660-driver) | `BaseGpio`, `BaseSpi`, `BaseUart` | `HF_CORE_ENABLE_TMC9660` | [`test_tmc9660.cpp`](../../tests/host/test_tmc9660.cpp) |
| [`As5047uHandler`](position.md#as5047uhandler) | Position and inertial sensing | [hf-as5047u-driver](https://github.com/hardfoc/hf-as5047u-driver) | `BaseSpi` | `HF_CORE_ENABLE_AS5047U` | [`test_as5047u.cpp`](../../tests/host/test_as5047u.cpp) |
| [`Bno08xHandler`](position.md#bno08xhandler) | Position and inertial sensing | [hf-bno08x-driver](https://github.com/hardfoc/hf-bno08x-driver) | `BaseGpio`, `BaseI2c`, `BaseSpi` | `HF_CORE_ENABLE_BNO08X` | [`test_bno08x.cpp`](../../tests/host/test_bno08x.cpp) |
| [`Max22200Handler`](actuators.md#max22200handler) | Solenoid and load drivers | [hf-max22200-driver](https://github.com/hardfoc/hf-max22200-driver) | `BaseGpio`, `BaseSpi` | `HF_CORE_ENABLE_MAX22200` |  |
| [`Tle92466edHandler`](actuators.md#tle92466edhandler) | Solenoid and load drivers | [hf-tle92466ed-driver](https://github.com/hardfoc/hf-tle92466ed-driver) | `BaseGpio`, `BaseSpi` | `HF_CORE_ENABLE_TLE92466ED` |  |
| [`Isf15acp4Handler`](io.md#isf15acp4handler) | I/O, PWM, LEDs and displays | [hf-isf15acp4-driver](https://github.com/hardfoc/hf-isf15acp4-driver) | `BaseGpio`, `BaseSpi` | `HF_CORE_ENABLE_ISF15ACP4` |  |
| [`Pca9685Handler`](io.md#pca9685handler) | I/O, PWM, LEDs and displays | [hf-pca9685-driver](https://github.com/hardfoc/hf-pca9685-driver) | `BaseI2c` | `HF_CORE_ENABLE_PCA9685` | [`test_pca9685.cpp`](../../tests/host/test_pca9685.cpp) |
| [`Pcal95555Handler`](io.md#pcal95555handler) | I/O, PWM, LEDs and displays | [hf-pcal95555-driver](https://github.com/hardfoc/hf-pcal95555-driver) | `BaseGpio`, `BaseI2c` | `HF_CORE_ENABLE_PCAL95555` | [`test_pcal95555.cpp`](../../tests/host/test_pcal95555.cpp) |
| [`Ws2812Handler`](io.md#ws2812handler) | I/O, PWM, LEDs and displays | [hf-ws2812-rmt-driver](https://github.com/hardfoc/hf-ws2812-rmt-driver) |  | `HF_CORE_ENABLE_WS2812` |  |
| [`Ads7952Handler`](analog.md#ads7952handler) | Analog and temperature | [hf-ads7952-driver](https://github.com/hardfoc/hf-ads7952-driver) | `BaseSpi` | `HF_CORE_ENABLE_ADS7952` |  |
| [`Ads9324Handler`](analog.md#ads9324handler) | Analog and temperature | [hf-ads9324-driver](https://github.com/hardfoc/hf-ads9324-driver) | `BaseGpio`, `BaseSpi` | `HF_CORE_ENABLE_ADS9324` |  |
| [`Mcp9700TemperatureHandler`](analog.md#mcp9700temperaturehandler) | Analog and temperature | [hf-mcp9700-driver](https://github.com/hardfoc/hf-mcp9700-driver) | `BaseAdc` | `HF_CORE_ENABLE_MCP9700` |  |
| [`NtcTemperatureHandler`](analog.md#ntctemperaturehandler) | Analog and temperature | [hf-ntc-thermistor-driver](https://github.com/hardfoc/hf-ntc-thermistor-driver) | `BaseAdc` | `HF_CORE_ENABLE_NTC_THERMISTOR` | [`test_ntc.cpp`](../../tests/host/test_ntc.cpp) |
| [`AlicatBasis2Handler`](environment.md#alicatbasis2handler) | Pressure, flow and gas | [hf-alicat-basis2-driver](https://github.com/hardfoc/hf-alicat-basis2-driver) | `BaseUart` | `HF_CORE_ENABLE_ALICAT_BASIS2` |  |
| [`Bmp581Handler`](environment.md#bmp581handler) | Pressure, flow and gas | [hf-bmp581-driver](https://github.com/hardfoc/hf-bmp581-driver) | `BaseI2c` | `HF_CORE_ENABLE_BMP581` |  |
| [`Fdo2Handler`](environment.md#fdo2handler) | Pressure, flow and gas | [hf-fdo2-driver](https://github.com/hardfoc/hf-fdo2-driver) | `BaseUart` | `HF_CORE_ENABLE_FDO2` | [`test_fdo2.cpp`](../../tests/host/test_fdo2.cpp) |
| [`SfmHandler`](environment.md#sfmhandler) | Pressure, flow and gas | [hf-sfm-flow-meter-driver](https://github.com/hardfoc/hf-sfm-flow-meter-driver) | `BaseI2c` | `HF_CORE_ENABLE_SFM` |  |
| [`At25040Handler`](power-security.md#at25040handler) | Power, memory and security | [hf-at25040-driver](https://github.com/hardfoc/hf-at25040-driver) | `BaseGpio`, `BaseSpi` | `HF_CORE_ENABLE_AT25040` |  |
| [`Pf1550Handler`](power-security.md#pf1550handler) | Power, memory and security | [hf-pf1550-driver](https://github.com/hardfoc/hf-pf1550-driver) | `BaseGpio`, `BaseI2c` | `HF_CORE_ENABLE_PF1550` | [`test_pf1550.cpp`](../../tests/host/test_pf1550.cpp) |
| [`Se050Handler`](power-security.md#se050handler) | Power, memory and security | [hf-se050-driver](https://github.com/hardfoc/hf-se050-driver) | `BaseGpio`, `BaseI2c` | `HF_CORE_ENABLE_SE050` |  |
| [`Logger`](logger.md#logger) | Logger | (in hf-core) | `BaseLogger` | `HF_CORE_ENABLE_LOGGER` |  |

Read from the headers in [`handlers/`](../../handlers) and [`cmake/hf_core_build_settings.cmake`](../../cmake/hf_core_build_settings.cmake).
<!-- /generated -->

A handler without a host test is still compiled by the host build (except WS2812 and Dynamixel, see
[Feature switches](../feature-switches.md)) and has an ESP32 test application in [Examples](../examples.md).
