/**
 * @file PipePort.cpp
 * @brief Host pipe bus (PipeHub) and its ICanPort (PipePort). POSIX only.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "PipePort.h"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <deque>

#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "HfCanFrameRecord.h"

namespace hf::can {
namespace {

#if defined(MSG_NOSIGNAL)
constexpr int kSendFlags = MSG_NOSIGNAL;
#else
constexpr int kSendFlags = 0;
#endif

/** Payload bytes of records per datagram the port emits. */
constexpr std::size_t kDatagramRecordBytes = 1024U;
/** Bytes the port's I/O thread keeps encoded ahead of the socket. */
constexpr std::size_t kPortOutHighWater = std::size_t{64} * 1024U;

bool SetNonBlocking(int fd) {
  const int fl = fcntl(fd, F_GETFL, 0);
  return fl >= 0 && fcntl(fd, F_SETFL, fl | O_NONBLOCK) == 0;
}

void NoSigPipe(int fd) {
#if defined(SO_NOSIGPIPE)
  int one = 1;
  (void)setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#else
  (void)fd;
#endif
}

bool MakeAddr(const std::string& path, sockaddr_un& addr) {
  std::memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (path.empty() || path.size() >= sizeof(addr.sun_path)) {
    return false;
  }
  std::memcpy(addr.sun_path, path.c_str(), path.size() + 1U);
  return true;
}

bool MakeWakePipe(int (&fds)[2]) {
  if (pipe(fds) != 0) {
    fds[0] = fds[1] = -1;
    return false;
  }
  (void)SetNonBlocking(fds[0]);
  (void)SetNonBlocking(fds[1]);
  return true;
}

/** Write one wake byte; a full pipe already means "woken", so the result is not needed. */
void PokeFd(int fd) {
  const uint8_t b = 1;
  if (write(fd, &b, 1) < 0) {
    // EAGAIN: a wake is already pending.
  }
}

void CloseFd(int& fd) {
  if (fd >= 0) {
    (void)close(fd);
    fd = -1;
  }
}

void DrainFd(int fd) {
  uint8_t buf[64];
  while (read(fd, buf, sizeof(buf)) > 0) {
  }
}

/** Write as much of @p buf (from @p off) as the socket takes; false on a hard error. */
bool Flush(int fd, std::vector<uint8_t>& buf, std::size_t& off) {
  while (off < buf.size()) {
    const ssize_t n = send(fd, buf.data() + off, buf.size() - off, kSendFlags);
    if (n > 0) {
      off += static_cast<std::size_t>(n);
      continue;
    }
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      break;
    }
    if (n < 0 && errno == EINTR) {
      continue;
    }
    return false;
  }
  if (off == buf.size()) {
    buf.clear();
    off = 0;
  } else if (off > kPortOutHighWater) {
    buf.erase(buf.begin(), buf.begin() + static_cast<std::ptrdiff_t>(off));
    off = 0;
  }
  return true;
}

/** Read what is available into @p in; false on EOF or a hard error. */
bool Fill(int fd, std::vector<uint8_t>& in) {
  uint8_t buf[16384];
  for (;;) {
    const ssize_t n = recv(fd, buf, sizeof(buf), 0);
    if (n > 0) {
      in.insert(in.end(), buf, buf + n);
      if (static_cast<std::size_t>(n) < sizeof(buf)) {
        return true;
      }
      continue;
    }
    if (n == 0) {
      return false;
    }
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return true;
    }
    if (errno != EINTR) {
      return false;
    }
  }
}

}  // namespace

// ───────────────────────────────────────────────────────────────────────────
// PipeHub
// ───────────────────────────────────────────────────────────────────────────

struct PipeHub::Client {
  int fd{-1};
  std::vector<uint8_t> in;
  std::vector<uint8_t> out;
  std::size_t out_off{0};
  std::size_t Backlog() const { return out.size() - out_off; }
};

PipeHub::~PipeHub() { Stop(); }

bool PipeHub::Start(const std::string& path) {
  if (running_.load()) {
    return false;
  }
  sockaddr_un addr{};
  if (!MakeAddr(path, addr)) {
    return false;
  }
  listen_fd_ = socket(AF_UNIX, SOCK_STREAM, 0);
  if (listen_fd_ < 0) {
    return false;
  }
  (void)unlink(path.c_str());
  if (bind(listen_fd_, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) != 0 || listen(listen_fd_, 16) != 0 ||
      !SetNonBlocking(listen_fd_) || !MakeWakePipe(wake_)) {
    CloseFd(listen_fd_);
    CloseFd(wake_[0]);
    CloseFd(wake_[1]);
    return false;
  }
  path_ = path;
  running_.store(true);
  thread_ = std::thread([this] { Loop(); });
  return true;
}

void PipeHub::Stop() {
  if (!running_.exchange(false)) {
    return;
  }
  PokeFd(wake_[1]);
  if (thread_.joinable()) {
    thread_.join();
  }
  CloseFd(listen_fd_);
  CloseFd(wake_[0]);
  CloseFd(wake_[1]);
  (void)unlink(path_.c_str());
}

void PipeHub::Loop() {
  std::vector<std::unique_ptr<Client>> clients;
  std::vector<pollfd> pfd;
  while (running_.load()) {
    bool backlogged = false;
    for (const auto& c : clients) {
      backlogged = backlogged || c->Backlog() > kMaxBacklog;
    }
    pfd.clear();
    pfd.push_back({wake_[0], POLLIN, 0});
    pfd.push_back({listen_fd_, POLLIN, 0});
    for (const auto& c : clients) {
      short ev = backlogged ? 0 : POLLIN;
      if (c->Backlog() != 0U) {
        ev = static_cast<short>(ev | POLLOUT);
      }
      pfd.push_back({c->fd, ev, 0});
    }
    if (poll(pfd.data(), static_cast<nfds_t>(pfd.size()), 200) < 0) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }
    if ((pfd[0].revents & POLLIN) != 0) {
      DrainFd(wake_[0]);
    }
    if ((pfd[1].revents & POLLIN) != 0) {
      for (;;) {
        const int fd = accept(listen_fd_, nullptr, nullptr);
        if (fd < 0) {
          break;
        }
        NoSigPipe(fd);
        (void)SetNonBlocking(fd);
        auto c = std::make_unique<Client>();
        c->fd = fd;
        clients.push_back(std::move(c));
      }
    }
    std::vector<std::size_t> dead;
    const std::size_t polled = pfd.size() - 2U;
    for (std::size_t i = 0; i < polled && i < clients.size(); ++i) {
      Client& c = *clients[i];
      const short re = pfd[i + 2U].revents;
      bool ok = true;
      if ((re & POLLOUT) != 0) {
        ok = Flush(c.fd, c.out, c.out_off);
      }
      if (ok && (re & (POLLIN | POLLHUP | POLLERR)) != 0) {
        ok = Fill(c.fd, c.in);
        std::size_t off = 0;
        while (c.in.size() - off >= 2U) {
          const std::size_t len = record::GetU16(&c.in[off]);
          if (len < record::kHeaderSize || len > record::kMaxDatagram) {
            ok = false;  // not a frame-record stream
            break;
          }
          if (c.in.size() - off < 2U + len) {
            break;
          }
          for (std::size_t j = 0; j < clients.size(); ++j) {
            if (j == i) {
              continue;
            }
            Client& o = *clients[j];
            o.out.insert(o.out.end(), c.in.begin() + static_cast<std::ptrdiff_t>(off),
                         c.in.begin() + static_cast<std::ptrdiff_t>(off + 2U + len));
            forwarded_.fetch_add(1);
          }
          off += 2U + len;
        }
        c.in.erase(c.in.begin(), c.in.begin() + static_cast<std::ptrdiff_t>(off));
      }
      if (!ok) {
        dead.push_back(i);
      }
    }
    for (auto& c : clients) {
      if (c->Backlog() != 0U && !Flush(c->fd, c->out, c->out_off)) {
        c->out.clear();
        c->out_off = 0;
      }
    }
    for (auto it = dead.rbegin(); it != dead.rend(); ++it) {
      CloseFd(clients[*it]->fd);
      clients.erase(clients.begin() + static_cast<std::ptrdiff_t>(*it));
    }
    participants_.store(clients.size());
  }
  for (auto& c : clients) {
    CloseFd(c->fd);
  }
  participants_.store(0);
}

// ───────────────────────────────────────────────────────────────────────────
// PipePort
// ───────────────────────────────────────────────────────────────────────────

PipePort::PipePort(Options options) : opt_(std::move(options)) {
  if (opt_.tx_queue_frames == 0U) {
    opt_.tx_queue_frames = 1U;
  }
  tx_ring_.resize(opt_.tx_queue_frames);
  epoch_ = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
}

PipePort::~PipePort() { Close(); }

bool PipePort::Connect() {
  sockaddr_un addr{};
  if (!MakeAddr(opt_.path, addr)) {
    return false;
  }
  for (int attempt = 0; attempt < 50; ++attempt) {
    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
      return false;
    }
    if (connect(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == 0) {
      NoSigPipe(fd);
      (void)SetNonBlocking(fd);
      fd_ = fd;
      return true;
    }
    const int err = errno;
    (void)close(fd);
    if ((err == ENOENT || err == ECONNREFUSED) && opt_.host_hub_if_absent && hub_ == nullptr) {
      auto hub = std::make_unique<PipeHub>();
      if (hub->Start(opt_.path)) {
        hub_ = std::move(hub);
        continue;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  return false;
}

bool PipePort::Open() {
  if (run_.load()) {
    return true;
  }
  if (!MakeWakePipe(wake_)) {
    return false;
  }
  if (!Connect()) {
    CloseFd(wake_[0]);
    CloseFd(wake_[1]);
    hub_.reset();
    return false;
  }
  connected_.store(true);
  run_.store(true);
  io_ = std::thread([this] { IoLoop(); });
  return true;
}

void PipePort::Close() {
  if (run_.exchange(false)) {
    Wake();
    if (io_.joinable()) {
      io_.join();
    }
  }
  connected_.store(false);
  CloseFd(fd_);
  CloseFd(wake_[0]);
  CloseFd(wake_[1]);
  if (hub_ != nullptr) {
    hub_->Stop();
    hub_.reset();
  }
}

void PipePort::Wake() const {
  if (wake_[1] >= 0) {
    PokeFd(wake_[1]);
  }
}

PortCaps PipePort::Caps() const {
  PortCaps c;
  c.fd = opt_.fd;
  c.max_len = opt_.fd ? kFdMaxLen : kClassicMaxLen;
  c.nominal_bps = 0;
  c.data_bps = 0;
  c.hw_filters = 0;
  return c;
}

TxResult PipePort::Send(const CanFrame& frame) {
  if (!IsWellFormed(frame) || (frame.IsFd() && !opt_.fd) || frame.len > Caps().max_len) {
    return TxResult::NotAllowed;
  }
  if (!connected_.load(std::memory_order_acquire)) {
    return TxResult::BusOff;
  }
  {
    std::lock_guard<std::mutex> lk(tx_mu_);
    if (tx_count_ == tx_ring_.size()) {
      tx_was_full_ = true;
      tx_queue_full_.fetch_add(1, std::memory_order_relaxed);
      return TxResult::QueueFull;
    }
    tx_ring_[(tx_head_ + tx_count_) % tx_ring_.size()] = frame;
    ++tx_count_;
  }
  Wake();
  return TxResult::Ok;
}

void PipePort::SetRxSink(RxSink sink, void* ctx) {
  std::lock_guard<std::mutex> lk(sink_mu_);
  rx_sink_ = sink;
  rx_ctx_ = ctx;
}

void PipePort::SetTxDoneSink(TxDoneSink sink, void* ctx) {
  std::lock_guard<std::mutex> lk(sink_mu_);
  tx_sink_ = sink;
  tx_ctx_ = ctx;
}

PortStatus PipePort::Status() const {
  PortStatus s;
  s.state = connected_.load() ? BusState::ErrorActive : BusState::Stopped;
  s.tx_queue_full = tx_queue_full_.load(std::memory_order_relaxed);
  return s;
}

bool PipePort::Recover() { return connected_.load(); }

bool PipePort::Deliver(const CanFrame& f) {
  std::lock_guard<std::mutex> lk(sink_mu_);
  return rx_sink_ == nullptr || rx_sink_(rx_ctx_, f);
}

void PipePort::IoLoop() {
  std::vector<uint8_t> out;
  std::size_t out_off = 0;
  std::vector<uint8_t> in;
  std::deque<CanFrame> rx_held;  // decoded, not yet taken by the RX sink
  std::vector<CanFrame> batch;
  batch.reserve(tx_ring_.size());
  uint8_t dgram[2U + record::kHeaderSize + kDatagramRecordBytes + record::kMaxRecordSize];

  while (run_.load()) {
    // 1. Move queued frames into the output buffer, as datagrams.
    bool freed = false;
    if (out.size() - out_off < kPortOutHighWater) {
      std::lock_guard<std::mutex> lk(tx_mu_);
      while (tx_count_ != 0U) {
        batch.push_back(tx_ring_[tx_head_]);
        tx_head_ = (tx_head_ + 1U) % tx_ring_.size();
        --tx_count_;
      }
      if (!batch.empty() && tx_was_full_) {
        tx_was_full_ = false;
        freed = true;
      }
    }
    std::size_t i = 0;
    while (i < batch.size()) {
      record::Header h;
      h.seq = seq_++;
      h.epoch = epoch_;
      std::size_t n = 2U + record::EncodeHeader(&dgram[2], h);
      while (i < batch.size() && n - 2U - record::kHeaderSize < kDatagramRecordBytes) {
        const std::size_t w = record::EncodeRecord(&dgram[n], sizeof(dgram) - n, batch[i]);
        if (w == 0U) {
          break;
        }
        n += w;
        ++i;
      }
      record::PutU16(&dgram[0], static_cast<uint16_t>(n - 2U));
      out.insert(out.end(), dgram, dgram + n);
    }
    batch.clear();
    if (freed) {
      std::lock_guard<std::mutex> lk(sink_mu_);
      if (tx_sink_ != nullptr) {
        tx_sink_(tx_ctx_);
      }
    }

    // 2. Hand held frames to the sink; while it refuses, stop reading.
    while (!rx_held.empty() && Deliver(rx_held.front())) {
      rx_held.pop_front();
    }
    const bool read_more = rx_held.empty();

    // 3. Write and read.
    if (!out.empty() && !Flush(fd_, out, out_off)) {
      break;
    }
    pollfd pfd[2] = {{fd_, static_cast<short>((read_more ? POLLIN : 0) | (out.empty() ? 0 : POLLOUT)), 0},
                     {wake_[0], POLLIN, 0}};
    if (poll(pfd, 2, read_more ? 100 : 1) < 0) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }
    if ((pfd[1].revents & POLLIN) != 0) {
      DrainFd(wake_[0]);
    }
    if ((pfd[0].revents & POLLOUT) != 0 && !Flush(fd_, out, out_off)) {
      break;
    }
    if ((pfd[0].revents & (POLLIN | POLLHUP | POLLERR)) != 0) {
      const bool alive = Fill(fd_, in);
      std::size_t off = 0;
      while (in.size() - off >= 2U) {
        const std::size_t len = record::GetU16(&in[off]);
        if (in.size() - off < 2U + len) {
          break;
        }
        (void)record::DecodeDatagram(&in[off + 2U], len, nullptr,
                                     [&rx_held](const CanFrame& f) { rx_held.push_back(f); });
        off += 2U + len;
      }
      in.erase(in.begin(), in.begin() + static_cast<std::ptrdiff_t>(off));
      while (!rx_held.empty() && Deliver(rx_held.front())) {
        rx_held.pop_front();
      }
      if (!alive) {
        break;
      }
    }
  }
  connected_.store(false, std::memory_order_release);
}

}  // namespace hf::can
