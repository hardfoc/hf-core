/**
 * @file PipePort.h
 * @brief Host pipe bus: a UNIX-socket hub that behaves like a CAN bus, and
 *        the ICanPort that joins it (POSIX hosts only).
 *
 * @details CI runners cannot always load `vcan`, so host tests can run the
 *          node over this bus instead (Vortex CANopen design §9.4). Every
 *          participant connects to the hub's UNIX stream socket; the hub
 *          forwards each datagram to every other participant, never back to
 *          the sender (like a CAN controller without self-reception). Frames
 *          travel as §7.2 frame records (HfCanFrameRecord.h), each datagram
 *          behind a u16 length, so the python-can plugin and the Wi-Fi tunnel
 *          share the codec.
 *
 *          The hub never drops: it buffers per participant and stops reading
 *          a sender while a receiver is behind (bounded by
 *          `PipeHub::kMaxBacklog`), so a full bus applies back-pressure
 *          instead of losing frames.
 *
 *          `PipePort::Send` queues the frame (bounded queue, QueueFull when
 *          full) and returns; an I/O thread batches queued frames into
 *          datagrams, writes them, reads incoming datagrams and calls the RX
 *          sink. After freeing queue space it calls the TX-done sink. When
 *          the RX sink refuses a frame (the CO driver's ring is full) the
 *          port holds it, stops reading the socket and offers it again a
 *          millisecond later, so back-pressure reaches the sender.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "HfCanPort.h"

namespace hf::can {

/** @brief The bus: forwards datagrams between participants. */
class PipeHub {
 public:
  /** Bytes buffered for one slow participant before the hub stops reading senders. */
  static constexpr std::size_t kMaxBacklog = 1U << 20U;

  PipeHub() = default;
  ~PipeHub();
  PipeHub(const PipeHub&) = delete;
  PipeHub& operator=(const PipeHub&) = delete;

  /** @brief Listen at @p path (an existing socket file is replaced) and start the hub thread. */
  bool Start(const std::string& path);
  /** @brief Stop the thread, close every connection and remove the socket file. */
  void Stop();
  bool Running() const noexcept { return running_.load(); }
  /** @brief Datagrams forwarded so far (each counted once per receiver). */
  uint64_t Forwarded() const noexcept { return forwarded_.load(); }
  std::size_t Participants() const noexcept { return participants_.load(); }

 private:
  struct Client;
  void Loop();

  std::string path_;
  int listen_fd_{-1};
  int wake_[2]{-1, -1};
  std::thread thread_;
  std::atomic<bool> running_{false};
  std::atomic<uint64_t> forwarded_{0};
  std::atomic<std::size_t> participants_{0};
};

/** @brief ICanPort over a PipeHub. */
class PipePort final : public ICanPort {
 public:
  struct Options {
    std::string path;               ///< hub socket path
    bool host_hub_if_absent{true};  ///< start an in-process hub when none answers at @c path
    std::size_t tx_queue_frames{256};
    bool fd{false};                 ///< advertise FD frames (64-byte payloads)
  };

  explicit PipePort(Options options);
  ~PipePort() override;
  PipePort(const PipePort&) = delete;
  PipePort& operator=(const PipePort&) = delete;

  /** @brief Connect (starting a hub if allowed and needed) and start the I/O thread. */
  bool Open();
  /** @brief Stop the I/O thread and disconnect (and stop a hub this port started). */
  void Close();
  bool IsOpen() const noexcept { return connected_.load(); }
  /** @brief True when this port started the hub it uses. */
  bool HostsHub() const noexcept { return hub_ != nullptr; }

  PortCaps Caps() const override;
  TxResult Send(const CanFrame& frame) override;
  void SetRxSink(RxSink sink, void* ctx) override;
  PortStatus Status() const override;
  bool Recover() override;
  void SetTxDoneSink(TxDoneSink sink, void* ctx) override;
  /** @brief The pipe bus never drops: a refused frame is held and offered again. */
  bool RxBackpressure() const override { return true; }

 private:
  bool Connect();
  void IoLoop();
  void Wake() const;
  bool Deliver(const CanFrame& f);

  Options opt_;
  std::unique_ptr<PipeHub> hub_;
  int fd_{-1};
  int wake_[2]{-1, -1};
  std::thread io_;
  std::atomic<bool> run_{false};
  std::atomic<bool> connected_{false};

  // TX queue (Send → I/O thread).
  std::mutex tx_mu_;
  std::vector<CanFrame> tx_ring_;
  std::size_t tx_head_{0};
  std::size_t tx_count_{0};
  bool tx_was_full_{false};

  // Sinks; the mutex keeps a sink alive while it runs (SetRxSink(nullptr) waits).
  std::mutex sink_mu_;
  RxSink rx_sink_{nullptr};
  void* rx_ctx_{nullptr};
  TxDoneSink tx_sink_{nullptr};
  void* tx_ctx_{nullptr};

  std::atomic<uint32_t> tx_queue_full_{0};
  uint16_t seq_{0};
  uint32_t epoch_{0};
};

}  // namespace hf::can
