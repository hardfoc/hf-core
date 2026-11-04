# Changelog

What changed in hf-core. No release has been tagged yet.

## [Unreleased]

### Breaking

- Bus-error ports follow the drivers' new contracts: `HalUartAlicatBasis2Comm::write()` and `HalUartFdo2Comm::write()`
  return `bool` (`BaseUart::Write()` succeeded), `HalIsf15acp4SpiAdapter::GpioSet()` and
  `HalSpiMax22200Comm::GpioSet()` return `bool` (the `BaseGpio` call succeeded). `At25040Handler::ProbeResult` has a
  new `BusError` value.

### Fixed

- Alicat BASIS-2 and FDO2: a request the UART did not send now fails with `DriverError::SerialError` at once; it was
  waited for until the reply timed out and reported as `Timeout`.
- ISF15ACP4: a control pin that cannot be driven fails `Initialize()` with `Error::GpioControl` (and
  `Error::PowerSequence` for RES# and VCC enable). The adapter's `BeginCommand()` / `BeginData()` fail when D/C or
  SS cannot be driven, and `EnsureInitialized()` when an idle level cannot be set; all three ignored the GPIO result.
- MAX22200: a CMD or ENABLE pin that cannot be driven is reported by the driver (`INITIALIZATION_ERROR`,
  `GPIO_ERROR`) instead of the frame being sent with the pin in the wrong state.
- AT25040: `At25040Handler::Probe()` reports a failed SPI transfer as `ProbeResult::BusError`, not `Absent`, and the
  new `LastStatus()` gives the driver status of the last `Probe()`, `Read()` or `Write()` (`Status::BusError` is
  distinct from `Status::Timeout`).
- `HalPf1550Comm::DelayUs()`, the PF1550 driver's wait hook (now required by hf-pf1550-driver), cast the delay to a
  `uint16_t` of milliseconds, so a wait above 65.535 s wrapped (70 s became 4.464 s). It now uses
  `handler_utils::DelayUs()`, which rounds up and splits long waits.
- `handler_utils::DelayUs()` rounded up with `us + 999`, which wrapped for `us` close to `UINT32_MAX` and turned the
  longest waits into none.
- AS5047U: the driver's value getters return `as5047u::Result<T>` (hf-as5047u-driver). Code that calls them through
  `As5047uHandler::visitDriver()` or `GetSensor()` reads `.value` after checking `.ok()`. `visitDriver()` now
  returns a Result with `error = AS5047U_Error::BusError` when the sensor is unavailable, instead of a
  value-initialised Result that would read as a successful 0.
- `As5047uHandler::RefreshDiagnostics()` reported `communication_ok` on a dead bus: the failed ERRFL read came back
  as 0, which looked like "no error". It now sets `communication_ok = false`, counts a communication error and
  records the cause in `GetLastError()`. `Initialize()` also fails when its test read fails on its own, not only
  through the sticky flags.
