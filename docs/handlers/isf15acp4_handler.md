---
layout: default
title: ISF15ACP4 Handler
parent: Handlers
nav_order: 11
---

# Isf15acp4Handler

NKK **ISF15ACP4** SmartDisplay handler — 96×64 RGB OLED (SSD1331) with integrated pushbutton.

> **Platform default:** `HF_CORE_ENABLE_ISF15ACP4` is **OFF**. Uncomment the opt-in line in `examples/esp32/components/hf_core/CMakeLists.txt` before building handler tests.

## Construction

```cpp
Isf15acp4Handler(BaseSpi& spi, BaseGpio& cs, BaseGpio& dc, BaseGpio& reset,
                 const Isf15acp4HandlerConfig& config = {},
                 BaseGpio* vcc_enable = nullptr, BaseGpio* switch_in = nullptr,
                 bool switch_active_low = true);
```

### Isf15acp4HandlerConfig

```cpp
struct Isf15acp4HandlerConfig {
    isf15acp4::ProductVariant variant{isf15acp4::ProductVariant::Isf15acp4};
    isf15acp4::GraphicsBackend graphics{isf15acp4::GraphicsBackend::BuiltinCanvas};
    bool enable_vcc_on_init{true};
};
```

## SPI / GPIO notes

- **Software chip select:** CS is driven via `BaseGpio`, not the SPI peripheral CS pin.
- Create the SPI device with a **dummy CS** GPIO for bus registration; control the real CS through the handler adapter.
- **D/C**, **RES**, optional **VCC_EN**, and **SWITCH** are separate GPIOs.

### Lifecycle

| Method | Description |
|:-------|:------------|
| `EnsureInitialized()` | Lazy init: adapter + `SmartDisplay::Initialize` |
| `Deinitialize()` / `EnsureDeinitialized()` | Release driver instance, display off |
| `IsInitialized()` | Initialization state |

### Display & graphics

| Method | Description |
|:-------|:------------|
| `DisplayOn()` / `DisplayOff()` | Panel power |
| `FillScreen(color)` | Solid fill via driver |
| `CreateGraphicsContext()` | Canvas/direct/animator per config |
| `Present(gfx)` | Upload framebuffer or present animation frame |
| `IsPressed()` | Read momentary switch |

### Driver access

| Method | Description |
|:-------|:------------|
| `GetDriver()` / `GetDisplay()` | Raw `SmartDisplay*` (same pointer) |
| `visitDriver(fn)` | Mutex-protected direct `SmartDisplay` access |
| `GetErrorFlags()` / `ClearErrorFlags(mask)` | Error status from underlying driver |
| `DumpDiagnostics()` | Log init state and error flags |

## Thread Safety

All public methods use an internal `RtosMutex`. `visitDriver()` acquires the lock before invoking the callable.

## CMake enable

```cmake
set(HF_CORE_ENABLE_ISF15ACP4 ON)
```

Defines `HARDFOC_ISF15ACP4_SUPPORT=1` and links `hf-isf15acp4-driver` sources plus `Isf15acp4Handler.cpp`.

## Test coverage

See `examples/esp32/main/handler_tests/isf15acp4_handler_comprehensive_test.cpp`.

Default test pins (`esp32_test_config.hpp`):

| Signal | GPIO |
|:-------|:-----|
| SPI dummy CS | 46 |
| CS | 14 |
| D/C | 15 |
| RES | 16 |
| VCC_EN | 17 |
| SWITCH | 18 |

Driver repo examples and docs: [hf-isf15acp4-driver](https://github.com/N3b3x/hf-isf15acp4-driver).
