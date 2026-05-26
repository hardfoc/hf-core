# STM32H7 / CubeMX porting notes for hf-core

This document records local compatibility changes applied to the vendored
`hf-core` tree under `hal/pw-hal-flux-v1/lib/core/`. The upstream repository is
`https://github.com/hardfoc/hf-core.git` (see `hal/pw-hal-flux-v1/.gitmodules`).

## Submodule status

The product tree vendors `lib/core` without initialized submodule git metadata.
To re-bind to upstream:

```bash
cd hal/pw-hal-flux-v1
git submodule update --init --recursive lib/core
```

If submodule initialization is not possible in a given workspace, apply the
patches below manually before building `pw-controller-sw` for STM32H747.

## Patch inventory

| Area | File | Change | Rationale |
|---|---|---|---|
| HAL type guards | `hf-core-drivers/internal/hf-internal-interface-wrap/inc/mcu/stm32/StmTypes.h` | Per-module `#ifndef STM32H7xx_HAL_*_H` guards around HAL handle forward declarations | Prevents redefinition when CubeMX HAL headers are included alongside HardFOC wrappers |
| GPIO guard | `hf-core-drivers/internal/hf-internal-interface-wrap/inc/mcu/stm32/StmGpio.h` | `#ifndef GPIOA` around `struct GPIO_TypeDef` forward declaration | Avoids conflict with CMSIS/HAL GPIO typedefs |
| Logger enums | `hf-core-drivers/internal/hf-internal-interface-wrap/src/mcu/stm32/StmLogger.cpp` | Use `hf_log_level_t::LOG_LEVEL_*` and `statistics_.write_errors` | Aligns with hf-core enum/stat field names |
| Logger factory | `hf_core_handlers/StmLoggerFactory.cpp` | `StmLogger::Backend::SWO_ITM` | Matches StmLogger backend enum |
| RTOS bring-up | `hf-core-utils/CMakeLists.txt` | `HF_RTOS_NONE=1` for host/STM32 pre-FreeRTOS builds | Allows handler/util compilation before CubeMX FreeRTOS is linked |
| RTOS abstraction | `hf-core-utils/hf-utils-rtos-wrap/include/OsAbstraction.h` | `HF_RTOS_NONE` stub path alongside `HF_RTOS_FREERTOS` | Host and bare-metal builds without FreeRTOS headers |
| RTOS mutex | `hf-core-utils/hf-utils-rtos-wrap/include/RtosMutex.h` | Stub mutex types when `HF_RTOS_NONE` | Keeps headers parseable without FreeRTOS |
| Handler CMake | `hf_core_handlers/CMakeLists.txt` | STM32 interface subset + conditional FreeRTOS include paths | Smaller STM32 surface until CubeMX buses are wired |
| Core features | `hal/pw-hal-flux-v1/cmake/pw_hal_core_features.cmake` | `HF_CORE_RTOS=NONE`, conditional TMC9660 gate | Progressive CubeMX integration |

## Upstream pull-request checklist

When opening PRs against `hardfoc/hf-core` (and nested `hf-internal-interface-wrap`):

1. **StmTypes.h** — Guard each HAL forward declaration with the matching
   `STM32H7xx_HAL_<MODULE>_H` symbol instead of a blanket `STM32H7xx_HAL_H` guard.
2. **StmGpio.h** — Guard `GPIO_TypeDef` forward declaration with `#ifndef GPIOA`.
3. **OsAbstraction.h / RtosMutex.h** — Add a documented `HF_RTOS_NONE` profile for
   host unit tests and pre-RTOS firmware bring-up (no FreeRTOS headers required).
4. **StmLogger.cpp** — Verify enum member names against `hf_log_level_t` in the
   public API header; do not prefix with `HF_LOG_LEVEL_`.
5. **CMake** — Expose `HF_CORE_RTOS` cache variable (`NONE` | `FREERTOS`) and
   exclude `BaseThreadHostStub.cpp` from embedded targets.
6. **Tests** — Confirm host build compiles `hf-core-utils` and STM32 handler stubs
   with `HF_RTOS_NONE=1`.

## Verification in pw-controller-sw

```bash
cmake --preset host-debug && cmake --build build/host-debug && ctest --preset host-debug
cmake --preset stm32-debug && cmake --build build/stm32-debug
./scripts/lint_hal_boundary.sh
```

## Notes

- Do not copy EvKit/Vortex pin maps into product HAL; board bindings live in
  `pw_hal_comm_platform.cpp` and ADR-007 pin freeze.
- Full FreeRTOS mode activates automatically when CubeMX generates the kernel
  (`PW_ENABLE_FREERTOS=1` via `cmake/pw_cubemx_detect.cmake`).
