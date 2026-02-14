/**
 * @file Max22200Handler.cpp
 * @brief Implementation of MAX22200 handler with SPI communication adapter.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Max22200Handler.h"
#include "Logger.h"

#if defined(ESP_PLATFORM)
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

static constexpr const char* TAG = "MAX22200";

///////////////////////////////////////////////////////////////////////////////
// HalSpiMax22200Comm Implementation
///////////////////////////////////////////////////////////////////////////////

HalSpiMax22200Comm::HalSpiMax22200Comm(
    BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
    BaseGpio* fault) noexcept
    : spi_(spi), enable_(enable), cmd_(cmd), fault_(fault) {}

bool HalSpiMax22200Comm::Initialize() {
    if (!spi_.EnsureInitialized()) {
        initialized_ = false;
        return false;
    }

    if (enable_.SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) !=
        hf_gpio_err_t::GPIO_SUCCESS) {
        initialized_ = false;
        return false;
    }
    enable_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!enable_.EnsureInitialized() ||
        enable_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        initialized_ = false;
        return false;
    }

    if (cmd_.SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT) !=
        hf_gpio_err_t::GPIO_SUCCESS) {
        initialized_ = false;
        return false;
    }
    cmd_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!cmd_.EnsureInitialized() ||
        cmd_.SetInactive() != hf_gpio_err_t::GPIO_SUCCESS) {
        initialized_ = false;
        return false;
    }

    if (fault_) {
        if (fault_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT) !=
            hf_gpio_err_t::GPIO_SUCCESS) {
            initialized_ = false;
            return false;
        }
        fault_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        if (!fault_->EnsureInitialized()) {
            initialized_ = false;
            return false;
        }
    }

    initialized_ = true;
    return true;
}

bool HalSpiMax22200Comm::Transfer(const uint8_t* tx_data, uint8_t* rx_data, size_t length) {
    if (!IsReady() || tx_data == nullptr || rx_data == nullptr || length == 0) {
        return false;
    }
    auto err = spi_.Transfer(tx_data, rx_data, static_cast<hf_u16_t>(length), hf_u32_t{0});
    return (err == hf_spi_err_t::SPI_SUCCESS);
}

bool HalSpiMax22200Comm::SetChipSelect(bool /*state*/) {
    // BaseSpi manages CS automatically per transfer
    return true;
}

bool HalSpiMax22200Comm::Configure(uint32_t /*speed_hz*/, uint8_t /*mode*/, bool /*msb_first*/) {
    // BaseSpi is pre-configured
    return true;
}

bool HalSpiMax22200Comm::IsReady() const {
    if (!initialized_) return false;
    if (!spi_.IsInitialized()) return false;
    if (!enable_.IsInitialized() || !cmd_.IsInitialized()) return false;
    if (fault_ && !fault_->IsInitialized()) return false;
    return true;
}

void HalSpiMax22200Comm::DelayUs(uint32_t us) {
#if defined(ESP_PLATFORM)
    if (us >= 1000) {
        vTaskDelay(pdMS_TO_TICKS(us / 1000));
    } else {
        esp_rom_delay_us(us);
    }
#else
    volatile uint32_t count = us * 10;
    while (count--) { __asm__ volatile(""); }
#endif
}

void HalSpiMax22200Comm::GpioSet(max22200::CtrlPin pin, max22200::GpioSignal signal) {
    if (!initialized_) return;

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case max22200::CtrlPin::ENABLE:  gpio = &enable_; break;
        case max22200::CtrlPin::CMD:     gpio = &cmd_;    break;
        case max22200::CtrlPin::FAULT:   gpio = fault_;   break;
        default: return;
    }

    if (gpio == nullptr) return;

    hf_gpio_err_t gpio_err = hf_gpio_err_t::GPIO_SUCCESS;
    if (signal == max22200::GpioSignal::ACTIVE) {
        gpio_err = gpio->SetActive();
    } else {
        gpio_err = gpio->SetInactive();
    }

    if (gpio_err != hf_gpio_err_t::GPIO_SUCCESS) {
        initialized_ = false;
        Logger::GetInstance().Error(TAG, "GPIO control failed for MAX22200 control pin");
    }
}

bool HalSpiMax22200Comm::GpioRead(max22200::CtrlPin pin, max22200::GpioSignal& signal) {
    if (!IsReady()) return false;

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case max22200::CtrlPin::ENABLE:  gpio = &enable_; break;
        case max22200::CtrlPin::CMD:     gpio = &cmd_;    break;
        case max22200::CtrlPin::FAULT:   gpio = fault_;   break;
        default: return false;
    }

    if (gpio == nullptr) return false;

    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) return false;

    signal = is_active ? max22200::GpioSignal::ACTIVE
                       : max22200::GpioSignal::INACTIVE;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Max22200Handler Implementation
///////////////////////////////////////////////////////////////////////////////

Max22200Handler::Max22200Handler(
    BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
    BaseGpio* fault) noexcept {
    comm_ = std::make_unique<HalSpiMax22200Comm>(spi, enable, cmd, fault);
    Logger::GetInstance().Info(TAG, "MAX22200 handler created");
}

Max22200Handler::~Max22200Handler() noexcept {
    if (initialized_) {
        Deinitialize();
    }
}

bool Max22200Handler::Initialize() noexcept {
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
    auto status = driver_->Initialize();
    if (status != max22200::DriverStatus::OK) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d", static_cast<int>(status));
        driver_.reset();
        return false;
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "MAX22200 initialized successfully");
    return true;
}

bool Max22200Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

bool Max22200Handler::Initialize(const max22200::BoardConfig& board_config) noexcept {
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

    // Set board config (void return)
    driver_->SetBoardConfig(board_config);

    // Then initialize
    auto status = driver_->Initialize();
    if (status != max22200::DriverStatus::OK) {
        Logger::GetInstance().Error(TAG, "Driver init failed: %d", static_cast<int>(status));
        driver_.reset();
        return false;
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "MAX22200 initialized with board config");
    return true;
}

bool Max22200Handler::EnsureInitializedLocked() noexcept {
    if (initialized_ && driver_) {
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }
    return Initialize();
}

bool Max22200Handler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return true;

    if (driver_) {
        driver_->Deinitialize();
    }
    driver_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "MAX22200 deinitialized");
    return true;
}

bool Max22200Handler::ConfigureChannel(uint8_t channel,
                                        const max22200::ChannelConfig& config) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    return (driver_->ConfigureChannel(channel, config) == max22200::DriverStatus::OK);
}

bool Max22200Handler::SetupCdrChannel(uint8_t channel, uint16_t hit_ma,
                                       uint16_t hold_ma, float hit_time_ms) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto s1 = driver_->SetHitCurrentMa(channel, hit_ma);
    if (s1 != max22200::DriverStatus::OK) return false;
    auto s2 = driver_->SetHoldCurrentMa(channel, hold_ma);
    if (s2 != max22200::DriverStatus::OK) return false;
    auto s3 = driver_->SetHitTimeMs(channel, hit_time_ms);
    return (s3 == max22200::DriverStatus::OK);
}

bool Max22200Handler::SetupVdrChannel(uint8_t channel, float hit_duty_pct,
                                       float hold_duty_pct, float hit_time_ms) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    auto s1 = driver_->SetHitDutyPercent(channel, hit_duty_pct);
    if (s1 != max22200::DriverStatus::OK) return false;
    auto s2 = driver_->SetHoldDutyPercent(channel, hold_duty_pct);
    if (s2 != max22200::DriverStatus::OK) return false;
    auto s3 = driver_->SetHitTimeMs(channel, hit_time_ms);
    return (s3 == max22200::DriverStatus::OK);
}

bool Max22200Handler::EnableChannel(uint8_t channel) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    return (driver_->EnableChannel(channel) == max22200::DriverStatus::OK);
}

bool Max22200Handler::DisableChannel(uint8_t channel) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    return (driver_->DisableChannel(channel) == max22200::DriverStatus::OK);
}

bool Max22200Handler::EnableAllChannels() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->EnableAllChannels() == max22200::DriverStatus::OK);
}

bool Max22200Handler::DisableAllChannels() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->DisableAllChannels() == max22200::DriverStatus::OK);
}

bool Max22200Handler::IsChannelEnabled(uint8_t channel) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    // Read STATUS to check channels_on_mask
    max22200::StatusConfig status{};
    auto s = driver_->ReadStatus(status);
    if (s != max22200::DriverStatus::OK) return false;
    return (status.channels_on_mask & (1u << channel)) != 0;
}

bool Max22200Handler::SetChannelsMask(uint8_t mask) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->SetChannelsOn(mask) == max22200::DriverStatus::OK);
}

bool Max22200Handler::GetStatus(max22200::StatusConfig& status) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->ReadStatus(status) == max22200::DriverStatus::OK);
}

bool Max22200Handler::GetChannelFaults(uint8_t channel,
                                        max22200::FaultStatus& faults) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    if (channel >= kNumChannels) return false;
    // Read fault register (covers all channels)
    auto s = driver_->ReadFaultRegister(faults);
    (void)channel; // FaultStatus is device-wide
    return (s == max22200::DriverStatus::OK);
}

bool Max22200Handler::HasFault() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    max22200::StatusConfig status{};
    auto s = driver_->ReadFaultFlags(status);
    if (s != max22200::DriverStatus::OK) return false;
    return status.hasFault();
}

bool Max22200Handler::ClearFaults() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->ClearAllFaults() == max22200::DriverStatus::OK);
}

bool Max22200Handler::ReadFaultRegister(max22200::FaultStatus& faults) noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !driver_) return false;
    return (driver_->ReadFaultRegister(faults) == max22200::DriverStatus::OK);
}

Max22200Handler::DriverType* Max22200Handler::GetDriver() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return driver_.get();
}

const Max22200Handler::DriverType* Max22200Handler::GetDriver() const noexcept {
    auto* self = const_cast<Max22200Handler*>(this);
    return self->GetDriver();
}

void Max22200Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    auto& log = Logger::GetInstance();
    if (!EnsureInitializedLocked() || !driver_) {
        log.Warn(TAG, "Not initialized — cannot dump diagnostics");
        return;
    }

    log.Info(TAG, "=== MAX22200 Diagnostics ===");

    // Status
    max22200::StatusConfig status{};
    if (driver_->ReadStatus(status) == max22200::DriverStatus::OK) {
        log.Info(TAG, "  Active: %s", status.active ? "yes" : "no");
        log.Info(TAG, "  Channels ON: 0x%02x", status.channels_on_mask);
        log.Info(TAG, "  Has fault: %s", status.hasFault() ? "YES" : "no");
    }

    // Fault register
    max22200::FaultStatus faults{};
    if (driver_->ReadFaultRegister(faults) == max22200::DriverStatus::OK) {
        log.Info(TAG, "  Faults: hasFault=%s", faults.hasFault() ? "YES" : "no");
    }

    // Statistics
    auto stats = driver_->GetStatistics();
    log.Info(TAG, "  Transfers: %lu, Success rate: %.1f%%",
             static_cast<unsigned long>(stats.total_transfers),
             static_cast<double>(stats.getSuccessRate()));

    log.Info(TAG, "=== End MAX22200 Diagnostics ===");
}
