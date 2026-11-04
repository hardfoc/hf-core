#include "At25040Handler.h"
#include "MutexGuard.h"

void At25040Handler::ReleaseWriteProtect() noexcept {
  if (wp_ == nullptr) {
    return;
  }
  (void)wp_->EnsureInitialized();
  (void)wp_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
  (void)wp_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
  (void)wp_->SetInactive();
}

At25040Handler::ProbeResult At25040Handler::Probe() noexcept {
  MutexLockGuard lock(mutex_);
  ReleaseWriteProtect();
  const at25040::Status st = device_.Probe();
  last_status_ = st;
  if (st == at25040::Status::Ok) {
    return ProbeResult::Present;
  }
  if (st == at25040::Status::BusError) {
    return ProbeResult::BusError;  // the port failed: not evidence that no chip is fitted
  }
  return ProbeResult::Absent;
}

bool At25040Handler::Read(uint16_t addr, uint8_t* dst, uint16_t len) noexcept {
  MutexLockGuard lock(mutex_);
  if (dst == nullptr) {
    return false;
  }
  ReleaseWriteProtect();
  /* The driver splits the range into READ transactions of
   * Device::kReadChunkBytes and returns Busy during a write cycle. */
  last_status_ = device_.Read(addr, dst, len);
  return last_status_ == at25040::Status::Ok;
}

void At25040Handler::ParkWriteProtect() noexcept {
  if (wp_ == nullptr) {
    return;
  }
  (void)wp_->SetActive();
}

bool At25040Handler::Write(uint16_t addr, const uint8_t* src,
                           uint16_t len) noexcept {
  MutexLockGuard lock(mutex_);
  if (src == nullptr || len == 0U) {
    return false;
  }
  ReleaseWriteProtect();
  /* The driver splits the range at 8-byte page boundaries and waits for the
   * write cycle (tWC, RDSR polls with the adapter's delay_us()) after each
   * page, so a following call is never ignored by the chip. */
  last_status_ = device_.Write(addr, src, len);
  const bool ok = last_status_ == at25040::Status::Ok;
  ParkWriteProtect();
  return ok;
}
