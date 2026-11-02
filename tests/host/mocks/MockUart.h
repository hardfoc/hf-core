/**
 * @file MockUart.h
 * @brief Scriptable BaseUart fake for host tests.
 *
 * @details
 * Written bytes accumulate in TxBytes(); each Write()/Printf() call is also
 * kept as one entry in TxWrites(). InjectRx() appends to the receive buffer
 * that Read() and BytesAvailable() drain. A Read() asking for more bytes
 * than are buffered returns UART_ERR_TIMEOUT and consumes nothing, matching
 * the BaseUart contract that success means the whole buffer was filled.
 * SetOnWrite() lets a test answer a request the moment it is sent (for
 * request/response protocols such as TMCL or Modbus).
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseUart.h"

#include <cstdint>
#include <deque>
#include <functional>
#include <vector>

namespace hf::host {

class MockUart : public BaseUart {
public:
    using OnWrite = std::function<void(MockUart& uart, const std::vector<uint8_t>& written)>;

    explicit MockUart(hf_port_num_t port = 0) noexcept;
    ~MockUart() noexcept override = default;

    // ── BaseUart ──────────────────────────────────────────────────────────
    bool Initialize() noexcept override { return init_result_; }
    bool Deinitialize() noexcept override { return true; }
    hf_uart_err_t Write(const hf_u8_t* data, hf_u16_t length, hf_u32_t timeout_ms = 0) noexcept override;
    hf_uart_err_t Read(hf_u8_t* data, hf_u16_t length, hf_u32_t timeout_ms = 0) noexcept override;
    hf_u16_t BytesAvailable() noexcept override;
    hf_uart_err_t FlushTx() noexcept override;
    hf_uart_err_t FlushRx() noexcept override;
    int Printf(const char* format, ...) noexcept override;

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    void InjectRx(const std::vector<uint8_t>& bytes);
    void InjectRx(const char* text);
    void SetOnWrite(OnWrite on_write) { on_write_ = std::move(on_write); }
    void FailNextWrite(hf_uart_err_t error, size_t count = 1) noexcept;

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<uint8_t>& TxBytes() const noexcept { return tx_; }
    [[nodiscard]] const std::vector<std::vector<uint8_t>>& TxWrites() const noexcept { return writes_; }
    void ClearTx() noexcept;
    [[nodiscard]] int FlushRxCalls() const noexcept { return flush_rx_calls_; }

private:
    bool init_result_{true};
    std::vector<uint8_t> tx_;
    std::vector<std::vector<uint8_t>> writes_;
    std::deque<uint8_t> rx_;
    OnWrite on_write_;
    hf_uart_err_t fail_error_{hf_uart_err_t::UART_ERR_TRANSMISSION_FAILED};
    size_t fail_count_{0};
    int flush_rx_calls_{0};
};

}  // namespace hf::host
