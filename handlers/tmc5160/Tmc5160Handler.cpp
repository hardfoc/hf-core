/**
 * @file Tmc5160Handler.cpp
 * @brief Implementation of TMC5160 handler with SPI/UART communication adapters.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Tmc5160Handler.h"
#include "Logger.h"
#include "HandlerCommon.h"

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
    handler_utils::RouteLogToLogger(level, tag, format, args);
}

void HalSpiTmc5160Comm::DelayMs(uint32_t ms) noexcept {
    handler_utils::DelayMs(ms);
}

void HalSpiTmc5160Comm::DelayUs(uint32_t us) noexcept {
    handler_utils::DelayUs(us);
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
    handler_utils::RouteLogToLogger(level, tag, format, args);
}

void HalUartTmc5160Comm::DelayMs(uint32_t ms) noexcept {
    handler_utils::DelayMs(ms);
}

void HalUartTmc5160Comm::DelayUs(uint32_t us) noexcept {
    handler_utils::DelayUs(us);
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

tmc51x0::ErrorCode Tmc5160Handler::Initialize(const tmc51x0::DriverConfig& config, bool verbose) noexcept {
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
            return tmc51x0::ErrorCode::NOT_INITIALIZED;
        }
        spi_driver_ = std::make_unique<SpiDriver>(*spi_comm_, address_);
        auto result = spi_driver_->Initialize(config_, verbose);
        if (!result) {
            Logger::GetInstance().Error(TAG, "SPI driver init failed: %s", result.ErrorMessage());
            auto err = result.Error();
            spi_driver_.reset();
            return err;
        }
    } else {
        if (!uart_comm_) {
            Logger::GetInstance().Error(TAG, "UART comm adapter not created");
            return tmc51x0::ErrorCode::NOT_INITIALIZED;
        }
        uart_driver_ = std::make_unique<UartDriver>(*uart_comm_, 0, address_);
        auto result = uart_driver_->Initialize(config_, verbose);
        if (!result) {
            Logger::GetInstance().Error(TAG, "UART driver init failed: %s", result.ErrorMessage());
            auto err = result.Error();
            uart_driver_.reset();
            return err;
        }
    }

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "TMC5160 initialized successfully");
    return tmc51x0::ErrorCode::OK;
}

bool Tmc5160Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

bool Tmc5160Handler::EnsureInitializedLocked() noexcept {
    if (initialized_) {
        return true;
    }
    return Initialize(config_, false) == tmc51x0::ErrorCode::OK;
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

Tmc5160Handler::SpiDriver* Tmc5160Handler::driverViaSpi() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || !is_spi_) {
        return nullptr;
    }
    return spi_driver_.get();
}

const Tmc5160Handler::SpiDriver* Tmc5160Handler::driverViaSpi() const noexcept {
    auto* self = const_cast<Tmc5160Handler*>(this);
    return self->driverViaSpi();
}

Tmc5160Handler::UartDriver* Tmc5160Handler::driverViaUart() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked() || is_spi_) {
        return nullptr;
    }
    return uart_driver_.get();
}

const Tmc5160Handler::UartDriver* Tmc5160Handler::driverViaUart() const noexcept {
    auto* self = const_cast<Tmc5160Handler*>(this);
    return self->driverViaUart();
}


void Tmc5160Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
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
