---
layout: default
title: RTOS Wrappers
parent: Utilities
nav_order: 2
---

# RTOS Wrappers (hf-utils-rtos-wrap)

C++ abstractions over FreeRTOS primitives for thread-safe embedded development.

## Synchronization

| Class | Description |
|:------|:------------|
| `RtosMutex` | Named mutex wrapper (`xSemaphoreCreateMutex`) |
| `MutexLockGuard` | RAII lock guard for `RtosMutex` |
| `OsRecursiveMutex` | Recursive mutex (same-thread re-entrant) |
| `OsSemaphore` | Counting semaphore |
| `SemaphoreLockGuard` | RAII guard for semaphores |

## Communication

| Class | Description |
|:------|:------------|
| `OsQueue<T>` | Type-safe FreeRTOS queue wrapper |
| `OsEventFlags` | Event group with set/wait/clear operations |

## Tasks & Timers

| Class | Description |
|:------|:------------|
| `BaseThread` | Abstract FreeRTOS task with Setup/Loop/Cleanup lifecycle |
| `PeriodicTimer` | FreeRTOS software timer wrapper |

## Delay Functions

| Function | Description |
|:---------|:------------|
| `os_delay_msec(ms)` | Blocking delay (milliseconds) |
| `os_delay_usec(us)` | Busy-wait delay (microseconds) |

## Usage Patterns

### Mutex-Protected Resource

```cpp
RtosMutex mtx("sensor_guard");

void read_sensor() {
    MutexLockGuard guard(mtx);
    // Protected access here
}
```

### Periodic Task

```cpp
class SensorTask : public BaseThread {
public:
    SensorTask() : BaseThread("Sensor", 4096, 5) {}
protected:
    void Setup() override { sensor_.Initialize(); }
    void Loop() override { sensor_.Read(); vTaskDelay(pdMS_TO_TICKS(100)); }
    void Cleanup() override { sensor_.Deinitialize(); }
    void OnStop() override {}
    void ResetVariables() override {}
};
```

## Test Coverage

See `examples/esp32/main/utils_tests/rtos_wrap_comprehensive_test.cpp` — tests all
synchronization primitives, queues, event flags, periodic timers, threads, and delays.
