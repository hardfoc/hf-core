# CANopen

Build and parse CANopen frames with hf-utils-canopen and send them on any `BaseCan` through hf-core's bridge, or
run a CANopenNode device over one or more CAN ports with hf-core's generic driver.

## The pieces

| Piece | Where | What it does |
| --- | --- | --- |
| `CanOpen::CanFrame` | hf-utils-canopen `CanFrame.h` | Classic frame: `id`, `dlc`, `extended`, `rtr`, `data[8]` |
| Builders | `CanOpenUtils.h`, `CanOpenMotorUtils.h` | `BuildNmt()`, `BuildSdoDownload()` (1, 2 or 4 bytes), `BuildSdoUpload()`, `BuildHeartbeat()`, and CiA 402 objects: `BuildControlWord()`, `BuildModeOfOperation()`, `BuildTargetVelocity()`, `BuildTargetPosition()`, `BuildTargetTorque()`, profile velocity, acceleration and deceleration, status and actual-value requests |
| Parsers | `CanOpenExtras.h` | `ParseSdoResponse()` (expedited: upload data, download ack, abort code), `ParseHeartbeat()`, `ParseEmergency()`, `DecodeStatusWord()`, `NextControlWord()` |
| Payload codec | `CanOpenExtras.h` | `FrameCodec` interface to encode or authenticate payloads; `PassthroughCodec` by default |
| Bridge | hf-core `handlers/common/canopen/` | `HfUtilsCanFrameToMessage()` / `HfUtilsMessageToCanFrame()`, `HfUtilsCanOpenTransport`, `CanOpenBaseCanLink` (`Open`, `Write`, `Read`, `Close`) |
| CANopenNode driver | `handlers/common/canopen/CO_driver_hardfoc.c`, `hf_co_driver.h` | Generic CANopenNode (CiA 301) target driver: one `CO_CANmodule` over up to four ports |
| Ports | `HfCanPort.h`, `HfCoPortBinding.h`, `EspTwaiPort`, `LinuxSocketCanPort`, `PipePort` | `hf::can::ICanPort` and its bindings: TWAI (EspCan), SocketCAN, and a host pipe bus |
| Frame records | `HfCanFrameRecord.h` | Tunnel codec for frames over a byte stream or datagram (pipe bus, Wi-Fi, ESP-NOW) |

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

## Running a CANopen device

`HF_CORE_ENABLE_CANOPENNODE` compiles upstream CANopenNode with the generic driver `CO_driver_hardfoc.c`. The lists
live in `cmake/hf_core_canopennode.cmake`, which a host build can also include on its own. The application provides:

| Variable | What it points at |
| --- | --- |
| `HF_CORE_CANOPENNODE_ROOT` | the CANopenNode v4 checkout (a HAL's `third_party/CANopenNode`) |
| `HF_CORE_CANOPENNODE_OD_DIR` | the application's `OD.c` / `OD.h`. CANopenNode's example OD is not compiled |
| `HF_CORE_CANOPENNODE_CONFIG_DIR` | optional: `hf_co_app_config.h` with the stack's `CO_CONFIG_*` values |

The driver connects the stack to its ports like this:

- **Ports.** A port implements `hf::can::ICanPort`: `Send` never blocks, and the RX sink may run in an ISR.
  `hf::can::CoPortBinding` attaches a port to a slot of an `hf_co_driver_t`, the driver instance passed to
  `CO_CANinit()` as `CANptr`.
- **Receive.** Each port pushes frames into its own lock-free ring. `CO_CANmodule_process()`, called from
  `CO_process()`, hands them to CANopenNode in thread context. An optional RX policy hook sees the consumer that
  matched.
- **Transmit.** `CO_CANsend()` offers each frame to every port. A port that is full leaves the frame pending in its
  CANopenNode buffer, and it goes out again in buffer order from `CO_CANmodule_process()`. A port triggers that
  retry early by signalling that it has room again (`hf_co_driver_tx_done()`). An optional TX policy hook decides
  per port.
- **Errors.** Port TEC, REC and bus-off map to the `CO_CAN_ERR*` bits, which become EMCY messages. A bus-off port
  gets a recovery request (`EspCan::RequestBusRecovery()` for TWAI).
- **Locks.** The CO locks are one recursive mutex per CAN module, FreeRTOS on target and pthreads on a POSIX host.
  Nothing that can block runs while one is held.

| Port | Platform | Notes |
| --- | --- | --- |
| `EspTwaiPort` | ESP32 | `TrySendMessage`, the TWAI RX ISR into the ring, `GetBusStatus`, `RequestBusRecovery` |
| `LinuxSocketCanPort` | Linux | non-blocking raw SocketCAN (`can0`, `vcan0`), error-frame state |
| `PipePort` / `PipeHub` | POSIX | UNIX-socket bus for host tests; frame records, back-pressure instead of loss |

The host tests `test_co_driver`, `test_frame_record` and `test_pipe_loopback` run in CI, and again under
ThreadSanitizer. `test_pipe_loopback` sends 10⁵ frames through the pipe bus. For a complete device, see
[hardfoc-vortex-v1](https://github.com/hardfoc/hardfoc-vortex-v1) (`CoNode`). For a CiA 402 host client, see the
[Vortex driver](https://github.com/hardfoc/hf-vortex-driver).

Next: [Utilities](utilities.md).
