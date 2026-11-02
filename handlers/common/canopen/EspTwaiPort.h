/**
 * @file EspTwaiPort.h
 * @brief ICanPort over the ESP32 TWAI controller (EspCan, BaseCan v2).
 *
 * @details
 *  - `Send` is `EspCan::TrySendMessage`: the frame takes a free TX slot or
 *    the call returns QueueFull at once (no wait, no delay).
 *  - RX uses `EspCan::SetReceiveCallbackEx`: the TWAI RX ISR converts the
 *    message to a CanFrame and calls the sink (hf_co_driver_rx_push(): a
 *    lock-free ring copy). Nothing else may register a receive callback on
 *    the same EspCan.
 *  - `Status` is `EspCan::GetBusStatus` (state, TEC/REC, overruns,
 *    bus-off count); `Recover` is `EspCan::RequestBusRecovery`.
 *  - EspCan v2 has no TX-done hook for users, so frames left pending are
 *    re-sent from CO_CANmodule_process() (the service thread runs at least
 *    every millisecond while frames are pending).
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <atomic>
#include <cstdint>

#include "HfCanPort.h"

class EspCan;
struct hf_can_message_t;

namespace hf::can {

class EspTwaiPort final : public ICanPort {
 public:
  /**
   * @param can initialised EspCan (the port does not own it)
   * @param nominal_bps arbitration bit rate, reported in Caps()
   */
  EspTwaiPort(EspCan& can, uint32_t nominal_bps) noexcept;
  ~EspTwaiPort() override;
  EspTwaiPort(const EspTwaiPort&) = delete;
  EspTwaiPort& operator=(const EspTwaiPort&) = delete;

  PortCaps Caps() const override;
  TxResult Send(const CanFrame& frame) override;
  void SetRxSink(RxSink sink, void* ctx) override;
  PortStatus Status() const override;
  bool Recover() override;

 private:
  static void OnRx(const hf_can_message_t& message, void* user_data);

  EspCan& can_;
  uint32_t nominal_bps_;
  std::atomic<RxSink> sink_{nullptr};
  std::atomic<void*> sink_ctx_{nullptr};
};

}  // namespace hf::can
