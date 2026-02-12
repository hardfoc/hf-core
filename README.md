# HardFOC Platform Core (hf-core)

Shared platform layer for all HardFOC boards: base interfaces, drivers, utilities, and handlers. No board-specific configuration—board HALs (e.g. hf-hal-vortex-v1) depend on this repo and add managers, API, and pin mapping.

## Contents

- **hf-core-drivers** (submodule): Base interfaces (BaseI2c, BaseSpi, BaseGpio, etc.), internal interface wrap, pincfg, and external device drivers (BNO08x, PCAL95555, TMC9660, AS5047U, etc.).
- **hf-core-utils** (submodule): General utilities, RTOS wrappers, CANopen helpers.
- **handlers**: HAL-level handlers that adapt external drivers to base classes (Bno08xHandler, Pcal95555Handler, Pca9685Handler, Tmc9660Handler, etc.).

## Clone with submodules

```bash
git clone --recurse-submodules https://github.com/hardfoc/hf-core.git
```

Or after a shallow clone:

```bash
git submodule update --init --recursive
```

## License

See LICENSE and per-submodule licenses.
