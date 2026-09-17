/**
 * @file SfmHandler.cpp
 * @brief Implementation of `SfmHandler` and its `BaseI2c` CRTP adapter.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#include "SfmHandler.h"

// MCU-agnostic timing primitive shared by every handler in hf-core.
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

namespace {

constexpr hf_u32_t kI2cTimeoutMs = 50;

}  // namespace

//==============================================================================
// HalI2cSfmComm
//==============================================================================

HalI2cSfmComm::HalI2cSfmComm(BaseI2c& i2c_device) noexcept : i2c_device_(i2c_device) {}

std::uint8_t HalI2cSfmComm::DeviceAddress() const noexcept {
    return static_cast<std::uint8_t>(i2c_device_.GetDeviceAddress() & 0x7FU);
}

bool HalI2cSfmComm::Write(std::uint8_t addr7, const std::uint8_t* data, std::size_t len) noexcept {
    if (addr7 != DeviceAddress()) {
        return false;
    }
    if (len > sizeof(tx_scratch_)) {
        return false;
    }
    if (len == 0U) {
        /* Address-only probe (driver ExitSleep). BaseI2c has no zero-length
         * transfer; a one-byte no-op write is the closest equivalent and is
         * ignored by the sensor in idle. */
        tx_scratch_[0] = 0x00;
        return i2c_device_.Write(tx_scratch_, 1, kI2cTimeoutMs) == hf_i2c_err_t::I2C_SUCCESS;
    }
    for (std::size_t i = 0; i < len; ++i) {
        tx_scratch_[i] = data[i];
    }
    return i2c_device_.Write(tx_scratch_, static_cast<hf_u16_t>(len), kI2cTimeoutMs) ==
           hf_i2c_err_t::I2C_SUCCESS;
}

bool HalI2cSfmComm::Read(std::uint8_t addr7, std::uint8_t* out, std::size_t len) noexcept {
    if (addr7 != DeviceAddress() || out == nullptr || len == 0U || len > sizeof(rx_scratch_)) {
        return false;
    }
    if (i2c_device_.Read(rx_scratch_, static_cast<hf_u16_t>(len), kI2cTimeoutMs) !=
        hf_i2c_err_t::I2C_SUCCESS) {
        return false;
    }
    for (std::size_t i = 0; i < len; ++i) {
        out[i] = rx_scratch_[i];
    }
    return true;
}

void HalI2cSfmComm::DelayMs(std::uint32_t ms) noexcept {
    if (ms == 0U) {
        return;
    }
    os_delay_msec(static_cast<std::uint16_t>(ms > 0xFFFFU ? 0xFFFFU : ms));
}

bool HalI2cSfmComm::EnsureInitialized() noexcept {
    return i2c_device_.EnsureInitialized();
}

//==============================================================================
// SfmHandler
//==============================================================================

SfmHandler::SfmHandler(BaseI2c& i2c_device, const SfmHandlerConfig& config,
                       RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(i2c_device),
      driver_(std::make_unique<DriverType>(comm_, comm_.DeviceAddress())),
      bus_mutex_(bus_mutex != nullptr ? bus_mutex : &private_mutex_) {}

bool SfmHandler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool SfmHandler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    if (!comm_.EnsureInitialized()) {
        return false;
    }
    /* The address may have been rebound after construction. */
    driver_->SetAddress(comm_.DeviceAddress());

    if (config_.soft_reset_on_init) {
        (void)driver_->SoftReset();  /* NotSupported on this transport — harmless */
    }
    /* A part left measuring by a previous boot NACKs the identifier command. */
    (void)driver_->Stop();
    comm_.DelayMs(1);

    const auto id = driver_->ReadProductIdentifier();
    if (!id.ok()) {
        bus_errors_.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    identity_ = id.value;
    initialized_.store(true, std::memory_order_release);
    return true;
}

bool SfmHandler::SupportsGas(sfm::Gas gas) const noexcept {
    return sfm::SupportsGas(identity_.variant, gas);
}

float SfmHandler::FullScaleSlm() const noexcept {
    return sfm::FullScaleSlm(identity_.variant);
}

sfm::DriverResult<void> SfmHandler::Start(sfm::Gas gas) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return sfm::DriverResult<void>::failure(sfm::DriverError::NotInitialized);
    }
    auto r = driver_->StartContinuous(gas, config_.o2_permille);
    if (!r.ok()) {
        if (r.error == sfm::DriverError::BusWrite || r.error == sfm::DriverError::BusRead) {
            bus_errors_.fetch_add(1, std::memory_order_relaxed);
        }
        return r;
    }
    if (config_.averaging_window != 0U) {
        r = driver_->ConfigureAveraging(config_.averaging_window);
    }
    return r;
}

sfm::DriverResult<void> SfmHandler::Stop() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return sfm::DriverResult<void>::failure(sfm::DriverError::NotInitialized);
    }
    return driver_->Stop();
}

sfm::DriverResult<sfm::Measurement> SfmHandler::ReadMeasurement() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!initialized_.load(std::memory_order_acquire)) {
        return sfm::DriverResult<sfm::Measurement>::failure(sfm::DriverError::NotInitialized);
    }
    const auto m = driver_->ReadMeasurement();
    if (m.ok()) {
        frames_.fetch_add(1, std::memory_order_relaxed);
    } else if (m.error == sfm::DriverError::NoData) {
        no_data_.fetch_add(1, std::memory_order_relaxed);
    } else if (m.error == sfm::DriverError::Crc) {
        crc_errors_.fetch_add(1, std::memory_order_relaxed);
    } else {
        bus_errors_.fetch_add(1, std::memory_order_relaxed);
    }
    return m;
}

sfm::DriverResult<void> SfmHandler::ConfigureAveraging(std::uint16_t window) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!initialized_.load(std::memory_order_acquire)) {
        return sfm::DriverResult<void>::failure(sfm::DriverError::NotInitialized);
    }
    const auto r = driver_->ConfigureAveraging(window);
    if (r.ok()) {
        config_.averaging_window = window;
    }
    return r;
}

sfm::DriverResult<void> SfmHandler::UpdateConcentration(std::uint16_t o2_permille) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!initialized_.load(std::memory_order_acquire)) {
        return sfm::DriverResult<void>::failure(sfm::DriverError::NotInitialized);
    }
    const auto r = driver_->UpdateConcentration(o2_permille);
    if (r.ok()) {
        config_.o2_permille = o2_permille;
    }
    return r;
}

sfm::Gas SfmHandler::ActiveGas() const noexcept { return driver_->ActiveGas(); }

sfm::Scaling SfmHandler::ActiveScaling() const noexcept { return driver_->ActiveScaling(); }

bool SfmHandler::Measuring() const noexcept { return driver_->Measuring(); }

SfmHandler::Counters SfmHandler::GetCounters() const noexcept {
    Counters c{};
    c.frames = frames_.load(std::memory_order_relaxed);
    c.no_data = no_data_.load(std::memory_order_relaxed);
    c.crc_errors = crc_errors_.load(std::memory_order_relaxed);
    c.bus_errors = bus_errors_.load(std::memory_order_relaxed);
    return c;
}
