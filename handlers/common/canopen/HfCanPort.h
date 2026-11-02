/**
 * @file HfCanPort.h
 * @brief Transport-neutral CAN frame and the ICanPort interface for port
 *        bindings (EspTwaiPort, LinuxSocketCanPort, PipePort, later FDCAN and
 *        the wireless tunnels).
 *
 * @details Mirrors `vortex::contracts::ICanPort` (hardfoc-vortex-v1
 *          `contracts/can_frame.hpp`) so a product can use either: same frame
 *          layout, same flag bits, same result and state enums. Two additions:
 *          - an optional TX-done sink, through which a port tells the CO
 *            driver that it has room again (hf_co_driver_tx_done());
 *          - the RX sink returns whether it took the frame. A hardware port
 *            ignores the answer (a frame it cannot hand over is an overrun);
 *            a port with `RxBackpressure()` (the host pipe bus) keeps the
 *            frame and offers it again later, so a slow receiver slows the
 *            bus down instead of losing frames.
 *
 *          `Send` never blocks. The RX sink may be called in ISR context: it
 *          copies the frame and returns.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <cstdint>

namespace hf::can {

/** @brief Largest classic CAN payload in bytes. */
inline constexpr uint8_t kClassicMaxLen = 8U;
/** @brief Largest CAN FD payload in bytes. */
inline constexpr uint8_t kFdMaxLen = 64U;
/** @brief Largest 11-bit identifier. */
inline constexpr uint32_t kMaxStdId = 0x7FFU;
/** @brief Largest 29-bit identifier. */
inline constexpr uint32_t kMaxExtId = 0x1FFFFFFFU;

/** @brief Bits of `CanFrame::flags`. */
namespace frame_flag {
inline constexpr uint8_t kExt = 0x01U;  ///< 29-bit identifier
inline constexpr uint8_t kRtr = 0x02U;  ///< remote request (classic only)
inline constexpr uint8_t kFdf = 0x04U;  ///< FD frame format
inline constexpr uint8_t kBrs = 0x08U;  ///< bit-rate switch (FD only)
inline constexpr uint8_t kEsi = 0x10U;  ///< error state indicator (FD only)
inline constexpr uint8_t kAll = kExt | kRtr | kFdf | kBrs | kEsi;
}  // namespace frame_flag

/** @brief One CAN or CAN FD frame; `len` is a byte count, never a DLC code. */
struct CanFrame {
  uint32_t id{0};
  uint8_t len{0};
  uint8_t flags{0};
  uint8_t data[kFdMaxLen]{};
  uint32_t ts_us{0};  ///< RX timestamp (µs, port clock); ignored on TX

  constexpr bool IsExtended() const noexcept { return (flags & frame_flag::kExt) != 0U; }
  constexpr bool IsRemote() const noexcept { return (flags & frame_flag::kRtr) != 0U; }
  constexpr bool IsFd() const noexcept { return (flags & frame_flag::kFdf) != 0U; }
};

/** @brief Is @p len an exact FD payload size (0..8, 12, 16, 20, 24, 32, 48, 64)? */
constexpr bool IsExactFdLength(uint8_t len) noexcept {
  return len <= 8U || len == 12U || len == 16U || len == 20U || len == 24U || len == 32U || len == 48U || len == 64U;
}

/** @brief Structurally valid frame (flags, id range, length for the frame kind)? */
constexpr bool IsWellFormed(const CanFrame& f) noexcept {
  if ((f.flags & static_cast<uint8_t>(~frame_flag::kAll)) != 0U) return false;
  if (f.id > (f.IsExtended() ? kMaxExtId : kMaxStdId)) return false;
  if (f.IsFd()) return !f.IsRemote() && f.len <= kFdMaxLen && IsExactFdLength(f.len);
  if ((f.flags & (frame_flag::kBrs | frame_flag::kEsi)) != 0U) return false;
  return f.len <= kClassicMaxLen;
}

/** @brief What a port binding can carry. */
struct PortCaps {
  bool fd{false};
  uint8_t max_len{kClassicMaxLen};
  uint32_t nominal_bps{0};  ///< 0 = not a physical bus
  uint32_t data_bps{0};
  uint8_t hw_filters{0};
};

/** @brief Result of a non-blocking `ICanPort::Send`. */
enum class TxResult : uint8_t { Ok = 0, QueueFull, BusOff, NotAllowed };

/** @brief Controller error state. */
enum class BusState : uint8_t { Stopped = 0, ErrorActive, ErrorWarning, ErrorPassive, BusOff };

/** @brief Error counters and statistics of one port. */
struct PortStatus {
  BusState state{BusState::Stopped};
  uint16_t tec{0};
  uint16_t rec{0};
  uint32_t rx_overruns{0};
  uint32_t tx_queue_full{0};
  uint32_t bus_off_count{0};
};

/** @brief RX sink; may run in ISR context (copy and return). Returns false if it could not take the frame. */
using RxSink = bool (*)(void* ctx, const CanFrame& frame);
/** @brief TX-done sink: the port has room again; may run in ISR context. */
using TxDoneSink = void (*)(void* ctx);

/** @brief One CAN transport binding. */
class ICanPort {
 public:
  virtual ~ICanPort() = default;

  /** @brief Static capabilities. */
  virtual PortCaps Caps() const = 0;
  /** @brief Queue @p frame without blocking. */
  virtual TxResult Send(const CanFrame& frame) = 0;
  /** @brief Install the RX sink (nullptr detaches). */
  virtual void SetRxSink(RxSink sink, void* ctx) = 0;
  /** @brief Error counters and state. */
  virtual PortStatus Status() const = 0;
  /** @brief Request bus-off recovery; false when not possible now. */
  virtual bool Recover() = 0;
  /** @brief True if the port holds a frame the RX sink refused and offers it again (no loss). */
  virtual bool RxBackpressure() const { return false; }
  /** @brief Install the TX-done sink. Ports without a TX-done event ignore it. */
  virtual void SetTxDoneSink(TxDoneSink sink, void* ctx) {
    (void)sink;
    (void)ctx;
  }

 protected:
  ICanPort() = default;
  ICanPort(const ICanPort&) = default;
  ICanPort& operator=(const ICanPort&) = default;
};

}  // namespace hf::can
