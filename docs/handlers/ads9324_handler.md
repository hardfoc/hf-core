---
layout: default
title: Ads9324Handler
parent: Handlers
nav_order: 21
---

# Ads9324Handler

Thread-safe `BaseAdc` facade for the TI ADS9324 (16-channel, 16-bit simultaneous
SAR with integrated PGA). Bridges `BaseSpi` plus CONVST GPIO (required) and
optional DRDY GPIO into `hf-ads9324-driver`.

## Feature flag

| Context | `HF_CORE_ENABLE_ADS9324` |
|:--------|:-------------------------|
| hf-core default | **OFF** |
| Product `pw_hal_core_features.cmake` | **OFF** (not in the insufflator image) |
| ESP32 examples (`examples/esp32/components/hf_core`) | **ON** so this handler and `ads9324_handler_test` compile |

Product `AdcManager` does **not** construct this handler. Live manifold AFE
remains ADS7952. When a pressure board carries ADS9324, bind SPI CS + CONVST +
DRDY in `bsp_bind/board` and attach from `AdcManager` behind a new probe path.

Compile the standalone driver (no hf-core, no product image):

```bash
cmake -S firmware/hal/pw-hal/lib/core/hf-core-drivers/external/hf-ads9324-driver \
      -B /tmp/hf-ads9324-build -DHF_ADS9324_BUILD_HOST_TESTS=ON
cmake --build /tmp/hf-ads9324-build
ctest --test-dir /tmp/hf-ads9324-build
```

ESP32-C6 driver examples live in the driver repo (`examples/esp32`, target
`esp32c6`). hf-core handler tests default to ESP32-S3.

## Construction

```cpp
explicit Ads9324Handler(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy = nullptr,
                        const Ads9324HandlerConfig& config = GetDefaultAds9324Config()) noexcept;
```

CONVST must be non-null and active-high (`SetActive` = CONVST high). DRDY may be
`nullptr` (timed wait after CONVST). SPI: mode 0, 24-bit config frames, 1-lane
16-bit conversion data on SDOUT (SBASB22 Table 7-18).

### Factory

```cpp
auto handler = CreateAds9324Handler(spi, &gpio_convst, &gpio_drdy);
handler->Initialize();
```

## Key Methods

### Lifecycle

| Method | Description |
|:-------|:------------|
| `Initialize()` | Create driver, 1-lane SDOUT bring-up, DEVICE_ID, default PGA. Returns `bool`. |
| `Deinitialize()` | Release driver. Returns `bool`. |
| `IsInitialized()` | Inherited from `BaseAdc` |

### BaseAdc

| Method | Description |
|:-------|:------------|
| `GetMaxChannels()` | 16 |
| `IsChannelAvailable(ch)` | `ch < 16` (API channel 0 = hardware AIN1) |
| `ReadChannel` / `ReadChannelV` / `ReadChannelCount` | CONVST snapshot then one channel |
| `ReadMultipleChannels` | One snapshot, then index requested channels |

### PGA and calibration

| Method | Description |
|:-------|:------------|
| `ConfigureChannel(ch, cfg)` | Input type, range (±2.5 … ±12.5 V), PGA bandwidth |
| `SetChannelOffset(ch, ofs)` | 10-bit two's-complement OFS_AINx |
| `SetChannelGain(ch, gan)` | 14-bit GAN_AINx |
| `ReadSnapshot(snap)` | All enabled channels after one CONVST |

### Driver Access

| Method | Description |
|:-------|:------------|
| `GetDriver()` | Pointer to `ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>*` |
| `visitDriver(fn)` | Callable under handler mutex |

```cpp
uint16_t id = handler.visitDriver([](auto& drv) { return drv.ReadDeviceId(); });
```

Expected DEVICE_ID after reset: `0x0004`.

### Utility

| Method | Description |
|:-------|:------------|
| `GetDescription()` | `"ADS9324_Handler_SPI_DevN"` |
| `GetHandlerDiagnostics(diag)` | Init state, DEVICE_ID, read/error counts |
| `DumpDiagnostics()` | Log the same fields via `Logger` |

## Thread Safety

All public methods are protected by a recursive `RtosMutex`. `visitDriver()`
holds the mutex for the duration of the callable.

## Test Coverage

See `examples/esp32/main/handler_tests/ads9324_handler_comprehensive_test.cpp`
(`APP_TYPE=ads9324_handler_test`). Construction and channel-map tests run
without silicon; live PGA/snapshot tests skip if `Initialize()` fails.

Driver-level ESP32-C6 apps: `driver_integration_test`, `basic_snapshot`,
`pga_offset` in `hf-ads9324-driver/examples/esp32`.
