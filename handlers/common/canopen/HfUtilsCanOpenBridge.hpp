/**
 * @file HfUtilsCanOpenBridge.hpp
 * @brief Convert `CanOpen::CanFrame` ↔ `hf_can_message_t` for `BaseCan` I/O.
 * @details Lives in **hf-core** (not `hf-utils-canopen`) so `hf-utils-canopen` stays free of
 *          `BaseCan` / hardware message types. Include when linking CANopen helpers to TWAI.
 */
#pragma once

#include "CanFrame.h"
#include "base/BaseCan.h"

#include <algorithm>
#include <cstring>

inline void HfUtilsCanFrameToMessage(const CanOpen::CanFrame& c, hf_can_message_t& m) noexcept {
  m.id = static_cast<hf_u32_t>(c.id);
  m.dlc = c.dlc;
  m.is_extended = c.extended;
  m.is_rtr = c.rtr;
  const std::size_t n = (std::min)(static_cast<std::size_t>(c.dlc), sizeof(m.data));
  (void)std::memcpy(m.data, c.data, n);
  if (n < sizeof(m.data)) {
    (void)std::memset(m.data + n, 0, sizeof(m.data) - n);
  }
}

inline void HfUtilsMessageToCanFrame(const hf_can_message_t& m, CanOpen::CanFrame& c) noexcept {
  c.id = m.id;
  c.dlc = m.dlc;
  c.extended = m.is_extended;
  c.rtr = m.is_rtr;
  const std::size_t n = (std::min)(static_cast<std::size_t>(m.dlc), sizeof(c.data));
  (void)std::memcpy(c.data, m.data, n);
  if (n < sizeof(c.data)) {
    (void)std::memset(c.data + n, 0, sizeof(c.data) - n);
  }
}
