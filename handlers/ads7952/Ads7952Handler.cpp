/**
 * @file Ads7952Handler.cpp
 * @brief Implementation of the ADS7952 12-channel SAR ADC handler.
 *
 * Implements the CRTP SPI bridge, BaseAdc interface, lazy initialization,
 * and thread-safe operations for the ADS7952 ADC. Follows the same
 * architecture as As5047uHandler.cpp.
 *
 * @author HardFOC Team
 * @version 1.0
 * @date 2026
 * @copyright HardFOC
 */

#include "Ads7952Handler.h"
#include <cstring>
#include <algorithm>
#include "handlers/logger/Logger.h"
#include "esp_log.h"

//======================================================//
// ADS7952 SPI ADAPTER IMPLEMENTATION
//======================================================//

static constexpr const char* TAG_SPI = "Ads7952Spi";

Ads7952SpiAdapter::Ads7952SpiAdapter(BaseSpi& spi_interface) noexcept
    : spi_interface_(spi_interface) {}

void Ads7952SpiAdapter::transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept {
    if (len == 0) return;

    ESP_LOGD(TAG_SPI, "SPI transfer: len=%u", static_cast<unsigned>(len));

    // Bridge BaseSpi::Transfer ↔ ads7952::SpiInterface<>::transfer
    // BaseSpi handles CS assertion/deassertion per transaction
    hf_spi_err_t result = spi_interface_.Transfer(
        tx, rx,
        static_cast<uint16_t>(len),
        1000  // 1 second timeout
    );

    ESP_LOGD(TAG_SPI, "SPI transfer done: result=%d", static_cast<int>(result));

    // ADS7952 driver detects errors through frame validation
    (void)result;
}

//======================================================//
// ADS7952 HANDLER IMPLEMENTATION
//======================================================//

static constexpr const char* TAG = "Ads7952Handler";

Ads7952Handler::Ads7952Handler(BaseSpi& spi_interface,
                               const Ads7952HandlerConfig& config) noexcept
    : BaseAdc(),
      spi_ref_(spi_interface),
      spi_adapter_(nullptr),
      adc_driver_(nullptr),
      config_(config) {
    snprintf(description_, sizeof(description_), "ADS7952_Handler_SPI_Dev%u", config_.device_index);
}

//======================================================//
// BaseAdc INTERFACE — INITIALIZATION
//======================================================//

bool Ads7952Handler::Initialize() noexcept {
    MutexLockGuard lock(handler_mutex_);

    // Already initialized — return success
    if (initialized_ && adc_driver_) {
        return true;
    }

    Logger::GetInstance().Info(TAG, "[Dev%u] Initializing ADS7952 handler (Vref=%.2fV, VA=%.1fV)",
                              config_.device_index,
                              static_cast<double>(config_.vref),
                              static_cast<double>(config_.va));

    // 1. Create CRTP SPI adapter
    spi_adapter_ = std::make_unique<Ads7952SpiAdapter>(spi_ref_);
    if (!spi_adapter_) {
        Logger::GetInstance().Error(TAG, "[Dev%u] Failed to allocate SPI adapter", config_.device_index);
        return false;
    }

    // 2. Create ADS7952 driver instance with Vref, VA, and initial range
    adc_driver_ = std::make_unique<ads7952::ADS7952<Ads7952SpiAdapter>>(
        *spi_adapter_, config_.vref, config_.va, config_.range);
    if (!adc_driver_) {
        Logger::GetInstance().Error(TAG, "[Dev%u] Failed to allocate ADS7952 driver", config_.device_index);
        spi_adapter_.reset();
        return false;
    }

    // 3. Initialize the driver (sends reset sequence, first-frame discard)
    if (!adc_driver_->EnsureInitialized()) {
        Logger::GetInstance().Error(TAG, "[Dev%u] ADS7952 driver initialization failed", config_.device_index);
        adc_driver_.reset();
        spi_adapter_.reset();
        return false;
    }

    // 4. Apply board-specific configuration
    if (!ApplyConfiguration()) {
        Logger::GetInstance().Warn(TAG, "[Dev%u] Configuration partially applied — continuing", config_.device_index);
        // Non-fatal: driver is initialized, some features may not be configured
    }

    // 5. Verify communication by reading channel 0
    auto test = adc_driver_->ReadChannel(0);
    if (!test.ok()) {
        Logger::GetInstance().Error(TAG, "[Dev%u] Communication test failed (err=%u)",
                                   config_.device_index, static_cast<unsigned>(test.error));
        adc_driver_.reset();
        spi_adapter_.reset();
        return false;
    }

    initialized_ = true;
    total_reads_ = 0;
    error_count_ = 0;

    Logger::GetInstance().Info(TAG, "[Dev%u] ADS7952 initialized (activeVref=%.2fV, mode=%u, test CH0=%u/%.3fV)",
                              config_.device_index,
                              static_cast<double>(adc_driver_->GetActiveVref()),
                              static_cast<unsigned>(adc_driver_->GetMode()),
                              test.count,
                              static_cast<double>(test.voltage));
    return true;
}

bool Ads7952Handler::Deinitialize() noexcept {
    MutexLockGuard lock(handler_mutex_);

    adc_driver_.reset();
    spi_adapter_.reset();
    initialized_ = false;
    total_reads_ = 0;
    error_count_ = 0;

    Logger::GetInstance().Info(TAG, "[Dev%u] Deinitialized", config_.device_index);
    return true;
}

//======================================================//
// BaseAdc INTERFACE — CHANNEL INFO
//======================================================//

hf_u8_t Ads7952Handler::GetMaxChannels() const noexcept {
    return 12;  // ADS7952 has 12 analog input channels
}

bool Ads7952Handler::IsChannelAvailable(hf_channel_id_t channel) const noexcept {
    return channel < 12;
}

//======================================================//
// BaseAdc INTERFACE — READ OPERATIONS
//======================================================//

hf_adc_err_t Ads7952Handler::ReadChannelV(hf_channel_id_t channel, float& voltage,
                                           hf_u8_t samples, hf_time_t /*timeout_ms*/) noexcept {
    if (channel >= 12) return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;

    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;

    float sum = 0.0f;
    const uint8_t n = (samples > 0) ? samples : 1;

    for (uint8_t i = 0; i < n; ++i) {
        auto result = ReadChannelLocked(static_cast<uint8_t>(channel));
        if (!result.ok()) {
            ++error_count_;
            return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
        }
        sum += result.voltage;
    }

    voltage = sum / static_cast<float>(n);
    ++total_reads_;
    statistics_.totalConversions++;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Ads7952Handler::ReadChannelCount(hf_channel_id_t channel, hf_u32_t& count,
                                               hf_u8_t samples, hf_time_t /*timeout_ms*/) noexcept {
    if (channel >= 12) return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;

    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;

    uint32_t sum = 0;
    const uint8_t n = (samples > 0) ? samples : 1;

    for (uint8_t i = 0; i < n; ++i) {
        auto result = ReadChannelLocked(static_cast<uint8_t>(channel));
        if (!result.ok()) {
            ++error_count_;
            return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
        }
        sum += result.count;
    }

    count = sum / n;
    ++total_reads_;
    statistics_.totalConversions++;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Ads7952Handler::ReadChannel(hf_channel_id_t channel, hf_u32_t& count, float& voltage,
                                          hf_u8_t samples, hf_time_t /*timeout_ms*/) noexcept {
    if (channel >= 12) return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;

    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;

    uint32_t count_sum = 0;
    float voltage_sum = 0.0f;
    const uint8_t n = (samples > 0) ? samples : 1;

    for (uint8_t i = 0; i < n; ++i) {
        auto result = ReadChannelLocked(static_cast<uint8_t>(channel));
        if (!result.ok()) {
            ++error_count_;
            return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
        }
        count_sum += result.count;
        voltage_sum += result.voltage;
    }

    count = count_sum / n;
    voltage = voltage_sum / static_cast<float>(n);
    ++total_reads_;
    statistics_.totalConversions++;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Ads7952Handler::ReadMultipleChannels(const hf_channel_id_t* channels,
                                                   hf_u8_t num_channels,
                                                   hf_u32_t* counts,
                                                   float* voltages) noexcept {
    if (!channels || num_channels == 0) return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;

    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;

    // Build a channel mask from the requested channels
    uint16_t mask = 0;
    for (uint8_t i = 0; i < num_channels; ++i) {
        if (channels[i] >= 12) return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
        mask |= static_cast<uint16_t>(1U << channels[i]);
    }

    // Use Auto-1 mode for efficient multi-channel reads
    adc_driver_->ProgramAuto1Channels(mask);
    adc_driver_->EnterAuto1Mode(true);

    auto all = adc_driver_->ReadAllChannels();
    if (!all.ok()) {
        ++error_count_;
        // Restore manual mode
        adc_driver_->EnterManualMode(0);
        return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
    }

    // Extract requested channels from the batch read
    for (uint8_t i = 0; i < num_channels; ++i) {
        uint8_t ch = static_cast<uint8_t>(channels[i]);
        if (all.hasChannel(ch)) {
            if (counts)   counts[i]   = all.count[ch];
            if (voltages) voltages[i] = all.voltage[ch];
        } else {
            if (counts)   counts[i]   = 0;
            if (voltages) voltages[i] = 0.0f;
        }
    }

    // Restore previous mode
    if (config_.initial_mode == ads7952::Mode::Manual) {
        adc_driver_->EnterManualMode(0);
    }

    ++total_reads_;
    statistics_.totalConversions++;
    return hf_adc_err_t::ADC_SUCCESS;
}

//======================================================//
// ADS7952-SPECIFIC METHODS
//======================================================//

bool Ads7952Handler::ReadAllChannels(ads7952::ChannelReadings& readings) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return false;

    // Save current mode and switch to Auto-1 with all channels
    adc_driver_->ProgramAuto1Channels(ads7952::kAllChannels);
    adc_driver_->EnterAuto1Mode(true);

    readings = adc_driver_->ReadAllChannels();

    // Restore manual mode
    adc_driver_->EnterManualMode(0);

    if (readings.ok()) {
        ++total_reads_;
        return true;
    }
    ++error_count_;
    return false;
}

bool Ads7952Handler::ProgramAlarm(uint8_t channel, ads7952::AlarmBound bound,
                                   uint16_t threshold_12bit) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return false;
    return adc_driver_->ProgramAlarm(channel, bound, threshold_12bit);
}

bool Ads7952Handler::ProgramAlarmVoltage(uint8_t channel, ads7952::AlarmBound bound,
                                          float voltage) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return false;
    return adc_driver_->ProgramAlarmVoltage(channel, bound, voltage);
}

bool Ads7952Handler::SetRange(ads7952::Range range) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return false;
    bool ok = adc_driver_->SetRange(range);
    if (ok) config_.range = range;
    return ok;
}

float Ads7952Handler::GetActiveVref() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!adc_driver_) return config_.vref;
    return adc_driver_->GetActiveVref();
}

//======================================================//
// DRIVER ACCESS
//======================================================//

ads7952::ADS7952<Ads7952SpiAdapter>* Ads7952Handler::GetDriver() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return nullptr;
    return adc_driver_.get();
}

const ads7952::ADS7952<Ads7952SpiAdapter>* Ads7952Handler::GetDriver() const noexcept {
    auto* self = const_cast<Ads7952Handler*>(this);
    return self->GetDriver();
}

//======================================================//
// DIAGNOSTICS
//======================================================//

const char* Ads7952Handler::GetDescription() const noexcept {
    return description_;
}

bool Ads7952Handler::GetHandlerDiagnostics(Ads7952Diagnostics& diag) const noexcept {
    MutexLockGuard lock(handler_mutex_);
    diag.initialized = initialized_;
    diag.driver_ready = (adc_driver_ != nullptr);
    diag.current_mode = adc_driver_ ? adc_driver_->GetMode() : ads7952::Mode::Manual;
    diag.current_range = adc_driver_ ? adc_driver_->GetRange() : config_.range;
    diag.vref = config_.vref;
    diag.active_vref = adc_driver_ ? adc_driver_->GetActiveVref() : config_.vref;
    diag.total_reads = total_reads_;
    diag.error_count = error_count_;
    diag.device_index = config_.device_index;
    return true;
}

void Ads7952Handler::DumpDiagnostics() const noexcept {
    auto& log = Logger::GetInstance();
    log.Info(TAG, "=== ADS7952 HANDLER DIAGNOSTICS [Dev%u] ===", config_.device_index);

    MutexLockGuard lock(handler_mutex_);

    log.Info(TAG, "System Health:");
    log.Info(TAG, "  Initialized: %s", initialized_ ? "YES" : "NO");
    log.Info(TAG, "  Driver Instance: %s", adc_driver_ ? "ACTIVE" : "NOT_INITIALIZED");
    log.Info(TAG, "  Description: %s", description_);

    log.Info(TAG, "Configuration:");
    log.Info(TAG, "  Vref: %.3f V", static_cast<double>(config_.vref));
    log.Info(TAG, "  VA: %.1f V", static_cast<double>(config_.va));
    log.Info(TAG, "  Range: %s", config_.range == ads7952::Range::Vref ? "Vref (0-2.5V)" : "2*Vref (0-5.0V)");
    log.Info(TAG, "  Initial Mode: %s",
             config_.initial_mode == ads7952::Mode::Manual ? "Manual" :
             config_.initial_mode == ads7952::Mode::Auto1 ? "Auto-1" : "Auto-2");
    log.Info(TAG, "  Auto1 Mask: 0x%03X", config_.auto1_channel_mask);
    log.Info(TAG, "  Auto2 Last CH: %u", config_.auto2_last_channel);

    if (adc_driver_) {
        log.Info(TAG, "Driver State:");
        log.Info(TAG, "  Active Vref: %.3f V", static_cast<double>(adc_driver_->GetActiveVref()));
        log.Info(TAG, "  Current Mode: %u", static_cast<unsigned>(adc_driver_->GetMode()));
        log.Info(TAG, "  Current Range: %s",
                 adc_driver_->GetRange() == ads7952::Range::Vref ? "Vref" : "2*Vref");
    }

    log.Info(TAG, "Statistics:");
    log.Info(TAG, "  Total Reads: %lu", static_cast<unsigned long>(total_reads_));
    log.Info(TAG, "  Error Count: %lu", static_cast<unsigned long>(error_count_));
    if (total_reads_ > 0) {
        float err_rate = static_cast<float>(error_count_) / static_cast<float>(total_reads_ + error_count_) * 100.0f;
        log.Info(TAG, "  Error Rate: %.2f%%", static_cast<double>(err_rate));
    }

    log.Info(TAG, "Memory:");
    size_t mem = sizeof(*this);
    if (adc_driver_) mem += sizeof(ads7952::ADS7952<Ads7952SpiAdapter>);
    if (spi_adapter_) mem += sizeof(Ads7952SpiAdapter);
    log.Info(TAG, "  Estimated Total: %d bytes", static_cast<int>(mem));

    log.Info(TAG, "=== END ADS7952 HANDLER DIAGNOSTICS ===");
}

//======================================================//
// PRIVATE HELPERS
//======================================================//

bool Ads7952Handler::EnsureInitializedLocked() noexcept {
    if (initialized_ && adc_driver_ && spi_adapter_) {
        return true;
    }
    // Attempt lazy initialization (mutex already held by caller)
    return Initialize();
}

bool Ads7952Handler::ApplyConfiguration() noexcept {
    if (!adc_driver_) return false;

    bool success = true;

    // Set input range
    success &= adc_driver_->SetRange(config_.range);

    // Program Auto-1 channel mask
    success &= adc_driver_->ProgramAuto1Channels(config_.auto1_channel_mask);

    // Program Auto-2 last channel
    success &= adc_driver_->ProgramAuto2LastChannel(config_.auto2_last_channel);

    // Enter configured mode
    switch (config_.initial_mode) {
        case ads7952::Mode::Manual:
            success &= adc_driver_->EnterManualMode(0);
            break;
        case ads7952::Mode::Auto1:
            success &= adc_driver_->EnterAuto1Mode(true);
            break;
        case ads7952::Mode::Auto2:
            success &= adc_driver_->EnterAuto2Mode(true);
            break;
    }

    return success;
}

ads7952::ReadResult Ads7952Handler::ReadChannelLocked(uint8_t channel) noexcept {
    if (!adc_driver_) {
        return ads7952::ReadResult{0, 0.0f, channel, ads7952::Error::NotInitialized};
    }
    return adc_driver_->ReadChannel(channel);
}

//======================================================//
// FACTORY METHOD
//======================================================//

std::unique_ptr<Ads7952Handler> CreateAds7952Handler(
    BaseSpi& spi_interface,
    const Ads7952HandlerConfig& config) noexcept {
    return std::make_unique<Ads7952Handler>(spi_interface, config);
}
