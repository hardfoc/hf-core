# CANopen

Build and parse CANopen frames with hf-utils-canopen and send them on any `BaseCan` through hf-core's bridge.

## The pieces

| Piece | Where | What it does |
| --- | --- | --- |
| `CanOpen::CanFrame` | hf-utils-canopen `CanFrame.h` | Classic frame: `id`, `dlc`, `extended`, `rtr`, `data[8]` |
| Builders | `CanOpenUtils.h`, `CanOpenMotorUtils.h` | `BuildNmt()`, `BuildSdoDownload()` (1, 2 or 4 bytes), `BuildSdoUpload()`, `BuildHeartbeat()`, and CiA 402 objects: `BuildControlWord()`, `BuildModeOfOperation()`, `BuildTargetVelocity()`, `BuildTargetPosition()`, `BuildTargetTorque()`, profile velocity, acceleration and deceleration, status and actual-value requests |
| Parsers | `CanOpenExtras.h` | `ParseSdoResponse()` (expedited: upload data, download ack, abort code), `ParseHeartbeat()`, `ParseEmergency()`, `DecodeStatusWord()`, `NextControlWord()` |
| Payload codec | `CanOpenExtras.h` | `FrameCodec` interface to encode or authenticate payloads; `PassthroughCodec` by default |
| Bridge | hf-core `handlers/common/canopen/` | `HfUtilsCanFrameToMessage()` / `HfUtilsMessageToCanFrame()`, `HfUtilsCanOpenTransport`, `CanOpenBaseCanLink` (`Open`, `Write`, `Read`, `Close`) |
| CANopenNode driver | `handlers/common/canopen/CO_driver_vortex.c` | A CANopenNode (CiA 301) target driver that sends and receives through registered hooks |

hf-utils-canopen has no dependency on `BaseCan`; the bridge lives in hf-core so the utilities stay portable.
Turn on `HF_CORE_ENABLE_UTILS_CANOPEN` for the utilities and `HF_CORE_ENABLE_CAN` as well for the bridge.

## Write an object with SDO

```cpp title="tests/docs/canopen.cpp"
--8<-- "tests/docs/canopen.cpp:sdo"
```

`CanOpenBaseCanLink::Write()` calls `BaseCan::SendMessage()` with a 50 ms timeout; `Read()` calls
`ReceiveMessage()`. The host build runs this function against `hf::host::MockCan` with an injected
acknowledgement (`tests/docs/doc_snippets_test.cpp`).

## Enable a CiA 402 drive

`DecodeStatusWord()` maps a status word to a `DriveState`; `NextControlWord()` gives the command that moves one
step toward the state you want:

```cpp title="tests/docs/canopen.cpp"
--8<-- "tests/docs/canopen.cpp:cia402"
```

| From `DriveState` | Command toward `OperationEnabled` |
| --- | --- |
| `Fault` | `FaultReset` |
| `SwitchOnDisabled` | `Shutdown` |
| `ReadyToSwitchOn` | `SwitchOn` |
| `SwitchedOn` (and any other) | `EnableOperation` |

## Frame size

`CanOpen::CanFrame` is classic CAN (8 bytes); `hf_can_message_t` is FD-sized. The bridge copies at most `dlc`
bytes, bounded by the 8-byte source buffer, sends classic frames (`is_canfd = false`) and zero-fills the rest.

## Running a CANopen slave

`HF_CORE_ENABLE_CANOPENNODE` compiles upstream CANopenNode with `CO_driver_vortex.c`. The application registers
its transmit function with `hf_co_driver_register_tx()` and feeds received standard frames to
`hf_co_driver_process_rx()` (`hf_co_driver_iface.h`). It needs `HF_CORE_ENABLE_UTILS_CANOPEN` and a CANopenNode
checkout at `HF_CORE_CANOPENNODE_ROOT`; the driver uses a FreeRTOS critical section, so it builds for ESP-IDF
only. For a CiA 402 host client, see the [Vortex driver](https://github.com/hardfoc/hf-vortex-driver).

Next: [Utilities](utilities.md).
