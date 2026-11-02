---
layout: default
title: Host Build and Tests
parent: Testing
nav_order: 3
---

# Host Build and Tests

hf-core builds and runs its unit tests on a Linux or macOS machine, with no
ESP32 or STM32 toolchain. The host build compiles everything above the MCU
layer — handlers, external chip drivers, hf-core-utils and the `Base*`
interfaces — with `HF_CORE_MCU=NONE` and `HF_CORE_RTOS=NONE`. Tests drive the
handlers through scriptable fakes of `BaseI2c`, `BaseSpi`, `BaseGpio`,
`BaseAdc`, `BaseCan`, `BaseNvs`, `BaseUart` and `BaseTemperature`.

On-target builds do not use this path. ESP-IDF projects keep including
`cmake/hf_core_build_settings.cmake` from their component
(`examples/esp32/components/hf_core`), and HAL repositories do the same.

## Build and run

Requirements: CMake 3.22 or newer, Ninja, and a C++20 compiler (GCC 13 and
Clang 17 or newer are tested). Clone with submodules:

```bash
git clone --recurse-submodules https://github.com/hardfoc/hf-core.git
cd hf-core
cmake --preset host-debug
cmake --build --preset host-debug
ctest --preset host-debug
```

Each file under `tests/host/` is one executable and one ctest entry. Run a
single executable to see each case, or pass part of a case name to run only
matching cases:

```bash
./build/host-debug/tests/host/test_pcal95555
./build/host-debug/tests/host/test_pcal95555 CreateGpioPin
```

To build with another compiler, set it at configure time, for example
`CC=clang CXX=clang++ cmake --preset host-debug`.

### Static analysis

Both tools read `build/host-debug/compile_commands.json`, so configure first.
They analyse hf-core's own sources only (`handlers/`, `tests/host/`; see
`tools/lint/own_sources.py`) and fail on any finding.

```bash
bash tools/lint/run-clang-tidy.sh build/host-debug   # checks: tools/lint/host.clang-tidy
bash tools/lint/run-cppcheck.sh build/host-debug     # policy: tools/lint/cppcheck-suppressions.txt
```

CI pins clang-tidy 18 and cppcheck 2.13 (Ubuntu 24.04). With a Homebrew
clang-tidy on macOS, point it at the SDK:
`CLANG_TIDY=$(brew --prefix llvm)/bin/clang-tidy CLANG_TIDY_EXTRA_ARG="-isysroot$(xcrun --show-sdk-path)"`.

`.github/workflows/host.yml` runs the build and tests with GCC and Clang, then
clang-tidy and cppcheck. Its `CI gate` job passes only when every other job
passed or was skipped.

## What gets built

| Target | Contents | Warnings |
|:-------|:---------|:---------|
| `hf_core_deps` | External driver sources, hf-utils-general, hf-utils-canopen, `RtosMutex`, `BaseThread` with its host stub, `SignalSemaphore` | Submodule code: not enforced |
| `hf_core` | Every handler in `handlers/` that does not need a specific MCU | `-Wall -Wextra -Werror` |
| `hf_core_host_clock` | `FakeClock`, the time source for the no-RTOS backend | `-Wall -Wextra -Werror` |
| `hf_core_host_mocks` | `Mock*` fakes in `tests/host/mocks/` | `-Wall -Wextra -Werror` |
| `hf_core_host_test_main` | The `HF_TEST` / `CHECK` harness and `main()` | `-Wall -Wextra -Werror` |

Handlers left out: WS2812 (needs the ESP32 RMT peripheral), Dynamixel and the
CANopenNode slave (both need third-party checkouts that live outside hf-core).
Set `HF_CORE_ENABLE_<CHIP>` before configuring to change the selection.

Submodule headers are included as system headers, so warnings inside a driver
do not fail hf-core's build. That is deliberate: each submodule is its own
repository with its own CI. hf-core's own files are held to the strict flags.

## Time and threads on the host

With no RTOS there is no tick. By default the no-RTOS backend returns 0 from
every clock and returns from every delay at once, so a driver that polls the
clock waiting for a timeout would spin forever. The host build defines
`HF_RTOS_NONE_EXTERNAL_CLOCK`, which routes `os_time_get()`,
`os_delay_msec()`, `os_get_elapsed_time_msec()` and
`RtosTime::GetCurrentTimeUs()` to `hf::host::FakeClock`
(`tests/host/mocks/FakeClock.h`):

- a delay advances the clock by the requested time and returns at once;
- every clock read advances the clock by 10 µs, so polling loops reach
  their timeout after a bounded number of reads;
- `FakeClock::AdvanceMs()`, `SetAutoAdvanceUs(0)`, `TotalDelayUs()` and
  `DelayCount()` let a test control and check time;
- the test runner calls `FakeClock::Reset()` before each case.

A 200 ms SH-2 timeout in the BNO08x driver therefore costs microseconds of
real time.

`BaseThread` works without a scheduler: `StartThreadAndWaitToVerify()` runs
`Setup()` inline and marks the thread running, and the test calls `Step()`
itself (applying the returned delay with `os_delay_msec()` if it matters).
`RtosMutex` and `MutexLockGuard` lock and unlock but never block.

## The mocks

Include `HfCoreMocks.h` and link `hf_core_host_mocks`. All fakes live in
namespace `hf::host`, record every call, and inject failures on demand.

| Fake | Answers with | Inspect / inject |
|:-----|:-------------|:-----------------|
| `MockI2c` | Queued reads (`QueueRead`), then a responder callback, then an optional register model (`EnableRegisterModel`, `SetRegister`, `SetReadOnly`), then zeros | `Transactions()`, `WritesTo(reg)`, `CountOf(kind)`, `FailNext(err, n)` |
| `MockSpi` | Queued MISO frames, then a full-duplex responder, then a fill byte | `Transactions()` (one entry per chip-select frame), `FailNext` |
| `MockGpio` | Inputs read `SetInputLevel()`; outputs read back what was driven | `LevelHistory()`, `DrivenLevel()`, `FireInterrupt()`, `FailReads/FailWrites` |
| `MockAdc` | Per-channel voltage or count, optional queued sequence | `ReadCount(ch)`, `FailChannel(ch)` |
| `MockCan` | `InjectRx()` frames go to the receive callback or the RX queue, through the acceptance filter | `SentFrames()`, `FailNextSend`, `SetBusOff` |
| `MockNvs` | Typed in-memory store (u32, string, blob) with ESP-IDF key limits | `CommitCalls()`, `FailNext`, `SetReadOnly` |
| `MockUart` | `InjectRx()` bytes; `SetOnWrite()` answers a request when it is sent | `TxBytes()`, `TxWrites()`, `FailNextWrite` |
| `MockTemperature` | `SetCelsius()` | `ReadCalls()`, `Fail()` |

Like real peripherals, the fakes follow `Base*` lazy initialization: call
`EnsureInitialized()` (handlers do this themselves) before using a fake
directly. `tests/host/test_mocks.cpp` is the executable description of their
behaviour.

For devices with a protocol, write a small model on top of a responder. The
handler tests show three patterns: a register file (`test_pcal95555.cpp`), a
pipelined SPI register map with CRC (`test_as5047u.cpp`), and a framed
request/response protocol (SHTP in `test_bno08x.cpp`, TMCL in
`test_tmc9660.cpp`).

## Using hf-core and the mocks from a board HAL

A HAL's host test build adds hf-core as a subdirectory. Its unit tests stay
off; the libraries, mocks and harness are built.

```cmake
# HAL host CMakeLists.txt (host builds only)
set(HF_CORE_ENABLE_TMC5160 OFF)          # optional: narrow the handler set
add_subdirectory(lib/core hf-core)

add_executable(test_fan_manager test_fan_manager.cpp src/FanManager.cpp)
target_link_libraries(test_fan_manager PRIVATE hf_core_host_mocks hf_core_host_test_main)
add_test(NAME fan_manager COMMAND test_fan_manager)
```

```cpp
#include "HfCoreMocks.h"
#include "HostTest.h"
#include "FanManager.h"   // takes BaseGpio& and BaseAdc& in its constructor

HF_TEST(FanManager_TurnsFanOnAboveThreshold) {
    hf::host::MockGpio fan_en(3, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    hf::host::MockAdc adc(8);
    adc.SetVoltage(5, 2.1f);                  // board-specific scaling in FanManager
    FanManager mgr(fan_en, adc, /*adc_channel=*/5);
    CHECK(mgr.Update());
    CHECK(fan_en.DrivenLevel() == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
}
```

Managers that take `Base*` references, not `Esp*` or `Stm*` types, can be
tested this way unchanged on any target MCU.

## The MCU-agnostic boundary

The host build is the check that code above the MCU layer stays portable: if
it compiles and links with `HF_CORE_MCU=NONE`, it does not depend on a vendor
SDK.

May include ESP-IDF, STM32 HAL, FreeRTOS or other vendor headers:

- `hf-core-drivers/internal/hf-internal-interface-wrap/inc/mcu/<mcu>/` and
  `src/mcu/<mcu>/` — the `Esp*` and `Stm*` implementations of `Base*`;
- `handlers/logger/factory/<Mcu>LoggerFactory.cpp` — the per-MCU default log
  backend (the host build links `NoBackendLoggerFactory.cpp`, which returns
  no backend; inject one with `Logger::Initialize(config, backend)`);
- `hf-utils-rtos-wrap/include/os/os_<rtos>.h` and the RTOS sources selected by
  `HF_CORE_RTOS`;
- MCU-gated handlers in the gate table of `hf_core_build_settings.cmake`
  (WS2812 on ESP32 RMT) and `handlers/common/canopen/CO_driver_*`.

Must not include them: everything else in `handlers/`, the external drivers
in `hf-core-drivers/external/`, `hf-utils-general` and `hf-utils-canopen`.
Use `Base*` interfaces, `OsAbstraction.h` / `OsUtility.h` for time and
locking, and `handler_utils::DelayMs()` / `DelayUs()` from `HandlerCommon.h`
for delays. Where a handler needs an MCU-specific path, guard it on the
platform macro (`ESP_PLATFORM`, `HF_MCU_FAMILY_STM32`) and keep a portable
fallback so the host build still compiles it.

Known leftovers inside portable handlers, kept as they are for now:

- `Pcal95555Handler` and `Se050Handler` contain board-specific blocks guarded
  by `PW_HAL_STM32H7` that include `StmI2c.h`. They are not compiled on the
  host.
- `HalSpiMax22200Comm::Transfer` copies the caller's buffers with aligned
  32-bit word accesses, a workaround for one MCU's memory map. It is compiled
  everywhere and touches up to three bytes on either side of the buffers, so
  it would trip an address sanitizer.
