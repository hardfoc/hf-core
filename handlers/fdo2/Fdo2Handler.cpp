/**
 * @file Fdo2Handler.cpp
 * @brief Implementation of `Fdo2Handler` and its `BaseUart` adapter.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "Fdo2Handler.h"

#include <cstring>

// MCU-agnostic timing / delay primitives — every handler in hf-core
// goes through the rtos-wrap utilities so the same code runs against
// FreeRTOS / ThreadX / bare-metal stubs without per-platform #ifdefs.
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

//==============================================================================
// HalUartFdo2Comm — BaseUart → fdo2::UartInterface CRTP adapter
//==============================================================================
//
// The driver parses what one `read()` returns as the whole reply: a line
// without its CR is a ProtocolError (FDO2 data sheet §4.1). `read()`
// therefore returns exactly one complete line, CR included, and keeps a
// partial line in `line_` across calls (the split exchange polls with a
// timeout of 0 while a reply arrives). `BaseUart::Read` reports a status,
// not a count, so bytes are taken one at a time while BytesAvailable() > 0.

bool HalUartFdo2Comm::write(const std::uint8_t* data,
                            std::size_t length) noexcept {
    if (length == 0) return true;
    if (data == nullptr || length > 0xFFFFU) return false;
    return uart_.Write(data, static_cast<hf_u16_t>(length), /*timeout_ms=*/0) ==
           hf_uart_err_t::UART_SUCCESS;
}

std::size_t HalUartFdo2Comm::read(std::uint8_t* out, std::size_t max,
                                   std::uint32_t timeout_ms) noexcept {
    if (out == nullptr || max == 0) return 0;

    // All timing goes through rtos-wrap so the handler doesn't take a
    // direct dependency on any specific RTOS API.
    const std::uint32_t start_ms = os_get_elapsed_time_msec();
    for (;;) {
        while (uart_.BytesAvailable() > 0) {
            std::uint8_t byte = 0;
            if (uart_.Read(&byte, 1, /*timeout_ms=*/0) != hf_uart_err_t::UART_SUCCESS) {
                break;
            }
            if (line_len_ == sizeof(line_)) {
                line_len_ = 0;  // longer than any PSUP reply: drop it, resync at the next CR
            }
            line_[line_len_++] = byte;
            if (byte == '\r') {
                const std::size_t n = line_len_ < max ? line_len_ : max;
                std::memcpy(out, line_, n);
                line_len_ = 0;
                return n;
            }
        }
        if (os_get_elapsed_time_msec() - start_ms >= timeout_ms) {
            return 0;  // no whole line yet; the partial line stays in line_
        }
        os_delay_msec(1);
    }
}

void HalUartFdo2Comm::flush_rx() noexcept {
    line_len_ = 0;
    (void)uart_.FlushRx();
}

void HalUartFdo2Comm::delay_ms_impl(std::uint32_t ms) noexcept {
    if (ms == 0) return;
    os_delay_msec(static_cast<std::uint16_t>(ms));
}

//==============================================================================
// Fdo2Handler
//==============================================================================

Fdo2Handler::Fdo2Handler(BaseUart& uart,
                         const Fdo2HandlerConfig& config,
                         RtosMutex* bus_mutex) noexcept
    : config_(config),
      comm_(uart),
      bus_mutex_(bus_mutex == nullptr ? &private_mutex_ : bus_mutex) {
    driver_ = std::make_unique<DriverType>(comm_);
    driver_->SetLineTimeoutMs(config_.line_timeout_ms);
    driver_->SetMeasureTimeoutMs(config_.measure_timeout_ms);
}

bool Fdo2Handler::EnsureInitialized() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    MutexLockGuard lock(*bus_mutex_);
    return EnsureInitializedLocked();
}

bool Fdo2Handler::EnsureInitializedLocked() noexcept {
    if (initialized_.load(std::memory_order_acquire)) return true;
    if (!driver_) return false;

    // FDO2-G2 needs ~1.1 s after power-up before it accepts commands.
    // First call only; subsequent EnsureInitialized() invocations skip
    // the settle.
    static bool waited_settle = false;
    if (!waited_settle) {
        comm_.delay_ms_impl(fdo2::kFdo2G2PowerUpSettleMs);
        waited_settle = true;
    }

    comm_.flush_rx();
    auto rc = driver_->ReadVersion();
    if (!rc.ok()) {
        return false;
    }
    identity_ = rc.value;
    initialized_.store(true, std::memory_order_release);
    return true;
}

fdo2::DriverResult<fdo2::MoxyReading> Fdo2Handler::MeasureMoxy() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return fdo2::DriverResult<fdo2::MoxyReading>::failure(
            fdo2::DriverError::DeviceError);
    }
    return driver_->MeasureMoxy();
}

fdo2::DriverResult<fdo2::MrawReading> Fdo2Handler::MeasureMraw() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return fdo2::DriverResult<fdo2::MrawReading>::failure(
            fdo2::DriverError::DeviceError);
    }
    return driver_->MeasureMraw();
}

fdo2::DriverResult<fdo2::VersionInfo> Fdo2Handler::ReadVersion() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!driver_) {
        return fdo2::DriverResult<fdo2::VersionInfo>::failure(
            fdo2::DriverError::DeviceError);
    }
    auto rc = driver_->ReadVersion();
    if (rc.ok()) {
        identity_ = rc.value;
    }
    return rc;
}

fdo2::DriverResult<std::uint64_t> Fdo2Handler::ReadUniqueId() noexcept {
    MutexLockGuard lock(*bus_mutex_);
    if (!EnsureInitializedLocked()) {
        return fdo2::DriverResult<std::uint64_t>::failure(
            fdo2::DriverError::DeviceError);
    }
    return driver_->ReadUniqueId();
}
