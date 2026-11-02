# Utilities

The utility libraries hf-core builds in: what each header provides and where its documentation lives.

| Library | Built when | Documentation |
| --- | --- | --- |
| [hf-utils-rtos-wrap](https://github.com/hardfoc/hf-utils-rtos-wrap) | Always | [Threading and RTOS](threading.md), [rtos-wrap site](https://hardfoc.github.io/hf-utils-rtos-wrap/) |
| [hf-utils-general](https://github.com/hardfoc/hf-utils-general) | Always (header-mostly; `SoftwareVersion`, `TimestampedVariable`, `CrcCalculator`, `Utility`, `VariableAnomalyMonitor`, `VariableMonitor` have sources) | This page |
| [hf-utils-canopen](https://github.com/hardfoc/hf-utils-canopen) | `HF_CORE_ENABLE_UTILS_CANOPEN` | [CANopen](canopen.md) |

All three live in the [hf-core-utils](https://github.com/hardfoc/hf-core-utils) submodule.

## hf-utils-general

Platform-free C++; the host build compiles it and `tests/host/test_utils_general.cpp` tests it.

| Header | Main type | Provides |
| --- | --- | --- |
| `ActionRunLimiter.h` | `ActionRunLimiter` | Caps how many times an action runs |
| `ActionTimer.h` | `ActionTimer` | Elapsed time of an action |
| `AveragingFilter.h` | `AveragingFilter` | Moving-average filter, templated on type and window |
| `BoundedLinearCurve.h` | `BoundedLinearCurve` | A line valid over an x range |
| `CircularBuffer.h` | `CircularBuffer` | Fixed-size circular buffer |
| `CrcCalculator.h` | `crc16()` | CRC-16/CCITT-FALSE over a buffer |
| `DestructAction.h` | `DestructAction` | Runs a function when the object goes out of scope |
| `DynamicArray.h` | `DynamicArray` | Array with a fixed maximum capacity and a variable size |
| `EnumArray.h` | `EnumArray` | Array indexed by an enumeration |
| `EnumeratedSetStatus.h` | `EnumeratedSetStatus` | A status per member of an enumeration |
| `LineEstimator.h` | `LineEstimator` | Slope of a line from data points |
| `MultiReadings.h` | `MultiReadings` | A set of sensor readings managed together |
| `MultibitSet.h` | `MultibitSet` | `std::bitset` with multi-bit entries |
| `PID.h` | `PID` | Discrete PID controller |
| `ParabolicCurveEstimator.h` | `ParabolicCurveEstimator` | Least-squares parabola fit |
| `PiecewiseBounds.h`, `PiecewiseLinearCurve.h` | `PiecewiseBounds`, `PiecewiseBoundedLinearCurve` | Bounds and curves from several `BoundedLinearCurve` segments |
| `RingBuffer.h` | `RingBuffer` | Windowed buffer with a compile-time size, forward and reverse iteration; not thread-safe |
| `SimpleStateMachine.h`, `SlightlyAdvancedStateMachine.h`, `StateMachine.h`, `StateActionsBase.h` | state machines | From a minimal state holder to an allocation-free machine with entry, loop and exit actions |
| `SoftwareVersion.h` | `SoftwareVersion` | Software version number |
| `TaskManager.h` | `Task` | Prioritised check-and-run tasks |
| `TestManager.h` | `TestManager` | A sequence of tests |
| `TimestampedVariable.h` | `TimestampedVariable` | A value with the time it was set |
| `VariableMonitor.h`, `VariableAnomalyMonitor.h`, `VariableTrackerBase.h` | monitors | Anomaly detection on a value from slope and thresholds |
| `VariableWithUnit.h` | `VariableWithUnit` | A value with its unit |
| `Utility.h` | functions | Generic helpers |

None of these classes lock: protect a shared instance with an `RtosMutex`.

Next: [Host build and mocks](host-build.md).
