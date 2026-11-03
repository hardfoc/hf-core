# Changelog

What changed in hf-core. No release has been tagged yet.

## [Unreleased]

### Fixed

- `HalPf1550Comm::DelayUs()`, the PF1550 driver's wait hook (now required by hf-pf1550-driver), cast the delay to a
  `uint16_t` of milliseconds, so a wait above 65.535 s wrapped (70 s became 4.464 s). It now uses
  `handler_utils::DelayUs()`, which rounds up and splits long waits.
- `handler_utils::DelayUs()` rounded up with `us + 999`, which wrapped for `us` close to `UINT32_MAX` and turned the
  longest waits into none.
