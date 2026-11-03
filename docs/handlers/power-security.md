# Power, memory and security

Handlers for the PF1550 PMIC, the AT25040B SPI EEPROM and the SE050 secure element.

## Pf1550Handler

<!-- generated: handler-pf1550 -->
HAL handler for NXP PF1550 PMIC (I2C + optional strap GPIOs).

- Header: [`handlers/pf1550/Pf1550Handler.h`](../../handlers/pf1550/Pf1550Handler.h)
- Switch: `HF_CORE_ENABLE_PF1550` (default OFF)
- Driver: [hf-pf1550-driver](https://github.com/hardfoc/hf-pf1550-driver)
- Host test: [`tests/host/test_pf1550.cpp`](../../tests/host/test_pf1550.cpp)

```cpp
Pf1550Handler(
    BaseI2c& i2c,
    BaseGpio* standby_gpio = nullptr,
    BaseGpio* usb_vbus_en_gpio = nullptr,
    BaseGpio* usb_otg_en_gpio = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `EnsureInitialized()`, `IsInitialized()`, `ApplyProfile()`, `ApplyPortentaH7Profile()`, `ApplyPortentaH7CarrierProfile()`, `SetPowerMode()`, `SetUsbRails()`, `ReadPmicStatus()`, `RefreshDiagnosticSnapshot()`, `RefreshStatusSnapshot()`, `ReadDiagnosticSnapshot()`, `RunPowerSelfTest()`, `ClearLatchedFaults()`, `HasMcuAffectingFault()`, `GetDriver()` |
<!-- /generated -->

## At25040Handler

<!-- generated: handler-at25040 -->
SPI AT25040B handler — BaseSpi + optional /WP, lazy Probe.

- Header: [`handlers/at25040/At25040Handler.h`](../../handlers/at25040/At25040Handler.h)
- Switch: `HF_CORE_ENABLE_AT25040` (default OFF)
- Driver: [hf-at25040-driver](https://github.com/hardfoc/hf-at25040-driver)

```cpp
At25040Handler(BaseSpi& spi, BaseGpio* wp = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `Probe()`, `LastRdsr()`, `Read()`, `Write()`, `ReleaseWriteProtect()`, `ParkWriteProtect()` |
<!-- /generated -->

## Se050Handler

<!-- generated: handler-se050 -->
HAL-level handler for the NXP SE050 / SE050A secure element (I²C, T=1oI2C).

- Header: [`handlers/se050/Se050Handler.h`](../../handlers/se050/Se050Handler.h)
- Switch: `HF_CORE_ENABLE_SE050` (default OFF)
- Driver: [hf-se050-driver](https://github.com/hardfoc/hf-se050-driver)

```cpp
Se050Handler(
    BaseI2c& i2c,
    const Se050HandlerConfig& config = Se050HandlerConfig{},
    BaseGpio* reset_gpio = nullptr,
    RtosMutex* bus_mutex = nullptr);
```

| Group | Public methods |
| --- | --- |
| All | `Config()`, `EnsureInitialized()`, `IsPresent()`, `GetDevice()` |
<!-- /generated -->

