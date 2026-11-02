/**
 * @file MockNvs.h
 * @brief In-memory BaseNvs fake with fault injection, for host tests.
 *
 * @details
 * Values live in memory, typed (u32, string, blob) like ESP-IDF NVS: reading
 * a key as a different type returns NVS_ERR_KEY_NOT_FOUND. Writes are
 * visible at once; Commit() only counts calls, so a test can check that the
 * code under test commits. FailNext() makes the next N operations fail;
 * SetReadOnly() makes every write fail with NVS_ERR_READ_ONLY.
 *
 * @copyright HardFOC
 */

#pragma once

#include "base/BaseNvs.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace hf::host {

class MockNvs : public BaseNvs {
public:
    explicit MockNvs(const char* namespace_name = "mock") noexcept;
    ~MockNvs() noexcept override = default;

    // ── BaseNvs ───────────────────────────────────────────────────────────
    hf_nvs_err_t Initialize() noexcept override { return init_result_; }
    hf_nvs_err_t Deinitialize() noexcept override { return hf_nvs_err_t::NVS_SUCCESS; }
    hf_nvs_err_t SetU32(const char* key, hf_u32_t value) noexcept override;
    hf_nvs_err_t GetU32(const char* key, hf_u32_t& value) noexcept override;
    hf_nvs_err_t SetString(const char* key, const char* value) noexcept override;
    hf_nvs_err_t GetString(const char* key, char* buffer, size_t buffer_size,
                           size_t* actual_size = nullptr) noexcept override;
    hf_nvs_err_t SetBlob(const char* key, const void* data, size_t data_size) noexcept override;
    hf_nvs_err_t GetBlob(const char* key, void* buffer, size_t buffer_size,
                         size_t* actual_size = nullptr) noexcept override;
    hf_nvs_err_t EraseKey(const char* key) noexcept override;
    hf_nvs_err_t Commit() noexcept override;
    bool KeyExists(const char* key) noexcept override;
    hf_nvs_err_t GetSize(const char* key, size_t& size) noexcept override;
    const char* GetDescription() const noexcept override { return "MockNvs"; }
    size_t GetMaxKeyLength() const noexcept override { return kMaxKeyLength; }
    size_t GetMaxValueSize() const noexcept override { return kMaxValueSize; }

    // ── Scripting ─────────────────────────────────────────────────────────
    static constexpr size_t kMaxKeyLength = 15;   ///< Same limit as ESP-IDF NVS.
    static constexpr size_t kMaxValueSize = 4000;
    void SetInitializeResult(hf_nvs_err_t result) noexcept { init_result_ = result; }
    void FailNext(hf_nvs_err_t error, size_t count = 1) noexcept;
    void SetReadOnly(bool read_only = true) noexcept { read_only_ = read_only; }
    void Clear() noexcept { entries_.clear(); }

    // ── Inspection ────────────────────────────────────────────────────────
    [[nodiscard]] int CommitCalls() const noexcept { return commit_calls_; }
    [[nodiscard]] size_t KeyCount() const noexcept { return entries_.size(); }

private:
    enum class Type : uint8_t { U32, String, Blob };
    struct Entry {
        Type type;
        std::vector<uint8_t> bytes;
    };
    hf_nvs_err_t CheckKey(const char* key) const noexcept;
    bool ConsumeFailure(hf_nvs_err_t& error) noexcept;
    hf_nvs_err_t Store(const char* key, Type type, const void* data, size_t size) noexcept;

    hf_nvs_err_t init_result_{hf_nvs_err_t::NVS_SUCCESS};
    std::map<std::string, Entry> entries_;
    hf_nvs_err_t fail_error_{hf_nvs_err_t::NVS_ERR_FAILURE};
    size_t fail_count_{0};
    bool read_only_{false};
    int commit_calls_{0};
};

}  // namespace hf::host
