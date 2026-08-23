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
  return device_.Read(addr, dst, len) == at25040::Status::Ok;
}
