/**
 * @file Pf1550Handler.cpp
 * @brief PF1550 PMIC handler implementation.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "Pf1550Handler.h"

#include <cstring>

#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

namespace {

void driveGpio(BaseGpio* pin, pf1550::GpioSignal signal) {
    if (pin == nullptr) {
        return;
    }
    if (signal == pf1550::GpioSignal::Active) {
        (void)pin->SetActive();
    } else {
        (void)pin->SetInactive();
    }
}

}  // namespace

HalPf1550Comm::HalPf1550Comm(BaseI2c& i2c, BaseGpio* standby_gpio, BaseGpio* usb_vbus_en_gpio,
                               BaseGpio* usb_otg_en_gpio) noexcept
    : i2c_(i2c),
      standby_gpio_(standby_gpio),
      usb_vbus_en_gpio_(usb_vbus_en_gpio),
      usb_otg_en_gpio_(usb_otg_en_gpio) {}

bool HalPf1550Comm::Write(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len) noexcept {
    MutexLockGuard lock(i2c_mutex_);
    if (addr != i2c_.GetDeviceAddress()) {
        return false;
    }
    uint8_t command[8];
    if (len + 1 > sizeof(command)) {
        return false;
    }
    command[0] = reg;
    if (len > 0 && data != nullptr) {
        std::memcpy(&command[1], data, len);
    }
    return i2c_.Write(command, static_cast<hf_u16_t>(len + 1)) == hf_i2c_err_t::I2C_SUCCESS;
}

bool HalPf1550Comm::Read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len) noexcept {
    MutexLockGuard lock(i2c_mutex_);
    if (addr != i2c_.GetDeviceAddress()) {
        return false;
    }
    return i2c_.WriteRead(&reg, 1, data, static_cast<hf_u16_t>(len)) == hf_i2c_err_t::I2C_SUCCESS;
}

bool HalPf1550Comm::EnsureInitialized() noexcept { return i2c_.EnsureInitialized(); }

void HalPf1550Comm::GpioSet(pf1550::CtrlPin pin, pf1550::GpioSignal signal) noexcept {
    switch (pin) {
    case pf1550::CtrlPin::Standby:
        driveGpio(standby_gpio_, signal);
        break;
    case pf1550::CtrlPin::UsbVbusEn:
        driveGpio(usb_vbus_en_gpio_, signal);
        break;
    case pf1550::CtrlPin::UsbOtgEn:
        driveGpio(usb_otg_en_gpio_, signal);
        break;
    default:
        break;
    }
}

void HalPf1550Comm::DelayUs(uint32_t us) noexcept {
    if (us == 0U) {
        return;
    }
    os_delay_msec(static_cast<uint16_t>((us + 999U) / 1000U));
}

Pf1550Handler::Pf1550Handler(BaseI2c& i2c, BaseGpio* standby_gpio, BaseGpio* usb_vbus_en_gpio,
                               BaseGpio* usb_otg_en_gpio) noexcept
    : i2c_(i2c),
      standby_gpio_(standby_gpio),
      usb_vbus_en_gpio_(usb_vbus_en_gpio),
      usb_otg_en_gpio_(usb_otg_en_gpio),
      comm_(nullptr),
      driver_(nullptr),
      initialized_(false),
      cached_snapshot_() {}

bool Pf1550Handler::ensureInitializedLocked() noexcept {
    if (initialized_) {
        return true;
    }
    if (!comm_) {
        comm_ = std::make_unique<HalPf1550Comm>(i2c_, standby_gpio_, usb_vbus_en_gpio_,
                                                 usb_otg_en_gpio_);
    }
    if (!driver_) {
        driver_ = std::make_unique<Pf1550Driver>(comm_.get(), pf1550::kDefaultI2cAddress);
    }
    if (!driver_->EnsureInitialized()) {
        return false;
    }
    initialized_ = true;
    return true;
}

bool Pf1550Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    return ensureInitializedLocked();
}

bool Pf1550Handler::ApplyPortentaH7Profile() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->ApplyPortentaH7DefaultProfile();
}

bool Pf1550Handler::ApplyPortentaH7CarrierProfile() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->ApplyPortentaH7CarrierProfile();
}

bool Pf1550Handler::SetPowerMode(pf1550::PowerMode mode) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->SetPowerMode(mode);
}

bool Pf1550Handler::SetUsbRails(bool vbus_en, bool otg_en) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->SetUsbRails(vbus_en, otg_en);
}

bool Pf1550Handler::ReadPmicStatus(uint8_t& status) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->ReadPmicStatus(status);
}

bool Pf1550Handler::RefreshDiagnosticSnapshot() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->ReadDiagnosticSnapshot(cached_snapshot_);
}

bool Pf1550Handler::ReadDiagnosticSnapshot(pf1550::DiagnosticSnapshot& out) noexcept {
    MutexLockGuard lock(handler_mutex_);
    out = cached_snapshot_;
    return cached_snapshot_.read_ok;
}

bool Pf1550Handler::RunPowerSelfTest(pf1550::SelfTestResult& out) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        out = pf1550::SelfTestResult{};
        out.ran = false;
        return false;
    }
    const bool ok = driver_->RunPowerSelfTest(out);
    cached_snapshot_ = out.snapshot;
    return ok;
}

bool Pf1550Handler::ClearLatchedFaults() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!ensureInitializedLocked() || driver_ == nullptr) {
        return false;
    }
    return driver_->ClearLatchedFaults();
}

bool Pf1550Handler::HasMcuAffectingFault() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!cached_snapshot_.read_ok) {
        return false;
    }
    const auto sev = pf1550::WorstSeverityPortentaH7(cached_snapshot_.faults);
    return static_cast<uint8_t>(sev) >=
           static_cast<uint8_t>(pf1550::FaultSeverity::kCritical);
}

Pf1550Handler::Pf1550Driver* Pf1550Handler::GetDriver() noexcept { return driver_.get(); }

const Pf1550Handler::Pf1550Driver* Pf1550Handler::GetDriver() const noexcept {
    return driver_.get();
}
