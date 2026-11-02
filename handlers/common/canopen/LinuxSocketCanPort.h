/**
 * @file LinuxSocketCanPort.h
 * @brief ICanPort over a Linux SocketCAN interface (can0, vcan0). Linux only.
 *
 * @details Raw CAN socket in non-blocking mode. `Send` writes one frame;
 *          a full socket queue (EAGAIN / ENOBUFS) answers QueueFull and the
 *          reader thread reports TX-done as soon as the socket is writable
 *          again. Error frames (controller warning / passive, bus-off,
 *          restarted, and the TEC/REC bytes when the kernel provides them)
 *          drive `Status()`. Bus-off recovery is the kernel's job
 *          (`ip link set canX type can restart-ms N`), so `Recover()` only
 *          reports whether the interface is still bus-off.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

#include "HfCanPort.h"

namespace hf::can {

class LinuxSocketCanPort final : public ICanPort {
 public:
  struct Options {
    std::string ifname{"vcan0"};
    bool fd{false};            ///< enable CAN_RAW_FD_FRAMES
    uint32_t nominal_bps{0};   ///< reported in Caps() (0 for vcan)
  };

  explicit LinuxSocketCanPort(Options options);
  ~LinuxSocketCanPort() override;
  LinuxSocketCanPort(const LinuxSocketCanPort&) = delete;
  LinuxSocketCanPort& operator=(const LinuxSocketCanPort&) = delete;

  bool Open();
  void Close();
  bool IsOpen() const noexcept { return fd_ >= 0; }

  PortCaps Caps() const override;
  TxResult Send(const CanFrame& frame) override;
  void SetRxSink(RxSink sink, void* ctx) override;
  PortStatus Status() const override;
  bool Recover() override;
  void SetTxDoneSink(TxDoneSink sink, void* ctx) override;

 private:
  void ReadLoop();
  void HandleErrorFrame(uint32_t can_id, const uint8_t* data, uint8_t len);

  Options opt_;
  int fd_{-1};
  int wake_[2]{-1, -1};
  std::thread reader_;
  std::atomic<bool> run_{false};
  std::atomic<bool> want_writable_{false};

  std::mutex sink_mu_;
  RxSink rx_sink_{nullptr};
  void* rx_ctx_{nullptr};
  TxDoneSink tx_sink_{nullptr};
  void* tx_ctx_{nullptr};

  std::atomic<uint8_t> state_{static_cast<uint8_t>(BusState::Stopped)};
  std::atomic<uint16_t> tec_{0};
  std::atomic<uint16_t> rec_{0};
  std::atomic<uint32_t> rx_overruns_{0};
  std::atomic<uint32_t> tx_queue_full_{0};
  std::atomic<uint32_t> bus_off_count_{0};
};

}  // namespace hf::can
