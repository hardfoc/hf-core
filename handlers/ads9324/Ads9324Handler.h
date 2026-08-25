/**
 * @file Ads9324Handler.h
 * @brief hf-core handler for ADS9324 (16-ch 16-bit simultaneous SAR + PGA AFE).
 *
 * Bridges BaseSpi + CONVST (required) and optional DRDY BaseGpio into the
 * portable ads9324 driver. Implements BaseAdc.
 *
 * `HF_CORE_ENABLE_ADS9324` is **OFF** on product images. ESP32 hf-core
 * examples turn the flag ON so this handler compiles in CI. AdcManager
 * does not construct this handler until ADS9324 silicon replaces ADS7952.
 *
 * @copyright HardFOC
 */
#ifndef COMPONENT_HANDLER_ADS9324_HANDLER_H_
#define COMPONENT_HANDLER_ADS9324_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "core/hf-core-drivers/external/hf-ads9324-driver/inc/ads9324.hpp"
#include "base/BaseSpi.h"
#include "base/BaseAdc.h"
#include "base/BaseGpio.h"
#include "RtosMutex.h"

class Ads9324SpiHostAdapter : public ads9324::SpiInterface<Ads9324SpiHostAdapter>,
                              public ads9324::HostInterface<Ads9324SpiHostAdapter> {
public:
    Ads9324SpiHostAdapter(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy) noexcept;

    void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept;
    void ConvstWrite(bool high) noexcept;
    bool DrdyRead() noexcept;
    bool HasDrdy() const noexcept;
    void DelayUs(uint32_t us) noexcept;
    void DelayMs(uint32_t ms) noexcept;

private:
    BaseSpi& spi_;
    BaseGpio* convst_;
    BaseGpio* drdy_;
};

struct Ads9324HandlerConfig {
    ads9324::ChannelConfig default_channel{};
    ads9324::DataFormat format = ads9324::DataFormat::TwosComplement;
    uint8_t device_index = 0;
};

inline Ads9324HandlerConfig GetDefaultAds9324Config() noexcept {
    Ads9324HandlerConfig cfg{};
    cfg.default_channel.type = ads9324::InputType::Differential;
    cfg.default_channel.range = ads9324::InputRange::PlusMinus5V;
    cfg.default_channel.bandwidth = ads9324::PgaBandwidth::Low;
    return cfg;
}

struct Ads9324Diagnostics {
    bool initialized = false;
    bool driver_ready = false;
    uint16_t device_id = 0;
    uint32_t total_reads = 0;
    uint32_t error_count = 0;
    uint8_t device_index = 0;
};

/**
 * @brief Thread-safe BaseAdc handler for one ADS9324 on a SPI bus.
 *
 * CONVST is required for conversion. DRDY may be nullptr (timed fallback).
 */
class Ads9324Handler : public BaseAdc {
public:
    Ads9324Handler(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy = nullptr,
                   const Ads9324HandlerConfig& config = GetDefaultAds9324Config()) noexcept;

    ~Ads9324Handler() noexcept override = default;

    Ads9324Handler(const Ads9324Handler&) = delete;
    Ads9324Handler& operator=(const Ads9324Handler&) = delete;
    Ads9324Handler(Ads9324Handler&&) = delete;
    Ads9324Handler& operator=(Ads9324Handler&&) = delete;

    bool Initialize() noexcept override;
    bool Deinitialize() noexcept override;
    hf_u8_t GetMaxChannels() const noexcept override;
    bool IsChannelAvailable(hf_channel_id_t channel) const noexcept override;
    hf_adc_err_t ReadChannelV(hf_channel_id_t channel, float& voltage, hf_u8_t samples = 1,
                              hf_time_t timeout_ms = 0) noexcept override;
    hf_adc_err_t ReadChannelCount(hf_channel_id_t channel, hf_u32_t& count, hf_u8_t samples = 1,
                                  hf_time_t timeout_ms = 0) noexcept override;
    hf_adc_err_t ReadChannel(hf_channel_id_t channel, hf_u32_t& count, float& voltage,
                             hf_u8_t samples = 1, hf_time_t timeout_ms = 0) noexcept override;
    hf_adc_err_t ReadMultipleChannels(const hf_channel_id_t* channels, hf_u8_t num_channels,
                                      hf_u32_t* counts, float* voltages) noexcept override;

    bool ConfigureChannel(uint8_t channel, const ads9324::ChannelConfig& cfg) noexcept;
    bool SetChannelOffset(uint8_t channel, int16_t ofs_10bit) noexcept;
    bool SetChannelGain(uint8_t channel, uint16_t gan_14bit) noexcept;
    bool ReadSnapshot(ads9324::Snapshot& snap) noexcept;

    ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>* GetDriver() noexcept;
    const ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>* GetDriver() const noexcept;

    template <typename Fn>
    auto visitDriver(Fn&& fn) noexcept -> decltype(fn(
        std::declval<ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>&>())) {
        using ReturnType = decltype(fn(
            std::declval<ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>&>()));
        MutexLockGuard lock(handler_mutex_);
        if (!EnsureInitializedLocked() || !adc_driver_) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        return fn(*adc_driver_);
    }

    const char* GetDescription() const noexcept;
    bool GetHandlerDiagnostics(Ads9324Diagnostics& diag) const noexcept;
    void DumpDiagnostics() const noexcept;
    uint8_t GetDeviceIndex() const noexcept { return config_.device_index; }

private:
    BaseSpi& spi_ref_;
    BaseGpio* convst_;
    BaseGpio* drdy_;
    std::unique_ptr<Ads9324SpiHostAdapter> adapter_;
    std::unique_ptr<ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>> adc_driver_;
    Ads9324HandlerConfig config_;
    mutable RtosMutex handler_mutex_;
    char description_[64]{};
    mutable uint32_t total_reads_{0};
    mutable uint32_t error_count_{0};
    uint16_t device_id_{0};

    bool EnsureInitializedLocked() noexcept;
};

std::unique_ptr<Ads9324Handler> CreateAds9324Handler(
    BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy = nullptr,
    const Ads9324HandlerConfig& config = GetDefaultAds9324Config()) noexcept;

#endif
