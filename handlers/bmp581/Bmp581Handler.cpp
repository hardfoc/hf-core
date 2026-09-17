/**
 * @file Bmp581Handler.cpp
 * @brief Implementation of `Bmp581Handler` and its `BaseI2c` CRTP register adapter.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#include "Bmp581Handler.h"

// MCU-agnostic timing primitive shared by every handler in hf-core.
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

namespace {

constexpr hf_u32_t kI2cTimeoutMs = 50;

}  // namespace

//==============================================================================
// HalI2cBmp581Comm
//==============================================================================

HalI2cBmp581Comm::HalI2cBmp581Comm(BaseI2c& i2c_device) noexcept : i2c_device_(i2c_device) {}

bool HalI2cBmp581Comm::WriteRegs(std::uint8_t reg, const std::uint8_t* data,
                                 std::size_t len) noexcept {
    if (data == nullptr || len == 0U || len + 1U > sizeof(tx_scratch_)) {
        return false;
    }
    tx_scratch_[0] = reg;
    for (std::size_t i = 0; i < len; ++i) {
        tx_scratch_[1U + i] = data[i];
    }
    return i2c_device_.Write(tx_scratch_, static_cast<hf_u16_t>(len + 1U), kI2cTimeoutMs) ==
           hf_i2c_err_t::I2C_SUCCESS;
}

bool HalI2cBmp581Comm::ReadRegs(std::uint8_t reg, std::uint8_t* out, std::size_t len) noexcept {
    if (out == nullptr || len == 0U || len > sizeof(rx_scratch_)) {
        return false;
    }
    reg_scratch_ = reg;
    if (i2c_device_.WriteRead(&reg_scratch_, 1, rx_scratch_, static_cast<hf_u16_t>(len),
                              kI2cTimeoutMs) != hf_i2c_err_t::I2C_SUCCESS) {
        return false;
    }
    for (std::size_t i = 0; i < len; ++i) {
        out[i] = rx_scratch_[i];
    }
    return true;
}

void HalI2cBmp581Comm::DelayUs(std::uint32_t us) noexcept {
    if (us == 0U) {
        return;
    }
    /* Sub-millisecond waits (NVM 0.8 ms) round up to one tick — the device
     * only needs a lower bound. */
    const std::uint32_t ms = (us + 999U) / 1000U;
    os_delay_msec(static_cast<std::uint16_t>(ms > 0xFFFFU ? 0xFFFFU : ms));
}

bool HalI2cBmp581Comm::EnsureInitialized() noexcept {
    return i2c_device_.EnsureInitialized();
}

//==============================================================================
// Bmp581Handler
//==============================================================================

Bmp581Handler::Bmp581Handler(BaseI2c& i2c_device, const Bmp581HandlerConfig& config,
                             RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(i2c_device),
      driver_(std::make_unique<DriverType>(comm_)),
      bus_mutex_(bus_mutex != nullptr ? bus_mutex : &private_mutex_) {}

bool Bmp581Handler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool Bmp581Handler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    if (!comm_.EnsureInitialized()) {
        return false;
    }
    auto r = driver_->Init();
    if (!r.ok()) {
        bus_errors_.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    chip_id_ = driver_->ChipId();

    if (config_.soft_reset_on_init) {
        r = driver_->SoftReset();
        if (!r.ok()) {
            bus_errors_.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
    }
    r = driver_->Configure(config_.config);
    if (!r.ok()) {
        bus_errors_.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    const auto eff = driver_->GetEffectiveOsr();
    if (eff.ok()) {
        effective_ = eff.value;
    }
    initialized_.store(true, std::memory_order_release);
    return true;
}

bmp581::DriverResult<bool> Bmp581Handler::DataReady() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!initialized_.load(std::memory_order_acquire)) {
        return bmp581::DriverResult<bool>::failure(bmp581::DriverError::NotInitialized);
    }
    return driver_->DataReady();
}

bmp581::DriverResult<bmp581::Sample> Bmp581Handler::ReadSample() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!initialized_.load(std::memory_order_acquire)) {
        return bmp581::DriverResult<bmp581::Sample>::failure(bmp581::DriverError::NotInitialized);
    }
    const auto s = driver_->ReadSample(config_.config.osr_odr.pressure_enable);
    if (s.ok()) {
        samples_.fetch_add(1, std::memory_order_relaxed);
    } else {
        bus_errors_.fetch_add(1, std::memory_order_relaxed);
    }
    return s;
}

bmp581::DriverResult<void> Bmp581Handler::Configure(const bmp581::Config& cfg) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return bmp581::DriverResult<void>::failure(bmp581::DriverError::NotInitialized);
    }
    const auto r = driver_->Configure(cfg);
    if (r.ok()) {
        config_.config = cfg;
        const auto eff = driver_->GetEffectiveOsr();
        if (eff.ok()) {
            effective_ = eff.value;
        }
    }
    return r;
}

bmp581::DriverResult<void> Bmp581Handler::Reset() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return bmp581::DriverResult<void>::failure(bmp581::DriverError::NotInitialized);
    }
    auto r = driver_->SoftReset();
    if (!r.ok()) {
        return r;
    }
    return driver_->Configure(config_.config);
}

Bmp581Handler::Counters Bmp581Handler::GetCounters() const noexcept {
    Counters c{};
    c.samples = samples_.load(std::memory_order_relaxed);
    c.bus_errors = bus_errors_.load(std::memory_order_relaxed);
    return c;
}
