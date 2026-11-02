/**
 * @file MockSpi.cpp
 * @brief MockSpi implementation.
 *
 * @copyright HardFOC
 */

#include "MockSpi.h"

#include <algorithm>

namespace hf::host {

bool MockSpi::Initialize() noexcept {
    ++init_calls_;
    return init_result_;
}

bool MockSpi::Deinitialize() noexcept { return true; }

void MockSpi::QueueResponse(std::vector<uint8_t> miso) { queued_.push_back(std::move(miso)); }

void MockSpi::FailNext(hf_spi_err_t error, size_t count) noexcept {
    fail_error_ = error;
    fail_count_ = count;
}

hf_spi_err_t MockSpi::Transfer(const hf_u8_t* tx_data, hf_u8_t* rx_data, hf_u16_t length,
                               hf_u32_t /*timeout_ms*/) noexcept {
    SpiTransaction txn{};
    if (tx_data != nullptr) {
        txn.tx.assign(tx_data, tx_data + length);
    } else {
        txn.tx.assign(length, 0);
    }

    if (fail_count_ > 0) {
        --fail_count_;
        txn.result = fail_error_;
        log_.push_back(std::move(txn));
        return fail_error_;
    }

    std::vector<uint8_t> miso(length, fill_);
    if (!queued_.empty()) {
        const std::vector<uint8_t> q = std::move(queued_.front());
        queued_.pop_front();
        std::copy_n(q.begin(), std::min<size_t>(q.size(), length), miso.begin());
    } else if (responder_) {
        responder_(txn.tx.data(), miso.data(), length);
    }
    if (rx_data != nullptr) {
        std::copy(miso.begin(), miso.end(), rx_data);
    }
    txn.rx = std::move(miso);
    txn.result = hf_spi_err_t::SPI_SUCCESS;
    log_.push_back(std::move(txn));
    return hf_spi_err_t::SPI_SUCCESS;
}

}  // namespace hf::host
