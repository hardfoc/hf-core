/**
 * @file LinuxSocketCanPort.cpp
 * @brief ICanPort over Linux SocketCAN.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "LinuxSocketCanPort.h"

#if defined(__linux__)

#include <cerrno>
#include <chrono>
#include <cstring>

#include <fcntl.h>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

namespace hf::can {
namespace {

uint32_t NowUs() {
  return static_cast<uint32_t>(
      std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch())
          .count());
}

void CloseFd(int& fd) {
  if (fd >= 0) {
    (void)close(fd);
    fd = -1;
  }
}

}  // namespace

LinuxSocketCanPort::LinuxSocketCanPort(Options options) : opt_(std::move(options)) {}

LinuxSocketCanPort::~LinuxSocketCanPort() { Close(); }

bool LinuxSocketCanPort::Open() {
  if (fd_ >= 0) {
    return true;
  }
  const int fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (fd < 0) {
    return false;
  }
  ifreq ifr{};
  if (opt_.ifname.empty() || opt_.ifname.size() >= sizeof(ifr.ifr_name)) {
    (void)close(fd);
    return false;
  }
  std::memcpy(ifr.ifr_name, opt_.ifname.c_str(), opt_.ifname.size() + 1U);
  if (ioctl(fd, SIOCGIFINDEX, &ifr) != 0) {
    (void)close(fd);
    return false;
  }
  if (opt_.fd) {
    const int on = 1;
    if (setsockopt(fd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &on, sizeof(on)) != 0) {
      (void)close(fd);
      return false;
    }
  }
  can_err_mask_t err_mask = CAN_ERR_CRTL | CAN_ERR_BUSOFF | CAN_ERR_RESTARTED;
#if defined(CAN_ERR_CNT)
  err_mask |= CAN_ERR_CNT;
#endif
  (void)setsockopt(fd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask, sizeof(err_mask));

  sockaddr_can addr{};
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;
  if (bind(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0) {
    (void)close(fd);
    return false;
  }
  const int fl = fcntl(fd, F_GETFL, 0);
  if (fl < 0 || fcntl(fd, F_SETFL, fl | O_NONBLOCK) != 0 || pipe(wake_) != 0) {
    (void)close(fd);
    return false;
  }
  (void)fcntl(wake_[0], F_SETFL, O_NONBLOCK);
  (void)fcntl(wake_[1], F_SETFL, O_NONBLOCK);
  fd_ = fd;
  state_.store(static_cast<uint8_t>(BusState::ErrorActive));
  run_.store(true);
  reader_ = std::thread([this] { ReadLoop(); });
  return true;
}

void LinuxSocketCanPort::Close() {
  if (run_.exchange(false)) {
    const uint8_t b = 1;
    (void)write(wake_[1], &b, 1);
    if (reader_.joinable()) {
      reader_.join();
    }
  }
  CloseFd(fd_);
  CloseFd(wake_[0]);
  CloseFd(wake_[1]);
  state_.store(static_cast<uint8_t>(BusState::Stopped));
}

PortCaps LinuxSocketCanPort::Caps() const {
  PortCaps c;
  c.fd = opt_.fd;
  c.max_len = opt_.fd ? kFdMaxLen : kClassicMaxLen;
  c.nominal_bps = opt_.nominal_bps;
  c.data_bps = 0;
  c.hw_filters = 0;
  return c;
}

TxResult LinuxSocketCanPort::Send(const CanFrame& frame) {
  if (fd_ < 0) {
    return TxResult::BusOff;
  }
  if (!IsWellFormed(frame) || (frame.IsFd() && !opt_.fd)) {
    return TxResult::NotAllowed;
  }
  canid_t id = frame.id;
  if (frame.IsExtended()) id |= CAN_EFF_FLAG;
  if (frame.IsRemote()) id |= CAN_RTR_FLAG;
  ssize_t n;
  if (frame.IsFd()) {
    canfd_frame f{};
    f.can_id = id;
    f.len = frame.len;
    f.flags = static_cast<uint8_t>(((frame.flags & frame_flag::kBrs) != 0U ? CANFD_BRS : 0) |
                                   ((frame.flags & frame_flag::kEsi) != 0U ? CANFD_ESI : 0));
    std::memcpy(f.data, frame.data, frame.len);
    n = write(fd_, &f, sizeof(f));
  } else {
    can_frame f{};
    f.can_id = id;
    f.can_dlc = frame.len;
    std::memcpy(f.data, frame.data, frame.len);
    n = write(fd_, &f, sizeof(f));
  }
  if (n > 0) {
    return TxResult::Ok;
  }
  if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENOBUFS) {
    tx_queue_full_.fetch_add(1, std::memory_order_relaxed);
    want_writable_.store(true, std::memory_order_release);
    const uint8_t b = 1;
    (void)write(wake_[1], &b, 1);
    return TxResult::QueueFull;
  }
  if (errno == ENETDOWN) {
    return TxResult::BusOff;
  }
  return TxResult::NotAllowed;
}

void LinuxSocketCanPort::SetRxSink(RxSink sink, void* ctx) {
  std::lock_guard<std::mutex> lk(sink_mu_);
  rx_sink_ = sink;
  rx_ctx_ = ctx;
}

void LinuxSocketCanPort::SetTxDoneSink(TxDoneSink sink, void* ctx) {
  std::lock_guard<std::mutex> lk(sink_mu_);
  tx_sink_ = sink;
  tx_ctx_ = ctx;
}

PortStatus LinuxSocketCanPort::Status() const {
  PortStatus s;
  s.state = static_cast<BusState>(state_.load());
  s.tec = tec_.load();
  s.rec = rec_.load();
  s.rx_overruns = rx_overruns_.load();
  s.tx_queue_full = tx_queue_full_.load();
  s.bus_off_count = bus_off_count_.load();
  return s;
}

bool LinuxSocketCanPort::Recover() {
  // The kernel restarts the controller (restart-ms); report whether it did.
  return static_cast<BusState>(state_.load()) != BusState::BusOff;
}

void LinuxSocketCanPort::HandleErrorFrame(uint32_t can_id, const uint8_t* data, uint8_t len) {
  if ((can_id & CAN_ERR_BUSOFF) != 0U) {
    if (state_.exchange(static_cast<uint8_t>(BusState::BusOff)) != static_cast<uint8_t>(BusState::BusOff)) {
      bus_off_count_.fetch_add(1);
    }
    return;
  }
  if ((can_id & CAN_ERR_RESTARTED) != 0U) {
    state_.store(static_cast<uint8_t>(BusState::ErrorActive));
  }
#if defined(CAN_ERR_CNT)
  if ((can_id & CAN_ERR_CNT) != 0U && len >= 8U) {
    tec_.store(data[6]);
    rec_.store(data[7]);
  }
#endif
  if ((can_id & CAN_ERR_CRTL) != 0U && len >= 2U) {
    const uint8_t c = data[1];
    if ((c & CAN_ERR_CRTL_RX_OVERFLOW) != 0U) {
      rx_overruns_.fetch_add(1);
    }
    if ((c & (CAN_ERR_CRTL_RX_PASSIVE | CAN_ERR_CRTL_TX_PASSIVE)) != 0U) {
      state_.store(static_cast<uint8_t>(BusState::ErrorPassive));
    } else if ((c & (CAN_ERR_CRTL_RX_WARNING | CAN_ERR_CRTL_TX_WARNING)) != 0U) {
      state_.store(static_cast<uint8_t>(BusState::ErrorWarning));
    }
#if defined(CAN_ERR_CRTL_ACTIVE)
    if ((c & CAN_ERR_CRTL_ACTIVE) != 0U) {
      state_.store(static_cast<uint8_t>(BusState::ErrorActive));
    }
#endif
  }
}

void LinuxSocketCanPort::ReadLoop() {
  while (run_.load()) {
    const bool want_out = want_writable_.load(std::memory_order_acquire);
    pollfd pfd[2] = {{fd_, static_cast<short>(POLLIN | (want_out ? POLLOUT : 0)), 0}, {wake_[0], POLLIN, 0}};
    if (poll(pfd, 2, 100) < 0) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }
    if ((pfd[1].revents & POLLIN) != 0) {
      uint8_t buf[64];
      while (read(wake_[0], buf, sizeof(buf)) > 0) {
      }
    }
    if ((pfd[0].revents & POLLOUT) != 0 && want_writable_.exchange(false)) {
      std::lock_guard<std::mutex> lk(sink_mu_);
      if (tx_sink_ != nullptr) {
        tx_sink_(tx_ctx_);
      }
    }
    if ((pfd[0].revents & POLLIN) == 0) {
      continue;
    }
    for (;;) {
      canfd_frame raw{};
      const ssize_t n = read(fd_, &raw, sizeof(raw));
      if (n <= 0) {
        break;
      }
      const bool fd = (n == static_cast<ssize_t>(CANFD_MTU));
      if ((raw.can_id & CAN_ERR_FLAG) != 0U) {
        HandleErrorFrame(raw.can_id & CAN_ERR_MASK, raw.data, raw.len);
        continue;
      }
      CanFrame f;
      f.ts_us = NowUs();
      if ((raw.can_id & CAN_EFF_FLAG) != 0U) {
        f.flags |= frame_flag::kExt;
        f.id = raw.can_id & CAN_EFF_MASK;
      } else {
        f.id = raw.can_id & CAN_SFF_MASK;
      }
      if (!fd && (raw.can_id & CAN_RTR_FLAG) != 0U) {
        f.flags |= frame_flag::kRtr;
      }
      if (fd) {
        f.flags |= frame_flag::kFdf;
        if ((raw.flags & CANFD_BRS) != 0U) f.flags |= frame_flag::kBrs;
        if ((raw.flags & CANFD_ESI) != 0U) f.flags |= frame_flag::kEsi;
      }
      f.len = raw.len > kFdMaxLen ? kFdMaxLen : raw.len;
      if (!fd && f.len > kClassicMaxLen) {
        f.len = kClassicMaxLen;
      }
      std::memcpy(f.data, raw.data, f.len);
      std::lock_guard<std::mutex> lk(sink_mu_);
      if (rx_sink_ != nullptr) {
        (void)rx_sink_(rx_ctx_, f);  // a refusal is an overrun (counted by the driver)
      }
    }
  }
}

}  // namespace hf::can

#endif  // __linux__
