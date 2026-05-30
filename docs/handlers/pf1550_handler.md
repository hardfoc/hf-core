# Pf1550Handler

HAL integration for NXP PF1550 PMIC via `hf-pf1550-driver`.

Enable with `-DHF_CORE_ENABLE_PF1550=ON`.

## Usage

```cpp
Pf1550Handler pmic(pmic_i2c, &standby_gpio, &vbus_en_gpio, &otg_en_gpio);
pmic.SetPowerMode(pf1550::PowerMode::Run);
pmic.SetUsbRails(true, true);
// Carrier / Portenta module with LDO inputs on +3V1SW (SW1):
pmic.ApplyPortentaH7CarrierProfile();
// Legacy VFR order:
// pmic.ApplyPortentaH7Profile();
```

## Profiles

| Method | Profile | Notes |
|--------|---------|-------|
| `ApplyPortentaH7Profile()` | `portenta_h7_default` | VFR heritage |
| `ApplyPortentaH7CarrierProfile()` | `portenta_h7_carrier` | SW1-first, SW2_CTRL=0x0F |

See [PMIC bring-up (Portenta H7)](../../../../../../docs/hardware/pmic-bringup-portenta-h7.md)
in pw-controller-sw for power tree and bench checks.

Driver repo: [`hf-pf1550-driver`](https://github.com/N3b3x/hf-pf1550-driver).
