/**
 * @file Fdo2Handler.cpp
 * @brief Implementation of `Fdo2Handler` and its `BaseUart` adapter.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#include "Fdo2Handler.h"

// MCU-agnostic timing / delay primitives — every handler in hf-core
// goes through the rtos-wrap utilities so the same code runs against
// FreeRTOS / ThreadX / bare-metal stubs without per-platform #ifdefs.
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

//==============================================================================
// HalUartFdo2Comm — BaseUart → fdo2::UartInterface CRTP adapter
//==============================================================================
//
// `BaseUart::Read(buf, length, timeout)` returns success / failure but
// not how many bytes were actually read (it asks the underlying ESP-IDF
// `uart_read_bytes` for `length` bytes and returns SUCCESS even on
// partial reads). The FDO2 driver however expects
// `read(buf, max, timeout) → actual_count` so it can scan up to a CR.
//
// Workaround: poll `BytesAvailable()` first — with the caller's timeout
// as the deadline for the first byte — then drain whatever arrived
// plus a small grace window. PSUP lines are short (~80 chars at 19200
// baud → ~42 ms for a full line) so the bounded drain works.

void HalUartFdo2Comm::write(const std::uint8_t* data,
                            std::size_t length) noexcept {
    if (length == 0 || data == nullptr) return;
    (void)uart_.Write(data, static_cast<hf_u16_t>(length), /*timeout_ms=*/0);
}

std::size_t HalUartFdo2Comm::read(std::uint8_t* out, std::size_t max,
                                   std::uint32_t timeout_ms) noexcept {
    if (out == nullptr || max == 0) return 0;

    // Wait until at least one byte has landed (blocking, bounded by
    // `timeout_ms`). All timing goes through rtos-wrap so the handler
    // doesn't take a direct dependency on any specific RTOS API.
    const std::uint32_t deadline_ms =
        os_get_elapsed_time_msec() + timeout_ms;
    while (uart_.BytesAvailable() == 0) {
        if (os_get_elapsed_time_msec() >= deadline_ms) {
            return 0;
        }
        os_delay_msec(1);
    }

    // Drain everything currently in the FIFO + a small grace window for
    // the rest of the (in-flight) line to arrive.
    std::size_t got = 0;
    constexpr std::uint16_t kPostFirstByteGraceMs = 5;
    bool waited_grace = false;
    while (got < max) {
        const hf_u16_t avail = uart_.BytesAvailable();
        if (avail == 0) {
            if (waited_grace) break;
            os_delay_msec(kPostFirstByteGraceMs);
            waited_grace = true;
            continue;
        }
        const hf_u16_t to_read = (max - got) < avail
                                   ? static_cast<hf_u16_t>(max - got)
                                   : avail;
        if (uart_.Read(out + got, to_read, /*timeout_ms=*/0) !=
            hf_uart_err_t::UART_SUCCESS) {
            break;
        }
        got += to_read;
    }
    return got;
}

void HalUartFdo2Comm::flush_rx() noexcept {
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
