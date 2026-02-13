---
layout: default
title: General Utilities
parent: Utilities
nav_order: 1
---

# General Utilities (hf-utils-general)

Header-only C++ utility library with no platform dependencies.

## Data Structures

| Class | Description |
|:------|:------------|
| `CircularBuffer<T, N>` | Fixed-size FIFO buffer with overflow semantics |
| `RingBuffer<T, N>` | Ring buffer with Front/Back access |
| `SimpleLinkedList<T>` | Minimal singly-linked list |
| `FlagSet<T>` | Bitfield flag management |

## Filters & Math

| Class | Description |
|:------|:------------|
| `AveragingFilter<T, N>` | Sliding-window moving average |
| `LinearInterpolation` | Point-based linear interpolation |
| `PiecewiseLinearSegment` | Multi-segment piecewise linear functions |
| `CrcUtil` | CRC-8 calculation |

## Timing & Scheduling

| Class | Description |
|:------|:------------|
| `ActionTimer` | Start/stop duration measurement |
| `IntervalAction` | Periodic action execution at fixed intervals |
| `TaskScheduler` | Multi-task cooperative scheduler |
| `DeferredAction` | RAII deferred execution (like Go's `defer`) |

## State Management

| Class | Description |
|:------|:------------|
| `StateMachineBase` | Abstract state machine with transitions |
| `ExtendedStateMachine` | State machine with guard conditions |

## Other

| Class | Description |
|:------|:------------|
| `SoftwareVersion` | Semantic versioning (major.minor.build) |
| `PhysicalUnit` | Value + unit string container |

## Test Coverage

See `examples/esp32/main/utils_tests/general_utils_comprehensive_test.cpp`.
