/**
 * @file HalSpiComm.h
 * @brief `hf::HalSpiComm`: the standard SPI port hooks on a `BaseSpi`.
 *
 * Hooks: `Transfer(tx, rx, len)`, `Write(data, len)`, `Read(data, len)` -> `bool`, `DelayUs()`,
 * `DelayMs()`, `EnsureInitialized()`. A `false` keeps the `hf_spi_err_t` behind it in `LastBusError()`
 * (and as `LastBusResult()`), so the handler can report the bus code next to the driver's.
 *
 * The bus timeout is `BaseSpi`'s default unless the adapter declares
 * `static constexpr std::uint32_t kBusTimeoutMs`.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include "HalCommBase.h"
#include "IiwStatus.h"
#include "base/BaseSpi.h"

namespace hf {

/**
 * @brief SPI adapter base.
 * @tparam Derived The adapter (CRTP).
 * @tparam PortBase The driver's port base, which this class derives from (default: none).
 */
template <typename Derived, typename PortBase = NoPort>
class HalSpiComm : public HalDelayHooks<PortBase> {
public:
    /// Adapter on @p spi; the object must outlive the adapter.
    /// Public so that an adapter can inherit it (`using HalSpiComm::HalSpiComm;`).
    // NOLINTNEXTLINE(bugprone-crtp-constructor-accessibility)
    explicit HalSpiComm(BaseSpi& spi) noexcept : spi_(spi) {}

    /// Full-duplex transfer of @p len bytes; @p tx or @p rx may be null.
    bool Transfer(const std::uint8_t* tx, std::uint8_t* rx, std::size_t len) noexcept {
        if (len > comm::kMaxTransfer) {
            return Note(hf_spi_err_t::SPI_ERR_TRANSFER_TOO_LONG);
        }
        return Note(spi_.Transfer(tx, rx, static_cast<hf_u16_t>(len), TimeoutMs()));
    }
    /// Write @p len bytes, discarding MISO.
    bool Write(const std::uint8_t* data, std::size_t len) noexcept {
        return Transfer(data, nullptr, len);
    }
    /// Read @p len bytes, clocking out zeros.
    bool Read(std::uint8_t* data, std::size_t len) noexcept {
        return Transfer(nullptr, data, len);
    }
    /// Bring the bus up (idempotent).
    bool EnsureInitialized() noexcept {
        return spi_.EnsureInitialized() || Note(hf_spi_err_t::SPI_ERR_NOT_INITIALIZED);
    }

    /// The code behind the last failed hook, `SPI_SUCCESS` if none failed since `ClearLastBusError()`.
    [[nodiscard]] hf_spi_err_t LastBusError() const noexcept {
        return last_error_;
    }
    /// `LastBusError()` as a `Result`.
    [[nodiscard]] Result<void> LastBusResult() const noexcept {
        return ToResult(last_error_);
    }
    /// Forget the last failure.
    void ClearLastBusError() noexcept {
        last_error_ = hf_spi_err_t::SPI_SUCCESS;
    }
    /// The underlying bus.
    [[nodiscard]] BaseSpi& Spi() noexcept {
        return spi_;
    }

protected:
    ~HalSpiComm() = default;

    /// Keep @p error if it is a failure; true on success.
    bool Note(hf_spi_err_t error) noexcept {
        if (error == hf_spi_err_t::SPI_SUCCESS) {
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

    BaseSpi& spi_;
    hf_spi_err_t last_error_{hf_spi_err_t::SPI_SUCCESS};
};

} // namespace hf
