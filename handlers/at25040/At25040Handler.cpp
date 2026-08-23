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
  if (st == at25040::Status::Ok) {
    return ProbeResult::Present;
  }
  return ProbeResult::Absent;
}

bool At25040Handler::Read(uint16_t addr, uint8_t* dst, uint16_t len) noexcept {
  MutexLockGuard lock(mutex_);
  ReleaseWriteProtect();
  /* Driver Read() caps at 16 B. Loop so AID1 (64 B) is one product call. */
  uint16_t off = 0;
  while (off < len) {
    const uint16_t chunk =
        static_cast<uint16_t>((len - off) > 16U ? 16U : (len - off));
    if (device_.Read(static_cast<uint16_t>(addr + off), dst + off, chunk) !=
        at25040::Status::Ok) {
      return false;
    }
    off = static_cast<uint16_t>(off + chunk);
  }
  return true;
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
  uint16_t off = 0;
  while (off < len) {
    const uint16_t page_off = static_cast<uint16_t>((addr + off) % 8U);
    const uint16_t room = static_cast<uint16_t>(8U - page_off);
    const uint16_t chunk =
        static_cast<uint16_t>((len - off) > room ? room : (len - off));
    if (device_.WritePage(static_cast<uint16_t>(addr + off), src + off,
                          chunk) != at25040::Status::Ok) {
      return false;
    }
    /* tWC typical 5 ms. Poll WIP via Probe/ReadStatus. */
    bool ready = false;
    for (int i = 0; i < 20; ++i) {
      uint8_t sr = 0xFF;
      if (device_.ReadStatus(sr) == at25040::Status::Ok && (sr & 0x01U) == 0U) {
        ready = true;
        break;
      }
    }
    if (!ready) {
      return false;
    }
    off = static_cast<uint16_t>(off + chunk);
  }
  ParkWriteProtect();
  return true;
}
