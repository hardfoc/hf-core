/**
 * @file HalSpiCommBase.h
 * @brief `hf::HalSpiCommBase`: the standard SPI port hooks on a `BaseSpi`.
 *
 * Hooks (standards/driver.md): `Transfer(tx, rx, len)` -> `bool`, one full-duplex transaction with
 * chip select held for its whole length (`tx` or `rx` may be null), `EnsureInitialized()`,
 * `DelayUs()` and `DelayMs()`. `Write()` and `Read()` are one-direction transfers for older
 * drivers.
 *
 * The bus timeout is `BaseSpi`'s default unless the adapter declares
 * `static constexpr std::uint32_t kBusTimeoutMs`.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "base/BaseSpi.h"

namespace hf {

/**
 * @brief SPI adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from.
 */
template <typename Derived, typename PortBase = NoPort>
class HalSpiCommBase : public HalDelayHooks<PortBase> {
public:
  /// Adapter on @p spi, which must outlive it. Public so an adapter can inherit it.
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): inherited by `using` in the adapter
  explicit HalSpiCommBase(BaseSpi& spi) noexcept : spi_(spi) {}

  /// One transaction of @p len bytes.
  bool Transfer(const std::uint8_t* tx, std::uint8_t* rx, std::size_t len) noexcept {
    if (len > kMaxTransferBytes) {
      return note(hf_spi_err_t::SPI_ERR_TRANSFER_TOO_LONG);
    }
    return note(spi_.Transfer(tx, rx, static_cast<hf_u16_t>(len), timeoutMs()));
  }
  /// Write @p len bytes, discarding MISO.
  bool Write(const std::uint8_t* data, std::size_t len) noexcept {
    return Transfer(data, nullptr, len);
  }
  /// Read @p len bytes.
  bool Read(std::uint8_t* data, std::size_t len) noexcept {
    return Transfer(nullptr, data, len);
  }
  /// Bring the bus up on first use (idempotent).
  bool EnsureInitialized() noexcept {
    return spi_.EnsureInitialized() || note(hf_spi_err_t::SPI_ERR_NOT_INITIALIZED);
  }

  /// The code behind the last failed hook; `SPI_SUCCESS` when none failed since the last clear.
  [[nodiscard]] hf_spi_err_t LastBusError() const noexcept {
    return last_error_;
  }
  /// `LastBusError()` as a `Result`.
  [[nodiscard]] Result<void> LastBusResult() const noexcept {
    return {ToStatus(last_error_), static_cast<std::uint16_t>(last_error_)};
  }
  /// Forget the last failure.
  void ClearLastBusError() noexcept {
    last_error_ = hf_spi_err_t::SPI_SUCCESS;
  }

protected:
  ~HalSpiCommBase() = default;

private:
  bool note(hf_spi_err_t error) noexcept {
    if (error == hf_spi_err_t::SPI_SUCCESS) {
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

  BaseSpi& spi_;
  hf_spi_err_t last_error_{hf_spi_err_t::SPI_SUCCESS};
};

} // namespace hf
