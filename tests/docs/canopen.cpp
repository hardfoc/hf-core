// CANopen snippets for docs/canopen.md. Compiled by the host build (target hf_core_doc_snippets).

#include <cstdint>

// --8<-- [start:sdo]
#include "CanOpenBaseCanLink.hpp"
#include "CanOpenExtras.h"

// Set a node's producer heartbeat time (object 0x1017) to 500 ms and wait for the reply.
bool SetHeartbeat(BaseCan& can, uint8_t node_id) {
  CanOpenBaseCanLink link(can);
  if (!link.Open()) {
    return false;
  }
  if (!link.Write(CanOpen::BuildSdoDownload(node_id, 0x1017, 0x00, 500, 2))) {
    return false;
  }
  CanOpen::CanFrame reply{};
  for (int tries = 0; tries < 10; ++tries) {
    if (!link.Read(reply, 10)) {
      continue;
    }
    const auto r = CanOpen::ParseSdoResponse(reply);
    if (r.valid && r.nodeId == node_id && r.index == 0x1017) {
      return r.kind == CanOpen::SdoResponseKind::DownloadAck;  // AbortTransfer: see r.abort_code
    }
  }
  return false;
}
// --8<-- [end:sdo]

// --8<-- [start:cia402]
// One step of bringing a CiA 402 drive to Operation enabled, from its status word.
CanOpen::CanFrame NextEnableStep(uint8_t node_id, uint16_t status_word) {
  const auto state = CanOpen::DecodeStatusWord(status_word);
  const auto cmd = CanOpen::NextControlWord(state, CanOpen::DriveState::OperationEnabled);
  return CanOpen::BuildControlWord(node_id, static_cast<uint16_t>(cmd));
}
// --8<-- [end:cia402]
