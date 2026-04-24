---
layout: default
title: SE050 Handler
parent: Handlers
nav_order: 15
---

# Se050Handler

HAL bridge from `BaseI2c` (and optional `BaseGpio` for `SE_RESET`) to the header-only
`se050::Device<>` stack in [`hf-se050-driver`](https://github.com/N3b3x/hf-se050-driver).
Low-level T=1, APDU shapes, and `se050::Device` methods are covered by **Doxygen** in that
repository; this page documents only the **hf-core** integration surface.

## When to enable

- **Board HAL** (e.g. `hf-hal-flux-v1/CMakeLists.txt`): set `HF_CORE_ENABLE_SE050 OFF` until
  the product PCB wires an SE050 on I²C (avoids pulling the driver and handler into firmware
  unnecessarily).
- **ESP32 hf-core examples** (`examples/esp32/components/hf_core/CMakeLists.txt`): set
  `HF_CORE_ENABLE_SE050 ON` so CI and local builds compile `Se050Handler` and
  `se050_handler_test` like other handler matrix entries.
- Other HALs that vendor `lib/core` as a submodule: after updating `lib/core` to a revision
  that includes SE050, mirror the same **OFF-by-default in HAL root, ON in example wrapper**
  pattern.

## Construction

```cpp
Se050Handler(BaseI2c& i2c,
              const Se050HandlerConfig& config = Se050HandlerConfig{},
              BaseGpio* reset_gpio = nullptr,
              RtosMutex* bus_mutex = nullptr);
```

`BaseI2c` must already target the SE050 7-bit address (typically `0x48`). Pass a
**shared** `bus_mutex` when multiple devices share the same logical bus.

## Initialization

`EnsureInitialized()` performs, under the handler mutex:

1. `BaseI2c::EnsureInitialized()`
2. Optional `SE_RESET` via `HardwareReset()` (`SetInactive()` assert, 2 ms, `SetActive()` release, 10 ms settle — see `Se050Handler.h`)
3. T=1 warm reset (`T1().ChipWarmReset`)
4. `SELECT` of the default IoT applet
5. `GetVersion()` probe

On success, `IsPresent()` becomes true.

## Using the driver API

```cpp
se050::cmd::VersionInfo v{};
se050::Error e = handler.GetDevice().GetVersion(&v, handler.Config().apdu_timeout_ms);
```

All methods on `se050::Device` match the **ESP32 reference examples** in
`hf-se050-driver/examples/esp32/main/`. The handler’s `GetDevice()` is the same object type
those examples use; only the **I²C transport** differs (`HalI2cSe050Comm` from `BaseI2c`
instead of the driver’s `HfSe050EspIdfI2c` helper).

### On-device example matrix

| Example | What it demonstrates |
|---------|----------------------|
| `se050_minimal_example.cpp` | Reset, T=1 warm reset, ATR, default applet `SELECT` |
| `se050_smoke_example.cpp` | `GetVersion`, `GetRandom`, `GetFreeMemory`, typed management APDUs |
| `se050_object_lifecycle_example.cpp` | Secure object create / read / delete |
| `se050_cloud_onboarding_example.cpp` | EC P-256 keygen + challenge sign / verify |
| `se050_cloud_registration_packet_example.cpp` | Idempotent enrollment + pubkey read + registration proof |
| `se050_secure_board_comms_example.cpp` | Payload hash + SE050 ECDSA between two boards |
| `se050_aws_iot_lifecycle/` | End-to-end: provision → network → TLS via SE050 → MQTT → OTA verify |

Matrix entries and CI flags live in
[`hf-se050-driver/examples/esp32/app_config.yml`](https://github.com/N3b3x/hf-se050-driver/blob/main/examples/esp32/app_config.yml).

### Host-side companion tooling (driver repo)

The **AWS IoT lifecycle** example ships Python utilities (factory provisioning, telemetry
verification, SBOM helpers, reprovision tokens, pytest) under
`examples/esp32/main/se050_aws_iot_lifecycle/tools/` in **hf-se050-driver**. Those scripts
complement firmware; they are not built into hf-core. Start from the driver’s
[`tools/README.md`](https://github.com/N3b3x/hf-se050-driver/blob/main/examples/esp32/main/se050_aws_iot_lifecycle/tools/README.md).

## Hardware and pins

- Default **7-bit I²C address** is `0x48` unless the board straps a different address; tests
  use `SE050_I2C_ADDR` in `examples/esp32/main/esp32_test_config.hpp` (override with `-D` at
  compile time if needed).
- **Reset**: optional `BaseGpio` in the constructor; when non-null, `EnsureInitialized()`
  calls `HardwareReset()` (assert with `SetInactive()`, release with `SetActive()`, with
  fixed delays) before T=1 warm reset; polarity must match `SE_RESET` wiring — see
  `Se050Handler.h`.
- **Bus sharing**: pass a shared `RtosMutex` when the same `BaseI2c` bus serves multiple devices.

## Test coverage

Build the core ESP32 test app from `examples/esp32/scripts`:

```bash
./scripts/build_app.sh se050_handler_test Debug
```

When `app_config.yml` lists multiple ESP-IDF versions, pass the third argument explicitly, for
example `release/v5.5`, so `config_loader.sh` can validate the matrix entry.

Source: `examples/esp32/main/handler_tests/se050_handler_comprehensive_test.cpp` (init,
`GetVersion`, `GetRandom`). For full applet and lifecycle flows, run the **driver**
examples above on the same hardware; they exercise identical `se050::Device` APIs.

## CMake

Set `HF_CORE_ENABLE_SE050 ON` **before** including `cmake/hf_core_build_settings.cmake` in the
build that should compile the handler (the `hf_core` ESP-IDF component does this for the
example project). The HAL product CMake should keep the flag **OFF** unless the board includes
an SE050.

## Driver API (Doxygen)

The **`hf-se050-driver`** headers (`se050::Device`, T=1, APDU, …) are documented with **Doxygen**
in that repo (`./scripts/build_doxygen.sh`, `_config/Doxyfile`). Published HTML follows the
same `_config/Doxyfile` as CI (`.github/workflows/ci-docs-publish.yml`).
