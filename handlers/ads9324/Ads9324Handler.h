/**
 * @file Ads9324Handler.h
 * @brief hf-core handler for ADS9324 (16-ch 16-bit simultaneous SAR + PGA AFE).
 *
 * Bridges `BaseSpi` plus CONVST (required) and optional DRDY `BaseGpio` into
 * the portable `ads9324` driver. Implements `BaseAdc`.
 *
 * Architecture matches `Ads7952Handler`:
 * - CRTP adapter (`Ads9324SpiHostAdapter`) for zero-overhead SPI + GPIO
 * - Lazy initialization (constructor does not touch the bus)
 * - Exception-free `noexcept` methods
 * - Thread-safe operations under `RtosMutex`
 * - Factory helper for multi-device CS maps
 *
 * `HF_CORE_ENABLE_ADS9324` is **OFF** by default. ESP32 hf-core examples turn
 * the flag ON so this handler compiles in CI.
 *
 * @note CONVST must be non-null. `SetActive()` = CONVST high; conversion
 *       starts on the falling edge.
 * @see docs/handlers/ads9324_handler.md
 * @see ads9324.hpp
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

//======================================================//
// ADS9324 SPI + HOST BRIDGE ADAPTER (CRTP)
//======================================================//

/**
 * @brief CRTP adapter connecting `BaseSpi` + CONVST/DRDY GPIO to the ADS9324 driver.
 *
 * Implements both `ads9324::SpiInterface` and `ads9324::HostInterface` so a
 * single object can be passed as the driver's SpiType and HostType.
 *
 * @note `DelayUs` is a calibrated busy loop (no RTOS microsecond delay in
 *       hf-core). `DelayMs` uses `os_delay_msec`.
 * @warning SPI `Transfer` return codes are currently ignored — the driver
 *          treats config writes as infallible.
 */
class Ads9324SpiHostAdapter : public ads9324::SpiInterface<Ads9324SpiHostAdapter>,
                              public ads9324::HostInterface<Ads9324SpiHostAdapter> {
public:
    /**
     * @brief Bind SPI and conversion pins.
     * @param spi    SPI device (CS owned by `BaseSpi`).
     * @param convst CONVST GPIO (may be nullptr; handler init will then fail).
     * @param drdy   Optional DRDY GPIO; nullptr selects timed CONVST fallback.
     */
    Ads9324SpiHostAdapter(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy) noexcept;

    /**
     * @brief Full-duplex SPI transfer (CRTP dispatch target).
     * @param tx  Transmit buffer, or nullptr to clock zeros.
     * @param rx  Receive buffer, or nullptr to discard MISO.
     * @param len Byte count (3 for config; N×2 or N×3 for conversion).
     */
    void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept;

    /**
     * @brief Drive CONVST.
     * @param high true = `SetActive()` (CONVST high), false = `SetInactive()`.
     */
    void ConvstWrite(bool high) noexcept;

    /**
     * @brief Sample DRDY.
     * @return `IsActive()` of the DRDY pin, or true when DRDY is not wired.
     */
    bool DrdyRead() noexcept;

    /**
     * @brief Whether a DRDY GPIO was supplied.
     * @return false → driver uses `ADS9324_CFG::CONV_FALLBACK_US`.
     */
    bool HasDrdy() const noexcept;

    /**
     * @brief Busy-wait microseconds (nested volatile loop).
     * @param us Delay duration.
     */
    void DelayUs(uint32_t us) noexcept;

    /**
     * @brief Sleep milliseconds via `os_delay_msec` (chunked at 65535 ms).
     * @param ms Delay duration.
     */
    void DelayMs(uint32_t ms) noexcept;

private:
    BaseSpi& spi_;      ///< SPI device.
    BaseGpio* convst_;  ///< CONVST (required at handler Initialize).
    BaseGpio* drdy_;    ///< Optional DRDY; nullptr = timed wait.
};

//======================================================//
// HANDLER CONFIGURATION
//======================================================//

/**
 * @brief Configuration applied at `Ads9324Handler::Initialize`.
 */
struct Ads9324HandlerConfig {
    ads9324::ChannelConfig default_channel{};  ///< PGA applied to all 16 channels at init.
    ads9324::DataFormat format = ads9324::DataFormat::TwosComplement;  ///< GEN_CFG3 coding.
    uint8_t device_index = 0;  ///< Logical index for logs / multi-CS maps.
};

/**
 * @brief Default handler config: differential ±5 V, low PGA bandwidth, two's-complement.
 * @return Value-initialized @ref Ads9324HandlerConfig.
 */
inline Ads9324HandlerConfig GetDefaultAds9324Config() noexcept {
    Ads9324HandlerConfig cfg{};
    cfg.default_channel.type = ads9324::InputType::Differential;
    cfg.default_channel.range = ads9324::InputRange::PlusMinus5V;
    cfg.default_channel.bandwidth = ads9324::PgaBandwidth::Low;
    return cfg;
}

//======================================================//
// HANDLER DIAGNOSTICS
//======================================================//

/**
 * @brief Diagnostic snapshot for one ADS9324 handler instance.
 */
struct Ads9324Diagnostics {
    bool initialized = false;    ///< `BaseAdc` initialized flag.
    bool driver_ready = false;   ///< Driver unique_ptr is non-null.
    uint16_t device_id = 0;      ///< DEVICE_ID read at init (expect 0x0004).
    uint32_t total_reads = 0;    ///< Successful BaseAdc / snapshot operations.
    uint32_t error_count = 0;    ///< Failed reads / snapshots.
    uint8_t device_index = 0;    ///< Copy of @ref Ads9324HandlerConfig::device_index.
};

//======================================================//
// HANDLER CLASS — implements BaseAdc
//======================================================//

/**
 * @brief Thread-safe `BaseAdc` handler for one ADS9324 on a SPI bus.
 *
 * CONVST is required for conversion. DRDY may be nullptr (timed fallback).
 * Construction is lightweight; SPI and GPIO traffic start in @ref Initialize.
 */
class Ads9324Handler : public BaseAdc {
public:
    /**
     * @brief Construct handler (lazy — no SPI).
     * @param spi    SPI device with the ADS9324 CS.
     * @param convst CONVST GPIO; must be non-null at @ref Initialize.
     * @param drdy   Optional DRDY; nullptr uses timed wait after CONVST.
     * @param config Default PGA, data format, device index.
     */
    Ads9324Handler(BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy = nullptr,
                   const Ads9324HandlerConfig& config = GetDefaultAds9324Config()) noexcept;

    /** @brief Releases driver/adapter unique_ptrs. */
    ~Ads9324Handler() noexcept override = default;

    Ads9324Handler(const Ads9324Handler&) = delete;
    Ads9324Handler& operator=(const Ads9324Handler&) = delete;
    Ads9324Handler(Ads9324Handler&&) = delete;
    Ads9324Handler& operator=(Ads9324Handler&&) = delete;

    /**
     * @brief Create adapter + driver, 1-lane SDOUT bring-up, DEVICE_ID, default PGA.
     * @retval true  Device ready.
     * @retval false CONVST missing, `EnsureInitialized` failed, or PGA write failed.
     */
    bool Initialize() noexcept override;

    /**
     * @brief Destroy driver and adapter; leave GPIOs as-is.
     * @return Always true.
     */
    bool Deinitialize() noexcept override;

    /**
     * @brief Channel count for `BaseAdc`.
     * @return 16.
     */
    hf_u8_t GetMaxChannels() const noexcept override;

    /**
     * @brief Whether @p channel is in `[0, 15]` (API 0 = AIN1).
     * @param channel 0-based channel id.
     */
    bool IsChannelAvailable(hf_channel_id_t channel) const noexcept override;

    /**
     * @brief Read voltage (averaged). Internally a full simultaneous snapshot per sample.
     * @param channel    0–15.
     * @param[out] voltage Averaged voltage.
     * @param samples    Average count (`0` treated as 1).
     * @param timeout_ms Unused (kept for `BaseAdc` signature).
     * @return `ADC_SUCCESS` or a `hf_adc_err_t` failure.
     */
    hf_adc_err_t ReadChannelV(hf_channel_id_t channel, float& voltage, hf_u8_t samples = 1,
                              hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief Read raw count (averaged). See @ref ReadChannel for signed-code caveat.
     * @param channel  0–15.
     * @param[out] count Averaged count stored as `uint16_t` inside `hf_u32_t`.
     * @param samples  Average count.
     * @param timeout_ms Unused.
     */
    hf_adc_err_t ReadChannelCount(hf_channel_id_t channel, hf_u32_t& count, hf_u8_t samples = 1,
                                  hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief Read count and voltage (averaged).
     *
     * Each sample calls `ads9324::ADS9324::ReadChannel`, which itself runs a
     * full 16-channel snapshot. Prefer @ref ReadSnapshot or
     * @ref ReadMultipleChannels when several channels are needed.
     *
     * @param channel     0–15.
     * @param[out] count  Mean of signed codes cast through `uint16_t` (two's
     *                    complement bit pattern, not a magnitude).
     * @param[out] voltage Mean voltage.
     * @param samples     Average count.
     * @param timeout_ms  Unused.
     */
    hf_adc_err_t ReadChannel(hf_channel_id_t channel, hf_u32_t& count, float& voltage,
                             hf_u8_t samples = 1, hf_time_t timeout_ms = 0) noexcept override;

    /**
     * @brief One snapshot, then index the requested channels.
     * @param channels     Array of channel ids.
     * @param num_channels Length of @p channels.
     * @param[out] counts  Optional per-channel counts (may be nullptr).
     * @param[out] voltages Optional per-channel voltages (may be nullptr).
     */
    hf_adc_err_t ReadMultipleChannels(const hf_channel_id_t* channels, hf_u8_t num_channels,
                                      hf_u32_t* counts, float* voltages) noexcept override;

    /**
     * @brief Program one channel PGA (mutex-protected).
     * @param channel 0–15.
     * @param cfg     Input type, range, bandwidth, CME.
     * @return true on success.
     */
    bool ConfigureChannel(uint8_t channel, const ads9324::ChannelConfig& cfg) noexcept;

    /**
     * @brief Program OFS_AINx (10-bit two's-complement).
     * @param channel   0–15.
     * @param ofs_10bit Offset field.
     */
    bool SetChannelOffset(uint8_t channel, int16_t ofs_10bit) noexcept;

    /**
     * @brief Program GAN_AINx (14-bit). Correction = GAN / 65536.
     * @param channel   0–15.
     * @param gan_14bit Gain field.
     */
    bool SetChannelGain(uint8_t channel, uint16_t gan_14bit) noexcept;

    /**
     * @brief Simultaneous capture of all enabled channels.
     * @param[out] snap Driver snapshot (valid_mask + counts + voltages).
     * @return true when @p snap reports `ok()`.
     */
    bool ReadSnapshot(ads9324::Snapshot& snap) noexcept;

    /**
     * @brief Raw driver pointer.
     * @return Driver or nullptr if not initialized.
     * @warning Not mutex-protected. Prefer @ref visitDriver.
     */
    ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>* GetDriver() noexcept;

    /** @copydoc GetDriver() */
    const ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>* GetDriver() const noexcept;

    /**
     * @brief Invoke @p fn with the driver while holding the handler mutex.
     * @tparam Fn Callable accepting `ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>&`.
     * @param fn  Visitor (window comparator, power-down, channel-count, …).
     * @return Visitor result, or default-constructed value if the driver is down.
     */
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

    /**
     * @brief Human-readable instance tag (`ADS9324_Handler_SPI_DevN`).
     */
    const char* GetDescription() const noexcept;

    /**
     * @brief Copy diagnostics under the handler mutex.
     * @param[out] diag Filled snapshot.
     * @return Always true.
     */
    bool GetHandlerDiagnostics(Ads9324Diagnostics& diag) const noexcept;

    /** @brief Log CONVST/DRDY presence, DEVICE_ID, and read/error counts. */
    void DumpDiagnostics() const noexcept;

    /**
     * @brief Logical device index from config.
     */
    uint8_t GetDeviceIndex() const noexcept { return config_.device_index; }

private:
    BaseSpi& spi_ref_;           ///< SPI device.
    BaseGpio* convst_;           ///< Required CONVST.
    BaseGpio* drdy_;             ///< Optional DRDY.
    std::unique_ptr<Ads9324SpiHostAdapter> adapter_;  ///< CRTP bridge.
    std::unique_ptr<ads9324::ADS9324<Ads9324SpiHostAdapter, Ads9324SpiHostAdapter>> adc_driver_;
    Ads9324HandlerConfig config_;  ///< Init defaults.
    mutable RtosMutex handler_mutex_;  ///< Serializes all driver access except GetDriver().
    char description_[64]{};       ///< @ref GetDescription buffer.
    mutable uint32_t total_reads_{0};  ///< Successful operations.
    mutable uint32_t error_count_{0};  ///< Failures.
    uint16_t device_id_{0};        ///< Cached DEVICE_ID.

    /**
     * @brief Initialize if needed while @ref handler_mutex_ is already held.
     * @retval false Driver still unavailable.
     */
    bool EnsureInitializedLocked() noexcept;
};

/**
 * @brief Factory: construct an `Ads9324Handler` on the heap.
 * @param spi    SPI device.
 * @param convst CONVST GPIO (required at Initialize).
 * @param drdy   Optional DRDY.
 * @param config Defaults for PGA / format / index.
 * @return Unique pointer (never null unless `bad_alloc`, which is not used here).
 */
std::unique_ptr<Ads9324Handler> CreateAds9324Handler(
    BaseSpi& spi, BaseGpio* convst, BaseGpio* drdy = nullptr,
    const Ads9324HandlerConfig& config = GetDefaultAds9324Config()) noexcept;

#endif
