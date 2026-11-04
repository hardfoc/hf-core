# Testing and CI

How hf-core is checked: host unit tests, compiled documentation, ESP32 builds, static analysis and the docs
site, on every pull request.

## Host unit tests

```bash
cmake --preset host-debug
cmake --build --preset host-debug
ctest --preset host-debug
```

| File | Covers |
| --- | --- |
| [`tests/host/test_mocks.cpp`](../tests/host/test_mocks.cpp) | The mocks themselves: the executable description of their behaviour |
| [`tests/host/test_rtos_host.cpp`](../tests/host/test_rtos_host.cpp) | The no-RTOS backend: fake clock, `BaseThread` without a scheduler, `RtosMutex` |
| [`tests/host/test_utils_general.cpp`](../tests/host/test_utils_general.cpp) | hf-utils-general |
| [`tests/host/test_pcal95555.cpp`](../tests/host/test_pcal95555.cpp) | `Pcal95555Handler` against a register model |
| [`tests/host/test_as5047u.cpp`](../tests/host/test_as5047u.cpp) | `As5047uHandler` against a pipelined SPI register map with CRC |
| [`tests/host/test_bno08x.cpp`](../tests/host/test_bno08x.cpp) | `Bno08xHandler` against an SHTP model |
| [`tests/host/test_tmc9660.cpp`](../tests/host/test_tmc9660.cpp) | `Tmc9660Handler` against a TMCL model |
| [`tests/host/test_ntc.cpp`](../tests/host/test_ntc.cpp) | `NtcTemperatureHandler` over `MockAdc` |
| [`tests/host/test_handler_foundation.cpp`](../tests/host/test_handler_foundation.cpp) | `hf::HandlerBase`, the `Hal{Bus}CommBase` adapter bases and `hf::DelayUs()` over the mocks |
| [`templates/handler/example/`](../templates/handler/example/CMakeLists.txt) | The reference handler (`test_example_bmp581`); `handler_template_fresh` checks it matches the template, `new_handler_tool` tests `tools/new_handler.py` |
| [`tests/docs/`](../tests/docs/CMakeLists.txt) | The code on this site, compiled with `-Werror` and run against the mocks (`test_doc_snippets`) |

[Host build and mocks](host-build.md) explains the build, the fake clock and the mocks.

## CI workflows

| Workflow | File | What it checks |
| --- | --- | --- |
| Host CI | [`host.yml`](../.github/workflows/host.yml) | Host build and ctest with GCC and Clang, clang-tidy 18 and cppcheck on hf-core's own sources, and a `CI gate` job |
| ESP32 examples build | [`esp32-examples-build-ci.yml`](../.github/workflows/esp32-examples-build-ci.yml) | Validates `app_config.yml`, builds every application with `ci_enabled` for each ESP-IDF version and build type, lints, and a `CI gate` job |
| C++ lint, C++ analysis | `ci-cpp-lint.yml`, `ci-cpp-analysis.yml` | clang-format and clang-tidy; cppcheck |
| Docs | [`ci-docs-publish.yml`](../.github/workflows/ci-docs-publish.yml) | Strict site build, figure and generated-table drift, Doxygen, offline link check, Lighthouse, screenshots; publishes on `main` and tags |
| Markdown lint, YAML lint, link check | `ci-markdown-lint.yml`, `ci-yaml-lint.yml`, `ci-docs-linkcheck.yml` | Repository text files |
| Release | `release.yml` | On a `v*` tag: a GitHub release, with example binaries attached when they were built |

The workflows call the shared, versioned workflows of
[hf-general-ci-tools](https://hardfoc.github.io/hf-general-ci-tools/) and
[hf-espidf-ci-tools](https://hardfoc.github.io/hf-espidf-ci-tools/) at `@v1`. `CI gate` is the check to require
on `main`: it passes only when every job it depends on passed or was skipped on purpose.

## ESP32 examples

The [examples](examples.md) are the hardware test suite. Two need a bench setup and are not built in CI:
`canopen_utils_test` (a CAN transceiver and a peer) and `full_integration_test` (every peripheral). To add one,
add its entry to `examples/esp32/app_config.yml`, its source under `examples/esp32/main/`, and its `APP_TYPE` to
`main/CMakeLists.txt`; CI picks it up from `app_config.yml`.

## Before a pull request

```bash
ctest --preset host-debug
bash tools/lint/run-clang-tidy.sh build/host-debug
bash tools/lint/run-cppcheck.sh build/host-debug
hardfoc-docs check && mkdocs build --strict      # when docs/, handlers/ or cmake/ changed
```

The docs check fails when a handler header or the build settings changed and the generated tables or the
figure were not refreshed: run `hardfoc-docs gen && hardfoc-docs figures`.
