/**
 * @file EspTwaiPort.cpp
 * @brief ICanPort over EspCan (TWAI). ESP32 builds only.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "EspTwaiPort.h"

#include "base/BaseCan.h"
#include "mcu/esp32/EspCan.h"

namespace hf::can {

EspTwaiPort::EspTwaiPort(EspCan& can, uint32_t nominal_bps) noexcept : can_(can), nominal_bps_(nominal_bps) {}

EspTwaiPort::~EspTwaiPort() { SetRxSink(nullptr, nullptr); }

PortCaps EspTwaiPort::Caps() const {
  PortCaps c;
  c.fd = false;  // the C6 TWAI controllers are classic CAN only
  c.max_len = kClassicMaxLen;
  c.nominal_bps = nominal_bps_;
  c.data_bps = 0;
  c.hw_filters = 1;
  return c;
}

TxResult EspTwaiPort::Send(const CanFrame& frame) {
  if (!IsWellFormed(frame) || frame.IsFd()) {
    return TxResult::NotAllowed;
  }
  const hf_can_message_t m = hf_can_message_t::FromFrame(frame);
  switch (can_.TrySendMessage(m)) {
    case hf_can_err_t::CAN_SUCCESS:
      return TxResult::Ok;
    case hf_can_err_t::CAN_ERR_QUEUE_FULL:
    case hf_can_err_t::CAN_ERR_RESOURCE_BUSY:
      return TxResult::QueueFull;
    case hf_can_err_t::CAN_ERR_BUS_OFF:
      return TxResult::BusOff;
    default:
      return TxResult::NotAllowed;
  }
}

void EspTwaiPort::OnRx(const hf_can_message_t& message, void* user_data) {
  // TWAI RX ISR: copy and return.
  auto* self = static_cast<EspTwaiPort*>(user_data);
  const RxSink sink = self->sink_.load(std::memory_order_acquire);
  if (sink == nullptr) {
    return;
  }
  CanFrame f;
  message.ToFrame(f);
  (void)sink(self->sink_ctx_.load(std::memory_order_relaxed), f);  // a refusal is an overrun (counted by the driver)
}

void EspTwaiPort::SetRxSink(RxSink sink, void* ctx) {
  if (sink == nullptr) {
    sink_.store(nullptr, std::memory_order_release);
    can_.ClearReceiveCallbackEx();
    sink_ctx_.store(nullptr, std::memory_order_relaxed);
    return;
  }
  sink_ctx_.store(ctx, std::memory_order_relaxed);
  sink_.store(sink, std::memory_order_release);
  (void)can_.SetReceiveCallbackEx(&EspTwaiPort::OnRx, this);
}

PortStatus EspTwaiPort::Status() const {
  PortStatus s;
  hf_can_bus_status_t bs{};
  if (can_.GetBusStatus(bs) != hf_can_err_t::CAN_SUCCESS) {
    return s;
  }
  s.state = static_cast<BusState>(bs.state);
  s.tec = bs.tec;
  s.rec = bs.rec;
  s.rx_overruns = bs.rx_overruns;
  s.tx_queue_full = bs.tx_queue_full;
  s.bus_off_count = bs.bus_off_count;
  return s;
}

bool EspTwaiPort::Recover() { return can_.RequestBusRecovery() == hf_can_err_t::CAN_SUCCESS; }

}  // namespace hf::can
