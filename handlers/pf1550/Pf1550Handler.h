/**
 * @file Pf1550Handler.h
 * @brief HAL handler for NXP PF1550 PMIC (I2C + optional strap GPIOs).
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_PF1550_HANDLER_H_
#define COMPONENT_HANDLER_PF1550_HANDLER_H_

#include <cstdint>
#include <memory>

#include "RtosMutex.h"
#include "base/BaseGpio.h"
#include "base/BaseI2c.h"

#include "core/hf-core-drivers/external/hf-pf1550-driver/inc/pf1550.hpp"

class HalPf1550Comm : public pf1550::BusInterface<HalPf1550Comm> {
public:
    HalPf1550Comm(BaseI2c& i2c, BaseGpio* standby_gpio, BaseGpio* usb_vbus_en_gpio,
                  BaseGpio* usb_otg_en_gpio) noexcept;

    bool Write(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len) noexcept;
    bool Read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len) noexcept;
    bool EnsureInitialized() noexcept;
    void GpioSet(pf1550::CtrlPin pin, pf1550::GpioSignal signal) noexcept;
    void DelayUs(uint32_t us) noexcept;

private:
    BaseI2c& i2c_;
    BaseGpio* standby_gpio_;
    BaseGpio* usb_vbus_en_gpio_;
    BaseGpio* usb_otg_en_gpio_;
    mutable RtosMutex i2c_mutex_;
};

/**
 * @brief Thread-safe PF1550 PMIC handler for hf-core / HAL integration.
 */
class Pf1550Handler {
public:
    using Pf1550Driver = pf1550::PF1550<HalPf1550Comm>;

    Pf1550Handler(BaseI2c& i2c, BaseGpio* standby_gpio = nullptr,
                  BaseGpio* usb_vbus_en_gpio = nullptr,
                  BaseGpio* usb_otg_en_gpio = nullptr) noexcept;

    Pf1550Handler(const Pf1550Handler&) = delete;
    Pf1550Handler& operator=(const Pf1550Handler&) = delete;

    bool EnsureInitialized() noexcept;
    bool IsInitialized() const noexcept { return initialized_; }

    bool ApplyPortentaH7Profile() noexcept;
    bool SetPowerMode(pf1550::PowerMode mode) noexcept;
    bool SetUsbRails(bool vbus_en, bool otg_en) noexcept;
    bool ReadPmicStatus(uint8_t& status) noexcept;

    Pf1550Driver* GetDriver() noexcept;
    const Pf1550Driver* GetDriver() const noexcept;

private:
    bool ensureInitializedLocked() noexcept;

    BaseI2c& i2c_;
    BaseGpio* standby_gpio_;
    BaseGpio* usb_vbus_en_gpio_;
    BaseGpio* usb_otg_en_gpio_;
    std::unique_ptr<HalPf1550Comm> comm_;
    std::unique_ptr<Pf1550Driver> driver_;
    bool initialized_;
    mutable RtosMutex handler_mutex_;
};

#endif  // COMPONENT_HANDLER_PF1550_HANDLER_H_
