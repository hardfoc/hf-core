# Threading and RTOS

How hf-core locks, waits and runs tasks without naming an RTOS: the selection, the primitives and what they do
on the host.

## The RTOS is chosen once

`HF_CORE_RTOS` picks the backend at configure time and defines `HF_RTOS_<NAME>=1`. Code above the backends uses
`OsAbstraction.h` / `OsUtility.h` (`os_delay_msec()`, `os_time_get()`, the `OS_*` types) and the C++ classes of
[hf-utils-rtos-wrap](https://hardfoc.github.io/hf-utils-rtos-wrap/), never FreeRTOS headers.

| `HF_CORE_RTOS` | Backend header | `HF_THREAD_SAFE` | Used by |
| --- | --- | --- | --- |
| `FREERTOS` | `os/os_freertos.h` | 1 | ESP-IDF builds, both board HALs |
| `THREADX` | `os/os_threadx.h` | 1 | Selectable; no consumer yet |
| `NONE` | `os/os_none.h` | not defined | The host build; bare-metal bring-up |

The rtos wrapper also has a CMSIS-RTOS header (`os/os_cmsis.h`, `HF_RTOS_CMSIS`); the build settings do not offer
it yet.

## The primitives

| Class | Header | Use it for |
| --- | --- | --- |
| `RtosMutex` | `RtosMutex.h` | The handler lock: recursive, `lock()`, `try_lock_for(ms)`, `unlock()` |
| `MutexLockGuard` | `RtosMutex.h` | RAII lock on an `RtosMutex` (`RtosUniqueLock<RtosMutex>`), optional timeout, `IsLocked()` |
| `BaseThread` | `BaseThread.h` | A task with `Setup()`, `Step()` (returns the delay to the next step) and `Cleanup()` |
| `PeriodicTimer` | `PeriodicTimer.h` | A software timer calling a function |
| `OsQueue`, `OsEventFlags`, `SignalSemaphore` | same names | Passing items, flags and signals between tasks |
| `CriticalGuard` | `CriticalGuard.h` | A short critical section |
| `handler_utils::DelayMs()`, `DelayUs()` | `handlers/common/HandlerCommon.h` | Delays inside handlers, on any platform |

HardFOC runs FreeRTOS at 1 kHz, so a millisecond delay or timeout is one tick. On a slower tick a non-zero
millisecond value still waits at least one tick, never zero: `os_convert_msec_to_delay_ticks()` in hf-utils-rtos-wrap
and `handler_utils::DelayMs()` round the same way, and `DelayUs()` rounds a wait of 1 ms or more up to whole
milliseconds.

A class guarding its own state:

```cpp title="tests/docs/threading.cpp"
--8<-- "tests/docs/threading.cpp:own-lock"
```

A task that owns a sensor read:

```cpp title="tests/docs/threading.cpp"
--8<-- "tests/docs/threading.cpp:thread"
```

## Mutexes inside the interface wrap

`hf-internal-interface-wrap` cannot depend on the RTOS wrapper, so its `PlatformMutex` defaults to a no-op
backend. hf-core supplies the real one: `handlers/common/PlatformMutexBackend.h` is on the include path, and
`PlatformMutex.h` picks it up with `__has_include`. With `HF_RTOS_FREERTOS` it maps to a FreeRTOS mutex; with
`HF_RTOS_NONE` it stays a no-op. A standalone build of the interface wrap therefore has no locking.

## On the host

With `HF_RTOS_NONE` there is no scheduler and no tick. The host build defines `HF_RTOS_NONE_EXTERNAL_CLOCK` and
routes every clock read and delay to `hf::host::FakeClock`: a delay advances the clock and returns, and each
clock read advances it by 10 µs, so polling loops reach their timeouts. `BaseThread` runs `Setup()` inline in
`StartThreadAndWaitToVerify()` and the test calls `Step()` itself. `RtosMutex` locks and unlocks but never
blocks. Details: [Host build and mocks](host-build.md#time-and-threads-on-the-host).

Next: [MCU boundary](mcu-boundary.md).
