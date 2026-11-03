# Changelog

What changed in hf-core. No release has been tagged yet.

## [Unreleased]

### Breaking

- AS5047U: the driver's value getters return `as5047u::Result<T>` (hf-as5047u-driver). Code that calls them through
  `As5047uHandler::visitDriver()` or `GetSensor()` reads `.value` after checking `.ok()`. `visitDriver()` now
  returns a Result with `error = AS5047U_Error::BusError` when the sensor is unavailable, instead of a
  value-initialised Result that would read as a successful 0.

### Fixed

- `As5047uHandler::RefreshDiagnostics()` reported `communication_ok` on a dead bus: the failed ERRFL read came back
  as 0, which looked like "no error". It now sets `communication_ok = false`, counts a communication error and
  records the cause in `GetLastError()`. `Initialize()` also fails when its test read fails on its own, not only
  through the sticky flags.
