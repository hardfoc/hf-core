/**
 * @file MockNvs.cpp
 * @brief MockNvs implementation.
 *
 * @copyright HardFOC
 */

#include "MockNvs.h"

#include <cstring>

namespace hf::host {

MockNvs::MockNvs(const char* namespace_name) noexcept : BaseNvs(namespace_name) {}

void MockNvs::FailNext(hf_nvs_err_t error, size_t count) noexcept {
    fail_error_ = error;
    fail_count_ = count;
}

bool MockNvs::ConsumeFailure(hf_nvs_err_t& error) noexcept {
    if (fail_count_ == 0) {
        return false;
    }
    --fail_count_;
    error = fail_error_;
    return true;
}

hf_nvs_err_t MockNvs::CheckKey(const char* key) const noexcept {
    if (key == nullptr) {
        return hf_nvs_err_t::NVS_ERR_NULL_POINTER;
    }
    const size_t len = std::strlen(key);
    if (len == 0) {
        return hf_nvs_err_t::NVS_ERR_INVALID_PARAMETER;
    }
    if (len > kMaxKeyLength) {
        return hf_nvs_err_t::NVS_ERR_KEY_TOO_LONG;
    }
    return hf_nvs_err_t::NVS_SUCCESS;
}

hf_nvs_err_t MockNvs::Store(const char* key, Type type, const void* data, size_t size) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS) {
        return err;
    }
    if (ConsumeFailure(err)) {
        return err;
    }
    if (read_only_) {
        return hf_nvs_err_t::NVS_ERR_READ_ONLY;
    }
    if (size > kMaxValueSize) {
        return hf_nvs_err_t::NVS_ERR_VALUE_TOO_LARGE;
    }
    const auto* p = static_cast<const uint8_t*>(data);
    entries_[key] = Entry{type, std::vector<uint8_t>(p, p + size)};
    return hf_nvs_err_t::NVS_SUCCESS;
}

hf_nvs_err_t MockNvs::SetU32(const char* key, hf_u32_t value) noexcept {
    return Store(key, Type::U32, &value, sizeof(value));
}

hf_nvs_err_t MockNvs::GetU32(const char* key, hf_u32_t& value) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS || ConsumeFailure(err)) {
        return err;
    }
    const auto it = entries_.find(key);
    if (it == entries_.end() || it->second.type != Type::U32) {
        return hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND;
    }
    std::memcpy(&value, it->second.bytes.data(), sizeof(value));
    return hf_nvs_err_t::NVS_SUCCESS;
}

hf_nvs_err_t MockNvs::SetString(const char* key, const char* value) noexcept {
    if (value == nullptr) {
        return hf_nvs_err_t::NVS_ERR_NULL_POINTER;
    }
    return Store(key, Type::String, value, std::strlen(value) + 1);  // keep the terminator
}

hf_nvs_err_t MockNvs::GetString(const char* key, char* buffer, size_t buffer_size,
                                size_t* actual_size) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS || ConsumeFailure(err)) {
        return err;
    }
    const auto it = entries_.find(key);
    if (it == entries_.end() || it->second.type != Type::String) {
        return hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND;
    }
    const size_t size = it->second.bytes.size();
    if (actual_size != nullptr) {
        *actual_size = size;
    }
    if (buffer == nullptr) {
        return hf_nvs_err_t::NVS_ERR_NULL_POINTER;
    }
    if (buffer_size < size) {
        return hf_nvs_err_t::NVS_ERR_VALUE_TOO_LARGE;
    }
    std::memcpy(buffer, it->second.bytes.data(), size);
    return hf_nvs_err_t::NVS_SUCCESS;
}

hf_nvs_err_t MockNvs::SetBlob(const char* key, const void* data, size_t data_size) noexcept {
    if (data == nullptr && data_size > 0) {
        return hf_nvs_err_t::NVS_ERR_NULL_POINTER;
    }
    return Store(key, Type::Blob, data, data_size);
}

hf_nvs_err_t MockNvs::GetBlob(const char* key, void* buffer, size_t buffer_size,
                              size_t* actual_size) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS || ConsumeFailure(err)) {
        return err;
    }
    const auto it = entries_.find(key);
    if (it == entries_.end() || it->second.type != Type::Blob) {
        return hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND;
    }
    const size_t size = it->second.bytes.size();
    if (actual_size != nullptr) {
        *actual_size = size;
    }
    if (buffer == nullptr) {
        return hf_nvs_err_t::NVS_ERR_NULL_POINTER;
    }
    if (buffer_size < size) {
        return hf_nvs_err_t::NVS_ERR_VALUE_TOO_LARGE;
    }
    std::memcpy(buffer, it->second.bytes.data(), size);
    return hf_nvs_err_t::NVS_SUCCESS;
}

hf_nvs_err_t MockNvs::EraseKey(const char* key) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS || ConsumeFailure(err)) {
        return err;
    }
    if (read_only_) {
        return hf_nvs_err_t::NVS_ERR_READ_ONLY;
    }
    return entries_.erase(key) > 0 ? hf_nvs_err_t::NVS_SUCCESS : hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND;
}

hf_nvs_err_t MockNvs::Commit() noexcept {
    ++commit_calls_;
    hf_nvs_err_t err = hf_nvs_err_t::NVS_SUCCESS;
    if (ConsumeFailure(err)) {
        return err;
    }
    return hf_nvs_err_t::NVS_SUCCESS;
}

bool MockNvs::KeyExists(const char* key) noexcept {
    return key != nullptr && entries_.find(key) != entries_.end();
}

hf_nvs_err_t MockNvs::GetSize(const char* key, size_t& size) noexcept {
    hf_nvs_err_t err = CheckKey(key);
    if (err != hf_nvs_err_t::NVS_SUCCESS) {
        return err;
    }
    const auto it = entries_.find(key);
    if (it == entries_.end()) {
        return hf_nvs_err_t::NVS_ERR_KEY_NOT_FOUND;
    }
    size = it->second.bytes.size();
    return hf_nvs_err_t::NVS_SUCCESS;
}

}  // namespace hf::host
