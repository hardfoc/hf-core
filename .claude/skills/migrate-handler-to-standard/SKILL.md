---
name: migrate-handler-to-standard
description: Move one existing hf-core handler (rich or bus family) onto hf::HandlerBase and the handler standard. Use for the per-handler migration PRs.
---

One handler per PR. Its driver should already follow the driver contract, or land with it in the same series
(driver PR, hf-core-drivers pin PR, then this one). Read `standards/handler.md`, `docs/handler-model.md` and the
reference handler in `templates/handler/example/bmp581/` first.

1. **Inventory.** List the handler's public methods and every caller in hf-core (`tests/`, `examples/esp32/`,
   other handlers) and in the HALs and products (search hardfoc for the class name). The PR lists them.
2. **Generate the target shape** into a scratch folder to copy from:
   `python3 tools/new_handler.py ... --example-dir /tmp/<chip>-target` with the driver's real names.
3. **Adapter.** Replace the hand-written adapter with `Hal<Bus><Chip>Comm` on `hf::Hal{Bus}CommBase`; keep only
   hooks the base lacks. Every hook returns `bool` (no `void` writes or pin sets). Move the driver-error mapping
   into one `ToStatus(<chip>::Error)` switch in the adapter header.
4. **Handler.**
   - Put the class in `namespace hf`. Derive from `hf::HandlerBase<Self, Driver>`; delete the own mutex, the
     `initialized_` flag and the driver pointer the base now holds.
   - `std::make_unique` / `new` / `nothrow new` of the driver becomes `EmplaceDriver(comm_, ...)` inside
     `initializeLocked()`; add `~Handler() { DestroyDriver(); }`.
   - Rich family: `Initialize()` becomes `EnsureInitialized()` (from the base); `visitDriver()` / `withDriver()`
     become `VisitDriver()` returning `hf::Status` (values by capture); `DumpDiagnostics()` becomes
     `GetDiagnostics()` returning `<Chip>Diagnostics` with `hf::DiagnosticsHeader` first (logging moves to the
     caller).
   - Bus family: keep the `bus_mutex` parameter last, typed `hf::HandlerMutex*`; driver result types
     (`sfm::DriverResult<T>`, `se050` codes) become `hf::Result<T>` via `ToResult()`; `GetCounters()` folds into
     `GetDiagnostics()`.
   - Every typed method becomes a `CallDriver()` body returning `hf::Result<T>`; no `bool` from a call that can
     fail. Remove public `GetDriver()` / `GetSensor()` / `GetDevice()`.
   - Wrappers (`Tmc9660AdcWrapper`, PCA9685/PCAL95555 pins, ...) move from `shared_ptr` to a fixed array built in
     `initializeLocked()`.
   - Outliers: `Ws2812Handler` needs a `Base*` interface for the RMT in interface-wrap first (no `gpio_num_t`,
     no `esp_err_t`); drop `PW_HAL_STM32H7` blocks; `Mcp9700TemperatureHandler`'s `nothrow new` goes.
5. **Compatibility.** HALs name the old global class. Either update them in the same series (one PR per HAL,
   after this one), or add `using <Chip>Handler = hf::<Chip>Handler;` at global scope for one release with a
   comment linking the HAL issue.
6. **Tests.** Rewrite `tests/host/test_<chip>.cpp` to the cases of `standards/handler.md` "Tests" plus every
   typed method; keep the existing behavioural cases.
7. **Docs.** Run `hardfoc-docs gen`; update the handler's family page; move the handler to "On the standard
   model" in the migration table of `docs/handler-model.md`.
8. **ESP32 examples.** Update `examples/esp32/main/handler_tests/<chip>_*` to the new API; CI builds them.
9. Run the `verify` skill. The PR says which behaviour changed for callers and what was not verified on
   hardware.
