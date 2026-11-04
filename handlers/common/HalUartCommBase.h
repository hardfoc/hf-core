/**
 * @file HalUartCommBase.h
 * @brief `hf::HalUartCommBase`: the standard UART port hooks on a `BaseUart`.
 *
 * Hooks (standards/driver.md), all `bool`: `Write(data, len)`, `Read(data, len, timeout_ms)` (all
 * `len` bytes or `false`), `FlushInput()`, `EnsureInitialized()`, plus `DelayUs()` and `DelayMs()`.
 * A write that fails returns `false` with the `hf_uart_err_t` in `LastBusError()`: it is never
 * silent. The write timeout is `BaseUart`'s default unless the adapter declares
 * `static constexpr std::uint32_t kBusTimeoutMs`.
 *
 * A protocol that frames its own replies (read to a delimiter, keep a partial line) builds that on
 * `Read()` in its adapter; the base stays byte-exact.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "base/BaseUart.h"

namespace hf {

/**
 * @brief UART adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from.
 */
template <typename Derived, typename PortBase = NoPort>
class HalUartCommBase : public HalDelayHooks<PortBase> {
public:
  /// Adapter on @p uart, which must outlive it. Public so an adapter can inherit it.
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): inherited by `using` in the adapter
  explicit HalUartCommBase(BaseUart& uart) noexcept : uart_(uart) {}

  /// Write @p len bytes.
  bool Write(const std::uint8_t* data, std::size_t len) noexcept {
    if (len > kMaxTransferBytes) {
      return note(hf_uart_err_t::UART_ERR_INVALID_PARAMETER);
    }
    return note(uart_.Write(data, static_cast<hf_u16_t>(len), timeoutMs()));
  }
  /// Read exactly @p len bytes within @p timeout_ms.
  bool Read(std::uint8_t* data, std::size_t len, std::uint32_t timeout_ms) noexcept {
    if (len > kMaxTransferBytes) {
      return note(hf_uart_err_t::UART_ERR_INVALID_PARAMETER);
    }
    return note(uart_.Read(data, static_cast<hf_u16_t>(len), timeout_ms));
  }
  /// Discard what is waiting in the receive buffer.
  bool FlushInput() noexcept {
    return note(uart_.FlushRx());
  }
  /// Bring the port up on first use (idempotent).
  bool EnsureInitialized() noexcept {
    return uart_.EnsureInitialized() || note(hf_uart_err_t::UART_ERR_NOT_INITIALIZED);
  }

  /// The code behind the last failed hook; `UART_SUCCESS` when none failed since the last clear.
  [[nodiscard]] hf_uart_err_t LastBusError() const noexcept {
    return last_error_;
  }
  /// `LastBusError()` as a `Result`.
  [[nodiscard]] Result<void> LastBusResult() const noexcept {
    return {ToStatus(last_error_), static_cast<std::uint16_t>(last_error_)};
  }
  /// Forget the last failure.
  void ClearLastBusError() noexcept {
    last_error_ = hf_uart_err_t::UART_SUCCESS;
  }

protected:
  ~HalUartCommBase() = default;

private:
  bool note(hf_uart_err_t error) noexcept {
    if (error == hf_uart_err_t::UART_SUCCESS) {
      return true;
    }
    last_error_ = error;
    return false;
  }
  static constexpr std::uint32_t timeoutMs() noexcept {
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
