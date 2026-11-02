# API reference

Every handler class, member and parameter, and the host mocks, generated from the headers by Doxygen.

[Open the API reference](api/index.html){ .md-button .md-button--primary }

## Start here

| Page | What it documents |
| --- | --- |
| [`Tmc9660Handler`](api/class_tmc9660_handler.html) | The motor controller handler and its GPIO, ADC and temperature wrappers |
| [`Pcal95555Handler`](api/class_pcal95555_handler.html) | The I/O expander handler, the simplest complete example |
| [`Logger`](api/class_logger.html) | The logger singleton |
| [`hf::host` namespace](api/namespacehf_1_1host.html) | The host mocks: `MockI2c`, `MockSpi`, `MockGpio`, `MockAdc`, `MockCan`, `MockNvs`, `MockUart`, `MockTemperature`, `FakeClock` |
| [File list](api/files.html) | Every header under `handlers/` and `tests/host/mocks/` |

The `Base*` interfaces are documented in the
[interface wrap API reference](https://hardfoc.github.io/hf-internal-interface-wrap/latest/api/), and each driver
on its own site.
