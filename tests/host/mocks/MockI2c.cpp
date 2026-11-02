/**
 * @file MockI2c.cpp
 * @brief MockI2c implementation.
 *
 * @copyright HardFOC
 */

#include "MockI2c.h"

#include <algorithm>

namespace hf::host {

MockI2c::MockI2c(uint16_t device_address) noexcept : address_(device_address) {}

bool MockI2c::Initialize() noexcept {
    ++init_calls_;
    return init_result_;
}

bool MockI2c::Deinitialize() noexcept { return true; }

void MockI2c::QueueRead(std::vector<uint8_t> bytes, hf_i2c_err_t result) {
    queued_.push_back({std::move(bytes), result});
}

void MockI2c::FailNext(hf_i2c_err_t error, size_t count) noexcept {
    fail_error_ = error;
    fail_count_ = count;
}

size_t MockI2c::CountOf(I2cTransaction::Kind kind) const noexcept {
    return static_cast<size_t>(std::count_if(log_.begin(), log_.end(),
                                             [kind](const I2cTransaction& t) { return t.kind == kind; }));
}

size_t MockI2c::WritesTo(uint8_t reg) const noexcept {
    return static_cast<size_t>(std::count_if(log_.begin(), log_.end(), [reg](const I2cTransaction& t) {
        return t.kind == I2cTransaction::Kind::Write && t.tx.size() >= 2 && t.tx[0] == reg &&
               t.result == hf_i2c_err_t::I2C_SUCCESS;
    }));
}

void MockI2c::ModelWrite(const std::vector<uint8_t>& tx) noexcept {
    if (tx.empty()) {
        return;
    }
    reg_pointer_ = tx[0];
    for (size_t i = 1; i < tx.size(); ++i) {
        const auto reg = static_cast<uint8_t>(tx[0] + (i - 1));
        if (!read_only_[reg]) {
            registers_[reg] = tx[i];
        }
    }
}

void MockI2c::ModelRead(uint8_t* rx, uint16_t length) noexcept {
    for (uint16_t i = 0; i < length; ++i) {
        rx[i] = registers_[static_cast<uint8_t>(reg_pointer_ + i)];
    }
}

hf_i2c_err_t MockI2c::Complete(I2cTransaction& txn, uint8_t* rx) {
    if (fail_count_ > 0) {
        --fail_count_;
        txn.result = fail_error_;
        log_.push_back(txn);
        return txn.result;
    }

    hf_i2c_err_t result = hf_i2c_err_t::I2C_SUCCESS;
    const bool reads = txn.kind != I2cTransaction::Kind::Write;
    if (reads && !queued_.empty()) {
        Queued q = std::move(queued_.front());
        queued_.pop_front();
        std::fill(rx, rx + txn.rx_length, uint8_t{0});
        std::copy_n(q.bytes.begin(), std::min<size_t>(q.bytes.size(), txn.rx_length), rx);
        result = q.result;
    } else if (responder_) {
        result = responder_(txn, rx);
    } else if (register_model_) {
        if (!txn.tx.empty()) {
            ModelWrite(txn.tx);
        }
        if (reads) {
            ModelRead(rx, txn.rx_length);
        }
    } else if (reads) {
        std::fill(rx, rx + txn.rx_length, uint8_t{0});
    }

    if (reads) {
        txn.rx.assign(rx, rx + txn.rx_length);
    }
    txn.result = result;
    log_.push_back(txn);
    return result;
}

hf_i2c_err_t MockI2c::Write(const hf_u8_t* data, hf_u16_t length, hf_u32_t /*timeout_ms*/) noexcept {
    if (data == nullptr && length > 0) {
        return hf_i2c_err_t::I2C_ERR_NULL_POINTER;
    }
    I2cTransaction txn{I2cTransaction::Kind::Write, address_, {data, data + length}, 0, {},
                       hf_i2c_err_t::I2C_SUCCESS};
    return Complete(txn, nullptr);
}

hf_i2c_err_t MockI2c::Read(hf_u8_t* data, hf_u16_t length, hf_u32_t /*timeout_ms*/) noexcept {
    if (data == nullptr) {
        return hf_i2c_err_t::I2C_ERR_NULL_POINTER;
    }
    I2cTransaction txn{I2cTransaction::Kind::Read, address_, {}, length, {}, hf_i2c_err_t::I2C_SUCCESS};
    return Complete(txn, data);
}

hf_i2c_err_t MockI2c::WriteRead(const hf_u8_t* tx_data, hf_u16_t tx_length, hf_u8_t* rx_data,
                                hf_u16_t rx_length, hf_u32_t /*timeout_ms*/) noexcept {
    if (tx_data == nullptr || rx_data == nullptr) {
        return hf_i2c_err_t::I2C_ERR_NULL_POINTER;
    }
    I2cTransaction txn{I2cTransaction::Kind::WriteRead, address_, {tx_data, tx_data + tx_length},
                       rx_length, {}, hf_i2c_err_t::I2C_SUCCESS};
    return Complete(txn, rx_data);
}

}  // namespace hf::host
