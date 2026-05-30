# Pf1550Handler

HAL integration for NXP PF1550 PMIC via `hf-pf1550-driver`.

Enable with `-DHF_CORE_ENABLE_PF1550=ON`.

## Usage

```cpp
Pf1550Handler pmic(pmic_i2c, &standby_gpio, &vbus_en_gpio, &otg_en_gpio);
pmic.SetPowerMode(pf1550::PowerMode::Run);
pmic.SetUsbRails(true, true);
pmic.ApplyPortentaH7Profile();
```

Driver repo: [`hf-pf1550-driver`](https://github.com/N3b3x/hf-pf1550-driver).
