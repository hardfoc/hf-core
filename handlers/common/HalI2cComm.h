/**
 * @file HalI2cComm.h
 * @brief `hf::HalI2cComm`: the standard I2C port hooks on a `BaseI2c`.
 *
 * Hooks, all returning `bool`:
 * - `Write(data, len)`, `Read(data, len)`, `WriteRead(tx, tx_len, rx, rx_len)`: raw transfers to the
 *   device address the `BaseI2c` was created with;
 * - `WriteRegs(reg, data, len)`: one write of `[reg, data...]` (register auto-increment);
 * - `ReadRegs(reg, out, len)`: write `reg`, repeated start, read `len` bytes;
 * - `DelayUs()`, `DelayMs()`, `EnsureInitialized()`.
 *
 * A `false` keeps the `hf_i2c_err_t` behind it in `LastBusError()`. The bus timeout is `BaseI2c`'s
 * default unless the adapter declares `static constexpr std::uint32_t kBusTimeoutMs`. `WriteRegs()`
 * stages the frame on the stack: at most `kMaxRegisterWrite` data bytes, unless the adapter declares its
 * own `static constexpr std::size_t kMaxRegisterWrite`.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "IiwStatus.h"
#include "base/BaseI2c.h"

namespace hf {

/**
 * @brief I2C adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from (default: none).
 */
template <typename Derived, typename PortBase = NoPort>
class HalI2cComm : public HalDelayHooks<PortBase> {
public:
    /// Adapter on @p i2c; the object must outlive the adapter.
    explicit HalI2cComm(BaseI2c& i2c) noexcept : i2c_(i2c) {}

    /// Default limit on the data bytes of one `WriteRegs()`.
    static constexpr std::size_t kDefaultMaxRegisterWrite = 32;

    /// Write @p len raw bytes.
    bool Write(const std::uint8_t* data, std::size_t len) noexcept {
        if (len > comm::kMaxTransfer) {
            return Note(hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
        }
        return Note(i2c_.Write(data, static_cast<hf_u16_t>(len), TimeoutMs()));
    }
    /// Read @p len raw bytes.
    bool Read(std::uint8_t* data, std::size_t len) noexcept {
        if (len > comm::kMaxTransfer) {
            return Note(hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
        }
        return Note(i2c_.Read(data, static_cast<hf_u16_t>(len), TimeoutMs()));
    }
    /// Write @p tx_len bytes, repeated start, read @p rx_len bytes.
    bool WriteRead(const std::uint8_t* tx, std::size_t tx_len, std::uint8_t* rx, std::size_t rx_len) noexcept {
        if (tx_len > comm::kMaxTransfer || rx_len > comm::kMaxTransfer) {
            return Note(hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
        }
        return Note(i2c_.WriteRead(tx, static_cast<hf_u16_t>(tx_len), rx, static_cast<hf_u16_t>(rx_len), TimeoutMs()));
    }
    /// Write @p len bytes starting at register @p reg, in one transfer.
    bool WriteRegs(std::uint8_t reg, const std::uint8_t* data, std::size_t len) noexcept {
        constexpr std::size_t kMax = MaxRegisterWrite();
        if (len > kMax || (data == nullptr && len != 0U)) {
            return Note(data == nullptr ? hf_i2c_err_t::I2C_ERR_NULL_POINTER : hf_i2c_err_t::I2C_ERR_DATA_TOO_LONG);
        }
        std::uint8_t frame[kMax + 1U];
        frame[0] = reg;
        for (std::size_t i = 0; i < len; ++i) {
            frame[1U + i] = data[i];
        }
        return Write(frame, len + 1U);
    }
    /// Read @p len bytes starting at register @p reg.
    bool ReadRegs(std::uint8_t reg, std::uint8_t* out, std::size_t len) noexcept {
        if (out == nullptr || len == 0U) {
            return Note(hf_i2c_err_t::I2C_ERR_INVALID_PARAMETER);
        }
        return WriteRead(&reg, 1U, out, len);
    }
    /// Bring the bus up (idempotent).
    bool EnsureInitialized() noexcept {
        return i2c_.EnsureInitialized() || Note(hf_i2c_err_t::I2C_ERR_NOT_INITIALIZED);
    }

    /// The code behind the last failed hook, `I2C_SUCCESS` if none failed since `ClearLastBusError()`.
    [[nodiscard]] hf_i2c_err_t LastBusError() const noexcept {
        return last_error_;
    }
    /// `LastBusError()` as a `Result`.
    [[nodiscard]] Result<void> LastBusResult() const noexcept {
        return ToResult(last_error_);
    }
    /// Forget the last failure.
    void ClearLastBusError() noexcept {
        last_error_ = hf_i2c_err_t::I2C_SUCCESS;
    }
    /// The underlying bus.
    [[nodiscard]] BaseI2c& I2c() noexcept {
        return i2c_;
    }

protected:
    ~HalI2cComm() = default;

    /// Keep @p error if it is a failure; true on success.
    bool Note(hf_i2c_err_t error) noexcept {
        if (error == hf_i2c_err_t::I2C_SUCCESS) {
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
    static constexpr std::size_t MaxRegisterWrite() noexcept {
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
