/**
 * @file test_pipe_loopback.cpp
 * @brief 10^5 frames from one CANopenNode CANmodule to another through the
 *        pipe bus (PipeHub + two PipePorts + CO_driver_hardfoc.c), with no
 *        loss and in order. Built and run under ThreadSanitizer in CI
 *        (HF_CORE_CANOPEN_SANITIZE=thread) to catch races and lock-order
 *        inversions between the CO lock, the port queues and the I/O threads.
 *
 * Threads: sender (CO_CANsend + CO_CANmodule_process on module A), receiver
 * (CO_CANmodule_process on module B, woken by the driver's wake hook), a
 * third thread that keeps taking module A's CO lock and reading statistics,
 * plus the hub thread and the two port I/O threads.
 */

#include <unistd.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <string>
#include <thread>

#include "301/CO_driver.h"
#include "HfCoPortBinding.h"
#include "HostTest.h"
#include "PipePort.h"

using hf::can::CoPortBinding;
using hf::can::PipeHub;
using hf::can::PipePort;

namespace {

constexpr uint32_t kFrames = 100000U;
constexpr uint16_t kCobId = 0x181U;

struct Receiver {
  std::atomic<uint32_t> received{0};
  std::atomic<uint32_t> out_of_order{0};
  uint32_t expected{0};  // receiver thread only

  static void OnFrame(void* object, void* message) {
    auto* self = static_cast<Receiver*>(object);
    const uint8_t* d = CO_CANrxMsg_readData(message);
    const uint32_t seq = static_cast<uint32_t>(d[0]) | (static_cast<uint32_t>(d[1]) << 8U) |
                         (static_cast<uint32_t>(d[2]) << 16U) | (static_cast<uint32_t>(d[3]) << 24U);
    const bool tail_ok = d[4] == 0xA5U && d[7] == static_cast<uint8_t>(seq) && CO_CANrxMsg_readDLC(message) == 8U;
    if (seq != self->expected || !tail_ok) {
      self->out_of_order.fetch_add(1);
    }
    self->expected = seq + 1U;
    self->received.fetch_add(1, std::memory_order_release);
  }
};

struct Waker {
  std::mutex mu;
  std::condition_variable cv;
  bool flag{false};
  static void Wake(void* ctx) {
    auto* self = static_cast<Waker*>(ctx);
    {
      std::lock_guard<std::mutex> lk(self->mu);
      self->flag = true;
    }
    self->cv.notify_one();
  }
  void WaitFor(std::chrono::microseconds t) {
    std::unique_lock<std::mutex> lk(mu);
    cv.wait_for(lk, t, [this] { return flag; });
    flag = false;
  }
};

std::string SocketPath() {
  char buf[96];
  std::snprintf(buf, sizeof(buf), "/tmp/hf_co_loop_%d.sock", static_cast<int>(getpid()));
  return buf;
}

}  // namespace

HF_TEST(PipeLoopback_100kFramesNoLossInOrder) {
  const std::string path = SocketPath();
  PipeHub hub;
  REQUIRE(hub.Start(path));

  PipePort::Options oa;
  oa.path = path;
  oa.host_hub_if_absent = false;
  PipePort port_a(oa);
  PipePort port_b(oa);
  REQUIRE(port_a.Open());
  REQUIRE(port_b.Open());

  hf_co_driver_t* drv_a = hf_co_driver_acquire();
  hf_co_driver_t* drv_b = hf_co_driver_acquire();
  REQUIRE(drv_a != nullptr && drv_b != nullptr);
  CoPortBinding bind_a;
  CoPortBinding bind_b;
  REQUIRE(bind_a.Attach(drv_a, 3, port_a, false));
  REQUIRE(bind_b.Attach(drv_b, 3, port_b, false));

  static CO_CANmodule_t mod_a{};
  static CO_CANmodule_t mod_b{};
  static CO_CANrx_t rx_a[1]{};
  static CO_CANtx_t tx_a[1]{};
  static CO_CANrx_t rx_b[1]{};
  static CO_CANtx_t tx_b[1]{};
  REQUIRE(CO_CANmodule_init(&mod_a, drv_a, rx_a, 1, tx_a, 1, 1000) == CO_ERROR_NO);
  REQUIRE(CO_CANmodule_init(&mod_b, drv_b, rx_b, 1, tx_b, 1, 1000) == CO_ERROR_NO);
  Receiver rcv;
  REQUIRE(CO_CANrxBufferInit(&mod_b, 0, kCobId, 0x7FF, false, &rcv, &Receiver::OnFrame) == CO_ERROR_NO);
  CO_CANtx_t* buf = CO_CANtxBufferInit(&mod_a, 0, kCobId, false, 8, false);
  REQUIRE(buf != nullptr);
  CO_CANsetNormalMode(&mod_a);
  CO_CANsetNormalMode(&mod_b);

  Waker wake_a;
  Waker wake_b;
  hf_co_driver_set_wake(drv_a, &Waker::Wake, &wake_a);
  hf_co_driver_set_wake(drv_b, &Waker::Wake, &wake_b);

  // Wait until the hub sees both ports (connections are accepted asynchronously).
  for (int i = 0; i < 500 && hub.Participants() < 2U; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
  REQUIRE(hub.Participants() == 2U);

  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(240);
  std::atomic<bool> stop{false};
  std::atomic<uint32_t> overflow_refusals{0};

  std::thread receiver([&] {
    while (rcv.received.load(std::memory_order_acquire) < kFrames && std::chrono::steady_clock::now() < deadline) {
      CO_CANmodule_process(&mod_b);
      if (hf_co_driver_rx_pending(drv_b) == 0U) {
        wake_b.WaitFor(std::chrono::microseconds(1000));
      }
    }
  });

  std::thread sender([&] {
    for (uint32_t seq = 0; seq < kFrames && std::chrono::steady_clock::now() < deadline; ++seq) {
      for (;;) {
        CO_CANmodule_process(&mod_a);
        CO_LOCK_CAN_SEND(&mod_a);
        const bool busy = buf->bufferFull;
        if (!busy) {
          buf->data[0] = static_cast<uint8_t>(seq);
          buf->data[1] = static_cast<uint8_t>(seq >> 8U);
          buf->data[2] = static_cast<uint8_t>(seq >> 16U);
          buf->data[3] = static_cast<uint8_t>(seq >> 24U);
          buf->data[4] = 0xA5U;
          buf->data[7] = static_cast<uint8_t>(seq);
        }
        CO_UNLOCK_CAN_SEND(&mod_a);
        if (!busy) {
          break;
        }
        wake_a.WaitFor(std::chrono::microseconds(1000));
      }
      if (CO_CANsend(&mod_a, buf) == CO_ERROR_TX_OVERFLOW) {
        overflow_refusals.fetch_add(1);
      }
    }
    while (hf_co_driver_tx_pending(drv_a) != 0U && std::chrono::steady_clock::now() < deadline) {
      CO_CANmodule_process(&mod_a);
      wake_a.WaitFor(std::chrono::microseconds(1000));
    }
  });

  // Lock and statistics traffic from a third thread, as an application thread would.
  std::thread observer([&] {
    while (!stop.load()) {
      CO_LOCK_OD(&mod_a);
      (void)mod_a.CANerrorStatus;
      CO_UNLOCK_OD(&mod_a);
      hf_co_port_stats_t s;
      hf_co_driver_get_stats(drv_b, 3, &s);
      (void)hf_co_driver_rx_pending(drv_b);
      std::this_thread::sleep_for(std::chrono::microseconds(200));
    }
  });

  sender.join();
  receiver.join();
  stop.store(true);
  observer.join();

  hf_co_port_stats_t sa;
  hf_co_port_stats_t sb;
  hf_co_driver_get_stats(drv_a, 3, &sa);
  hf_co_driver_get_stats(drv_b, 3, &sb);
  std::printf("  sent %u (queue-full %u, retried %u), received %u, B ring overruns %u, hub datagrams %llu\n",
              sa.tx_frames, sa.tx_queue_full, sa.tx_retried, rcv.received.load(), sb.rx_ring_overruns,
              static_cast<unsigned long long>(hub.Forwarded()));

  CHECK_EQ(rcv.received.load(), kFrames);
  CHECK_EQ(rcv.out_of_order.load(), 0U);
  CHECK_EQ(overflow_refusals.load(), 0U);
  CHECK_EQ(sa.tx_frames, kFrames);
  CHECK_EQ(sa.tx_dropped, 0U);
  CHECK_EQ(sb.rx_ring_overruns, 0U);
  CHECK_EQ(sb.rx_dispatched, kFrames);
  CHECK_EQ(sb.rx_unhandled, 0U);

  bind_a.Detach();
  bind_b.Detach();
  port_a.Close();
  port_b.Close();
  hub.Stop();
  hf_co_driver_release(drv_a);
  hf_co_driver_release(drv_b);
}

HF_TEST(PipePort_HostsAHubWhenNoneAnswers) {
  char buf[96];
  std::snprintf(buf, sizeof(buf), "/tmp/hf_co_host_%d.sock", static_cast<int>(getpid()));
  PipePort::Options o;
  o.path = buf;
  PipePort a(o);
  REQUIRE(a.Open());
  CHECK(a.HostsHub());
  o.host_hub_if_absent = false;
  PipePort b(o);
  REQUIRE(b.Open());
  CHECK(!b.HostsHub());

  static std::atomic<int> got{0};
  got = 0;
  b.SetRxSink(
      [](void*, const hf::can::CanFrame& f) {
        if (f.id == 0x705U && f.len == 1U && f.data[0] == 0x7FU) {
          got.fetch_add(1);
        }
        return true;
      },
      nullptr);
  hf::can::CanFrame f;
  f.id = 0x705;
  f.len = 1;
  f.data[0] = 0x7F;
  // The hub may not have accepted b yet: resend until it arrives.
  for (int i = 0; i < 200 && got.load() == 0; ++i) {
    CHECK(a.Send(f) == hf::can::TxResult::Ok);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  CHECK(got.load() > 0);
  b.SetRxSink(nullptr, nullptr);
  b.Close();
  a.Close();
}
