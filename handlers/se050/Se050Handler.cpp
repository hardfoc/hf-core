/**
 * @file Se050Handler.cpp
 * @brief `Se050Handler` and `HalI2cSe050Comm` implementations.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "Se050Handler.h"

#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

#include "HardwareTypes.h"

namespace {

[[nodiscard]] se050::Error MapI2c(hf_i2c_err_t e) noexcept {
    if (e == hf_i2c_err_t::I2C_SUCCESS) {
        return se050::Error::Ok;
    }
    if (e == hf_i2c_err_t::I2C_ERR_TIMEOUT || e == hf_i2c_err_t::I2C_ERR_BUS_TIMEOUT) {
        return se050::Error::Timeout;
    }
    return se050::Error::Transport;
}

[[nodiscard]] hf_u16_t ClampU16(std::size_t n) noexcept {
    return static_cast<hf_u16_t>(n > 65535U ? 65535U : n);
}

}  // namespace

//==============================================================================
// HalI2cSe050Comm
//==============================================================================

se050::Error HalI2cSe050Comm::I2cWrite(const std::uint8_t* tx, const std::size_t tx_len,
                                       const std::uint32_t timeout_ms) noexcept {
    if (tx == nullptr || tx_len == 0U) {
        return se050::Error::InvalidArgument;
    }
    if (!EnsureInitialized()) {
        return se050::Error::NotInitialized;
    }
    return MapI2c(i2c_.Write(tx, ClampU16(tx_len), timeout_ms));
}

se050::Error HalI2cSe050Comm::I2cRead(std::uint8_t* rx, const std::size_t rx_len,
                                     const std::uint32_t timeout_ms) noexcept {
    if (rx == nullptr || rx_len == 0U) {
        return se050::Error::InvalidArgument;
    }
    if (!EnsureInitialized()) {
        return se050::Error::NotInitialized;
    }
    return MapI2c(i2c_.Read(rx, ClampU16(rx_len), timeout_ms));
}

se050::Error HalI2cSe050Comm::Transceive(const std::uint8_t* tx, const std::size_t tx_len, std::uint8_t* rx,
                                         const std::size_t rx_cap, std::size_t* rx_len_out,
                                         const std::uint32_t timeout_ms) noexcept {
    if (rx_len_out == nullptr) {
        return se050::Error::InvalidArgument;
    }
    *rx_len_out = 0;
    if (tx_len == 0U) {
        return se050::Error::InvalidArgument;
    }
    if (!EnsureInitialized()) {
        return se050::Error::NotInitialized;
    }
    if (rx == nullptr || rx_cap == 0U) {
        return MapI2c(i2c_.Write(tx, ClampU16(tx_len), timeout_ms));
    }
    const hf_i2c_err_t e =
        i2c_.WriteRead(tx, ClampU16(tx_len), rx, ClampU16(rx_cap), timeout_ms);
    if (e == hf_i2c_err_t::I2C_SUCCESS) {
        *rx_len_out = rx_cap;
    }
    return MapI2c(e);
}

se050::Error HalI2cSe050Comm::HardwareReset() noexcept {
    if (reset_gpio_ == nullptr) {
        return se050::Error::Ok;
    }
    if (!EnsureInitialized()) {
        return se050::Error::NotInitialized;
    }
    // Assert reset (logical inactive → electrical low on typical active-high config)
    (void)reset_gpio_->SetInactive();
    delay_ms_impl(2);
    (void)reset_gpio_->SetActive();
    delay_ms_impl(10);
    return se050::Error::Ok;
}

void HalI2cSe050Comm::delay_ms_impl(const std::uint32_t ms) noexcept { os_delay_msec(ms); }

//==============================================================================
// Se050Handler
//==============================================================================

Se050Handler::Se050Handler(BaseI2c& i2c, const Se050HandlerConfig& config, BaseGpio* reset_gpio,
                           RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(i2c, reset_gpio),
      device_(comm_),
      bus_mutex_(bus_mutex == nullptr ? &private_mutex_ : bus_mutex) {}

bool Se050Handler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool Se050Handler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    if (!device_.EnsureInitialized()) {
        return false;
    }
    device_.T1().SetInterFrameDelayMs(config_.t1_inter_frame_delay_ms);
    (void)device_.HardwareReset();
    const se050::Error wr = device_.T1().ChipWarmReset(config_.warm_reset_timeout_ms);
    if (wr != se050::Error::Ok) {
        return false;
    }
    std::uint8_t sel_rsp[128]{};
    std::size_t   sel_len = 0;
    const se050::Error se =
        device_.SelectDefaultIoTApplet(sel_rsp, sizeof(sel_rsp), &sel_len, config_.apdu_timeout_ms);
    if (se != se050::Error::Ok) {
        return false;
    }
    se050::cmd::VersionInfo ver{};
    const se050::Error ve = device_.GetVersion(&ver, config_.apdu_timeout_ms);
    if (ve != se050::Error::Ok) {
        return false;
    }
    (void)ver;
    initialized_.store(true, std::memory_order_release);
    return true;
}
