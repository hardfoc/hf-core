/**
 * @file MockUart.cpp
 * @brief MockUart implementation.
 *
 * @copyright HardFOC
 */

#include "MockUart.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace hf::host {

MockUart::MockUart(hf_port_num_t port) noexcept : BaseUart(port) {}

hf_uart_err_t MockUart::Write(const hf_u8_t* data, hf_u16_t length, hf_u32_t /*timeout_ms*/) noexcept {
    if (data == nullptr && length > 0) {
        return hf_uart_err_t::UART_ERR_NULL_POINTER;
    }
    if (fail_count_ > 0) {
        --fail_count_;
        return fail_error_;
    }
    std::vector<uint8_t> chunk(data, data + length);
    tx_.insert(tx_.end(), chunk.begin(), chunk.end());
    writes_.push_back(chunk);
    if (on_write_) {
        on_write_(*this, chunk);
    }
    return hf_uart_err_t::UART_SUCCESS;
}

hf_uart_err_t MockUart::Read(hf_u8_t* data, hf_u16_t length, hf_u32_t /*timeout_ms*/) noexcept {
    if (data == nullptr) {
        return hf_uart_err_t::UART_ERR_NULL_POINTER;
    }
    if (rx_.size() < length) {
        return hf_uart_err_t::UART_ERR_TIMEOUT;
    }
    for (hf_u16_t i = 0; i < length; ++i) {
        data[i] = rx_.front();
        rx_.pop_front();
    }
    return hf_uart_err_t::UART_SUCCESS;
}

hf_u16_t MockUart::BytesAvailable() noexcept {
    return static_cast<hf_u16_t>(rx_.size() > 0xFFFFU ? 0xFFFFU : rx_.size());
}

hf_uart_err_t MockUart::FlushTx() noexcept { return hf_uart_err_t::UART_SUCCESS; }

hf_uart_err_t MockUart::FlushRx() noexcept {
    ++flush_rx_calls_;
    rx_.clear();
    return hf_uart_err_t::UART_SUCCESS;
}

int MockUart::Printf(const char* format, ...) noexcept {
    char buf[512];
    va_list args;
    va_start(args, format);
    const int n = std::vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    if (n <= 0) {
        return n;
    }
    const auto len = static_cast<hf_u16_t>(std::strlen(buf));
    return Write(reinterpret_cast<const hf_u8_t*>(buf), len) == hf_uart_err_t::UART_SUCCESS ? len : -1;
}

void MockUart::InjectRx(const std::vector<uint8_t>& bytes) { rx_.insert(rx_.end(), bytes.begin(), bytes.end()); }

void MockUart::InjectRx(const char* text) {
    if (text != nullptr) {
        rx_.insert(rx_.end(), text, text + std::strlen(text));
    }
}

void MockUart::FailNextWrite(hf_uart_err_t error, size_t count) noexcept {
    fail_error_ = error;
    fail_count_ = count;
}

void MockUart::ClearTx() noexcept {
    tx_.clear();
    writes_.clear();
}

}  // namespace hf::host
