# SfmHandler

HAL integration for Sensirion SF06-family gas mass flow meters (SFM4300-20/-50,
SFM3003, SFM3013, SFM3019, SFM3119) via `hf-sfm-flow-meter-driver`.

Enable with `-DHF_CORE_ENABLE_SFM=ON`.

## Usage

```cpp
SfmHandlerConfig cfg{};
cfg.default_gas = sfm::Gas::CO2;      // refused on SFM4300-50-x (no CO2 table)
cfg.averaging_window = 0;             // average-until-read
SfmHandler flow(sensor_i2c /* BaseI2c @ 0x2A */, cfg);

if (flow.EnsureInitialized()) {       // product identifier → Variant
    auto r = flow.Start();            // reads scale/offset, starts table
    if (!r.ok() && r.error == sfm::DriverError::UnsupportedGas) {
        r = flow.Start(sfm::Gas::Air);   // caller decides the fallback and flags it
    }
    auto m = flow.ReadMeasurement();  // NoData when no fresh sample yet
}
```

## Notes

- The `BaseI2c` device address must equal the sensor address (0x2A default);
  the adapter rejects mismatches.
- Reads NACK while no fresh sample exists (`sfm::DriverError::NoData`) — treat
  as "no update", not a fault. Counters (`GetCounters()`) expose frames /
  no-data / CRC / bus errors for diagnostics.
- `SoftReset()` needs an I2C general-call write; `BaseI2c` has none, so
  `soft_reset_on_init` is a no-op on this transport and `Stop()` is used to
  bring a part left measuring back to idle.

Driver repo: [`hf-sfm-flow-meter-driver`](https://github.com/N3b3x/hf-sfm-flow-meter-driver).
