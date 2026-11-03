/**
 * @file HfStatus.h
 * @brief `hf::Status`: the one error vocabulary shared by every layer above the drivers.
 *
 * Each layer keeps its own detailed codes (a driver's `enum class Error`, an interface's `hf_*_err_t`) and
 * maps them once onto these twelve. Code above the handlers branches on `Status`; the layer's own code
 * travels next to it as `hf::Result::detail()` for logs and diagnostics.
 *
 * Header-only, no heap, no RTOS, no MCU.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstdint>

namespace hf {

/**
 * @brief Outcome of an operation, coarse enough to act on.
 *
 * The numeric values are stable: they appear in logs and diagnostics and must not be renumbered.
 */
enum class Status : std::uint8_t {
    Ok = 0,                 ///< Success.
    NotInitialized = 1,     ///< The object, or the device behind it, has not been initialised or started.
    InvalidArgument = 2,    ///< A parameter, channel, address or configuration value is not valid.
    Busy = 3,               ///< The resource is in use or not ready yet; retrying later can succeed.
    Timeout = 4,            ///< The operation did not finish in time.
    Bus = 5,                ///< A transfer on SPI, I2C, UART, CAN or a pin failed, or the device did not answer.
    NotFound = 6,           ///< The device, key, channel or item does not exist, or the chip ID is wrong.
    Fault = 7,              ///< The device or a lower layer reported a fault, or the data is corrupt.
    Unsupported = 8,        ///< The operation is not supported or not permitted here.
    DependencyNotReady = 9, ///< Something the operation relies on (a bus, a pin, another device) is not ready.
    NoMemory = 10,          ///< Out of memory, buffer space or storage.
    Unknown = 11,           ///< Any failure that has no better code.
};

/// Number of `Status` values; `Unknown` is the last.
inline constexpr std::uint8_t kStatusCount = 12;

/// Name of a status, for logs (`"Ok"`, `"Timeout"`, ...). Never null.
[[nodiscard]] constexpr const char* ToString(Status status) noexcept {
    switch (status) {
    case Status::Ok:
        return "Ok";
    case Status::NotInitialized:
        return "NotInitialized";
    case Status::InvalidArgument:
        return "InvalidArgument";
    case Status::Busy:
        return "Busy";
    case Status::Timeout:
        return "Timeout";
    case Status::Bus:
        return "Bus";
    case Status::NotFound:
        return "NotFound";
    case Status::Fault:
        return "Fault";
    case Status::Unsupported:
        return "Unsupported";
    case Status::DependencyNotReady:
        return "DependencyNotReady";
    case Status::NoMemory:
        return "NoMemory";
    case Status::Unknown:
        return "Unknown";
    }
    return "Unknown";
}

/// True for `Status::Ok`.
[[nodiscard]] constexpr bool IsOk(Status status) noexcept {
    return status == Status::Ok;
}

} // namespace hf
