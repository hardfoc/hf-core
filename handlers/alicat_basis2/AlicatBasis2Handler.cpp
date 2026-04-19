/**
 * @file AlicatBasis2Handler.cpp
 * @brief Implementation of AlicatBasis2Handler.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "AlicatBasis2Handler.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace al = alicat_basis2;

//==============================================================================
// HalUartAlicatBasis2Comm
//==============================================================================

void HalUartAlicatBasis2Comm::write(const std::uint8_t* data,
                                    std::size_t length) noexcept {
    if (length == 0 || data == nullptr) return;
    (void)uart_.Write(data, static_cast<hf_u16_t>(length));
}

std::size_t HalUartAlicatBasis2Comm::read(std::uint8_t* out, std::size_t max,
                                          std::uint32_t timeout_ms) noexcept {
    if (out == nullptr || max == 0) return 0;
    // BaseUart::Read returns an error code; on success it has filled `max`
    // bytes (see BaseUart contract).
    const auto rc = uart_.Read(out, static_cast<hf_u16_t>(max),
                               static_cast<hf_u32_t>(timeout_ms));
    return (rc == hf_uart_err_t::UART_SUCCESS) ? max : 0;
}

void HalUartAlicatBasis2Comm::flush_rx() noexcept {
    (void)uart_.FlushRx();
}

void HalUartAlicatBasis2Comm::delay_ms_impl(std::uint32_t ms) noexcept {
    if (ms == 0) return;
    vTaskDelay(pdMS_TO_TICKS(ms));
}

//==============================================================================
// AlicatBasis2Handler
//==============================================================================

AlicatBasis2Handler::AlicatBasis2Handler(BaseUart& uart,
                                         const AlicatBasis2HandlerConfig& config,
                                         RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(uart),
      flow_decimals_(config.flow_decimals_default),
      bus_mutex_(bus_mutex == nullptr ? &private_mutex_ : bus_mutex) {
    driver_ = std::make_unique<DriverType>(comm_, config_.modbus_address,
                                           config_.timeout_ms);
}

bool AlicatBasis2Handler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool AlicatBasis2Handler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    if (!driver_) return false;

    auto rc = driver_->ReadIdentity();
    if (!rc.ok()) return false;
    identity_      = rc.value;
    flow_decimals_ = config_.flow_decimals_default;  // datasheet: not reported via Modbus
    initialized_.store(true, std::memory_order_release);
    return true;
}

al::DriverResult<al::InstantaneousData>
AlicatBasis2Handler::ReadInstantaneous() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return al::DriverResult<al::InstantaneousData>::failure(al::DriverError::NotInitialized);
    }
    return driver_->ReadInstantaneous(flow_decimals_);
}

al::DriverResult<al::InstrumentIdentity>
AlicatBasis2Handler::RereadIdentity() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<al::InstrumentIdentity>::failure(al::DriverError::NotInitialized);
    }
    auto rc = driver_->ReadIdentity();
    if (rc.ok()) identity_ = rc.value;
    return rc;
}

#define BASIS2_FORWARD_VOID(method, ...)                                       \
    do {                                                                       \
        MutexLockGuard lock(*bus_mutex_);                                      \
        if (!EnsureInitializedLocked()) {                                      \
            return al::DriverResult<void>::failure(al::DriverError::NotInitialized); \
        }                                                                      \
        return driver_->method(__VA_ARGS__);                                   \
    } while (0)

#define BASIS2_FORWARD_T(T, method, ...)                                       \
    do {                                                                       \
        MutexLockGuard lock(*bus_mutex_);                                      \
        if (!EnsureInitializedLocked()) {                                      \
            return al::DriverResult<T>::failure(al::DriverError::NotInitialized); \
        }                                                                      \
        return driver_->method(__VA_ARGS__);                                   \
    } while (0)

al::DriverResult<void> AlicatBasis2Handler::Tare() noexcept              { BASIS2_FORWARD_VOID(Tare); }
al::DriverResult<void> AlicatBasis2Handler::SetGas(al::Gas g) noexcept   { BASIS2_FORWARD_VOID(SetGas, g); }

al::DriverResult<void> AlicatBasis2Handler::SetSetpoint(float user_units) noexcept {
    BASIS2_FORWARD_VOID(SetSetpoint, user_units, flow_decimals_);
}

al::DriverResult<void> AlicatBasis2Handler::SetSetpointSource(al::SetpointSource s) noexcept {
    BASIS2_FORWARD_VOID(SetSetpointSource, s);
}

al::DriverResult<void> AlicatBasis2Handler::SetCommWatchdogMs(std::uint16_t ms) noexcept {
    BASIS2_FORWARD_VOID(SetCommWatchdogMs, ms);
}

al::DriverResult<void> AlicatBasis2Handler::ResetTotalizer() noexcept {
    BASIS2_FORWARD_VOID(ResetTotalizer);
}

al::DriverResult<void> AlicatBasis2Handler::SetTotalizerLimitMode(al::TotalizerLimitMode m) noexcept {
    BASIS2_FORWARD_VOID(SetTotalizerLimitMode, m);
}

al::DriverResult<void> AlicatBasis2Handler::SetFlowAveragingMs(std::uint16_t ms) noexcept {
    BASIS2_FORWARD_VOID(SetFlowAveragingMs, ms);
}

al::DriverResult<void> AlicatBasis2Handler::SetReferenceTemperatureC(float c) noexcept {
    BASIS2_FORWARD_VOID(SetReferenceTemperatureC, c);
}

al::DriverResult<void> AlicatBasis2Handler::ConfigureMeasurementTrigger(std::uint16_t bits) noexcept {
    BASIS2_FORWARD_VOID(ConfigureMeasurementTrigger, bits);
}

al::DriverResult<void> AlicatBasis2Handler::StartMeasurementSamples(std::uint16_t n) noexcept {
    BASIS2_FORWARD_VOID(StartMeasurementSamples, n);
}

al::DriverResult<al::MeasurementData> AlicatBasis2Handler::ReadMeasurement() noexcept {
    BASIS2_FORWARD_T(al::MeasurementData, ReadMeasurement);
}

al::DriverResult<void> AlicatBasis2Handler::SetModbusAddress(std::uint8_t addr) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return al::DriverResult<void>::failure(al::DriverError::NotInitialized);
    }
    auto rc = driver_->SetModbusAddress(addr);
    if (rc.ok()) {
        config_.modbus_address = addr;
    }
    return rc;
}

al::DriverResult<void> AlicatBasis2Handler::SetAsciiUnitId(char id) noexcept {
    BASIS2_FORWARD_VOID(SetAsciiUnitId, id);
}

al::DriverResult<void> AlicatBasis2Handler::SetBaudRate(al::BaudRate br) noexcept {
    BASIS2_FORWARD_VOID(SetBaudRate, br);
}

al::DriverResult<void> AlicatBasis2Handler::FactoryRestore() noexcept {
    BASIS2_FORWARD_VOID(FactoryRestore);
}

al::DriverResult<std::uint8_t>
AlicatBasis2Handler::Discover(std::uint8_t* present_bitmap, std::size_t bitmap_bytes,
                              std::uint16_t probe_timeout_ms) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return al::DriverResult<std::uint8_t>::failure(al::DriverError::NotInitialized);
    }
    return driver_->DiscoverPresentAddresses(present_bitmap, bitmap_bytes, probe_timeout_ms);
}

#undef BASIS2_FORWARD_VOID
#undef BASIS2_FORWARD_T
