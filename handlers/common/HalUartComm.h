/**
 * @file HalUartComm.h
 * @brief `hf::HalUartComm`: the standard UART port hooks on a `BaseUart`.
 *
 * Hooks, all returning `bool`:
 * - `Write(data, len)`: queue @p len bytes for transmission;
 * - `Read(out, len, timeout_ms)`: receive exactly @p len bytes within @p timeout_ms;
 * - `FlushRx()`: drop whatever is waiting in the receive buffer;
 * - `DelayUs()`, `DelayMs()`, `EnsureInitialized()`.
 *
 * A `false` keeps the `hf_uart_err_t` behind it in `LastBusError()`: a write that failed is never
 * silent. The write timeout is `BaseUart`'s default unless the adapter declares
 * `static constexpr std::uint32_t kBusTimeoutMs`.
 *
 * A protocol that frames its own replies (read up to a delimiter, keep a partial line) builds that on
 * `Read()` in its adapter; the base stays byte-exact.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "IiwStatus.h"
#include "base/BaseUart.h"

namespace hf {

/**
 * @brief UART adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from (default: none).
 */
template <typename Derived, typename PortBase = NoPort>
class HalUartComm : public HalDelayHooks<PortBase> {
public:
    /// Adapter on @p uart; the object must outlive the adapter.
    explicit HalUartComm(BaseUart& uart) noexcept : uart_(uart) {}

    /// Write @p len bytes.
    bool Write(const std::uint8_t* data, std::size_t len) noexcept {
        if (len > comm::kMaxTransfer) {
            return Note(hf_uart_err_t::UART_ERR_INVALID_PARAMETER);
        }
        return Note(uart_.Write(data, static_cast<hf_u16_t>(len), TimeoutMs()));
    }
    /// Read exactly @p len bytes, waiting at most @p timeout_ms.
    bool Read(std::uint8_t* out, std::size_t len, std::uint32_t timeout_ms) noexcept {
        if (len > comm::kMaxTransfer) {
            return Note(hf_uart_err_t::UART_ERR_INVALID_PARAMETER);
        }
        return Note(uart_.Read(out, static_cast<hf_u16_t>(len), timeout_ms));
    }
    /// Discard pending received bytes.
    bool FlushRx() noexcept {
        return Note(uart_.FlushRx());
    }
    /// Bring the port up (idempotent).
    bool EnsureInitialized() noexcept {
        return uart_.EnsureInitialized() || Note(hf_uart_err_t::UART_ERR_NOT_INITIALIZED);
    }

    /// The code behind the last failed hook, `UART_SUCCESS` if none failed since `ClearLastBusError()`.
    [[nodiscard]] hf_uart_err_t LastBusError() const noexcept {
        return last_error_;
    }
    /// `LastBusError()` as a `Result`.
    [[nodiscard]] Result<void> LastBusResult() const noexcept {
        return ToResult(last_error_);
    }
    /// Forget the last failure.
    void ClearLastBusError() noexcept {
        last_error_ = hf_uart_err_t::UART_SUCCESS;
    }
    /// The underlying port.
    [[nodiscard]] BaseUart& Uart() noexcept {
        return uart_;
    }

protected:
    ~HalUartComm() = default;

    /// Keep @p error if it is a failure; true on success.
    bool Note(hf_uart_err_t error) noexcept {
        if (error == hf_uart_err_t::UART_SUCCESS) {
            return true;
        }
        last_error_ = error;
        return false;
    }

private:
    static constexpr std::uint32_t TimeoutMs() noexcept {
        if constexpr (requires { Derived::kBusTimeoutMs; }) {
            return Derived::kBusTimeoutMs;
        } else {
            return 0U;
        }
    }

    BaseUart& uart_;
    hf_uart_err_t last_error_{hf_uart_err_t::UART_SUCCESS};
};

} // namespace hf
