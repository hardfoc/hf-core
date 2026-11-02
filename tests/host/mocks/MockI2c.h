/**
 * @file MockI2c.h
 * @brief Scriptable BaseI2c fake for host tests.
 *
 * @details
 * Every call is recorded as an I2cTransaction. Reads are answered, in order
 * of preference, by:
 *   1. a queued response (QueueRead), consumed first-in first-out;
 *   2. a responder callback (SetResponder), for protocol models;
 *   3. the register model, when enabled: a write of [reg, b0, b1, ...] stores
 *      b0.. at reg, reg+1, ...; a WriteRead of [reg] returns reg, reg+1, ...;
 *      a plain Read continues from the last register pointer;
 *   4. zero bytes.
 * FailNext() makes the next N calls (of any kind) return an error without
 * touching data; the failed calls are still recorded.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseI2c.h"

#include <array>
#include <cstdint>
#include <deque>
#include <functional>
#include <vector>

namespace hf::host {

struct I2cTransaction {
    enum class Kind : uint8_t { Write, Read, WriteRead };
    Kind kind;
    uint16_t address;              ///< Device address at the time of the call.
    std::vector<uint8_t> tx;       ///< Bytes written (empty for Read).
    uint16_t rx_length;            ///< Bytes requested (0 for Write).
    std::vector<uint8_t> rx;       ///< Bytes returned to the caller.
    hf_i2c_err_t result;           ///< Value returned to the caller.
};

class MockI2c : public BaseI2c {
public:
    /** Responder: fill @p rx (rx_length bytes) for a Read/WriteRead, or observe a Write. */
    using Responder = std::function<hf_i2c_err_t(const I2cTransaction& txn, uint8_t* rx)>;

    explicit MockI2c(uint16_t device_address = 0x20) noexcept;
    ~MockI2c() noexcept override = default;

    // ── BaseI2c ───────────────────────────────────────────────────────────
    bool Initialize() noexcept override;
    bool Deinitialize() noexcept override;
    hf_i2c_err_t Write(const hf_u8_t* data, hf_u16_t length, hf_u32_t timeout_ms = 0) noexcept override;
    hf_i2c_err_t Read(hf_u8_t* data, hf_u16_t length, hf_u32_t timeout_ms = 0) noexcept override;
    hf_i2c_err_t WriteRead(const hf_u8_t* tx_data, hf_u16_t tx_length, hf_u8_t* rx_data,
                           hf_u16_t rx_length, hf_u32_t timeout_ms = 0) noexcept override;
    hf_u16_t GetDeviceAddress() const noexcept override { return address_; }

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetDeviceAddress(uint16_t address) noexcept { address_ = address; }
    void SetInitializeResult(bool ok) noexcept { init_result_ = ok; }
    void QueueRead(std::vector<uint8_t> bytes, hf_i2c_err_t result = hf_i2c_err_t::I2C_SUCCESS);
    void FailNext(hf_i2c_err_t error, size_t count = 1) noexcept;
    void SetResponder(Responder responder) { responder_ = std::move(responder); }

    void EnableRegisterModel(bool enable = true) noexcept { register_model_ = enable; }
    void SetRegister(uint8_t reg, uint8_t value) noexcept { registers_[reg] = value; }
    [[nodiscard]] uint8_t GetRegister(uint8_t reg) const noexcept { return registers_[reg]; }
    /** Writes to a read-only register are recorded but do not change it. */
    void SetReadOnly(uint8_t reg, bool read_only = true) noexcept { read_only_[reg] = read_only; }

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<I2cTransaction>& Transactions() const noexcept { return log_; }
    void ClearTransactions() noexcept { log_.clear(); }
    [[nodiscard]] size_t CountOf(I2cTransaction::Kind kind) const noexcept;
    /** Number of single-register writes (Write of [reg, value...]) that targeted @p reg. */
    [[nodiscard]] size_t WritesTo(uint8_t reg) const noexcept;
    [[nodiscard]] int InitializeCalls() const noexcept { return init_calls_; }

private:
    hf_i2c_err_t Complete(I2cTransaction& txn, uint8_t* rx);
    void ModelWrite(const std::vector<uint8_t>& tx) noexcept;
    void ModelRead(uint8_t* rx, uint16_t length) noexcept;

    uint16_t address_;
    bool init_result_{true};
    int init_calls_{0};
    bool register_model_{false};
    uint8_t reg_pointer_{0};
    std::array<uint8_t, 256> registers_{};
    std::array<bool, 256> read_only_{};
    struct Queued {
        std::vector<uint8_t> bytes;
        hf_i2c_err_t result;
    };
    std::deque<Queued> queued_;
    hf_i2c_err_t fail_error_{hf_i2c_err_t::I2C_ERR_FAILURE};
    size_t fail_count_{0};
    Responder responder_;
    std::vector<I2cTransaction> log_;
};

}  // namespace hf::host
