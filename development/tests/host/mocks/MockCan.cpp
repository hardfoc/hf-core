/**
 * @file MockCan.cpp
 * @brief MockCan implementation.
 *
 * @copyright HardFOC
 */

#include "MockCan.h"

#include <algorithm>

namespace hf::host {

hf_can_err_t MockCan::Initialize() noexcept { return init_result_; }

hf_can_err_t MockCan::SendMessage(const hf_can_message_t& message, hf_u32_t /*timeout_ms*/) noexcept {
    if (bus_off_) {
        ++tx_failed_;
        return hf_can_err_t::CAN_ERR_BUS_OFF;
    }
    if (fail_count_ > 0) {
        --fail_count_;
        ++tx_failed_;
        return fail_error_;
    }
    if (message.dlc > message.GetMaxDataLength()) {
        return hf_can_err_t::CAN_ERR_MESSAGE_INVALID_DLC;
    }
    sent_.push_back(message);
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::ReceiveMessage(hf_can_message_t& message, hf_u32_t /*timeout_ms*/) noexcept {
    if (rx_.empty()) {
        return hf_can_err_t::CAN_ERR_QUEUE_EMPTY;
    }
    message = rx_.front();
    rx_.pop_front();
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::SetReceiveCallback(hf_can_receive_callback_t callback) noexcept {
    callback_ = callback;
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::SetAcceptanceFilter(hf_u32_t id, hf_u32_t mask, bool extended) noexcept {
    filter_set_ = (mask != 0U);
    filter_id_ = id;
    filter_mask_ = mask;
    filter_extended_ = extended;
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::ClearAcceptanceFilter() noexcept {
    filter_set_ = false;
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::GetStatus(hf_can_status_t& status) noexcept {
    status = hf_can_status_t{};
    status.bus_off = bus_off_;
    status.tx_failed_count = tx_failed_;
    return hf_can_err_t::CAN_SUCCESS;
}

hf_can_err_t MockCan::Reset() noexcept {
    ++reset_calls_;
    bus_off_ = false;
    rx_.clear();
    return hf_can_err_t::CAN_SUCCESS;
}

bool MockCan::Accepts(const hf_can_message_t& message) const noexcept {
    if (!filter_set_) {
        return true;
    }
    if (message.is_extended != filter_extended_) {
        return false;
    }
    return (message.id & filter_mask_) == (filter_id_ & filter_mask_);
}

bool MockCan::InjectRx(const hf_can_message_t& message) {
    if (!Accepts(message)) {
        return false;
    }
    if (callback_ != nullptr) {
        callback_(message);
    } else {
        rx_.push_back(message);
    }
    return true;
}

void MockCan::FailNextSend(hf_can_err_t error, size_t count) noexcept {
    fail_error_ = error;
    fail_count_ = count;
}

hf_can_message_t MockCan::Frame(hf_u32_t id, std::initializer_list<uint8_t> data, bool extended) noexcept {
    hf_can_message_t msg{};
    msg.id = id;
    msg.is_extended = extended;
    msg.dlc = static_cast<hf_u8_t>(std::min<size_t>(data.size(), 8U));
    std::copy_n(data.begin(), msg.dlc, msg.data);
    return msg;
}

}  // namespace hf::host
