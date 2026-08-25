/**
 * @file Ads9324Handler.cpp
 * @brief ADS9324 handler — CRTP SPI/host bridge + BaseAdc.
 * @copyright HardFOC
 */

#include "Ads9324Handler.h"
#include <cstdio>
#include <cstring>
#include "handlers/logger/Logger.h"
#include "OsUtility.h"

static constexpr const char* TAG = "Ads9324Handler";

Ads9324SpiHostAdapter::Ads9324SpiHostAdapter(BaseSpi& spi, BaseGpio* convst,
                                             BaseGpio* drdy) noexcept
    : spi_(spi), convst_(convst), drdy_(drdy) {}

void Ads9324SpiHostAdapter::transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept {
    if (len == 0) {
        return;
    }
    (void)spi_.Transfer(tx, rx, static_cast<uint16_t>(len), 1000);
}

void Ads9324SpiHostAdapter::ConvstWrite(bool high) noexcept {
    if (convst_ == nullptr) {
        return;
    }
    if (high) {
        (void)convst_->SetActive();
    } else {
        (void)convst_->SetInactive();
    }
}

bool Ads9324SpiHostAdapter::DrdyRead() noexcept {
    if (drdy_ == nullptr) {
        return true;
    }
    bool active = false;
    (void)drdy_->IsActive(active);
    return active;
}

bool Ads9324SpiHostAdapter::HasDrdy() const noexcept { return drdy_ != nullptr; }

void Ads9324SpiHostAdapter::DelayUs(uint32_t us) noexcept {
    for (uint32_t i = 0; i < us; ++i) {
        for (volatile int j = 0; j < 40; ++j) {
        }
    }
}

void Ads9324SpiHostAdapter::DelayMs(uint32_t ms) noexcept {
    while (ms > 0) {
        const uint16_t chunk = static_cast<uint16_t>(ms > 65535u ? 65535u : ms);
        os_delay_msec(chunk);
        ms -= chunk;
    }
}

Ads9324Handler::Ads9324Handler(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy,
                               const Ads9324HandlerConfig& config) noexcept
    : BaseAdc(),
      spi_ref_(spi),
      convst_(convst),
      drdy_(drdy),
      config_(config) {
    snprintf(description_, sizeof(description_), "ADS9324_Handler_SPI_Dev%u", config_.device_index);
}

bool Ads9324Handler::EnsureInitializedLocked() noexcept {
    if (initialized_ && adc_driver_) {
        return true;
    }
    return Initialize();
}

bool Ads9324Handler::Initialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (initialized_ && adc_driver_) {
        return true;
    }

    Logger::GetInstance().Info(TAG, "[Dev%u] Initializing ADS9324 handler", config_.device_index);

    if (convst_ == nullptr) {
        Logger::GetInstance().Error(TAG, "[Dev%u] CONVST GPIO is required", config_.device_index);
        return false;
    }

    adapter_ = std::make_unique<Ads9324SpiHostAdapter>(spi_ref_, convst_, drdy_);
    if (!adapter_) {
        return false;
    }
    adc_driver_ = std::make_unique<
        ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>>(*adapter_, *adapter_);
    if (!adc_driver_) {
        return false;
    }

    if (!adc_driver_->EnsureInitialized()) {
        Logger::GetInstance().Error(TAG, "[Dev%u] ADS9324 EnsureInitialized failed",
                                    config_.device_index);
        adc_driver_.reset();
        adapter_.reset();
        return false;
    }

    if (!adc_driver_->ConfigureAllChannels(config_.default_channel)) {
        Logger::GetInstance().Error(TAG, "[Dev%u] default PGA config failed", config_.device_index);
        adc_driver_.reset();
        adapter_.reset();
        return false;
    }

    if (config_.format != ads9324::DataFormat::TwosComplement) {
        (void)adc_driver_->SetDataFormat(config_.format);
    }

    device_id_ = adc_driver_->ReadDeviceId();
    initialized_ = true;
    Logger::GetInstance().Info(TAG, "[Dev%u] ADS9324 ready (id=0x%04X)", config_.device_index,
                               device_id_);
    return true;
}

bool Ads9324Handler::Deinitialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    adc_driver_.reset();
    adapter_.reset();
    initialized_ = false;
    return true;
}

hf_u8_t Ads9324Handler::GetMaxChannels() const noexcept { return ads9324::kNumChannels; }

bool Ads9324Handler::IsChannelAvailable(hf_channel_id_t channel) const noexcept {
    return channel < ads9324::kNumChannels;
}

hf_adc_err_t Ads9324Handler::ReadChannel(hf_channel_id_t channel, hf_u32_t& count, float& voltage,
                                         hf_u8_t samples, hf_time_t) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        ++error_count_;
        return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;
    }
    if (channel >= ads9324::kNumChannels) {
        ++error_count_;
        return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }

    const uint8_t n = (samples == 0) ? 1 : samples;
    int32_t acc = 0;
    float vacc = 0.0f;
    for (uint8_t i = 0; i < n; ++i) {
        const auto r = adc_driver_->ReadChannel(static_cast<uint8_t>(channel));
        if (!r.ok()) {
            ++error_count_;
            return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
        }
        acc += r.count;
        vacc += r.voltage;
    }
    count = static_cast<hf_u32_t>(static_cast<uint16_t>(acc / n));
    voltage = vacc / static_cast<float>(n);
    ++total_reads_;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Ads9324Handler::ReadChannelV(hf_channel_id_t channel, float& voltage, hf_u8_t samples,
                                          hf_time_t timeout_ms) noexcept {
    hf_u32_t unused = 0;
    return ReadChannel(channel, unused, voltage, samples, timeout_ms);
}

hf_adc_err_t Ads9324Handler::ReadChannelCount(hf_channel_id_t channel, hf_u32_t& count,
                                              hf_u8_t samples, hf_time_t timeout_ms) noexcept {
    float unused = 0.0f;
    return ReadChannel(channel, count, unused, samples, timeout_ms);
}

hf_adc_err_t Ads9324Handler::ReadMultipleChannels(const hf_channel_id_t* channels,
                                                  hf_u8_t num_channels, hf_u32_t* counts,
                                                  float* voltages) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        ++error_count_;
        return hf_adc_err_t::ADC_ERR_NOT_INITIALIZED;
    }
    if (channels == nullptr || num_channels == 0) {
        return hf_adc_err_t::ADC_ERR_INVALID_PARAMETER;
    }
    const ads9324::Snapshot snap = adc_driver_->ReadSnapshot();
    if (!snap.ok()) {
        ++error_count_;
        return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;
    }
    for (hf_u8_t i = 0; i < num_channels; ++i) {
        const auto ch = channels[i];
        if (ch >= ads9324::kNumChannels || !snap.hasChannel(static_cast<uint8_t>(ch))) {
            ++error_count_;
            return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
        }
        if (counts != nullptr) {
            counts[i] = static_cast<hf_u32_t>(static_cast<uint16_t>(snap.count[ch]));
        }
        if (voltages != nullptr) {
            voltages[i] = snap.voltage[ch];
        }
    }
    ++total_reads_;
    return hf_adc_err_t::ADC_SUCCESS;
}

bool Ads9324Handler::ConfigureChannel(uint8_t channel, const ads9324::ChannelConfig& cfg) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        return false;
    }
    return adc_driver_->ConfigureChannel(channel, cfg);
}

bool Ads9324Handler::SetChannelOffset(uint8_t channel, int16_t ofs_10bit) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        return false;
    }
    return adc_driver_->SetChannelOffset(channel, ofs_10bit);
}

bool Ads9324Handler::SetChannelGain(uint8_t channel, uint16_t gan_14bit) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        return false;
    }
    return adc_driver_->SetChannelGain(channel, gan_14bit);
}

bool Ads9324Handler::ReadSnapshot(ads9324::Snapshot& snap) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked() || !adc_driver_) {
        return false;
    }
    snap = adc_driver_->ReadSnapshot();
    if (!snap.ok()) {
        ++error_count_;
        return false;
    }
    ++total_reads_;
    return true;
}

ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>* Ads9324Handler::GetDriver() noexcept {
    return adc_driver_.get();
}

const ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>*
Ads9324Handler::GetDriver() const noexcept {
    return adc_driver_.get();
}

const char* Ads9324Handler::GetDescription() const noexcept { return description_; }

bool Ads9324Handler::GetHandlerDiagnostics(Ads9324Diagnostics& diag) const noexcept {
    MutexLockGuard lock(handler_mutex_);
    diag.initialized = initialized_;
    diag.driver_ready = adc_driver_ != nullptr;
    diag.device_index = config_.device_index;
    diag.total_reads = total_reads_;
    diag.error_count = error_count_;
    diag.device_id = device_id_;
    return true;
}

void Ads9324Handler::DumpDiagnostics() const noexcept {
    auto& log = Logger::GetInstance();
    log.Info(TAG, "=== ADS9324 HANDLER DIAGNOSTICS [Dev%u] ===", config_.device_index);

    MutexLockGuard lock(handler_mutex_);

    log.Info(TAG, "System Health:");
    log.Info(TAG, "  Initialized: %s", initialized_ ? "YES" : "NO");
    log.Info(TAG, "  Driver Instance: %s", adc_driver_ ? "ACTIVE" : "NOT_INITIALIZED");
    log.Info(TAG, "  CONVST GPIO: %s", convst_ ? "present" : "MISSING");
    log.Info(TAG, "  DRDY GPIO: %s", drdy_ ? "present" : "timed fallback");
    log.Info(TAG, "  Description: %s", description_);

    log.Info(TAG, "Configuration:");
    log.Info(TAG, "  Default input type: %u", static_cast<unsigned>(config_.default_channel.type));
    log.Info(TAG, "  Default range: %u", static_cast<unsigned>(config_.default_channel.range));
    log.Info(TAG, "  Data format: %u", static_cast<unsigned>(config_.format));

    log.Info(TAG, "Device:");
    log.Info(TAG, "  DEVICE_ID: 0x%04X (expect 0x%04X)", device_id_, ads9324::kDeviceIdAds9324);

    log.Info(TAG, "Statistics:");
    log.Info(TAG, "  Total Reads: %lu", static_cast<unsigned long>(total_reads_));
    log.Info(TAG, "  Error Count: %lu", static_cast<unsigned long>(error_count_));
}

std::unique_ptr<Ads9324Handler> CreateAds9324Handler(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy,
                                                     const Ads9324HandlerConfig& config) noexcept {
    return std::make_unique<Ads9324Handler>(spi, convst, drdy, config);
}
