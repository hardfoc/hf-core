/**
 * @file MockSpi.h
 * @brief Scriptable BaseSpi fake for host tests.
 *
 * @details
 * Every Transfer() is recorded as an SpiTransaction (one chip-select frame).
 * MISO bytes come from, in order of preference:
 *   1. a queued response (QueueResponse), consumed first-in first-out;
 *   2. a responder callback (SetResponder), for full-duplex protocol models
 *      such as pipelined register reads;
 *   3. FillByte() (default 0x00).
 * FailNext() makes the next N transfers return an error.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseSpi.h"

#include <cstdint>
#include <deque>
#include <functional>
#include <vector>

namespace hf::host {

struct SpiTransaction {
    std::vector<uint8_t> tx;  ///< MOSI bytes (zeros when the caller passed no tx buffer).
    std::vector<uint8_t> rx;  ///< MISO bytes handed back.
    hf_spi_err_t result;
};

class MockSpi : public BaseSpi {
public:
    /** Responder: fill @p rx (len bytes) for the MOSI frame @p tx. */
    using Responder = std::function<void(const uint8_t* tx, uint8_t* rx, uint16_t len)>;

    MockSpi() noexcept = default;
    ~MockSpi() noexcept override = default;

    // ── BaseSpi ───────────────────────────────────────────────────────────
    bool Initialize() noexcept override;
    bool Deinitialize() noexcept override;
    hf_spi_err_t Transfer(const hf_u8_t* tx_data, hf_u8_t* rx_data, hf_u16_t length,
                          hf_u32_t timeout_ms = 0) noexcept override;
    const void* GetDeviceConfig() const noexcept override { return nullptr; }

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    void QueueResponse(std::vector<uint8_t> miso);
    void FailNext(hf_spi_err_t error, size_t count = 1) noexcept;
    void SetResponder(Responder responder) { responder_ = std::move(responder); }
    void SetFillByte(uint8_t fill) noexcept { fill_ = fill; }

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<SpiTransaction>& Transactions() const noexcept { return log_; }
    void ClearTransactions() noexcept { log_.clear(); }
    [[nodiscard]] int InitializeCalls() const noexcept { return init_calls_; }

private:
    bool init_result_{true};
    int init_calls_{0};
    uint8_t fill_{0x00};
    std::deque<std::vector<uint8_t>> queued_;
    hf_spi_err_t fail_error_{hf_spi_err_t::SPI_ERR_TRANSFER_FAILED};
    size_t fail_count_{0};
    Responder responder_;
    std::vector<SpiTransaction> log_;
};

}  // namespace hf::host
