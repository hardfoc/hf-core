---
name: add-handler
description: Add a new standard handler for a chip driver to hf-core with tools/new_handler.py. Use when a driver has no handler yet.
---

The result follows `standards/handler.md`; `docs/handler-model.md` explains the pieces.

1. Make sure the driver is a submodule under `hf-core-drivers/external/<repo>` at the commit you want (its own
   pin PR on hf-core-drivers, then the hf-core bump; see the `bump-driver` skill). Read its port base, its
   `Error` enum, its `Result<T>` and its start-up method.
2. Pick the family page under `docs/handlers/` (`tools/docs/generators/hfcore.py` FAMILIES lists them).
3. Generate and register:

   ```bash
   python3 tools/new_handler.py --chip <Chip> --bus <spi|i2c|uart|adc> --driver-repo <repo> \
       --driver-header <chip>.hpp --family <family>
   ```

   The defaults are the driver contract's names (`<chip>::<CHIP>`, `<chip>::<Bus>Interface`, `<chip>::Error`,
   `<chip>::Result`, `Initialize()`). For a driver that differs pass `--driver-class`, `--port-base`,
   `--error-enum`, `--result-type`, `--init-method`, `--driver-ns`. `--folder` and `--switch` override the
   folder and `HF_CORE_ENABLE_<SWITCH>`.
4. Review `handlers/<chip>/Hal<Bus><Chip>Comm.h`: the `ToStatus()` switch is a name-based first draft. The base
   set must match the table in `standards/errors.md`; decide each chip-specific value and note it on the
   handler's docs section.
5. Fill in the handler:
   - `<Chip>HandlerConfig`: bus address, the driver's `Config`; apply it in `initializeLocked()`.
   - The typed API: one method per operation a HAL needs, each
     `return CallDriver([&](Driver& d) { return ToResult(d.ReadX()); });`, named per
     `standards/naming-and-style.md` (`Read*` does I/O, units as suffixes).
   - Chip counters in `<Chip>Diagnostics` after the header, if any.
   - Control pins: chain `HalGpioCommBase` under the bus base in the adapter and add `GpioFor()`.
   - Wrappers (a `Base*` view of the chip) in a fixed array owned by the handler, never `shared_ptr`.
6. Extend `tests/host/test_<chip>.cpp` after the generated cases: script the mock like the chip, test every typed
   method, a bus failure on each one (`Status::Bus`), and the diagnostics counters.
7. `hardfoc-docs gen` fills the new docs section; add a sentence above it on what the chip is.
8. Run the `verify` skill. The PR lists anything that needs hardware as not verified on hardware.
