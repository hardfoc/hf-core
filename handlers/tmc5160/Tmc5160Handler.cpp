/**
 * @file Tmc5160Handler.cpp
 * @brief Implementation of TMC5160 handler with SPI/UART communication adapters.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Tmc5160Handler.h"
#include "Logger.h"

#if defined(ESP_PLATFORM)
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
// Fallback delay implementations
static inline void os_delay_msec(uint32_t ms) {
    volatile uint32_t count = ms * 10000;
    while (count--) { __asm__ volatile(""); }
}
#endif

static constexpr const char* TAG = "TMC5160";

///////////////////////////////////////////////////////////////////////////////
// HalSpiTmc5160Comm Implementation
///////////////////////////////////////////////////////////////////////////////

HalSpiTmc5160Comm::HalSpiTmc5160Comm(
    BaseSpi& spi, BaseGpio& enable,
    BaseGpio* diag0, BaseGpio* diag1,
    const tmc51x0::PinActiveLevels& active_levels) noexcept
    : spi_(spi)
    , ctrl_pins_{enable, diag0, diag1}
    , active_levels_(active_levels) {}

tmc51x0::Result<void> HalSpiTmc5160Comm::SpiTransfer(
    const uint8_t* tx, uint8_t* rx, size_t length) noexcept {
    if (tx == nullptr || rx == nullptr || length == 0) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::INVALID_VALUE);
    }
    auto err = spi_.Transfer(tx, rx, static_cast<hf_u16_t>(length), hf_u32_t{0});
    if (err != hf_spi_err_t::SPI_SUCCESS) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::COMM_ERROR);
    }
    return tmc51x0::Result<void>();
}

tmc51x0::Result<void> HalSpiTmc5160Comm::GpioSet(
    tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept {
    BaseGpio* gpio = ctrl_pins_.get(pin);
    if (gpio == nullptr) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::UNSUPPORTED);
    }
    const bool pin_active_high = active_levels_.GetActiveLevel(pin);
    const bool want_active = (signal == tmc51x0::GpioSignal::ACTIVE) == pin_active_high;
    hf_gpio_state_t state = want_active
                                ? hf_gpio_state_t::HF_GPIO_STATE_ACTIVE
                                : hf_gpio_state_t::HF_GPIO_STATE_INACTIVE;
    auto err = gpio->SetState(state);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::HARDWARE_ERROR);
    }
    return tmc51x0::Result<void>();
}

tmc51x0::Result<tmc51x0::GpioSignal> HalSpiTmc5160Comm::GpioRead(
    tmc51x0::TMC51x0CtrlPin pin) noexcept {
    BaseGpio* gpio = ctrl_pins_.get(pin);
    if (gpio == nullptr) {
        return tmc51x0::Result<tmc51x0::GpioSignal>(tmc51x0::ErrorCode::UNSUPPORTED);
    }
    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        return tmc51x0::Result<tmc51x0::GpioSignal>(tmc51x0::ErrorCode::HARDWARE_ERROR);
    }
    const bool pin_active_high = active_levels_.GetActiveLevel(pin);
    tmc51x0::GpioSignal signal = (is_active == pin_active_high)
        ? tmc51x0::GpioSignal::ACTIVE
        : tmc51x0::GpioSignal::INACTIVE;
    return tmc51x0::Result<tmc51x0::GpioSignal>(signal);
}

void HalSpiTmc5160Comm::DebugLog(int level, const char* tag,
                                  const char* format, va_list args) noexcept {
    char buf[256];
    vsnprintf(buf, sizeof(buf), format, args);
    auto& log = Logger::GetInstance();
    switch (level) {
        case 0: log.Error(tag, "%s", buf); break;
        case 1: log.Warn(tag, "%s", buf); break;
        case 2: log.Info(tag, "%s", buf); break;
        default: log.Debug(tag, "%s", buf); break;
    }
}

void HalSpiTmc5160Comm::DelayMs(uint32_t ms) noexcept {
#if defined(ESP_PLATFORM)
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    os_delay_msec(ms);
#endif
}

void HalSpiTmc5160Comm::DelayUs(uint32_t us) noexcept {
#if defined(ESP_PLATFORM)
    esp_rom_delay_us(us);
#else
    volatile uint32_t count = us * 10;
    while (count--) { __asm__ volatile(""); }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// HalUartTmc5160Comm Implementation
///////////////////////////////////////////////////////////////////////////////

HalUartTmc5160Comm::HalUartTmc5160Comm(
    BaseUart& uart, BaseGpio& enable,
    BaseGpio* diag0, BaseGpio* diag1,
    const tmc51x0::PinActiveLevels& active_levels) noexcept
    : uart_(uart)
    , ctrl_pins_{enable, diag0, diag1}
    , active_levels_(active_levels) {}

tmc51x0::Result<void> HalUartTmc5160Comm::UartSend(
    const uint8_t* data, size_t length) noexcept {
    if (data == nullptr || length == 0) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::INVALID_VALUE);
    }
    auto err = uart_.Write(data, length);
    if (err != hf_uart_err_t::UART_SUCCESS) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::COMM_ERROR);
    }
    return tmc51x0::Result<void>();
}

tmc51x0::Result<void> HalUartTmc5160Comm::UartReceive(
    uint8_t* data, size_t length) noexcept {
    if (data == nullptr || length == 0) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::INVALID_VALUE);
    }
    auto err = uart_.Read(data, length);
    if (err != hf_uart_err_t::UART_SUCCESS) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::COMM_ERROR);
    }
    return tmc51x0::Result<void>();
}

tmc51x0::Result<void> HalUartTmc5160Comm::GpioSet(
    tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept {
    BaseGpio* gpio = ctrl_pins_.get(pin);
    if (gpio == nullptr) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::UNSUPPORTED);
    }
    const bool pin_active_high = active_levels_.GetActiveLevel(pin);
    const bool want_active = (signal == tmc51x0::GpioSignal::ACTIVE) == pin_active_high;
    hf_gpio_state_t state = want_active
                                ? hf_gpio_state_t::HF_GPIO_STATE_ACTIVE
                                : hf_gpio_state_t::HF_GPIO_STATE_INACTIVE;
    auto err = gpio->SetState(state);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        return tmc51x0::Result<void>(tmc51x0::ErrorCode::HARDWARE_ERROR);
    }
    return tmc51x0::Result<void>();
}

tmc51x0::Result<tmc51x0::GpioSignal> HalUartTmc5160Comm::GpioRead(
    tmc51x0::TMC51x0CtrlPin pin) noexcept {
    BaseGpio* gpio = ctrl_pins_.get(pin);
    if (gpio == nullptr) {
        return tmc51x0::Result<tmc51x0::GpioSignal>(tmc51x0::ErrorCode::UNSUPPORTED);
    }
    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        return tmc51x0::Result<tmc51x0::GpioSignal>(tmc51x0::ErrorCode::HARDWARE_ERROR);
    }
    const bool pin_active_high = active_levels_.GetActiveLevel(pin);
    tmc51x0::GpioSignal signal = (is_active == pin_active_high)
        ? tmc51x0::GpioSignal::ACTIVE
        : tmc51x0::GpioSignal::INACTIVE;
    return tmc51x0::Result<tmc51x0::GpioSignal>(signal);
}

void HalUartTmc5160Comm::DebugLog(int level, const char* tag,
                                   const char* format, va_list args) noexcept {
    char buf[256];
    vsnprintf(buf, sizeof(buf), format, args);
    auto& log = Logger::GetInstance();
    switch (level) {
        case 0: log.Error(tag, "%s", buf); break;
        case 1: log.Warn(tag, "%s", buf); break;
        case 2: log.Info(tag, "%s", buf); break;
        default: log.Debug(tag, "%s", buf); break;
    }
}

void HalUartTmc5160Comm::DelayMs(uint32_t ms) noexcept {
#if defined(ESP_PLATFORM)
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    os_delay_msec(ms);
#endif
}

void HalUartTmc5160Comm::DelayUs(uint32_t us) noexcept {
#if defined(ESP_PLATFORM)
    esp_rom_delay_us(us);
#else
    volatile uint32_t count = us * 10;
    while (count--) { __asm__ volatile(""); }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Tmc5160Handler Implementation
///////////////////////////////////////////////////////////////////////////////

Tmc5160Handler::Tmc5160Handler(
    BaseSpi& spi, BaseGpio& enable,
    BaseGpio* diag0, BaseGpio* diag1,
    uint8_t daisy_chain_position,
    const tmc51x0::PinActiveLevels& active_levels) noexcept
    : is_spi_(true)
    , address_(daisy_chain_position)
{
    spi_comm_ = std::make_unique<HalSpiTmc5160Comm>(spi, enable, diag0, diag1, active_levels);
    Logger::GetInstance().Info(TAG, "TMC5160 handler created (SPI, daisy_pos=%u)", static_cast<unsigned>(daisy_chain_position));
}

Tmc5160Handler::Tmc5160Handler(
    BaseUart& uart, BaseGpio& enable,
    BaseGpio* diag0, BaseGpio* diag1,
    uint8_t uart_node_address,
    const tmc51x0::PinActiveLevels& active_levels) noexcept
    : is_spi_(false)
    , address_(uart_node_address)
{
    uart_comm_ = std::make_unique<HalUartTmc5160Comm>(uart, enable, diag0, diag1, active_levels);
    Logger::GetInstance().Info(TAG, "TMC5160 handler created (UART, node_addr=%u)", static_cast<unsigned>(uart_node_address));
}

Tmc5160Handler::~Tmc5160Handler() noexcept {
    if (initialized_) {
        Deinitialize();
    }
}

bool Tmc5160Handler::Initialize(const tmc51x0::DriverConfig& config, bool verbose) noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized, deinitializing first");
        initialized_ = false;
        spi_driver_.reset();
        uart_driver_.reset();
    }

    config_ = config;

    if (is_spi_) {
        if (!spi_comm_) {
            Logger::GetInstance().Error(TAG, "SPI comm adapter not created");
            return false;
        }
        spi_driver_ = std::make_unique<SpiDriver>(*spi_comm_, address_);
        auto result = spi_driver_->Initialize(config_, verbose);
        if (!result) {
            Logger::GetInstance().Error(TAG, "SPI driver init failed: %s", result.ErrorMessage());
            spi_driver_.reset();
            return false;
        }
    } else {
        if (!uart_comm_) {
            Logger::GetInstance().Error(TAG, "UART comm adapter not created");
            return false;
        }
        uart_driver_ = std::make_unique<UartDriver>(*uart_comm_, 0, address_);
        auto result = uart_driver_->Initialize(config_, verbose);
        if (!result) {
            Logger::GetInstance().Error(TAG, "UART driver init failed: %s", result.ErrorMessage());
            uart_driver_.reset();
            return false;
        }
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TMC5160 initialized successfully");
    return true;
}

bool Tmc5160Handler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) {
        return true;
    }

    // Disable motor before destroying driver
    visitDriverInternal([](auto& drv) {
        drv.motorControl.Disable();
    });

    spi_driver_.reset();
    uart_driver_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "TMC5160 deinitialized");
    return true;
}

//=============================================================================
// Convenience Methods — Motor Control
//=============================================================================

bool Tmc5160Handler::EnableMotor() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.motorControl.Enable();
        return r.IsOk();
    });
}

bool Tmc5160Handler::DisableMotor() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.motorControl.Disable();
        return r.IsOk();
    });
}

bool Tmc5160Handler::IsMotorEnabled() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.motorControl.IsEnabled();
        return r.IsOk() && r.Value();
    });
}

bool Tmc5160Handler::SetCurrent(uint8_t irun, uint8_t ihold) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([irun, ihold](auto& drv) -> bool {
        auto r = drv.motorControl.SetCurrent(irun, ihold);
        return r.IsOk();
    });
}

//=============================================================================
// Convenience Methods — Motion Control
//=============================================================================

bool Tmc5160Handler::SetTargetPosition(int32_t position) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([position](auto& drv) -> bool {
        auto r = drv.rampControl.SetTargetPosition(
            static_cast<float>(position), tmc51x0::Unit::Steps);
        return r.IsOk();
    });
}

bool Tmc5160Handler::SetTargetVelocity(int32_t velocity) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([velocity](auto& drv) -> bool {
        auto r = drv.rampControl.SetMaxSpeed(static_cast<float>(velocity), tmc51x0::Unit::Steps);
        return r.IsOk();
    });
}

bool Tmc5160Handler::SetMaxSpeed(float speed, tmc51x0::Unit unit) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([speed, unit](auto& drv) -> bool {
        auto r = drv.rampControl.SetMaxSpeed(speed, unit);
        return r.IsOk();
    });
}

bool Tmc5160Handler::SetAcceleration(float accel, tmc51x0::Unit unit) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([accel, unit](auto& drv) -> bool {
        auto r = drv.rampControl.SetAcceleration(accel, unit);
        return r.IsOk();
    });
}

bool Tmc5160Handler::Stop() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.rampControl.Stop();
        return r.IsOk();
    });
}

int32_t Tmc5160Handler::GetCurrentPosition() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return 0;
    return visitDriverInternal([](auto& drv) -> int32_t {
        auto r = drv.rampControl.GetCurrentPositionMicrosteps();
        return r.IsOk() ? r.Value() : 0;
    });
}

int32_t Tmc5160Handler::GetCurrentVelocity() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return 0;
    return visitDriverInternal([](auto& drv) -> int32_t {
        auto r = drv.rampControl.GetCurrentSpeed(tmc51x0::Unit::Steps);
        return r.IsOk() ? static_cast<int32_t>(r.Value()) : 0;
    });
}

bool Tmc5160Handler::IsTargetReached() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.rampControl.IsTargetReached();
        return r.IsOk() && r.Value();
    });
}

//=============================================================================
// Convenience Methods — Status & Diagnostics
//=============================================================================

bool Tmc5160Handler::IsStandstill() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.rampControl.IsStandstill();
        return r.IsOk() && r.Value();
    });
}

bool Tmc5160Handler::IsOvertemperature() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.status.IsOvertemperature();
        return r.IsOk() && r.Value();
    });
}

bool Tmc5160Handler::IsStallDetected() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return false;
    return visitDriverInternal([](auto& drv) -> bool {
        auto r = drv.stallGuard.IsStallDetected();
        return r.IsOk() && r.Value();
    });
}

int32_t Tmc5160Handler::GetStallGuardResult() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return -1;
    return visitDriverInternal([](auto& drv) -> int32_t {
        auto r = drv.stallGuard.GetStallGuardResult();
        return r.IsOk() ? static_cast<int32_t>(r.Value()) : -1;
    });
}

uint32_t Tmc5160Handler::GetChipVersion() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return 0;
    return visitDriverInternal([](auto& drv) -> uint32_t {
        return static_cast<uint32_t>(drv.status.GetChipVersion());
    });
}

void Tmc5160Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) {
        Logger::GetInstance().Warn(TAG, "Not initialized — cannot dump diagnostics");
        return;
    }
    auto& log = Logger::GetInstance();
    log.Info(TAG, "=== TMC5160 Diagnostics ===");
    log.Info(TAG, "  Mode: %s", is_spi_ ? "SPI" : "UART");
    log.Info(TAG, "  Address: %u", static_cast<unsigned>(address_));

    visitDriverInternal([](auto& drv) {
        drv.printer.PrintAll();
    });

    log.Info(TAG, "=== End TMC5160 Diagnostics ===");
}
