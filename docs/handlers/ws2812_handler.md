---
layout: default
title: WS2812 Handler
parent: Handlers
nav_order: 10
---

# Ws2812Handler

Addressable LED strip handler for WS2812/NeoPixel LEDs using the ESP32 RMT peripheral.

## Construction

```cpp
Ws2812Handler(const Config& config);
```

### Config Structure

```cpp
struct Config {
    gpio_num_t gpio_pin{};       // GPIO pin connected to LED data line
    uint32_t num_leds{1};        // Number of LEDs in the strip
    LedType led_type{LedType::RGB};
    uint8_t brightness{255};     // Global brightness (0–255)
    int rmt_channel{0};          // RMT channel number
    uint16_t t0h{400};           // T0H timing (ns)
    uint16_t t1h{800};           // T1H timing (ns)
    uint16_t t0l{850};           // T0L timing (ns)
    uint16_t t1l{450};           // T1L timing (ns)
};
```

## Key Methods

| Method | Description |
|:-------|:------------|
| `Initialize()` | Initialize RMT channel and LED strip |
| `EnsureInitialized()` | Lazy init entrypoint |
| `Deinitialize()` | Release RMT resources |
| `SetPixel(index, r, g, b)` | Set a single pixel color |
| `SetAllPixels(r, g, b)` | Set all pixels to the same color |
| `Clear()` | Turn off all pixels |
| `Show()` | Transmit pixel data to the strip |
| `SetBrightness(val)` | Set global brightness (0–255) |
| `GetNumLeds()` | Get the number of LEDs |
| `SetEffect(effect, color)` | Set animation effect |
| `Tick()` | Advance animation by time delta |
| `Step()` | Advance animation by one step |
| `GetStrip()` | Get underlying `WS2812Strip*` |
| `GetAnimator()` | Get underlying `WS2812Animator*` |

## Thread Safety

All methods are protected by an internal `RtosMutex`.

## Test Coverage

See `examples/esp32/main/handler_tests/ws2812_handler_comprehensive_test.cpp`.
