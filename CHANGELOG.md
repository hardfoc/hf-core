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
