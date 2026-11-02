# HardFOC Core API reference {#mainpage}

The classes, members and parameters of hf-core's handlers and host mocks, generated from the headers.

Each handler lives in its own folder under `handlers/` and wraps one HardFOC device driver behind a thread-safe
API that takes `Base*` interfaces. Shared pieces are in `handlers/common/` (`HandlerCommon.h`, the CANopen
bridge) and the logger in `handlers/logger/`. The host fakes of the `Base*` interfaces are in namespace
`hf::host` (`tests/host/mocks/`).

The guides are on the documentation site: <a href="../">HardFOC Core documentation</a>.
