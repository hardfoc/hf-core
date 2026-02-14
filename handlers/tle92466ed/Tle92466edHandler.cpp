/**
 * @file Tle92466edHandler.cpp
 * @brief Implementation of TLE92466ED handler with SPI communication adapter.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Tle92466edHandler.h"
#include "Logger.h"

#if defined(ESP_PLATFORM)
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

static constexpr const char* TAG = "TLE92466ED";

///////////////////////////////////////////////////////////////////////////////
// HalSpiTle92466edComm Implementation
///////////////////////////////////////////////////////////////////////////////

HalSpiTle92466edComm::HalSpiTle92466edComm(
    BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
    BaseGpio* faultn) noexcept
    : spi_(spi), resn_(resn), en_(en), faultn_(faultn) {}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Init() noexcept {
    if (!spi_.EnsureInitialized()) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }

    // Configure required control pins to known-safe defaults.
    if (resn_.SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) !=
        hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }
    resn_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
    if (!resn_.EnsureInitialized() ||
        resn_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }

    if (en_.SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) !=
        hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }
    en_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!en_.EnsureInitialized() ||
        en_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }

    if (faultn_) {
        if (faultn_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT) !=
            hf_gpio_err_t::GPIO_SUCCESS) {
            last_error_ = tle92466ed::CommError::HardwareNotReady;
            return std::unexpected(last_error_);
        }
        faultn_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        if (!faultn_->EnsureInitialized()) {
            last_error_ = tle92466ed::CommError::HardwareNotReady;
            return std::unexpected(last_error_);
        }
    }

    initialized_ = true;
    last_error_ = tle92466ed::CommError::None;
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Deinit() noexcept {
    initialized_ = false;
    return {};
}

tle92466ed::CommResult<uint32_t> HalSpiTle92466edComm::Transfer32(uint32_t tx_data) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }
    // 32-bit full-duplex: 4 bytes MSB-first
    uint8_t tx_buf[4] = {
        static_cast<uint8_t>((tx_data >> 24) & 0xFF),
        static_cast<uint8_t>((tx_data >> 16) & 0xFF),
        static_cast<uint8_t>((tx_data >>  8) & 0xFF),
        static_cast<uint8_t>((tx_data >>  0) & 0xFF)
    };
    uint8_t rx_buf[4] = {};

    auto err = spi_.Transfer(tx_buf, rx_buf, hf_u16_t{4}, hf_u32_t{0});
    if (err != hf_spi_err_t::SPI_SUCCESS) {
        last_error_ = tle92466ed::CommError::TransferError;
        return std::unexpected(last_error_);
    }

    uint32_t rx_data = (static_cast<uint32_t>(rx_buf[0]) << 24) |
                       (static_cast<uint32_t>(rx_buf[1]) << 16) |
                       (static_cast<uint32_t>(rx_buf[2]) <<  8) |
                       (static_cast<uint32_t>(rx_buf[3]) <<  0);
    last_error_ = tle92466ed::CommError::None;
    return rx_data;
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::TransferMulti(
    std::span<const uint32_t> tx_data,
    std::span<uint32_t> rx_data) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }
    if (tx_data.size() != rx_data.size()) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return std::unexpected(last_error_);
    }
    for (size_t i = 0; i < tx_data.size(); ++i) {
        auto result = Transfer32(tx_data[i]);
        if (!result) {
            return std::unexpected(result.error());
        }
        rx_data[i] = *result;
    }
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Delay(uint32_t microseconds) noexcept {
#if defined(ESP_PLATFORM)
    if (microseconds >= 1000) {
        vTaskDelay(pdMS_TO_TICKS(microseconds / 1000));
    } else {
        esp_rom_delay_us(microseconds);
    }
#else
    volatile uint32_t count = microseconds * 10;
    while (count--) { __asm__ volatile(""); }
#endif
    return {};
}

tle92466ed::CommResult<void> HalSpiTle92466edComm::Configure(
    const tle92466ed::SPIConfig& /*config*/) noexcept {
    // BaseSpi is pre-configured — nothing to do here
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
        return std::unexpected(last_error_);
    }

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case tle92466ed::CtrlPin::RESN:   gpio = &resn_;   break;
        case tle92466ed::CtrlPin::EN:     gpio = &en_;     break;
        case tle92466ed::CtrlPin::FAULTN: gpio = faultn_;  break;
        default:
            last_error_ = tle92466ed::CommError::InvalidParameter;
            return std::unexpected(last_error_);
    }

    if (gpio == nullptr) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return std::unexpected(last_error_);
    }

    // BaseGpio active level is configured per-pin — use SetActive/SetInactive
    hf_gpio_err_t gpio_err = hf_gpio_err_t::GPIO_SUCCESS;
    if (signal == tle92466ed::GpioSignal::ACTIVE) {
        gpio_err = gpio->SetActive();
    } else {
        gpio_err = gpio->SetInactive();
    }

    if (gpio_err != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::BusError;
        return std::unexpected(last_error_);
    }

    last_error_ = tle92466ed::CommError::None;
    return {};
}

tle92466ed::CommResult<tle92466ed::GpioSignal> HalSpiTle92466edComm::GpioRead(
    tle92466ed::CtrlPin pin) noexcept {
    if (!initialized_) {
        last_error_ = tle92466ed::CommError::HardwareNotReady;
        return std::unexpected(last_error_);
    }

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case tle92466ed::CtrlPin::RESN:   gpio = &resn_;   break;
        case tle92466ed::CtrlPin::EN:     gpio = &en_;     break;
        case tle92466ed::CtrlPin::FAULTN: gpio = faultn_;  break;
        default:
            last_error_ = tle92466ed::CommError::InvalidParameter;
            return std::unexpected(last_error_);
    }

    if (gpio == nullptr) {
        last_error_ = tle92466ed::CommError::InvalidParameter;
        return std::unexpected(last_error_);
    }

    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        last_error_ = tle92466ed::CommError::BusError;
        return std::unexpected(last_error_);
    }

    last_error_ = tle92466ed::CommError::None;
    return is_active ? tle92466ed::GpioSignal::ACTIVE
                     : tle92466ed::GpioSignal::INACTIVE;
}

void HalSpiTle92466edComm::Log(tle92466ed::LogLevel level, const char* tag,
                                 const char* format, va_list args) noexcept {
    char buf[256];
    vsnprintf(buf, sizeof(buf), format, args);
    auto& log = Logger::GetInstance();
    switch (level) {
        case tle92466ed::LogLevel::Error: log.Error(tag, "%s", buf); break;
        case tle92466ed::LogLevel::Warn:  log.Warn(tag, "%s", buf);  break;
        case tle92466ed::LogLevel::Info:  log.Info(tag, "%s", buf);  break;
        default:                          log.Debug(tag, "%s", buf); break;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Helper: convert uint8_t channel to tle92466ed::Channel enum
///////////////////////////////////////////////////////////////////////////////

static inline tle92466ed::Channel toChannel(uint8_t ch) {
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
    if (initialized_) {
        Deinitialize();
    }
}

bool Tle92466edHandler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }

    driver_ = std::make_unique<DriverType>(*comm_);
    auto result = driver_->Init();
    if (!result) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d",
                                   static_cast<int>(result.error()));
        driver_.reset();
        return false;
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TLE92466ED initialized successfully");
    return true;
}

bool Tle92466edHandler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

bool Tle92466edHandler::Initialize(const tle92466ed::GlobalConfig& config) noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }

    driver_ = std::make_unique<DriverType>(*comm_);

    // Init first, then configure global settings
    auto result = driver_->Init();
    if (!result) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d",
                                   static_cast<int>(result.error()));
        driver_.reset();
        return false;
    }

    auto cfg_result = driver_->ConfigureGlobal(config);
    if (!cfg_result) {
        Logger::GetInstance().Error(TAG, "Global config failed: %d",
                                   static_cast<int>(cfg_result.error()));
        driver_.reset();
        return false;
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TLE92466ED initialized with config");
    return true;
}

bool Tle92466edHandler::EnsureInitializedLocked() noexcept {
    if (initialized_ && driver_) {
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }
    return Initialize();
}

bool Tle92466edHandler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return true;

    if (driver_) {
        // Disable all channels before shutdown
        (void)driver_->DisableAllChannels();
    }
    driver_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "TLE92466ED deinitialized");
    return true;
}

bool Tle92466edHandler::ConfigureChannel(uint8_t channel,
                                          const tle92466ed::ChannelConfig& config) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->ConfigureChannel(toChannel(channel), config);
    return result.has_value();
}

bool Tle92466edHandler::EnableChannel(uint8_t channel) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->EnableChannel(toChannel(channel), true);
    return result.has_value();
}

bool Tle92466edHandler::DisableChannel(uint8_t channel) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->EnableChannel(toChannel(channel), false);
    return result.has_value();
}

bool Tle92466edHandler::EnableAllChannels() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->EnableAllChannels();
    return result.has_value();
}

bool Tle92466edHandler::DisableAllChannels() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->DisableAllChannels();
    return result.has_value();
}

bool Tle92466edHandler::SetChannelCurrent(uint8_t channel, uint16_t current_ma) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->SetCurrentSetpoint(toChannel(channel), current_ma);
    return result.has_value();
}

bool Tle92466edHandler::ConfigurePwmRaw(uint8_t channel, uint8_t mantissa,
                                         uint8_t exponent, bool low_freq_range) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->ConfigurePwmPeriodRaw(toChannel(channel), mantissa,
                                                  exponent, low_freq_range);
    return result.has_value();
}

bool Tle92466edHandler::EnterMissionMode() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->EnterMissionMode();
    return result.has_value();
}

bool Tle92466edHandler::EnterConfigMode() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->EnterConfigMode();
    return result.has_value();
}

bool Tle92466edHandler::IsMissionMode() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return driver_->IsMissionMode();
}

bool Tle92466edHandler::GetStatus(tle92466ed::DeviceStatus& status) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->GetDeviceStatus();
    if (!result) return false;
    status = *result;
    return true;
}

bool Tle92466edHandler::GetChannelDiagnostics(uint8_t channel,
                                               tle92466ed::ChannelDiagnostics& diag) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto result = driver_->GetChannelDiagnostics(toChannel(channel));
    if (!result) return false;
    diag = *result;
    return true;
}

bool Tle92466edHandler::GetFaultReport(tle92466ed::FaultReport& report) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->GetAllFaults();
    if (!result) return false;
    report = *result;
    return true;
}

bool Tle92466edHandler::ClearFaults() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->ClearFaults();
    return result.has_value();
}

bool Tle92466edHandler::HasFault() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->HasAnyFault();
    if (!result) return false;
    return *result;
}

bool Tle92466edHandler::KickWatchdog(uint16_t reload_value) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    auto result = driver_->ReloadSpiWatchdog(reload_value);
    return result.has_value();
}

uint32_t Tle92466edHandler::GetChipId() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return 0;
    auto result = driver_->GetChipId();
    if (!result) return 0;
    // Combine first two 16-bit registers into a 32-bit ID
    auto& arr = *result;
    return (static_cast<uint32_t>(arr[1]) << 16) | static_cast<uint32_t>(arr[0]);
}

uint32_t Tle92466edHandler::GetIcVersion() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return 0;
    auto result = driver_->GetIcVersion();
    if (!result) return 0;
    return static_cast<uint32_t>(*result);
}

Tle92466edHandler::DriverType* Tle92466edHandler::GetDriver() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return driver_.get();
}

const Tle92466edHandler::DriverType* Tle92466edHandler::GetDriver() const noexcept {
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
            log.Info(TAG, "  CH%u: OC=%s SG=%s OL=%s OT=%s AvgI=%u",
                     ch,
                     d.overcurrent ? "Y" : "n",
                     d.short_to_ground ? "Y" : "n",
                     d.open_load ? "Y" : "n",
                     d.over_temperature ? "Y" : "n",
                     d.average_current);
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
