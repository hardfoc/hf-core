/**
 * @file HfCoPortBinding.h
 * @brief Attaches an `hf::can::ICanPort` to a CO driver port slot
 *        (hf_co_driver_attach_port()).
 *
 * @details The binding owns the C trampolines: driver → port (send, status,
 *          recover) and port → driver (RX sink → hf_co_driver_rx_push(),
 *          TX-done sink → hf_co_driver_tx_done()). Keep it alive while the
 *          port is attached.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <cstddef>
#include <cstring>

#include "HfCanPort.h"
#include "hf_co_driver.h"

namespace hf::can {

static_assert(sizeof(CanFrame) == sizeof(hf_co_frame_t), "CanFrame and hf_co_frame_t must match");
static_assert(offsetof(CanFrame, id) == offsetof(hf_co_frame_t, id), "CanFrame layout");
static_assert(offsetof(CanFrame, len) == offsetof(hf_co_frame_t, len), "CanFrame layout");
static_assert(offsetof(CanFrame, flags) == offsetof(hf_co_frame_t, flags), "CanFrame layout");
static_assert(offsetof(CanFrame, data) == offsetof(hf_co_frame_t, data), "CanFrame layout");
static_assert(offsetof(CanFrame, ts_us) == offsetof(hf_co_frame_t, ts_us), "CanFrame layout");
static_assert(frame_flag::kExt == HF_CO_FRAME_EXT && frame_flag::kRtr == HF_CO_FRAME_RTR &&
                  frame_flag::kFdf == HF_CO_FRAME_FDF && frame_flag::kBrs == HF_CO_FRAME_BRS &&
                  frame_flag::kEsi == HF_CO_FRAME_ESI,
              "flag bits");

/** @brief Copy between the C++ and C frame types (identical layout). */
inline void ToC(const CanFrame& in, hf_co_frame_t& out) noexcept {
  out.id = in.id;
  out.len = in.len;
  out.flags = in.flags;
  std::memcpy(out.data, in.data, sizeof(out.data));
  out.ts_us = in.ts_us;
}
inline void FromC(const hf_co_frame_t& in, CanFrame& out) noexcept {
  out.id = in.id;
  out.len = in.len;
  out.flags = in.flags;
  std::memcpy(out.data, in.data, sizeof(out.data));
  out.ts_us = in.ts_us;
}

/** @brief One ICanPort attached to one CO driver port slot. */
class CoPortBinding {
 public:
  CoPortBinding() = default;
  ~CoPortBinding() { Detach(); }
  CoPortBinding(const CoPortBinding&) = delete;
  CoPortBinding& operator=(const CoPortBinding&) = delete;

  /**
   * @brief Attach @p port at slot @p index of @p drv.
   * @param error_source the port is a physical bus (its counters drive CANerrorStatus)
   */
  bool Attach(hf_co_driver_t* drv, uint8_t index, ICanPort& port, bool error_source) noexcept {
    if (drv_ != nullptr || drv == nullptr) {
      return false;
    }
    static constexpr hf_co_port_ops_t kOps{&SendTrampoline, &StatusTrampoline, &RecoverTrampoline};
    const PortCaps caps = port.Caps();
    drv_ = drv;
    index_ = index;
    port_ = &port;
    backpressure_ = port.RxBackpressure();
    if (!hf_co_driver_attach_port(drv, index, &kOps, this, caps.max_len,
                                  error_source ? HF_CO_PORT_ERROR_SOURCE : 0U)) {
      drv_ = nullptr;
      port_ = nullptr;
      return false;
    }
    port.SetTxDoneSink(&TxDoneTrampoline, this);
    port.SetRxSink(&RxTrampoline, this);
    return true;
  }

  /** @brief Stop RX delivery and detach the slot. */
  void Detach() noexcept {
    if (drv_ == nullptr) {
      return;
    }
    port_->SetRxSink(nullptr, nullptr);
    port_->SetTxDoneSink(nullptr, nullptr);
    hf_co_driver_detach_port(drv_, index_);
    drv_ = nullptr;
    port_ = nullptr;
  }

  ICanPort* Port() const noexcept { return port_; }
  uint8_t Index() const noexcept { return index_; }

 private:
  static hf_co_tx_result_t SendTrampoline(void* ctx, const hf_co_frame_t* frame) {
    auto* self = static_cast<CoPortBinding*>(ctx);
    CanFrame f;
    FromC(*frame, f);
    return static_cast<hf_co_tx_result_t>(self->port_->Send(f));
  }
  static bool StatusTrampoline(void* ctx, hf_co_port_status_t* out) {
    const PortStatus s = static_cast<CoPortBinding*>(ctx)->port_->Status();
    out->state = static_cast<uint8_t>(s.state);
    out->tec = s.tec;
    out->rec = s.rec;
    out->rx_overruns = s.rx_overruns;
    out->tx_queue_full = s.tx_queue_full;
    out->bus_off_count = s.bus_off_count;
    return true;
  }
  static bool RecoverTrampoline(void* ctx) { return static_cast<CoPortBinding*>(ctx)->port_->Recover(); }
  static bool RxTrampoline(void* ctx, const CanFrame& frame) {
    auto* self = static_cast<CoPortBinding*>(ctx);
    hf_co_frame_t f;
    ToC(frame, f);
    return self->backpressure_ ? hf_co_driver_rx_offer(self->drv_, self->index_, &f)
                               : hf_co_driver_rx_push(self->drv_, self->index_, &f);
  }
  static void TxDoneTrampoline(void* ctx) {
    auto* self = static_cast<CoPortBinding*>(ctx);
    hf_co_driver_tx_done(self->drv_, self->index_);
  }

  hf_co_driver_t* drv_{nullptr};
  ICanPort* port_{nullptr};
  uint8_t index_{0};
  bool backpressure_{false};
};

}  // namespace hf::can
