/**
 * @file MockCan.h
 * @brief Scriptable BaseCan fake for host tests.
 *
 * @details
 * SendMessage() appends to SentFrames(). InjectRx() makes a frame available:
 * it is handed to the receive callback if one is registered, otherwise it is
 * queued for ReceiveMessage(). The acceptance filter set through
 * SetAcceptanceFilter() is applied to injected frames, as a controller
 * would. FailNextSend() and SetBusOff() script transmit failures.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseCan.h"

#include <cstdint>
#include <deque>
#include <vector>

namespace hf::host {

class MockCan : public BaseCan {
public:
    MockCan() noexcept = default;
    ~MockCan() noexcept override = default;

    // ── BaseCan ───────────────────────────────────────────────────────────
    hf_can_err_t Initialize() noexcept override;
    hf_can_err_t Deinitialize() noexcept override { return hf_can_err_t::CAN_SUCCESS; }
    hf_can_err_t SendMessage(const hf_can_message_t& message, hf_u32_t timeout_ms = 1000) noexcept override;
    hf_can_err_t ReceiveMessage(hf_can_message_t& message, hf_u32_t timeout_ms = 0) noexcept override;
    hf_can_err_t SetReceiveCallback(hf_can_receive_callback_t callback) noexcept override;
    void ClearReceiveCallback() noexcept override { callback_ = nullptr; }
    hf_can_err_t SetAcceptanceFilter(hf_u32_t id, hf_u32_t mask, bool extended = false) noexcept override;
    hf_can_err_t ClearAcceptanceFilter() noexcept override;
    hf_can_err_t GetStatus(hf_can_status_t& status) noexcept override;
    hf_can_err_t Reset() noexcept override;

    // ── Scripting ─────────────────────────────────────────────────────────
    void SetInitializeResult(hf_can_err_t result) noexcept { init_result_ = result; }
    /** Deliver a frame from the bus. Returns false if the acceptance filter dropped it. */
    bool InjectRx(const hf_can_message_t& message);
    void FailNextSend(hf_can_err_t error, size_t count = 1) noexcept;
    void SetBusOff(bool bus_off) noexcept { bus_off_ = bus_off; }

    /** Build a classic data frame. */
    static hf_can_message_t Frame(hf_u32_t id, std::initializer_list<uint8_t> data, bool extended = false) noexcept;

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] const std::vector<hf_can_message_t>& SentFrames() const noexcept { return sent_; }
    void ClearSent() noexcept { sent_.clear(); }
    [[nodiscard]] size_t PendingRx() const noexcept { return rx_.size(); }
    [[nodiscard]] int ResetCalls() const noexcept { return reset_calls_; }

private:
    [[nodiscard]] bool Accepts(const hf_can_message_t& message) const noexcept;

    hf_can_err_t init_result_{hf_can_err_t::CAN_SUCCESS};
    hf_can_receive_callback_t callback_{nullptr};
    std::vector<hf_can_message_t> sent_;
    std::deque<hf_can_message_t> rx_;
    bool filter_set_{false};
    hf_u32_t filter_id_{0};
    hf_u32_t filter_mask_{0};
    bool filter_extended_{false};
    hf_can_err_t fail_error_{hf_can_err_t::CAN_ERR_TX_FAILED};
    size_t fail_count_{0};
    bool bus_off_{false};
    int reset_calls_{0};
    hf_u32_t tx_failed_{0};
};

}  // namespace hf::host
