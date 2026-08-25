/**
 * @file Isf15acp4Handler.cpp
 * @brief Implementation of ISF15ACP4 SmartDisplay HAL handler.
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#include "Isf15acp4Handler.h"

#include "handlers/common/HandlerCommon.h"
#include "handlers/logger/Logger.h"

void HalIsf15acp4SpiAdapter::setGpio(BaseGpio& gpio, bool electrical_high) noexcept {
    const bool pin_active_high =
        gpio.GetActiveState() == hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH;
    if (electrical_high == pin_active_high) {
        (void)gpio.SetActive();
    } else {
        (void)gpio.SetInactive();
    }
}

bool HalIsf15acp4SpiAdapter::readGpio(BaseGpio& gpio, bool active_low) noexcept {
    bool is_active = false;
    if (gpio.IsActive(is_active) != hf_gpio_err_t::GPIO_SUCCESS) {
        return false;
    }
    const bool pin_active_low =
        gpio.GetActiveState() == hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW;
    return (pin_active_low == active_low) ? is_active : !is_active;
}

HalIsf15acp4SpiAdapter::HalIsf15acp4SpiAdapter(BaseSpi& spi, BaseGpio& cs, BaseGpio& dc,
                                               BaseGpio& reset, BaseGpio* vcc_enable,
                                               BaseGpio* switch_in, bool switch_active_low) noexcept
    : spi_(spi),
      cs_(cs),
      dc_(dc),
      reset_(reset),
      vcc_enable_(vcc_enable),
      switch_in_(switch_in),
      switch_active_low_(switch_active_low) {}

bool HalIsf15acp4SpiAdapter::EnsureInitialized() noexcept {
    if (ready_) {
        return true;
    }
    ready_ = spi_.EnsureInitialized();
    if (ready_) {
        setGpio(cs_, true);
        setGpio(dc_, true);
        setGpio(reset_, true);
        if (vcc_enable_ != nullptr) {
            setGpio(*vcc_enable_, false);
        }
    }
    return ready_;
}

bool HalIsf15acp4SpiAdapter::BeginCommand() noexcept {
    setGpio(dc_, false);
    setGpio(cs_, false);
    return true;
}

bool HalIsf15acp4SpiAdapter::BeginData() noexcept {
    setGpio(dc_, true);
    setGpio(cs_, false);
    return true;
}

void HalIsf15acp4SpiAdapter::EndTransaction() noexcept { setGpio(cs_, true); }

bool HalIsf15acp4SpiAdapter::WriteBytes(const uint8_t* data, std::size_t len) noexcept {
    if (data == nullptr || len == 0) {
        return false;
    }
    return spi_.Transfer(const_cast<uint8_t*>(data), nullptr, static_cast<uint16_t>(len), 1000) ==
           hf_spi_err_t::SPI_SUCCESS;
}

void HalIsf15acp4SpiAdapter::GpioSet(isf15acp4::CtrlPin pin,
                                     isf15acp4::GpioSignal signal) noexcept {
    const bool active = signal == isf15acp4::GpioSignal::Active;
    switch (pin) {
        case isf15acp4::CtrlPin::ChipSelect:
            setGpio(cs_, !active);
            break;
        case isf15acp4::CtrlPin::DataCommand:
            setGpio(dc_, active);
            break;
        case isf15acp4::CtrlPin::Reset:
            setGpio(reset_, !active);
            break;
        case isf15acp4::CtrlPin::VccEnable:
            if (vcc_enable_ != nullptr) {
                setGpio(*vcc_enable_, active);
            }
            break;
        case isf15acp4::CtrlPin::Switch:
            break;
    }
}

bool HalIsf15acp4SpiAdapter::GpioRead(isf15acp4::CtrlPin pin) noexcept {
    if (pin != isf15acp4::CtrlPin::Switch || switch_in_ == nullptr) {
        return false;
    }
    return readGpio(*switch_in_, switch_active_low_);
}

void HalIsf15acp4SpiAdapter::DelayUs(uint32_t us) noexcept { handler_utils::DelayUs(us); }

Isf15acp4Handler::Isf15acp4Handler(BaseSpi& spi, BaseGpio& cs, BaseGpio& dc, BaseGpio& reset,
                                   const Isf15acp4HandlerConfig& config, BaseGpio* vcc_enable,
                                   BaseGpio* switch_in, bool switch_active_low) noexcept
    : spi_ref_(spi),
      cs_ref_(cs),
      dc_ref_(dc),
      reset_ref_(reset),
      vcc_enable_(vcc_enable),
      switch_in_(switch_in),
      switch_active_low_(switch_active_low),
      config_(config) {}

void Isf15acp4Handler::deinitializeUnlocked() noexcept {
    if (display_) {
        display_->DisplayOff();
        display_->ClearErrorFlags();
    }
    display_.reset();
    adapter_.reset();
    initialized_ = false;
}

bool Isf15acp4Handler::ensureInitializedUnlocked() noexcept {
    if (initialized_ && display_) {
        return true;
    }

    adapter_ = std::make_unique<HalIsf15acp4SpiAdapter>(spi_ref_, cs_ref_, dc_ref_, reset_ref_,
                                                        vcc_enable_, switch_in_, switch_active_low_);

    isf15acp4::SmartDisplayConfig drv_cfg{
        .variant = config_.variant,
        .graphics = config_.graphics,
        .enable_vcc_on_init = config_.enable_vcc_on_init,
    };

    display_ = std::make_unique<isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>>(adapter_.get(),
                                                                                  drv_cfg);
    if (!display_->Initialize(config_.enable_vcc_on_init)) {
        Logger::GetInstance().Error("ISF15ACP4", "SmartDisplay initialization failed");
        deinitializeUnlocked();
        return false;
    }

    auto gfx = display_->CreateGraphicsContext();
    display_->ConfigurePresenter(gfx);
    initialized_ = true;
    Logger::GetInstance().Info("ISF15ACP4", "Handler initialized (driver v%s)",
                               isf15acp4::GetDriverVersion());
    return true;
}

bool Isf15acp4Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    return ensureInitializedUnlocked();
}

bool Isf15acp4Handler::Deinitialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    deinitializeUnlocked();
    return true;
}

bool Isf15acp4Handler::EnsureDeinitialized() noexcept { return Deinitialize(); }

bool Isf15acp4Handler::DisplayOn() noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ && display_->DisplayOn();
}

bool Isf15acp4Handler::DisplayOff() noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ && display_->DisplayOff();
}

bool Isf15acp4Handler::FillScreen(isf15acp4::Color565 color) noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ && display_->FillScreen(color);
}

bool Isf15acp4Handler::Present(const isf15acp4::GraphicsContext& gfx) noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ && display_->Present(gfx);
}

isf15acp4::GraphicsContext Isf15acp4Handler::CreateGraphicsContext() const noexcept {
    return isf15acp4::GraphicsContext(config_.variant, config_.graphics);
}

bool Isf15acp4Handler::IsPressed() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ && display_->IsPressed();
}

uint16_t Isf15acp4Handler::GetErrorFlags() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return display_ ? display_->GetErrorFlags() : 0;
}

void Isf15acp4Handler::ClearErrorFlags(uint16_t mask) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (display_) {
        display_->ClearErrorFlags(mask);
    }
}

void Isf15acp4Handler::DumpDiagnostics() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    Logger::GetInstance().Info("ISF15ACP4", "initialized=%d errors=0x%04X", initialized_ ? 1 : 0,
                               display_ ? display_->GetErrorFlags() : 0);
}
