# Bmp581Handler

HAL integration for the Bosch BMP581 barometric pressure sensor via
`hf-bmp581-driver`.

Enable with `-DHF_CORE_ENABLE_BMP581=ON`.

## Usage

```cpp
Bmp581HandlerConfig cfg{};            // BarometricReferencePreset(): 100 Hz, OSR x8, IIR 3
Bmp581Handler baro(sensor_i2c /* BaseI2c @ 0x46 or 0x47 */, cfg);

if (baro.EnsureInitialized()) {       // chip id + NVM ready, reset, configure
    auto rdy = baro.DataReady();
    if (rdy.ok() && rdy.value) {
        auto s = baro.ReadSample();   // s.value.pressure_pa, s.value.temperature_c
    }
}
```

## Notes

- `EffectiveOsr().odr_valid` is false when the OSR pair cannot meet the ODR;
  the device lowers OSR silently, so check it after `Configure()`.
- IIR registers are standby-only; the driver performs the standby hop and
  restores the previous mode.
- Sub-millisecond driver delays round up to one RTOS tick.

Driver repo: [`hf-bmp581-driver`](https://github.com/N3b3x/hf-bmp581-driver).
