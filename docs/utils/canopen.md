---
layout: default
title: CANopen
parent: Utilities
nav_order: 3
---

# CANopen Utilities (hf-utils-canopen)

CAN frame construction and CANopen protocol helpers.

## Components

| Header | Description |
|:-------|:------------|
| `CanFrame.h` | `CanOpen::CanFrame` struct (ID, DLC, data, extended, RTR) |
| `SdoProtocol.h` | SDO COB-ID calculation, expedited download/upload frames |
| `NmtProtocol.h` | NMT commands and state machine enums |

## Usage

```cpp
#include "CanFrame.h"
#include "SdoProtocol.h"
#include "NmtProtocol.h"

// Build an SDO write frame
auto frame = CanOpen::BuildSdoDownloadExpedited(
    0x01,     // node_id
    0x1017,   // index (producer heartbeat time)
    0x00,     // sub-index
    1000,     // value (1000ms)
    2         // data_size
);

// Build an NMT start command
auto nmt = CanOpen::BuildNmtCommand(
    CanOpen::NmtCommand::START_REMOTE_NODE,
    0x01
);
```

## Test Coverage

See `examples/esp32/main/utils_tests/canopen_utils_comprehensive_test.cpp`.
