/**
 * @file HfUtilsCanOpenTransport.hpp
 * @brief Send/receive `CanOpen::CanFrame` on a `BaseCan` instance.
 * @details **hf-core** adapter between `hf-utils-canopen` framing and `hf-internal-interface-wrap` `BaseCan`.
 */
#pragma once

#include "HfUtilsCanOpenBridge.hpp"
#include "base/BaseCan.h"
#include "CanFrame.h"

class HfUtilsCanOpenTransport {
public:
  explicit HfUtilsCanOpenTransport(BaseCan& can) noexcept : can_(can) {}

  BaseCan& can() noexcept { return can_; }

  bool send(const CanOpen::CanFrame& f, hf_u32_t timeout_ms = 50U) noexcept {
    hf_can_message_t m{};
    HfUtilsCanFrameToMessage(f, m);
    return can_.SendMessage(m, timeout_ms) == hf_can_err_t::CAN_SUCCESS;
  }

  bool receive(CanOpen::CanFrame& f, hf_u32_t timeout_ms) noexcept {
    hf_can_message_t m{};
    if (can_.ReceiveMessage(m, timeout_ms) != hf_can_err_t::CAN_SUCCESS) {
      return false;
    }
    HfUtilsMessageToCanFrame(m, f);
    return true;
  }

private:
  BaseCan& can_;
};
