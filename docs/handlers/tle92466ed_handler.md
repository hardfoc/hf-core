---
layout: default
title: TLE92466ED Handler
parent: Handlers
nav_order: 8
---

# Tle92466edHandler

Six-channel solenoid gate-driver handler for the **Infineon TLE92466ED**, bridging **`BaseSpi`**
and **`BaseGpio`** to the header-only **`tle92466ed::Driver<>`** stack in
[`hf-tle92466ed-driver`](https://github.com/N3b3x/hf-tle92466ed-driver).

Low-level SPI framing, register maps, and Doxygen for `tle92466ed::Driver` live in that
repository (including **wiring / SPI frame SVGs** and the **`lm_pro_cycle_test`** hardware
exercise). This page is the **hf-core integration** contract: construction, threading, which
driver entry points are wrapped here, and which ones you reach through **`GetDriver()`**.

---

## Construction

```cpp
Tle92466edHandler(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                  BaseGpio* faultn = nullptr);
```

| Pin | Role |
|-----|------|
| **SPI** | Pre-created `BaseSpi` device (mode **1**, ≤ **10 MHz** per datasheet). |
| **RESN** | Reset, **active-low** at the silicon; logical driver state uses `GpioSignal` (see below). |
| **EN** | Output-stage enable; **active-high** typical. |
| **FAULTN** | Optional fault input, **active-low**; may be `nullptr`. |

### RESN / EN and `BaseGpio` polarity

The driver’s `SpiInterface` documents logical **`GpioSignal::ACTIVE`** for **RESN** as
*not in reset* (device operational). `HalSpiTle92466edComm::GpioSet` maps that to
`BaseGpio::SetActive()` / `SetInactive()`.

Configure each `BaseGpio` with the **`hf_gpio_active_state_t`** that matches your PCB so that:

- Released reset (part running) ⇒ driver drives **RESN logical ACTIVE** ⇒ the correct **physical** level on the pin.
- Asserted reset ⇒ **RESN logical INACTIVE**.

The standalone ESP32 examples in the driver repo use `EspGpio` with explicit active states;
mirror that pattern when wiring the handler on a new board.

---

## Thread safety

All public handler methods run under an internal **`RtosMutex`**. **`GetDriver()`** also locks
the mutex and **requires** the device to already be initialized (it returns `nullptr`
otherwise). For production code, prefer **`EnsureInitialized()`** / **`Initialize()`** before
calling **`GetDriver()`**, and do **not** hold other locks that the driver callbacks might
invert.

---

## Handler API vs `tle92466ed::Driver`

| Area | On the handler | On `GetDriver()` (advanced) |
|------|----------------|-----------------------------|
| Init / deinit | `Initialize()`, `Initialize(GlobalConfig)`, `EnsureInitialized()`, `Deinitialize()` | — |
| Channels | `ConfigureChannel`, `EnableChannel`, `DisableChannel`, `EnableAllChannels` / `DisableAllChannels`, `SetChannelCurrent` | `SetCurrentSetpoint`, `GetCurrentSetpoint`, … |
| PWM | **`ConfigurePwmRaw`** → driver `ConfigurePwmPeriodRaw` | **`ConfigurePwmPeriod(period_us)`** (datasheet clamp + range selection) |
| Dither | — | **`ConfigureDither`**, **`ConfigureDitherRaw`**, **`ConfigureDitherClock`** (programs `DITHER_CLK_DIV`; required for FB updates) |
| Output stage | **`EnableOutputStage`** / **`DisableOutputStage`** → `Enable()` / `Disable()` | Same |
| Feedback | **`EnableFeedbackUpdates()`** (clears `FB_FRZ` so FB registers run) | **`GetAverageCurrent`**, **`GetDutyCycle`**, rail helpers (`GetVbatVoltage`, …) |
| Mode / faults | `EnterMissionMode`, `EnterConfigMode`, `IsMissionMode`, `GetStatus`, `GetChannelDiagnostics`, `GetFaultReport`, `ClearFaults`, `HasFault` | Richer variants returning `DriverResult` (`GetDeviceStatus`, `GetAllFaults`, `HasAnyFault`, …) |
| Watchdog / ID | `KickWatchdog`, `GetChipId`, `GetIcVersion`, `DumpDiagnostics` | Underlying equivalents |

**Product pattern (Flux):** `ValveManager` calls **`EnableOutputStage()`**, **`EnableFeedbackUpdates()`**, then uses **`GetDriver()->ConfigurePwmPeriod`** and **`GetDriver()->ConfigureDither`** for proportional valves — the same ordering you need whenever FB readback or dither must be live.

---

## PWM and dither (recent driver behaviour)

- **`ConfigurePwmPeriod(float period_us)`** enforces the datasheet **110 Hz–4 kHz** usable band
  (with optional low-frequency range). Values outside the combined spec window return
  **`InvalidParameter`**; near-gap frequencies may log a warning — see driver Doxygen.
- **`ConfigureDither`** computes step / flat parameters and programs **`DITHER_CLK_DIV`** so
  the averager and dither timers run (**POR `0x0000` freezes FB** until configured).
- Raw APIs **`ConfigurePwmPeriodRaw`** / **`ConfigureDitherRaw`** remain available for
  bring-up; the handler exposes **`ConfigurePwmRaw`** as a thin wrapper around
  **`ConfigurePwmPeriodRaw`**.

If **`GetAverageCurrent`** or **`GetDutyCycle`** read **zero** while the coil is active, verify
you called **`EnableFeedbackUpdates()`** and allowed at least one measurement interval after
configuring dither / PWM.

---

## Reference material in hf-tle92466ed-driver

| Asset | Purpose |
|-------|---------|
| `examples/esp32/app_config.yml` | CI matrix for minimal/smoke/LM-Pro and lifecycle-style demos |
| `examples/esp32/main/lm_pro_cycle_test.cpp` | CH5 tracking / characterization on real hardware |
| `_config/` + `docs/` + `*.svg` | Topology, frame format, bring-up |

---

## Test coverage (hf-core)

Handler integration test:

```bash
./scripts/build_app.sh tle92466ed_handler_test Debug
```

Source: `examples/esp32/main/handler_tests/tle92466ed_handler_comprehensive_test.cpp` — includes
optional **feedback + dither** checks when hardware is present (mission mode, output stage,
`EnableFeedbackUpdates`, `ConfigureDither`, `GetAverageCurrent`).

---

## CMake

Set **`HF_CORE_ENABLE_TLE92466ED ON`** before including **`cmake/hf_core_build_settings.cmake`**
in builds that should compile the handler. Board HALs should keep it **OFF** unless the BOM
includes a TLE92466ED.
