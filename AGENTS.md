# AGENTS.md

hf-core is the HardFOC core platform: thread-safe handlers over the HardFOC chip drivers, with the MCU
interfaces, RTOS wrappers and utilities they need, buildable on the host with mocks. It follows the HardFOC
engineering standard: <https://github.com/hardfoc/.github/blob/main/standards/README.md> (agents:
`standards/agents.md`; this repository's layer: `standards/handler.md`).

## Layout

- `handlers/<chip>/`: one handler per chip. `handlers/common/`: the handler foundation (`HandlerBase.h`, the
  `Hal{Bus}CommBase.h` adapter bases, `Delay.h`) and older shared code. `handlers/logger/`: the logger.
- `hf-core-drivers/` (submodule): the chip drivers under `external/`, interface-wrap and pincfg under
  `internal/`. `hf::Status` and `hf::Result` come from interface-wrap (`hf/iiw/Status.h`).
- `hf-core-utils/` (submodule): rtos-wrap, utils-general, utils-canopen.
- `cmake/hf_core_build_settings.cmake`: the `HF_CORE_ENABLE_*` switches, sources and include directories every
  consumer uses. `CMakeLists.txt` is only the host build.
- `templates/handler/`: the standard handler template; `templates/handler/example/` is its reference output.
- `tests/host/`: host tests and mocks. `tests/docs/`: the code shown on the site. `docs/`: the site.
- `tools/new_handler.py`: scaffolds a handler. `tools/lint/`: the clang-tidy and cppcheck gates.

## Build and check (run all of these before pushing)

```bash
git submodule update --init --recursive
cmake --preset host-debug && cmake --build --preset host-debug && ctest --preset host-debug
bash tools/lint/run-clang-tidy.sh build/host-debug
bash tools/lint/run-cppcheck.sh build/host-debug
hardfoc-docs check && mkdocs build --strict -d /tmp/site    # when docs/, handlers/ or cmake/ changed
```

Build with GCC and with Clang when you can (`CC=gcc CXX=g++ cmake -S . -B build/gcc -G Ninja`); CI runs both. The
`verify` skill runs the whole list. The ESP32 examples (`examples/esp32/`) build in CI; do not run ESP-IDF or
Docker builds locally unless the task asks for it. Delete build directories when you are done.

## Layer rules

- A handler includes only `Base*` headers, its driver, rtos-wrap, `hf_time` and utils. No `esp_*`, `gpio_num_t`,
  `esp_err_t`, FreeRTOS calls or MCU `#ifdef` in `handlers/`; those belong in interface-wrap's `mcu/` or a HAL's
  bind.
- New and migrated handlers follow `standards/handler.md` through `hf::HandlerBase`: `namespace hf`, driver in
  `std::optional`, no heap, `hf::Result` from typed calls, `VisitDriver()` returning `hf::Status`, no public
  `GetDriver()`, `GetDiagnostics()` with `hf::DiagnosticsHeader` first. See `docs/handler-model.md`.
- Driver errors are mapped once, by `ToStatus(<chip>::Error)` in the adapter header, as a `switch` over every
  enumerator with no `default`.
- Locks: the handler's `hf::HandlerMutex`, then the bus mutex. One clock (`hf_time::MonotonicUs()`), one delay
  (`hf::DelayUs()` / `hf::DelayMs()`).
- Never edit code inside `hf-core-drivers/` or `hf-core-utils/`: change the submodule's own repository and bump
  the pin here in its own PR.
- New C++ files use the org `.clang-format` and `.clang-tidy` (100 columns) from
  `hardfoc/.github/standards/config/`. Do not reformat existing files in a feature PR.

## Definition of done

The checklist in `standards/agents.md#definition-of-done`, plus for this repository:

- `ctest --preset host-debug` passes, including `handler_template_fresh` and `new_handler_tool`.
- A new or changed handler has a host test over the mocks covering construction without I/O, retry after a
  failed `EnsureInitialized()`, a bus failure mapped to `Status::Bus`, `VisitDriver()` before init, and the
  diagnostics header.
- `docs/handler-model.md`, the handler's family page under `docs/handlers/` and `hardfoc-docs gen` output are
  current; the migration table says which handlers are on the standard model.
- No attribution lines anywhere. Hardware behaviour is listed as not verified on hardware, with bench steps.
