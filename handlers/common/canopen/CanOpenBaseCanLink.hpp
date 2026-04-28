/**
 * @file CanOpenBaseCanLink.hpp
 * @brief Facade: `CanOpen::CanFrame` I/O on HAL `BaseCan` via `HfUtilsCanOpenTransport`.
 * @details Use with `CommChannelsManager::GetCanOpenBus()` after HAL init. Same role as app-local
 *          wrappers; kept in hf-core so `main/` only includes one canonical adapter.
 */
#pragma once

#include "HfUtilsCanOpenTransport.hpp"
#include "CanFrame.h"

/**
 * @brief Facade used by `CANOpenBLDCThread` (`Open` / `Write` / `Read` / `Close`).
 */
class CanOpenBaseCanLink {
public:
  explicit CanOpenBaseCanLink(BaseCan& can) noexcept : transport_(can) {}

  bool Open() noexcept { return transport_.can().EnsureInitialized(); }

  void Close() noexcept { (void)transport_.can().EnsureDeinitialized(); }

  bool Write(const CanOpen::CanFrame& f) noexcept { return transport_.send(f); }

  bool Read(CanOpen::CanFrame& f, int timeoutMs) noexcept {
    return transport_.receive(f, static_cast<hf_u32_t>(timeoutMs < 0 ? 0 : timeoutMs));
  }

private:
  HfUtilsCanOpenTransport transport_;
};
