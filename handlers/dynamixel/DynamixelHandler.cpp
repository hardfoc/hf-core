/**
 * @file DynamixelHandler.cpp
 * @brief Implementation of DynamixelHandler and its BaseUart adapter.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "DynamixelHandler.h"

#include "OsUtility.h"

//==============================================================================
// HalUartDynamixelComm
//==============================================================================

dynamixel::DriverError HalUartDynamixelComm::open(const dynamixel::TransportConfig&) noexcept {
    return uart_.IsInitialized() ? dynamixel::DriverError::None
                                 : dynamixel::DriverError::NotInitialized;
}

void HalUartDynamixelComm::close() noexcept {}

dynamixel::DriverError HalUartDynamixelComm::set_baud_rate(std::uint32_t) noexcept {
    // Host baud is owned by the already-configured BaseUart.
    return dynamixel::DriverError::None;
}

dynamixel::TransportIo HalUartDynamixelComm::write_some(const std::uint8_t* data,
                                                        std::size_t length) noexcept {
    if (data == nullptr || length == 0) {
        return {0, dynamixel::DriverError::InvalidParameter};
    }
    const hf_u16_t n = (length > 0xFFFFu) ? 0xFFFFu : static_cast<hf_u16_t>(length);
    if (uart_.Write(data, n, /*timeout_ms=*/20) != hf_uart_err_t::UART_SUCCESS) {
        return {0, dynamixel::DriverError::SerialError};
    }
    return {static_cast<std::size_t>(n), dynamixel::DriverError::None};
}

dynamixel::DriverError HalUartDynamixelComm::finish_transmit() noexcept {
    return (uart_.FlushTx() == hf_uart_err_t::UART_SUCCESS)
               ? dynamixel::DriverError::None
               : dynamixel::DriverError::SerialError;
}

dynamixel::TransportIo HalUartDynamixelComm::read_some(std::uint8_t* out,
                                                       std::size_t max) noexcept {
    if (out == nullptr || max == 0) {
        return {0, dynamixel::DriverError::None};
    }
    const hf_u16_t avail = uart_.BytesAvailable();
    if (avail == 0) {
        return {0, dynamixel::DriverError::None};
    }
    const hf_u16_t n = (max < avail) ? static_cast<hf_u16_t>(max) : avail;
    if (uart_.Read(out, n, /*timeout_ms=*/0) != hf_uart_err_t::UART_SUCCESS) {
        return {0, dynamixel::DriverError::SerialError};
    }
    return {static_cast<std::size_t>(n), dynamixel::DriverError::None};
}

void HalUartDynamixelComm::discard_stale_input() noexcept {
    (void)uart_.FlushRx();
}

std::uint64_t HalUartDynamixelComm::now_us() noexcept {
    return RtosTime::GetCurrentTimeUs();
}

void HalUartDynamixelComm::delay_ms_impl(std::uint32_t ms) noexcept {
    if (ms == 0) {
        return;
    }
    const std::uint32_t chunk = (ms > 0xFFFFu) ? 0xFFFFu : ms;
    os_delay_msec(static_cast<std::uint16_t>(chunk));
}

//==============================================================================
// DynamixelHandler
//==============================================================================

DynamixelHandler::DynamixelHandler(BaseUart& uart, const DynamixelHandlerConfig& config,
                                   RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(uart),
      bus_{dynamixel::TransportView{comm_}},
      device_(bus_, config.servo_id),
      bus_mutex_(bus_mutex == nullptr ? &private_mutex_ : bus_mutex) {}

bool DynamixelHandler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool DynamixelHandler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) {
        return true;
    }
    const auto opened = bus_.Open(config_.transport);
    if (!opened.ok()) {
        return false;
    }
    auto ping = device_.Identify();
    if (!ping.ok() && ping.error == dynamixel::DriverError::UnsupportedRequest &&
        config_.bind_xseries_fallback && ping.value.model_number != 0) {
        device_.BindXSeriesFallback();
        ping.error = dynamixel::DriverError::None;
    }
    if (!ping.ok()) {
        return false;
    }
    identity_ = ping.value;
    initialized_.store(true, std::memory_order_release);
    return true;
}

void DynamixelHandler::BindModel(const dynamixel::ModelDescriptor& desc) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    device_.BindModel(desc);
}

void DynamixelHandler::BindXSeriesFallback() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    device_.BindXSeriesFallback();
}

dynamixel::DriverResult<dynamixel::PingInfo> DynamixelHandler::Ping() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<dynamixel::PingInfo>::failure(
            dynamixel::DriverError::NotInitialized);
    }
    auto ping = bus_.Ping(config_.servo_id);
    if (ping.ok()) {
        identity_ = ping.value;
    }
    return ping;
}

dynamixel::DriverResult<uint8_t> DynamixelHandler::Scan(uint8_t* out_ids, uint8_t max_ids,
                                                        uint8_t first_id,
                                                        uint8_t last_id) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<uint8_t>::failure(dynamixel::DriverError::NotInitialized);
    }
    return bus_.Scan(out_ids, max_ids, first_id, last_id);
}

dynamixel::DriverResult<void> DynamixelHandler::SetTorqueEnabled(bool enabled) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<void>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.SetTorqueEnabled(enabled);
}

dynamixel::DriverResult<void> DynamixelHandler::SetOperatingMode(
    dynamixel::OperatingMode mode) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<void>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.SetOperatingMode(mode);
}

dynamixel::DriverResult<void> DynamixelHandler::SetMotionProfile(
    uint32_t profile_acceleration, uint32_t profile_velocity) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<void>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.SetMotionProfile(profile_acceleration, profile_velocity);
}

dynamixel::DriverResult<void> DynamixelHandler::SetGoalPosition(int32_t ticks) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<void>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.SetGoalPosition(ticks);
}

dynamixel::DriverResult<void> DynamixelHandler::SetGoalVelocity(int32_t ticks_per_sec) noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<void>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.SetGoalVelocity(ticks_per_sec);
}

dynamixel::DriverResult<int32_t> DynamixelHandler::ReadPosition() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<int32_t>::failure(dynamixel::DriverError::NotInitialized);
    }
    return device_.ReadPosition();
}

dynamixel::DriverResult<dynamixel::Telemetry> DynamixelHandler::ReadTelemetry() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return dynamixel::DriverResult<dynamixel::Telemetry>::failure(
            dynamixel::DriverError::NotInitialized);
    }
    return device_.ReadTelemetry();
}
