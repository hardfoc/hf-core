/**
 * @file Tle92466edHandler.h
 * @brief Unified handler for TLE92466ED six-channel low-side solenoid driver IC.
 *
 * @details
 * Provides HAL-level integration for the TLE92466ED gate driver using BaseSpi and BaseGpio.
 * Tracks **hf-tle92466ed-driver** `tle92466ed::Driver<>` (PWM period clamp to datasheet range,
 * dither + `DITHER_CLK_DIV` configuration, FB_DC / FB_I_AVG decode, corrected `SetReset` polarity).
 * Features:
 * - CRTP SPI adapter bridging BaseSpi to the TLE92466ED driver (`SpiInterface` CRTP)
 * - 6-channel solenoid/valve control with current regulation
 * - Comprehensive fault monitoring and diagnostic reporting
 * - SPI watchdog management
 * - Thread-safe operations with RtosMutex
 * - Lazy initialization pattern
 * - @ref WithDriver for production access to advanced driver APIs
 *   (`ConfigureDither`, `GetAverageCurrent`, rail reads, …); @ref GetDriver is
 *   bring-up / bench only — see `docs/handlers/tle92466ed_handler.md`
 *
 * @note **RESN / EN semantics:** RESN is active-low. With `HF_GPIO_ACTIVE_LOW`,
 *       `GpioSignal::ACTIVE` / `SetReset(true)` → physical **LOW** (in reset);
 *       `INACTIVE` / `SetReset(false)` → physical **HIGH** (released). EN is
 *       active-high. Configure each `BaseGpio` active state to match the schematic.
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_TLE92466ED_HANDLER_H_
#define COMPONENT_HANDLER_TLE92466ED_HANDLER_H_

#include <cstdint>
#include <memory>
#include <cstdarg>
#include <span>
#include <type_traits>
#include <utility>
#include "core/hf-core-drivers/external/hf-tle92466ed-driver/inc/tle92466ed.hpp"
#include "core/hf-core-drivers/external/hf-tle92466ed-driver/inc/tle92466ed_spi_interface.hpp"
#include "base/BaseSpi.h"
#include "base/BaseGpio.h"
#include "RtosMutex.h"

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TLE92466ED_HAL_CommAdapter HAL Communication Adapter
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class HalSpiTle92466edComm
 * @brief CRTP SPI communication adapter for TLE92466ED using BaseSpi and BaseGpio.
 *
 * Implements all methods required by tle92466ed::SpiInterface<HalSpiTle92466edComm>.
 *
 * @note Bring-up order: @c Init holds EN LOW only for the RESN pulse, then
 *       asserts EN HIGH before the SPI identity read. Channel outputs remain
 *       gated by CH_CTRL after POR — EN high is not “outputs energized”.
 *       When EN/RESN are expander-backed, claim ownership of those pins before
 *       constructing this adapter.
 */
class HalSpiTle92466edComm : public tle92466ed::SpiInterface<HalSpiTle92466edComm> {
public:
    /**
     * @brief Construct the SPI adapter.
     * @param spi    Reference to pre-configured BaseSpi (Mode 1, 32-bit frames).
     * @param resn   BaseGpio connected to RESN (reset, active LOW).
     * @param en     BaseGpio connected to EN (enable, active HIGH).
     * @param faultn Optional BaseGpio connected to FAULTN (active LOW input).
     */
    HalSpiTle92466edComm(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                          BaseGpio* faultn = nullptr) noexcept;

    /// @name CRTP-Required Methods
    /// @{

    // Bring base class variadic Log into scope (avoids name hiding)
    using tle92466ed::SpiInterface<HalSpiTle92466edComm>::Log;

    /**
     * @brief Reset pulse, assert EN, then SPI identity / fault clear.
     * @return Failure leaves last RX in @ref LastRxWord for CDC diag.
     */
    tle92466ed::CommResult<void> Init() noexcept;

    /** @brief Mark adapter deinitialized; does not change GPIO or SPI state. */
    tle92466ed::CommResult<void> Deinit() noexcept;

    /**
     * @brief Single 32-bit SPI frame (Mode 1, MSB first).
     * @param tx_data Command or data word to shift out.
     * @return RX word from this CS window (pipelined reply from the prior frame).
     * @note Inserts @c 20 µs between CS cycles; prefer @ref TransferMulti for reads.
     */
    tle92466ed::CommResult<uint32_t> Transfer32(uint32_t tx_data) noexcept;

    /**
     * @brief Pipelined multi-frame SPI transfer under one SPI2 bus lock.
     * @param tx_data TX words (command frame followed by dummy frame(s) for reads).
     * @param rx_data RX buffer, same length as @p tx_data.
     * @return @ref CommError::TransferError if @ref BaseSpi::TransferChain fails.
     * @warning @p tx_data and @p rx_data must be equal length and non-empty.
     *          Releasing the bus between frames lets a peer consume this device's
     *          pipeline slot (sticky-zero reads).
     */
    tle92466ed::CommResult<void> TransferMulti(std::span<const uint32_t> tx_data,
                                                std::span<uint32_t> rx_data) noexcept;

    /** @brief Busy-wait delay in microseconds (datasheet timing gaps). */
    tle92466ed::CommResult<void> Delay(uint32_t microseconds) noexcept;

    /** @brief No-op; @p BaseSpi is pre-configured for Mode 1 / 32-bit frames. */
    tle92466ed::CommResult<void> Configure(const tle92466ed::SPIConfig& config) noexcept;

    /** @brief True after @ref Init succeeded (GPIO polarity parked, SPI ready). */
    [[nodiscard]] bool IsReady() const noexcept;

    /** @brief Last comm-layer error from @ref Transfer32 or GPIO helpers. */
    [[nodiscard]] tle92466ed::CommError GetLastError() const noexcept;

    /** @brief Clear @ref last_error_; does not clear chip fault registers. */
    tle92466ed::CommResult<void> ClearErrors() noexcept;

    /**
     * @brief Drive RESN, EN, or FAULTN via configured @ref BaseGpio active levels.
     * @param pin    RESN (active LOW), EN (active HIGH), or FAULTN input.
     * @param signal ACTIVE / INACTIVE in driver semantics, not raw pin voltage.
     */
    tle92466ed::CommResult<void> GpioSet(tle92466ed::CtrlPin pin,
                                          tle92466ed::GpioSignal signal) noexcept;

    /**
     * @brief Read RESN, EN, or FAULTN logical state.
     * @return ACTIVE when the pin reads as asserted per @ref BaseGpio polarity.
     */
    tle92466ed::CommResult<tle92466ed::GpioSignal> GpioRead(tle92466ed::CtrlPin pin) noexcept;

    /** @brief Route driver log lines to the HAL @ref Logger. */
    void Log(tle92466ed::LogLevel level, const char* tag,
             const char* format, va_list args) noexcept;

    /** @brief Last 32-bit SPI RX word (bring-up / WrongDeviceID diagnostics). */
    [[nodiscard]] uint32_t LastRxWord() const noexcept { return last_rx_; }

    /**
     * @brief RX word from the FIRST frame of the last @ref TransferMulti chain.
     *
     * @details A register read is two CS windows and the reply is expected on
     *          the second. Keeping the first word lets bring-up tools prove
     *          which slot actually carried the reply instead of inferring it
     *          from a register that read back as zero.
     */
    [[nodiscard]] uint32_t LastRxWordFirst() const noexcept { return last_rx_first_; }

    /// @}

private:
    BaseSpi&   spi_;
    BaseGpio&  resn_;
    BaseGpio&  en_;
    BaseGpio*  faultn_;
    bool       initialized_{false};
    tle92466ed::CommError last_error_{tle92466ed::CommError::None};
    uint32_t   last_rx_{0};
    uint32_t   last_rx_first_{0};
};

/// @}

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TLE92466ED_Handler Main Handler Class
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Tle92466edHandler
 * @brief Unified handler for TLE92466ED six-channel solenoid driver.
 *
 * Provides thread-safe access to all TLE92466ED driver features:
 * - Channel enable/disable and current control
 * - PWM configuration per channel
 * - Fault monitoring and diagnostics
 * - Watchdog management
 * - Device status: supply rails (VBAT/VIO/VDD) and over-temperature flags.
 *   The chip exposes an OT indication, not a temperature value — there is no
 *   degrees-Celsius register to read.
 */
class Tle92466edHandler {
public:
    /// @brief Driver type alias (`tle92466ed::Driver` bound to @ref HalSpiTle92466edComm).
    using DriverType = tle92466ed::Driver<HalSpiTle92466edComm>;

    /** @brief Number of solenoid channels (0–5); only CH5 is populated on this board. */
    static constexpr uint8_t kNumChannels = 6;

    //=========================================================================
    // Construction
    //=========================================================================

    /**
     * @brief Construct TLE92466ED handler.
     * @param spi    Reference to pre-configured BaseSpi.
     * @param resn   BaseGpio for RESN pin (active LOW reset).
     * @param en     BaseGpio for EN pin (active HIGH enable).
     * @param faultn Optional BaseGpio for FAULTN (active LOW fault indicator).
     */
    Tle92466edHandler(BaseSpi& spi, BaseGpio& resn, BaseGpio& en,
                      BaseGpio* faultn = nullptr) noexcept;

    /** @brief Best-effort @ref Deinitialize when the handler was initialized. */
    ~Tle92466edHandler() noexcept;

    // Non-copyable
    Tle92466edHandler(const Tle92466edHandler&) = delete;
    Tle92466edHandler& operator=(const Tle92466edHandler&) = delete;

    // Non-movable
    Tle92466edHandler(Tle92466edHandler&&) = delete;
    Tle92466edHandler& operator=(Tle92466edHandler&&) = delete;

    //=========================================================================
    // Initialization
    //=========================================================================

    /**
     * @brief Initialize driver (hardware reset + SPI init + config mode).
     * @param perform_hardware_reset Pulse RESN on first bring-up; @c false keeps
     *        RESN released for SPI-only retries (see `Driver::Init`).
     * @return Empty on success; @ref tle92466ed::DriverError on failure.
     * @note Acquires @ref mutex_; blocks concurrent handler callers.
     */
    tle92466ed::DriverResult<void> Initialize(bool perform_hardware_reset = true) noexcept;

    /**
     * @brief Lazy-init entrypoint: initialize on first use if needed.
     * @return @c true when @ref driver_ is ready after this call.
     * @note Acquires @ref mutex_. Lazy path always performs a hardware reset.
     */
    [[nodiscard]] bool EnsureInitialized() noexcept;

    /**
     * @brief Initialize, then apply @p config before mission mode.
     * @param config Global device configuration (watchdog, dither clock, …).
     * @return Empty on success; rolls back @ref driver_ on configure failure.
     * @note Acquires @ref mutex_; always performs hardware reset via `Driver::Init()`.
     */
    tle92466ed::DriverResult<void> Initialize(const tle92466ed::GlobalConfig& config) noexcept;

    /**
     * @brief Disable all channels and tear down @ref driver_.
     * @return Empty (always succeeds at handler layer).
     * @note Acquires @ref mutex_; does not pulse RESN or change EN parking.
     */
    tle92466ed::DriverResult<void> Deinitialize() noexcept;

    /** @brief @c true after a successful @ref Initialize or lazy @ref EnsureInitialized. */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    //=========================================================================
    // Channel Control
    //=========================================================================

    /**
     * @brief Configure a channel (PWM, dither, current limit, …).
     * @param channel Channel index 0–5 (@ref kNumChannels).
     * @param config  Per-channel register image.
     * @retval InvalidChannel when @p channel >= @ref kNumChannels.
     */
    tle92466ed::DriverResult<void> ConfigureChannel(uint8_t channel, const tle92466ed::ChannelConfig& config) noexcept;

    /**
     * @brief Enable a channel in CH_CTRL (requires @ref EnableOutputStage for load current).
     * @param channel Channel index 0–5.
     */
    tle92466ed::DriverResult<void> EnableChannel(uint8_t channel) noexcept;

    /**
     * @brief Disable a channel in CH_CTRL.
     * @param channel Channel index 0–5.
     */
    tle92466ed::DriverResult<void> DisableChannel(uint8_t channel) noexcept;

    /** @brief Set CH_CTRL enable for every channel. */
    tle92466ed::DriverResult<void> EnableAllChannels() noexcept;

    /** @brief Clear CH_CTRL enable for every channel. */
    tle92466ed::DriverResult<void> DisableAllChannels() noexcept;

    /**
     * @brief Set channel current setpoint.
     * @param channel Channel index 0–5.
     * @param current_ma Target coil current in milliamps (e.g. 115 mA at full flow on CH5).
     */
    tle92466ed::DriverResult<void> SetChannelCurrent(uint8_t channel, uint16_t current_ma) noexcept;

    /**
     * @brief Read back programmed current setpoint for SW-HAL-01 verification.
     * @param channel Channel index 0–5.
     * @param parallel_mode When @c true, read parallel-setpoint register path.
     * @return Setpoint in milliamps.
     */
    tle92466ed::DriverResult<uint16_t> GetChannelCurrentSetpoint(
        uint8_t channel, bool parallel_mode = false) noexcept;

    /**
     * @brief Configure raw TP_MANT / TP_EXP PWM period fields.
     * @param channel Channel index 0–5.
     * @param mantissa PWM period mantissa (datasheet TP_MANT).
     * @param exponent PWM period exponent (datasheet TP_EXP).
     * @param low_freq_range Select low-frequency PWM range when @c true.
     */
    tle92466ed::DriverResult<void> ConfigurePwmRaw(uint8_t channel, uint8_t mantissa,
                         uint8_t exponent, bool low_freq_range = false) noexcept;

    //=========================================================================
    // Device Mode
    //=========================================================================

    /**
     * @brief Enter mission mode after configuration (required before driving loads).
     * @note Does not assert EN; call @ref EnableOutputStage separately.
     */
    tle92466ed::DriverResult<void> EnterMissionMode() noexcept;

    /** @brief Enter config mode so register writes are accepted. */
    tle92466ed::DriverResult<void> EnterConfigMode() noexcept;

    /** @brief @c true when the device is in mission (non-config) mode. */
    [[nodiscard]] bool IsMissionMode() noexcept;

    //=========================================================================
    // Hardware Output Stage (EN pin)
    //=========================================================================

    /**
     * @brief Drive the chip's hardware EN pin HIGH so the output stage can
     *        deliver current to the load.
     *
     * @details The TLE92466ED's EN input gates the entire output stage. While
     *          EN is LOW (the state the driver puts it in during reset), no
     *          current is delivered regardless of the per-channel
     *          `EnableChannel` bits or `SetChannelCurrent` setpoints. This
     *          method just calls `Driver::Enable()` which writes
     *          `GpioSignal::ACTIVE` to the configured EN GPIO.
     *
     * @return Empty result on success or the underlying driver error.
     */
    tle92466ed::DriverResult<void> EnableOutputStage() noexcept;

    /**
     * @brief Drive the chip's EN pin LOW to gate off the output stage. SPI
     *        access remains functional; only the low-side driver outputs are
     *        disabled. Useful as an emergency stop.
     */
    tle92466ed::DriverResult<void> DisableOutputStage() noexcept;

    /**
     * @brief Clear the FB_FRZ register so the chip updates FB_DC / FB_I_AVG /
     *        FB_VBAT on every channel.
     *
     * @details Per the TLE92466ED datasheet §4.10 and the FB_FRZ register
     *          description (offset 0x0007): "Setting the <CH> bit in FB_FRZ
     *          to 0 enables the update of the feedback values; setting it to
     *          1 freezes the values." The chip's POR default leaves all six
     *          channel bits set (frozen), which is why `FB_DC` / `FB_I_AVG`
     *          read back as 0 even when the chip is actively driving a coil.
     *          Writing 0x0000 unfreezes every channel. Averaged current
     *          feedback also requires non-zero DITHER_CLK_DIV. Safe to call
     *          multiple times; idempotent.
     */
    tle92466ed::DriverResult<void> EnableFeedbackUpdates() noexcept;

    //=========================================================================
    // Status & Diagnostics
    //=========================================================================

    /**
     * @brief Read global device status (mode, rail voltages, OT flags, …).
     * @param[out] status Populated on success.
     */
    tle92466ed::DriverResult<void> GetStatus(tle92466ed::DeviceStatus& status) noexcept;

    /**
     * @brief Read per-channel diagnostics (faults, FB_I_AVG, duty cycle, …).
     * @param channel Channel index 0–5.
     * @param[out] diag Populated on success.
     * @note FB_I_AVG stays 0 when FB_FRZ freezes the channel or DITHER_CLK_DIV is 0.
     */
    tle92466ed::DriverResult<void> GetChannelDiagnostics(uint8_t channel, tle92466ed::ChannelDiagnostics& diag) noexcept;

    /**
     * @brief Aggregate fault snapshot across device and all channels.
     * @param[out] report Populated on success.
     */
    tle92466ed::DriverResult<void> GetFaultReport(tle92466ed::FaultReport& report) noexcept;

    /** @brief Clear latched fault registers after the fault condition is removed. */
    tle92466ed::DriverResult<void> ClearFaults() noexcept;

    /**
     * @brief Check if any fault is present.
     * @return @c false when the driver is unavailable or the query fails.
     */
    [[nodiscard]] bool HasFault() noexcept;

    //=========================================================================
    // Watchdog
    //=========================================================================

    /**
     * @brief Reload the SPI watchdog (must be kicked periodically in mission mode).
     * @param reload_value Watchdog reload counter written to the device (default 1000).
     */
    tle92466ed::DriverResult<void> KickWatchdog(uint16_t reload_value = 1000) noexcept;

    //=========================================================================
    // Device Info
    //=========================================================================

    /**
     * @brief Read combined chip ID words for identity checks.
     * @return 32-bit ID or @c 0 when uninitialized / SPI failure.
     */
    uint32_t GetChipId() noexcept;

    /**
     * @brief Read IC version register (ICVID).
     * @return Version field or @c 0 when uninitialized / SPI failure.
     */
    uint32_t GetIcVersion() noexcept;

    /**
     * @brief Last SPI RX word from @ref HalSpiTle92466edComm (init / WrongDeviceID diag).
     * @note Does not acquire @ref mutex_; safe for read-only post-mortem logging.
     */
    [[nodiscard]] uint32_t LastSpiRxWord() const noexcept {
        return comm_ ? comm_->LastRxWord() : 0U;
    }

    /**
     * @brief RX word from the first frame of the last pipelined SPI chain.
     * @see HalSpiTle92466edComm::LastRxWordFirst
     */
    [[nodiscard]] uint32_t LastSpiRxWordFirst() const noexcept {
        return comm_ ? comm_->LastRxWordFirst() : 0U;
    }

    /**
     * @brief Clock @p tx out as one bus-ownership chain and capture every reply.
     *
     * @param tx  Frames to send (CRC must already be set by the caller).
     * @param rx  Receives one MISO word per frame; same size as @p tx.
     * @return true when the chain completed on the wire.
     *
     * @details The driver's own read path decodes only the slot it expects the
     *          reply in, so a register that reads back zero cannot be told apart
     *          from a reply that arrived one slot early or late. This hands back
     *          the untouched words for exactly that comparison.
     *
     * @note Holds @ref mutex_ across the whole chain, so it cannot interleave
     *       with control-loop writes inside the device's reply pipeline.
     * @warning Bench diagnostic (`tle reg raw`). Arbitrary frames can change
     *          device state — not for production paths.
     */
    [[nodiscard]] bool RawSpiChain(std::span<const uint32_t> tx,
                                   std::span<uint32_t> rx) noexcept {
        MutexLockGuard lock(mutex_);
        if (!EnsureInitializedLocked() || !comm_) {
            return false;
        }
        return comm_->TransferMulti(tx, rx).has_value();
    }

    //=========================================================================
    // Direct Driver Access
    //=========================================================================

    /**
     * @brief Run @p fn against the raw driver with @ref mutex_ held.
     *
     * This is the **only** safe way to reach advanced driver APIs
     * (`ConfigureDither`, `GetAverageCurrent`, `ReadPinStatus`, rail reads, …)
     * from production code. The TLE92466ED answers a read on the *next* CS
     * window, so two threads interleaving driver calls consume each other's
     * pipeline slots — which surfaced as sticky-zero register readback and
     * impossible FB_I_AVG ratios. Holding the handler mutex for the whole
     * sequence keeps a multi-register operation coherent across InnerControl
     * (~500 Hz), ValveDriverDiagnostics (~19 Hz), HardwareActuation (200 Hz),
     * and SelfTest threads; @ref HalSpiTle92466edComm::TransferMulti
     * additionally keeps the SPI2 bus lock across the frames of a single
     * pipelined access.
     *
     * @tparam Fn Callable taking `DriverType&`.
     * @return `fn`'s result, or a `NotInitialized` error / value-initialized
     *         result when the driver is unavailable. When `fn` returns void,
     *         this returns `bool` (true when `fn` actually ran).
     */
    template <typename Fn>
    auto WithDriver(Fn&& fn) noexcept {
        using R = std::invoke_result_t<Fn, DriverType&>;
        if constexpr (std::is_void_v<R>) {
            MutexLockGuard lock(mutex_);
            if (!EnsureInitializedLocked() || !driver_) return false;
            fn(*driver_);
            return true;
        } else {
            return withDriver(std::forward<Fn>(fn));
        }
    }

    /**
     * @brief Get the underlying driver for advanced operations.
     * @return Pointer to driver, or nullptr if not initialized.
     *
     * @warning The returned pointer is **not** protected: the mutex is
     *          released when this call returns. Using it from more than one
     *          thread corrupts the chip's pipelined reply sequence. Production
     *          code must use @ref WithDriver instead; this accessor remains
     *          for single-threaded bring-up and bench example programs.
     */
    [[nodiscard]] DriverType* GetDriver() noexcept;
    [[nodiscard]] const DriverType* GetDriver() const noexcept;

    /**
     * @brief Log device status, per-channel diagnostics, and chip ID to @ref Logger.
     * @note Acquires @ref mutex_ for the duration of the dump.
     */
    void DumpDiagnostics() noexcept;

private:
    /** @brief Initialize while @ref mutex_ is already held. */
    tle92466ed::DriverResult<void> InitializeLocked(bool perform_hardware_reset) noexcept;
    bool EnsureInitializedLocked() noexcept;

    /// @brief Type trait to detect tle92466ed::DriverResult<T> types
    template <typename T>
    struct is_driver_result : std::false_type {};
    template <typename T>
    struct is_driver_result<tle92466ed::DriverResult<T>> : std::true_type {};

    /**
     * @brief Execute a lambda with a locked, initialized driver.
     *
     * Acquires the mutex, ensures initialization, and invokes @p fn(*driver_).
     * Returns a default-constructed R on failure, or for DriverResult types,
     * returns an error indicating the driver is not initialized.
     */
    template <typename Fn>
    auto withDriver(Fn&& fn) noexcept {
        using R = std::invoke_result_t<Fn, DriverType&>;
        static_assert(!std::is_void_v<R>, "withDriver requires non-void return");
        MutexLockGuard lock(mutex_);
        if (!EnsureInitializedLocked() || !driver_) {
            if constexpr (is_driver_result<R>::value)
                return R{tle::unexpected(tle92466ed::DriverError::NotInitialized)};
            else
                return R{};
        }
        return fn(*driver_);
    }

    bool initialized_{false};
    mutable RtosMutex mutex_;
    std::unique_ptr<HalSpiTle92466edComm> comm_;
    std::unique_ptr<DriverType> driver_;
};

/// @}

#endif // COMPONENT_HANDLER_TLE92466ED_HANDLER_H_
