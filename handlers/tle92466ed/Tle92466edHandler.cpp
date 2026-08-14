/**
 * @file Tle92466edHandler.cpp
 * @brief Implementation of TLE92466ED handler with SPI communication adapter.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Tle92466edHandler.h"
#include "Logger.h"
#include "HandlerCommon.h"

static constexpr const char* TAG = "TLE92466ED";

namespace {
/* Fixed TX/RX frame buffers in TU static storage (internal SRAM).
 * Do not stage 32-bit SPI frames on a task stack or external RAM — the MCU
 * SPI path must always see tightly-coupled / internal SRAM pointers. The
 * Comm adapter object itself may still live elsewhere. */
uint8_t g_tle_spi_tx[4]{};
uint8_t g_tle_spi_rx[4]{};

/* Chain staging for the pipelined command+dummy read. Frame pointers are
 * handed to BaseSpi::TransferChain, which holds ONE bus-ownership window
 * across every frame — see kTleChainGapUs. */
constexpr size_t kTleChainMaxFrames = 4;
constexpr uint32_t kTleChainGapUs = 20U;
uint8_t g_tle_chain_tx[kTleChainMaxFrames][4]{};
uint8_t g_tle_chain_rx[kTleChainMaxFrames][4]{};
}  // namespace

///////////////////////////////////////////////////////////////////////////////
// HalSpiTle92466edComm Implementation
///////////////////////////////////////////////////////////////////////////////

HalSpiTle92466edComm::HalSpiTle92466edComm(
    BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
    BaseGpio* faultn) noexcept
    : spi_(spi), resn_(resn), en_(en), faultn_(faultn) {}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Init() noexcept {
    /* Idempotent: Driver::Init calls Comm::Init on every bring-up retry.
     * Re-parking EN low here toggled the enable rail at the retry rate
     * while ICVID was failing — EN must latch once, then only move for an
     * intentional HW reset (Driver::SetEnable) or safe-off. */
    if (initialized_) {
        last_error_ = tle92466ed::CommError::None;
        return {};
    }

    if (!spi_.EnsureInitialized()) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }

    /* Pins are batch-programmed by the board GPIO map — only confirm polarity
     * and park levels. Extra SetDirection RMW on a shared I2C expander bus
     * has wedged the expander after map bring-up. First Init only: park EN
     * low / RESN released; Driver::Init then owns the reset+enable sequence. */
    resn_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    if (!resn_.EnsureInitialized() ||
        resn_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }

    en_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!en_.EnsureInitialized() ||
        en_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }

    if (faultn_) {
        faultn_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        if (!faultn_->EnsureInitialized()) {
            last_error_ = tle92466ed::CommError::HardwareNotReady;
            return tle::unexpected(last_error_);
        }
    }

    initialized_ = true;
    last_error_ = tle92466ed::CommError::None;
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Deinit() noexcept {
    /* Comm deinit is a flag only — Driver::Deinit owns safe-off sequencing. */
    initialized_ = false;
    return {};
}

tle92466ed::CommResult<uint32_t> HalSpiTle92466edComm::Transfer32(uint32_t tx_data) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }
    /* MSB-first on the wire (matches byte_swap + LE DMA staging). Uses TU-static
     * internal SRAM buffers — not stack or external RAM (see g_tle_spi_tx). */
    g_tle_spi_tx[0] = static_cast<uint8_t>((tx_data >> 24) & 0xFF);
    g_tle_spi_tx[1] = static_cast<uint8_t>((tx_data >> 16) & 0xFF);
    g_tle_spi_tx[2] = static_cast<uint8_t>((tx_data >> 8) & 0xFF);
    g_tle_spi_tx[3] = static_cast<uint8_t>((tx_data >> 0) & 0xFF);
    g_tle_spi_rx[0] = g_tle_spi_rx[1] = g_tle_spi_rx[2] = g_tle_spi_rx[3] = 0;

    auto err = spi_.Transfer(g_tle_spi_tx, g_tle_spi_rx, hf_u16_t{4}, hf_u32_t{0});
    if (err != hf_spi_err_t::SPI_SUCCESS) {
        Logger::GetInstance().Error(TAG,
            "Transfer32: SPI Transfer failed (err=%d) tx=%02X %02X %02X %02X",
            static_cast<int>(err), g_tle_spi_tx[0], g_tle_spi_tx[1],
            g_tle_spi_tx[2], g_tle_spi_tx[3]);
        last_error_ = tle92466ed::CommError::TransferError;
        return tle::unexpected(last_error_);
    }

    const uint32_t rx_data =
        (static_cast<uint32_t>(g_tle_spi_rx[0]) << 24) |
        (static_cast<uint32_t>(g_tle_spi_rx[1]) << 16) |
        (static_cast<uint32_t>(g_tle_spi_rx[2]) << 8) |
        (static_cast<uint32_t>(g_tle_spi_rx[3]) << 0);
    last_rx_ = rx_data;
    /* Inter-frame gap between CS cycles (TLE two-transfer read protocol).
     * 20 µs covers long soft-CS leads; ESP bench uses ~10 µs on short PCB. */
    handler_utils::DelayUs(20U);
    last_error_ = tle92466ed::CommError::None;
    return rx_data;
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::TransferMulti(
    std::span<const uint32_t> tx_data,
    std::span<uint32_t> rx_data) noexcept {
    return TransferMulti(tx_data, rx_data, 0U);
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::TransferMulti(
    std::span<const uint32_t> tx_data,
    std::span<uint32_t> rx_data,
    uint32_t gap_us) noexcept {
    if (gap_us == 0U) {
        gap_us = kTleChainGapUs;
    }
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }
    if (tx_data.size() != rx_data.size() || tx_data.empty()) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return tle::unexpected(last_error_);
    }

    /* The TLE92466ED reply pipeline spans two CS windows: the response to the
     * command frame only appears on the NEXT frame. Issuing those frames as
     * independent BaseSpi::Transfer calls releases the SPI2 bus mutex between
     * them, so a peer transaction can consume this device's pipeline slot —
     * observed as "sticky-zero" register reads and impossible FB_I_AVG ratios
     * (TP_MANT spliced from another frame). TransferChain keeps ONE bus
     * ownership window across the whole pipeline while still raising CS
     * between frames, which is what the protocol requires. */
    if (tx_data.size() <= kTleChainMaxFrames) {
        const size_t n = tx_data.size();
        const uint8_t* tx_ptrs[kTleChainMaxFrames]{};
        uint8_t* rx_ptrs[kTleChainMaxFrames]{};
        for (size_t i = 0; i < n; ++i) {
            g_tle_chain_tx[i][0] = static_cast<uint8_t>((tx_data[i] >> 24) & 0xFF);
            g_tle_chain_tx[i][1] = static_cast<uint8_t>((tx_data[i] >> 16) & 0xFF);
            g_tle_chain_tx[i][2] = static_cast<uint8_t>((tx_data[i] >> 8) & 0xFF);
            g_tle_chain_tx[i][3] = static_cast<uint8_t>((tx_data[i] >> 0) & 0xFF);
            g_tle_chain_rx[i][0] = g_tle_chain_rx[i][1] = 0;
            g_tle_chain_rx[i][2] = g_tle_chain_rx[i][3] = 0;
            tx_ptrs[i] = g_tle_chain_tx[i];
            rx_ptrs[i] = g_tle_chain_rx[i];
        }

        const auto err = spi_.TransferChain(tx_ptrs, rx_ptrs, hf_u16_t{4},
                                            static_cast<hf_u16_t>(n),
                                            hf_u32_t{gap_us},
                                            hf_u32_t{0});
        if (err != hf_spi_err_t::SPI_SUCCESS) {
            Logger::GetInstance().Error(
                TAG, "TransferMulti: TransferChain failed (err=%d, frames=%u)",
                static_cast<int>(err), static_cast<unsigned>(n));
            last_error_ = tle92466ed::CommError::TransferError;
            return tle::unexpected(last_error_);
        }

        for (size_t i = 0; i < n; ++i) {
            rx_data[i] = (static_cast<uint32_t>(g_tle_chain_rx[i][0]) << 24) |
                         (static_cast<uint32_t>(g_tle_chain_rx[i][1]) << 16) |
                         (static_cast<uint32_t>(g_tle_chain_rx[i][2]) << 8) |
                         (static_cast<uint32_t>(g_tle_chain_rx[i][3]) << 0);
        }
        last_rx_ = rx_data[n - 1];
        last_rx_first_ = rx_data[0];
        last_error_ = tle92466ed::CommError::None;
        return {};
    }

    /* Longer bursts are not part of any TLE protocol sequence; fall back to
     * per-frame transfers rather than silently truncating the chain. */
    for (size_t i = 0; i < tx_data.size(); ++i) {
        auto result = Transfer32(tx_data[i]);
        if (!result) {
            return tle::unexpected(result.error());
        }
        rx_data[i] = *result;
    }
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Delay(uint32_t microseconds) noexcept {
    handler_utils::DelayUs(microseconds);
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Configure(
    const tle92466ed::SPIConfig& /*config*/) noexcept {
    /* Board bind configures SPI2 Mode 1 / 32-bit before this adapter exists. */
    return {};
}

bool HalSpiTle92466edComm::IsReady() const noexcept {
    return initialized_;
}

tle92466ed::CommError HalSpiTle92466edComm::GetLastError() const noexcept {
    return last_error_;
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::ClearErrors() noexcept {
    last_error_ = tle92466ed::CommError::None;
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::GpioSet(
    tle92466ed::CtrlPin pin, tle92466ed::GpioSignal signal) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case tle92466ed::CtrlPin::RESN:   gpio = &resn_;   break;
        case tle92466ed::CtrlPin::EN:     gpio = &en_;     break;
        case tle92466ed::CtrlPin::FAULTN: gpio = faultn_;  break;
        default:
            last_error_ = tle92466ed::CommError::InvalidParameter;
            return tle::unexpected(last_error_);
    }

    if (gpio == nullptr) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return tle::unexpected(last_error_);
    }

    // BaseGpio active level is configured per-pin — map driver ACTIVE/INACTIVE
    // to SetActive/SetInactive rather than raw pin writes.
    hf_gpio_err_t gpio_err = hf_gpio_err_t::GPIO_SUCCESS;
    if (signal == tle92466ed::GpioSignal::ACTIVE) {
        gpio_err = gpio->SetActive();
    } else {
        gpio_err = gpio->SetInactive();
    }

    if (gpio_err != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::BusError;
        return tle::unexpected(last_error_);
    }

    last_error_ = tle92466ed::CommError::None;
    return {};
}

tle92466ed::CommResult<tle92466ed::GpioSignal> HalSpiTle92466edComm::GpioRead(
    tle92466ed::CtrlPin pin) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return tle::unexpected(last_error_);
    }

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case tle92466ed::CtrlPin::RESN:   gpio = &resn_;   break;
        case tle92466ed::CtrlPin::EN:     gpio = &en_;     break;
        case tle92466ed::CtrlPin::FAULTN: gpio = faultn_;  break;
        default:
            last_error_ = tle92466ed::CommError::InvalidParameter;
            return tle::unexpected(last_error_);
    }

    if (gpio == nullptr) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return tle::unexpected(last_error_);
    }

    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::BusError;
        return tle::unexpected(last_error_);
    }

    last_error_ = tle92466ed::CommError::None;
    return is_active ? tle92466ed::GpioSignal::ACTIVE
                     : tle92466ed::GpioSignal::INACTIVE;
}

void HalSpiTle92466edComm::Log(tle92466ed::LogLevel level, const char* tag,
                                 const char* format, va_list args) noexcept {
    handler_utils::RouteLogToLogger(static_cast<int>(level), tag, format, args);
}

///////////////////////////////////////////////////////////////////////////////
// Helper: convert uint8_t channel to tle92466ed::Channel enum
///////////////////////////////////////////////////////////////////////////////

static inline tle92466ed::Channel toChannel(uint8_t ch) {
    /* Bounds are validated by every public handler entrypoint before call. */
    return static_cast<tle92466ed::Channel>(ch);
}

///////////////////////////////////////////////////////////////////////////////
// Tle92466edHandler Implementation
///////////////////////////////////////////////////////////////////////////////

Tle92466edHandler::Tle92466edHandler(
    BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
    BaseGpio* faultn) noexcept {
    comm_ = std::make_unique<HalSpiTle92466edComm>(spi, resn, en, faultn);
    Logger::GetInstance().Info(TAG, "TLE92466ED handler created");
}

Tle92466edHandler::~Tle92466edHandler() noexcept {
    /* Best-effort safe-off; do not throw from destructor. */
    if (initialized_) {
        Deinitialize();
    }
}

tle92466ed::DriverResult<void> Tle92466edHandler::Initialize(
    bool perform_hardware_reset) noexcept {
    MutexLockGuard lock(mutex_);
    return InitializeLocked(perform_hardware_reset);
}

tle92466ed::DriverResult<void> Tle92466edHandler::InitializeLocked(
    bool perform_hardware_reset) noexcept {
    /* Caller holds mutex_. Comm::Init is idempotent; Driver::Init owns RESN/EN
     * sequencing and the first ICVID read after optional hardware reset. */
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return {};
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return tle::unexpected(tle92466ed::DriverError::NotInitialized);
    }

    driver_ = std::make_unique<DriverType>(*comm_);
    auto result = driver_->Init(perform_hardware_reset);
    if (!result) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d",
                                   static_cast<int>(result.error()));
        driver_.reset();
        return tle::unexpected(result.error());
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TLE92466ED initialized successfully");
    return {};
}

bool Tle92466edHandler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

tle92466ed::DriverResult<void> Tle92466edHandler::Initialize(const tle92466ed::GlobalConfig& config) noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return {};
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return tle::unexpected(tle92466ed::DriverError::NotInitialized);
    }

    driver_ = std::make_unique<DriverType>(*comm_);

    /* Init() always hardware-resets; GlobalConfig overload has no reset flag. */
    auto result = driver_->Init();
    if (!result) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d",
                                   static_cast<int>(result.error()));
        driver_.reset();
        return tle::unexpected(result.error());
    }

    auto cfg_result = driver_->ConfigureGlobal(config);
    if (!cfg_result) {
        Logger::GetInstance().Error(TAG, "Global config failed: %d",
                                   static_cast<int>(cfg_result.error()));
        driver_.reset();
        return tle::unexpected(cfg_result.error());
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TLE92466ED initialized with config");
    return {};
}

bool Tle92466edHandler::EnsureInitializedLocked() noexcept {
    /* Lazy path always hardware-resets so first consumer sees a known device. */
    if (initialized_ && driver_) {
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }
    return InitializeLocked(/*perform_hardware_reset=*/true).has_value();
}

tle92466ed::DriverResult<void> Tle92466edHandler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return {};

    if (driver_) {
        /* Drop channel enables before destroying driver_; does not pulse RESN. */
        (void)driver_->DisableAllChannels();
    }
    driver_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "TLE92466ED deinitialized");
    return {};
}

tle92466ed::DriverResult<void> Tle92466edHandler::ConfigureChannel(uint8_t channel,
                                          const tle92466ed::ChannelConfig& config) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.ConfigureChannel(toChannel(channel), config);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnableChannel(uint8_t channel) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.EnableChannel(toChannel(channel), true);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::DisableChannel(uint8_t channel) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.EnableChannel(toChannel(channel), false);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnableAllChannels() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.EnableAllChannels();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::DisableAllChannels() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.DisableAllChannels();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::SetChannelCurrent(uint8_t channel, uint16_t current_ma) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.SetCurrentSetpoint(toChannel(channel), current_ma);
    });
}

tle92466ed::DriverResult<uint16_t> Tle92466edHandler::GetChannelCurrentSetpoint(
    uint8_t channel, bool parallel_mode) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<uint16_t> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.GetCurrentSetpoint(toChannel(channel), parallel_mode);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::ConfigurePwmRaw(uint8_t channel, uint8_t mantissa,
                                         uint8_t exponent, bool low_freq_range) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        return drv.ConfigurePwmPeriodRaw(toChannel(channel), mantissa,
                                          exponent, low_freq_range);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnterMissionMode() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.EnterMissionMode();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnableOutputStage() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.Enable();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::DisableOutputStage() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.Disable();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnableFeedbackUpdates() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        // FB_FRZ register lives at central register offset 0x0007. Bit n of
        // the lower byte freezes channel n's feedback when set, unfreezes
        // when cleared. Writing 0x0000 unfreezes every channel. The driver's
        // public API doesn't expose an FB_FRZ helper, so go through the
        // generic WriteRegister.
        //
        // Skip write-verify: shared Mode1 soft-CS readback of FB_FRZ is often a
        // phantom (same class as CH_CTRL sticky-zero). A hard verify fail would
        // leave channels frozen and FB_I_AVG stuck at 0 even when the clear
        // write landed.
        constexpr uint16_t kFbFrzAddr = tle92466ed::CentralReg::FB_FRZ;
        return drv.WriteRegister(kFbFrzAddr, 0x0000, false, false);
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::EnterConfigMode() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.EnterConfigMode();
    });
}

bool Tle92466edHandler::IsMissionMode() noexcept {
    return withDriver([](auto& drv) -> bool {
        return drv.IsMissionMode();
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::GetStatus(tle92466ed::DeviceStatus& status) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        auto result = drv.GetDeviceStatus();
        if (!result) return tle::unexpected(result.error());
        status = *result;
        return {};
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::GetChannelDiagnostics(uint8_t channel,
                                               tle92466ed::ChannelDiagnostics& diag) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        if (channel >= kNumChannels)
            return tle::unexpected(tle92466ed::DriverError::InvalidChannel);
        auto result = drv.GetChannelDiagnostics(toChannel(channel));
        if (!result) return tle::unexpected(result.error());
        diag = *result;
        return {};
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::GetFaultReport(tle92466ed::FaultReport& report) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        auto result = drv.GetAllFaults();
        if (!result) return tle::unexpected(result.error());
        report = *result;
        return {};
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::ClearFaults() noexcept {
    return withDriver([](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.ClearFaults();
    });
}

bool Tle92466edHandler::HasFault() noexcept {
    return withDriver([](auto& drv) -> bool {
        auto result = drv.HasAnyFault();
        if (!result) return false;
        return *result;
    });
}

tle92466ed::DriverResult<void> Tle92466edHandler::KickWatchdog(uint16_t reload_value) noexcept {
    return withDriver([&](auto& drv) -> tle92466ed::DriverResult<void> {
        return drv.ReloadSpiWatchdog(reload_value);
    });
}

uint32_t Tle92466edHandler::GetChipId() noexcept {
    return withDriver([](auto& drv) -> uint32_t {
        auto result = drv.GetChipId();
        if (!result) return 0;
        auto& arr = *result;
        return (static_cast<uint32_t>(arr[1]) << 16) | static_cast<uint32_t>(arr[0]);
    });
}

uint32_t Tle92466edHandler::GetIcVersion() noexcept {
    return withDriver([](auto& drv) -> uint32_t {
        auto result = drv.GetIcVersion();
        if (!result) return 0;
        return static_cast<uint32_t>(*result);
    });
}

Tle92466edHandler::DriverType* Tle92466edHandler::GetDriver() noexcept {
    MutexLockGuard lock(mutex_);
    /* Never HW-reset from GetDriver — upper layers own RESN policy. Callers
     * that need init must go through Initialize() / EnsureInitialized(). */
    if (!initialized_ || !driver_) {
        return nullptr;
    }
    return driver_.get();
}

const Tle92466edHandler::DriverType* Tle92466edHandler::GetDriver() const noexcept {
    /* Delegate to non-const impl: lazy init is intentionally unavailable here. */
    auto* self = const_cast<Tle92466edHandler*>(this);
    return self->GetDriver();
}

void Tle92466edHandler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    auto& log = Logger::GetInstance();
    if (!EnsureInitializedLocked() || !driver_) {
        log.Warn(TAG, "Not initialized — cannot dump diagnostics");
        return;
    }

    log.Info(TAG, "=== TLE92466ED Diagnostics ===");

    // Device status
    auto status_result = driver_->GetDeviceStatus();
    if (status_result) {
        auto& s = *status_result;
        log.Info(TAG, "  Config mode: %s", s.config_mode ? "yes" : "no");
        log.Info(TAG, "  Init done: %s", s.init_done ? "yes" : "no");
        log.Info(TAG, "  Any fault: %s", s.any_fault ? "YES" : "no");
        log.Info(TAG, "  OT warning: %s", s.ot_warning ? "YES" : "no");
        log.Info(TAG, "  OT error: %s", s.ot_error ? "YES" : "no");
        log.Info(TAG, "  VBAT: %u mV", s.vbat_voltage);
        log.Info(TAG, "  VIO: %u mV", s.vio_voltage);
    }

    // Per-channel diagnostics
    for (uint8_t ch = 0; ch < kNumChannels; ++ch) {
        auto diag_result = driver_->GetChannelDiagnostics(toChannel(ch));
        if (diag_result) {
            auto& d = *diag_result;
            log.Info(TAG,
                     "  CH%u: OC=%s SG=%s OL=%s OTE=%s AvgI=%u DC=%u Imin=%d Imax=%d OTW=%s I_REG=%s",
                     ch,
                     d.overcurrent ? "Y" : "n",
                     d.short_to_ground ? "Y" : "n",
                     d.open_load ? "Y" : "n",
                     d.over_temperature ? "Y" : "n",
                     static_cast<unsigned>(d.average_current),
                     static_cast<unsigned>(d.duty_cycle),
                     static_cast<int>(d.min_current_mA),
                     static_cast<int>(d.max_current_mA),
                     d.ot_warning ? "Y" : "n",
                     d.current_regulation_warning ? "Y" : "n");
        }
    }

    // Chip ID
    auto id_result = driver_->GetChipId();
    if (id_result) {
        auto& arr = *id_result;
        uint32_t combined = (static_cast<uint32_t>(arr[1]) << 16) | static_cast<uint32_t>(arr[0]);
        log.Info(TAG, "  Chip ID: 0x%04X 0x%04X 0x%04X (combined 0x%08lX)",
                 arr[0], arr[1], arr[2], static_cast<unsigned long>(combined));
    }

    log.Info(TAG, "=== End TLE92466ED Diagnostics ===");
}
