/**
 * @file Isf15acp4Handler.h
 * @brief HAL handler for NKK ISF15ACP4 SmartDisplay OLED pushbutton (SSD1331 SPI).
 *
 * @details
 * Bridges BaseSpi + BaseGpio to the CRTP isf15acp4::SmartDisplay driver.
 * Software chip select is required — CS is driven via BaseGpio, not SPI hardware CS.
 *
 * @copyright Copyright (c) 2024-2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_ISF15ACP4_HANDLER_H_
#define COMPONENT_HANDLER_ISF15ACP4_HANDLER_H_

#include <cstdint>
#include <memory>

#include "base/BaseGpio.h"
#include "base/BaseSpi.h"
#include "core/hf-core-drivers/external/hf-isf15acp4-driver/inc/isf15acp4.hpp"
#include "RtosMutex.h"

/**
 * @brief CRTP SPI/GPIO adapter bridging BaseSpi + BaseGpio to isf15acp4::SpiInterface.
 */
class HalIsf15acp4SpiAdapter : public isf15acp4::SpiInterface<HalIsf15acp4SpiAdapter> {
public:
    HalIsf15acp4SpiAdapter(BaseSpi& spi, BaseGpio& cs, BaseGpio& dc, BaseGpio& reset,
                             BaseGpio* vcc_enable = nullptr, BaseGpio* switch_in = nullptr,
                             bool switch_active_low = true) noexcept;

    bool EnsureInitialized() noexcept;
    bool BeginCommand() noexcept;
    bool BeginData() noexcept;
    void EndTransaction() noexcept;
    bool WriteBytes(const uint8_t* data, std::size_t len) noexcept;
    void GpioSet(isf15acp4::CtrlPin pin, isf15acp4::GpioSignal signal) noexcept;
    bool GpioRead(isf15acp4::CtrlPin pin) noexcept;
    void DelayUs(uint32_t us) noexcept;

private:
    BaseSpi& spi_;
    BaseGpio& cs_;
    BaseGpio& dc_;
    BaseGpio& reset_;
    BaseGpio* vcc_enable_;
    BaseGpio* switch_in_;
    bool switch_active_low_;
    bool ready_{false};

    static void setGpio(BaseGpio& gpio, bool active) noexcept;
    static bool readGpio(BaseGpio& gpio, bool active_low) noexcept;
};

/** @brief Handler configuration mirroring driver SmartDisplayConfig. */
struct Isf15acp4HandlerConfig {
    isf15acp4::ProductVariant variant{isf15acp4::ProductVariant::Isf15acp4};
    isf15acp4::GraphicsBackend graphics{isf15acp4::GraphicsBackend::BuiltinCanvas};
    bool enable_vcc_on_init{true};
};

/**
 * @brief Unified handler for ISF15ACP4 SmartDisplay modules.
 */
class Isf15acp4Handler {
public:
    Isf15acp4Handler(BaseSpi& spi, BaseGpio& cs, BaseGpio& dc, BaseGpio& reset,
                       const Isf15acp4HandlerConfig& config = {},
                       BaseGpio* vcc_enable = nullptr, BaseGpio* switch_in = nullptr,
                       bool switch_active_low = true) noexcept;

    Isf15acp4Handler(const Isf15acp4Handler&) = delete;
    Isf15acp4Handler& operator=(const Isf15acp4Handler&) = delete;
    Isf15acp4Handler(Isf15acp4Handler&&) = delete;
    Isf15acp4Handler& operator=(Isf15acp4Handler&&) = delete;

    bool EnsureInitialized() noexcept;
    bool EnsureDeinitialized() noexcept;
    bool Deinitialize() noexcept;
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    bool DisplayOn() noexcept;
    bool DisplayOff() noexcept;
    bool FillScreen(isf15acp4::Color565 color) noexcept;
    bool Present(const isf15acp4::GraphicsContext& gfx) noexcept;
    [[nodiscard]] isf15acp4::GraphicsContext CreateGraphicsContext() const noexcept;
    [[nodiscard]] bool IsPressed() const noexcept;

    template <typename Fn>
    bool visitDriver(Fn&& fn) noexcept {
        MutexLockGuard lock(handler_mutex_);
        if (!ensureInitializedUnlocked()) {
            return false;
        }
        return fn(*display_);
    }

    [[nodiscard]] isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>* GetDriver() noexcept {
        return display_.get();
    }
    [[nodiscard]] const isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>* GetDriver() const noexcept {
        return display_.get();
    }
    [[nodiscard]] isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>* GetDisplay() noexcept {
        return GetDriver();
    }

    [[nodiscard]] uint16_t GetErrorFlags() const noexcept;
    void ClearErrorFlags(uint16_t mask = 0xFFFF) noexcept;
    void DumpDiagnostics() const noexcept;

private:
    bool ensureInitializedUnlocked() noexcept;
    void deinitializeUnlocked() noexcept;

    BaseSpi& spi_ref_;
    BaseGpio& cs_ref_;
    BaseGpio& dc_ref_;
    BaseGpio& reset_ref_;
    BaseGpio* vcc_enable_;
    BaseGpio* switch_in_;
    bool switch_active_low_;
    Isf15acp4HandlerConfig config_;

    std::unique_ptr<HalIsf15acp4SpiAdapter> adapter_;
    std::unique_ptr<isf15acp4::SmartDisplay<HalIsf15acp4SpiAdapter>> display_;
    mutable RtosMutex handler_mutex_;
    bool initialized_{false};
};

#endif  // COMPONENT_HANDLER_ISF15ACP4_HANDLER_H_
