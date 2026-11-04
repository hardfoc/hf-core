/**
 * @file HalI2cCommBase.h
 * @brief `hf::HalI2cCommBase`: the standard I2C port hooks on a `BaseI2c`.
 *
 * Hooks (standards/driver.md), all `bool`: `Write(addr, data, len)`,
 * `WriteRead(addr, tx, tx_len, rx, rx_len)` (repeated start), `Read(addr, data, len)`,
 * `EnsureInitialized()`, plus `DelayUs()` and `DelayMs()`. A `BaseI2c` is one device: a hook called
 * with another 7-bit address fails with `I2C_ERR_INVALID_ADDRESS` rather than reach the wrong chip.
 *
 * For drivers that still use register hooks: `WriteRegs(reg, data, len)` writes `[reg, data...]` in
 * one transfer and `ReadRegs(reg, out, len)` writes `reg` then reads `len` bytes. `WriteRegs()`
 * stages the frame on the stack: at most `kDefaultMaxRegisterWrite` data bytes, unless the adapter
 * declares `static constexpr std::size_t kMaxRegisterWrite`.
 *
 * The bus timeout is `BaseI2c`'s default unless the adapter declares
 * `static constexpr std::uint32_t kBusTimeoutMs`.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "base/BaseI2c.h"

namespace hf {

/**
 * @brief I2C adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from.
 */
template <typename Derived, typename PortBase = NoPort>
class HalI2cCommBase : public HalDelayHooks<PortBase> {
public:
  /// Default limit on the data bytes of one `WriteRegs()`.
  static constexpr std::size_t kDefaultMaxRegisterWrite = 32;

  /// Adapter on @p i2c, which must outlive it. Public so an adapter can inherit it.
  // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility): inherited by `using` in the adapter
  explicit HalI2cCommBase(BaseI2c& i2c) noexcept : i2c_(i2c) {}

  /// Write @p len bytes to the device at @p addr.
  bool Write(std::uint8_t addr, const std::uint8_t* data, std::size_t len) noexcept {
    if (!checkAddress(addr) || !checkLength(len)) {
      return false;
    }
    return note(i2c_.Write(data, static_cast<hf_u16_t>(len), timeoutMs()));
  }
  /// Read @p len bytes from the device at @p addr.
  bool Read(std::uint8_t addr, std::uint8_t* data, std::size_t len) noexcept {
    if (!checkAddress(addr) || !checkLength(len)) {
      return false;
    }
    return note(i2c_.Read(data, static_cast<hf_u16_t>(len), timeoutMs()));
  }
  /// Write @p tx_len bytes, repeated start, read @p rx_len bytes.
  bool WriteRead(std::uint8_t addr, const std::uint8_t* tx, std::size_t tx_len, std::uint8_t* rx,
                 std::size_t rx_len) noexcept {
    if (!checkAddress(addr) || !checkLength(tx_len) || !checkLength(rx_len)) {
      return false;
    }
    return note(i2c_.WriteRead(tx, static_cast<hf_u16_t>(tx_len), rx, static_cast<hf_u16_t>(rx_len),
                               timeoutMs()));
  }
  /// Write @p len bytes starting at register @p reg, in one transfer.
  bool WriteRegs(std::uint8_t reg, const std::uint8_t* data, std::size_t len) noexcept {
    constexpr std::size_t kMax = maxRegisterWrite();
    if (data == nullptr && len != 0U) {
      return note(hf_i2c_err_t::I2C_ERR_NULL_POINTER);
    }
    if (len > kMax) {
      return note(hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
    }
    std::uint8_t frame[kMax + 1U]; // NOLINT(cppcoreguidelines-avoid-c-arrays): fixed stack frame
    frame[0] = reg;
    for (std::size_t i = 0; i < len; ++i) {
      frame[1U + i] = data[i];
    }
    return note(i2c_.Write(frame, static_cast<hf_u16_t>(len + 1U), timeoutMs()));
  }
  /// Read @p len bytes starting at register @p reg.
  bool ReadRegs(std::uint8_t reg, std::uint8_t* out, std::size_t len) noexcept {
    if (out == nullptr || len == 0U) {
      return note(hf_i2c_err_t::I2C_ERR_INVALID_PARAMETER);
    }
    if (!checkLength(len)) {
      return false;
    }
    return note(i2c_.WriteRead(&reg, 1U, out, static_cast<hf_u16_t>(len), timeoutMs()));
  }
  /// Bring the bus up on first use (idempotent).
  bool EnsureInitialized() noexcept {
    return i2c_.EnsureInitialized() || note(hf_i2c_err_t::I2C_ERR_NOT_INITIALIZED);
  }

  /// The 7-bit address of the device this adapter talks to.
  [[nodiscard]] std::uint8_t Address() const noexcept {
    return static_cast<std::uint8_t>(i2c_.GetDeviceAddress());
  }
  /// The code behind the last failed hook; `I2C_SUCCESS` when none failed since the last clear.
  [[nodiscard]] hf_i2c_err_t LastBusError() const noexcept {
    return last_error_;
  }
  /// `LastBusError()` as a `Result`.
  [[nodiscard]] Result<void> LastBusResult() const noexcept {
    return {ToStatus(last_error_), static_cast<std::uint16_t>(last_error_)};
  }
  /// Forget the last failure.
  void ClearLastBusError() noexcept {
    last_error_ = hf_i2c_err_t::I2C_SUCCESS;
  }

protected:
  ~HalI2cCommBase() = default;

private:
  bool note(hf_i2c_err_t error) noexcept {
    if (error == hf_i2c_err_t::I2C_SUCCESS) {
      return true;
    }
    last_error_ = error;
    return false;
  }
  bool checkAddress(std::uint8_t addr) noexcept {
    return addr == Address() || note(hf_i2c_err_t::I2C_ERR_INVALID_ADDRESS);
  }
  bool checkLength(std::size_t len) noexcept {
    return len <= kMaxTransferBytes || note(hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
  }
  static constexpr std::uint32_t timeoutMs() noexcept {
    if constexpr (requires { Derived::kBusTimeoutMs; }) {
      return Derived::kBusTimeoutMs;
    } else {
      return 0U;
    }
  }
  static constexpr std::size_t maxRegisterWrite() noexcept {
    if constexpr (requires { Derived::kMaxRegisterWrite; }) {
      return Derived::kMaxRegisterWrite;
    } else {
      return kDefaultMaxRegisterWrite;
    }
  }

  BaseI2c& i2c_;
  hf_i2c_err_t last_error_{hf_i2c_err_t::I2C_SUCCESS};
};

} // namespace hf
