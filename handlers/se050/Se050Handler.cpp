/**
 * @file Se050Handler.cpp
 * @brief `Se050Handler` and `HalI2cSe050Comm` implementations.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "Se050Handler.h"

#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

#include "HardwareTypes.h"

#if defined(PW_HAL_STM32H7) && defined(PW_CORE_CM4)
#include "StmI2c.h"
#include "pw_se050_cm4_probe.h"
#define PW_SE050_RAW_HAL 1
#else
#define PW_SE050_RAW_HAL 0
#endif

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

#if PW_SE050_RAW_HAL
namespace {

I2C_HandleTypeDef* Se050RawHal(BaseI2c& i2c) noexcept {
    if (i2c.GetDeviceAddress() != 0x48) {
        return nullptr;
    }
    auto* stm = static_cast<StmI2cDevice*>(&i2c);
    StmI2cBus* bus = stm->GetParentBus();
    return (bus != nullptr) ? bus->GetHalHandle() : nullptr;
}

StmI2cBus* Se050RawBus(BaseI2c& i2c) noexcept {
    return static_cast<StmI2cDevice&>(i2c).GetParentBus();
}

se050::Error MapHal(HAL_StatusTypeDef st) noexcept {
    return (st == HAL_OK) ? se050::Error::Ok : se050::Error::Transport;
}

}  // namespace
#endif

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
#if PW_SE050_RAW_HAL
    /* Same Master_Transmit + AF poll as PwSe050Cm4_ProbeT1. Do not use
     * StmI2c PrepareMasterXfer / RecoverI2cAfterError on 0x48 — those
     * scramble T=1 after a busy NACK. Keep the bus mutex. */
    I2C_HandleTypeDef* hi2c = Se050RawHal(i2c_);
    StmI2cBus* bus = Se050RawBus(i2c_);
    if (hi2c != nullptr && bus != nullptr) {
        const hf_u32_t lock_ms = (timeout_ms > 0U) ? timeout_ms : 1000U;
        if (!bus_held_ && !bus->LockBus(lock_ms)) {
            return se050::Error::Timeout;
        }
        const se050::Error e = MapHal(PwSe050Cm4_I2cWrite(hi2c, tx, ClampU16(tx_len)));
        if (!bus_held_) {
            bus->UnlockBus();
        }
        return e;
    }
#endif
    /* SE050 NACKs 0x48 while busy (UM11225). Empty address frames are
     * forbidden — only retry real writes, matching pw_se050_cm4_probe. */
    constexpr int kPoll = 10;
    se050::Error last = se050::Error::Transport;
    for (int i = 0; i <= kPoll; ++i) {
        last = MapI2c(i2c_.Write(tx, ClampU16(tx_len), timeout_ms));
        if (last == se050::Error::Ok) {
            return last;
        }
        if (last != se050::Error::Transport && last != se050::Error::Timeout) {
            return last;
        }
        delay_ms_impl(3);
    }
    return last;
}

se050::Error HalI2cSe050Comm::I2cRead(std::uint8_t* rx, const std::size_t rx_len,
                                     const std::uint32_t timeout_ms) noexcept {
    if (rx == nullptr || rx_len == 0U) {
        return se050::Error::InvalidArgument;
    }
    if (!EnsureInitialized()) {
        return se050::Error::NotInitialized;
    }
#if PW_SE050_RAW_HAL
    I2C_HandleTypeDef* hi2c = Se050RawHal(i2c_);
    StmI2cBus* bus = Se050RawBus(i2c_);
    if (hi2c != nullptr && bus != nullptr) {
        const hf_u32_t lock_ms = (timeout_ms > 0U) ? timeout_ms : 1000U;
        if (!bus_held_ && !bus->LockBus(lock_ms)) {
            return se050::Error::Timeout;
        }
        const se050::Error e = MapHal(PwSe050Cm4_I2cRead(hi2c, rx, ClampU16(rx_len)));
        if (!bus_held_) {
            bus->UnlockBus();
        }
        return e;
    }
#endif
    constexpr int kPoll = 10;
    se050::Error last = se050::Error::Transport;
    for (int i = 0; i <= kPoll; ++i) {
        last = MapI2c(i2c_.Read(rx, ClampU16(rx_len), timeout_ms));
        if (last == se050::Error::Ok) {
            return last;
        }
        if (last != se050::Error::Transport && last != se050::Error::Timeout) {
            return last;
        }
        delay_ms_impl(3);
    }
    return last;
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
        return I2cWrite(tx, tx_len, timeout_ms);
    }
#if PW_SE050_RAW_HAL
    I2C_HandleTypeDef* hi2c = Se050RawHal(i2c_);
    StmI2cBus* bus = Se050RawBus(i2c_);
    if (hi2c != nullptr && bus != nullptr) {
        const hf_u32_t lock_ms = (timeout_ms > 0U) ? timeout_ms : 1000U;
        if (!bus_held_ && !bus->LockBus(lock_ms)) {
            return se050::Error::Timeout;
        }
        se050::Error e = MapHal(PwSe050Cm4_I2cWrite(hi2c, tx, ClampU16(tx_len)));
        if (e == se050::Error::Ok) {
            e = MapHal(PwSe050Cm4_I2cRead(hi2c, rx, ClampU16(rx_cap)));
            if (e == se050::Error::Ok) {
                *rx_len_out = rx_cap;
            }
        }
        if (!bus_held_) {
            bus->UnlockBus();
        }
        return e;
    }
#endif
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

bool HalI2cSe050Comm::HoldBus(const std::uint32_t timeout_ms) noexcept {
#if PW_SE050_RAW_HAL
    if (bus_held_) {
        return true;
    }
    StmI2cBus* bus = Se050RawBus(i2c_);
    if (bus == nullptr) {
        return false;
    }
    const hf_u32_t lock_ms = (timeout_ms > 0U) ? timeout_ms : 1000U;
    if (!bus->LockBus(lock_ms)) {
        return false;
    }
    bus_held_ = true;
    return true;
#else
    (void)timeout_ms;
    bus_held_ = true;
    return true;
#endif
}

void HalI2cSe050Comm::ReleaseBus() noexcept {
    if (!bus_held_) {
        return;
    }
#if PW_SE050_RAW_HAL
    if (StmI2cBus* bus = Se050RawBus(i2c_)) {
        bus->UnlockBus();
    }
#endif
    bus_held_ = false;
}

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
    /* PF1550 shares I2C1. SELECT is a multi-frame T=1 exchange; without a
     * hold the PMIC 50 ms poll can splice NAD/PCB and the applet looks
     * selected while N(S) is already wrong. */
    const std::uint32_t hold_ms =
        (config_.apdu_timeout_ms > 0U) ? config_.apdu_timeout_ms : 1000U;
    if (!comm_.HoldBus(hold_ms)) {
        return false;
    }
    bool ok = false;
    if (device_.EnsureInitialized()) {
        device_.T1().SetInterFrameDelayMs(config_.t1_inter_frame_delay_ms);
        (void)device_.HardwareReset();
        bool reset_ok = true;
        if (!config_.skip_chip_warm_reset) {
            reset_ok = (device_.T1().ChipWarmReset(config_.warm_reset_timeout_ms) ==
                        se050::Error::Ok);
        }
        if (reset_ok) {
            std::uint8_t sel_rsp[128]{};
            std::size_t sel_len = 0;
            const se050::Error se = device_.SelectDefaultIoTApplet(
                sel_rsp, sizeof(sel_rsp), &sel_len, config_.apdu_timeout_ms);
            if (se == se050::Error::Ok) {
                /* SELECT 0x9000 is the session gate. GetVersion is an extended
                 * APDU and must not run here — a failed T=1 exchange desyncs
                 * N(S). */
                bool sw_ok = true;
                if (sel_len >= 2U) {
                    const std::uint16_t sw = static_cast<std::uint16_t>(
                        (static_cast<std::uint16_t>(sel_rsp[sel_len - 2U]) << 8) |
                        sel_rsp[sel_len - 1U]);
                    sw_ok = (sw == 0x9000U);
                }
                if (sw_ok) {
                    device_.T1().SetReadRetries(16);
                    device_.T1().SetReadRetryDelayMs(5);
                    initialized_.store(true, std::memory_order_release);
                    ok = true;
                }
            }
        }
    }
    comm_.ReleaseBus();
    return ok;
}
