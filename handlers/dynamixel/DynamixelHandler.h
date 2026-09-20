/**
 * @file DynamixelHandler.h
 * @brief HAL-level handler for Protocol 2.0 Dynamixel servos on BaseUart.
 *
 * @details Bridges the portable `dynamixel::Bus` / `Device` API (from
 *          `hf-core-drivers/external/hf-dynamixel-driver`) to a
 *          `BaseUart&`. Apps that already own a configured UART construct
 *          `DynamixelHandler(uart, cfg)` and consume the narrow API.
 *          The CRTP adapter never leaks above the HAL boundary (same
 *          pattern as `Fdo2Handler` / `AlicatBasis2Handler`).
 *
 *          `EnsureInitialized()` opens the bus and identifies the servo.
 *          It does **not** enable torque or change operating mode.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#ifndef COMPONENT_HANDLER_DYNAMIXEL_HANDLER_H_
#define COMPONENT_HANDLER_DYNAMIXEL_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "RtosMutex.h"
#include "base/BaseUart.h"

#include "dynamixel.hpp"

//==============================================================================
// CRTP UART ADAPTER FOR BaseUart
//==============================================================================

/**
 * @brief CRTP adapter that turns a `BaseUart&` into `dynamixel::Transport`.
 *
 * Electrical mode (DIR GPIO, open-drain, auto-direction) stays with the
 * UART owner. This adapter only moves bytes and waits for TX drain.
 */
class HalUartDynamixelComm : public dynamixel::Transport<HalUartDynamixelComm> {
public:
    explicit HalUartDynamixelComm(BaseUart& uart) noexcept : uart_(uart) {}

    dynamixel::DriverError open(const dynamixel::TransportConfig&) noexcept;
    void close() noexcept;
    dynamixel::DriverError set_baud_rate(std::uint32_t baud) noexcept;
    dynamixel::TransportIo write_some(const std::uint8_t* data, std::size_t length) noexcept;
    dynamixel::DriverError finish_transmit() noexcept;
    dynamixel::TransportIo read_some(std::uint8_t* out, std::size_t max) noexcept;
    void discard_stale_input() noexcept;
    std::uint64_t now_us() noexcept;
    void delay_ms_impl(std::uint32_t ms) noexcept;

private:
    BaseUart& uart_;
};

//==============================================================================
// HANDLER CONFIGURATION
//==============================================================================

struct DynamixelHandlerConfig {
    std::uint8_t              servo_id{1};
    dynamixel::TransportConfig transport{};
    bool                      bind_xseries_fallback{true};
    std::uint8_t              device_index{0};
};

//==============================================================================
// HANDLER
//==============================================================================

/**
 * @brief HAL handler wrapping one Dynamixel servo on a `BaseUart`.
 *
 * Ownership: the host owns the UART. The handler owns the CRTP adapter,
 * `Bus`, and `Device`. Share `bus_mutex` when another protocol uses the
 * same UART (unusual for Dynamixel half-duplex).
 */
class DynamixelHandler {
public:
    explicit DynamixelHandler(BaseUart& uart,
                              const DynamixelHandlerConfig& config = DynamixelHandlerConfig{},
                              RtosMutex* bus_mutex = nullptr) noexcept;

    DynamixelHandler(const DynamixelHandler&) = delete;
    DynamixelHandler& operator=(const DynamixelHandler&) = delete;
    DynamixelHandler(DynamixelHandler&&) = delete;
    DynamixelHandler& operator=(DynamixelHandler&&) = delete;

    bool EnsureInitialized() noexcept;

    [[nodiscard]] bool IsPresent() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    [[nodiscard]] const dynamixel::PingInfo& Identity() const noexcept {
        return identity_;
    }

    [[nodiscard]] const dynamixel::ModelDescriptor* Model() const noexcept {
        return device_.model();
    }

    void BindModel(const dynamixel::ModelDescriptor& desc) noexcept;
    void BindXSeriesFallback() noexcept;

    dynamixel::DriverResult<dynamixel::PingInfo> Ping() noexcept;
    dynamixel::DriverResult<uint8_t> Scan(uint8_t* out_ids, uint8_t max_ids,
                                          uint8_t first_id = 1,
                                          uint8_t last_id = dynamixel::kMaxDeviceId) noexcept;

    dynamixel::DriverResult<void> SetTorqueEnabled(bool enabled) noexcept;
    dynamixel::DriverResult<void> SetOperatingMode(dynamixel::OperatingMode mode) noexcept;
    dynamixel::DriverResult<void> SetMotionProfile(uint32_t profile_acceleration,
                                                   uint32_t profile_velocity) noexcept;
    dynamixel::DriverResult<void> SetGoalPosition(int32_t ticks) noexcept;
    dynamixel::DriverResult<void> SetGoalVelocity(int32_t ticks_per_sec) noexcept;

    dynamixel::DriverResult<int32_t> ReadPosition() noexcept;
    dynamixel::DriverResult<dynamixel::Telemetry> ReadTelemetry() noexcept;

    dynamixel::Bus& bus() noexcept { return bus_; }
    dynamixel::Device& device() noexcept { return device_; }

    template <typename Fn>
    auto visitDevice(Fn&& fn) noexcept {
        MutexLockGuard lock(*bus_mutex_);
        return fn(device_);
    }

private:
    bool EnsureInitializedLocked() noexcept;

    DynamixelHandlerConfig config_;
    HalUartDynamixelComm   comm_;
    dynamixel::Bus         bus_;
    dynamixel::Device      device_;
    dynamixel::PingInfo    identity_{};
    std::atomic<bool>      initialized_{false};
    RtosMutex              private_mutex_;
    RtosMutex*             bus_mutex_;
};

#endif  // COMPONENT_HANDLER_DYNAMIXEL_HANDLER_H_
