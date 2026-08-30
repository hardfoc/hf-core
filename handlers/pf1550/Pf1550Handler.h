/**
 * @file Pf1550Handler.h
 * @brief HAL handler for NXP PF1550 PMIC (I2C + optional strap GPIOs).
 *
 * Bridges the **portable** `pf1550::PF1550<HalPf1550Comm>` driver to HardFOC
 * `BaseI2c` / `BaseGpio` abstractions. Adds:
 *
 *  - **Thread-safe** access via @c RtosMutex for both the I²C transport and
 *    the handler-level state (init flag, last snapshot).
 *  - **Diagnostic-snapshot caching** for non-blocking reads from monitor
 *    threads (see @ref Pf1550Handler::ReadDiagnosticSnapshot).
 *  - **Boot self-test** entry point for a host boot recipe
 *    (see @ref Pf1550Handler::RunPowerSelfTest).
 *  - **Generic profile apply** via @ref Pf1550Handler::ApplyProfile. Named
 *    `ApplyPortentaH7*` methods are eval-board sequences in the driver.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_PF1550_HANDLER_H_
#define COMPONENT_HANDLER_PF1550_HANDLER_H_

#include <cstdint>
#include <memory>
#include <span>

#include "RtosMutex.h"
#include "base/BaseGpio.h"
#include "base/BaseI2c.h"

#include "core/hf-core-drivers/external/hf-pf1550-driver/inc/pf1550.hpp"

/**
 * @class HalPf1550Comm
 * @brief CRTP `pf1550::BusInterface` adapter mapping driver calls onto
 *        `BaseI2c` + optional `BaseGpio` strap pins.
 *
 * The adapter is **not** intended to be constructed directly by application
 * code — it is owned by @ref Pf1550Handler.
 */
class HalPf1550Comm : public pf1550::BusInterface<HalPf1550Comm> {
public:
    HalPf1550Comm(BaseI2c& i2c, BaseGpio* standby_gpio, BaseGpio* usb_vbus_en_gpio,
                  BaseGpio* usb_otg_en_gpio) noexcept;

    /// @brief Write `len` data bytes after register pointer `reg`.
    bool Write(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len) noexcept;
    /// @brief Repeated-start read of `len` bytes from register `reg`.
    bool Read(uint8_t addr, uint8_t reg, uint8_t* data, size_t len) noexcept;
    /// @brief Idempotently bring up the underlying I²C peripheral.
    bool EnsureInitialized() noexcept;
    /// @brief Drive a `CtrlPin` strap GPIO Active/Inactive.
    void GpioSet(pf1550::CtrlPin pin, pf1550::GpioSignal signal) noexcept;
    /// @brief Coarse `µs`-precision delay used between profile writes.
    void DelayUs(uint32_t us) noexcept;

private:
    BaseI2c& i2c_;
    BaseGpio* standby_gpio_;
    BaseGpio* usb_vbus_en_gpio_;
    BaseGpio* usb_otg_en_gpio_;
    mutable RtosMutex i2c_mutex_;
};

/**
 * @class Pf1550Handler
 * @brief Thread-safe PF1550 PMIC HAL handler.
 *
 * Owns:
 *  - a @ref HalPf1550Comm bus adapter,
 *  - a `pf1550::PF1550<HalPf1550Comm>` driver instance,
 *  - the most recently captured @ref pf1550::DiagnosticSnapshot (cached for
 *    cross-thread reads).
 *
 * Lifecycle:
 *  1. Construct with bound `BaseI2c` and optional strap `BaseGpio*` pins.
 *  2. Call @ref EnsureInitialized (idempotent) before any other method.
 *  3. (Optional) host calls @ref ApplyProfile with a register-write table.
 *     @ref ApplyPortentaH7CarrierProfile is a driver-named eval sequence for
 *     ESP examples — not a product board bind.
 *  4. Periodically call @ref RefreshDiagnosticSnapshot from a monitor thread.
 */
class Pf1550Handler {
public:
    /// @brief Alias for the templated driver type.
    using Pf1550Driver = pf1550::PF1550<HalPf1550Comm>;

    /**
     * @brief Construct with required I²C and optional strap pins.
     * @param i2c I²C device whose 7-bit address matches the PF1550.
     * @param standby_gpio Optional STANDBY strap; nullptr if unused.
     * @param usb_vbus_en_gpio Optional USB_VBUS_EN strap; nullptr if unused.
     * @param usb_otg_en_gpio Optional USB_OTG_EN strap; nullptr if unused.
     */
    Pf1550Handler(BaseI2c& i2c, BaseGpio* standby_gpio = nullptr,
                  BaseGpio* usb_vbus_en_gpio = nullptr,
                  BaseGpio* usb_otg_en_gpio = nullptr) noexcept;

    Pf1550Handler(const Pf1550Handler&) = delete;
    Pf1550Handler& operator=(const Pf1550Handler&) = delete;

    /// @brief Idempotent — verify DEVICE_ID and prime the bus adapter.
    bool EnsureInitialized() noexcept;
    /// @brief Has @ref EnsureInitialized ever succeeded?
    bool IsInitialized() const noexcept { return initialized_; }

    /**
     * @brief Apply an ordered register-write sequence.
     * @param profile Writes applied in order; each entry may delay after the write.
     */
    bool ApplyProfile(std::span<const pf1550::profiles::RegisterWrite> profile) noexcept;

    /**
     * @brief Driver-named Arduino Portenta H7 eval sequence (legacy VFR order).
     * @note Hosts that are not that eval board skip this and pass their own table
     *       to @ref ApplyProfile.
     */
    bool ApplyPortentaH7Profile() noexcept;
    /**
     * @brief Driver-named Portenta-H7-on-carrier eval sequence (SW1-first).
     * @note Hosts that are not that eval board skip this and pass their own table
     *       to @ref ApplyProfile.
     */
    bool ApplyPortentaH7CarrierProfile() noexcept;

    /// @brief Drive STANDBY strap to RUN or STANDBY.
    bool SetPowerMode(pf1550::PowerMode mode) noexcept;
    /// @brief Drive USB_VBUS_EN / USB_OTG_EN straps.
    bool SetUsbRails(bool vbus_en, bool otg_en) noexcept;
    /// @brief Read STATE_INFO (0x67).
    bool ReadPmicStatus(uint8_t& status) noexcept;

    // --- Diagnostics surface ----------------------------------------------

    /**
     * @brief Refresh the cached snapshot by re-reading the PMIC.
     *
     * Safe to call from any thread; serialised by the handler mutex.
     * @return `true` if all I²C transactions completed.
     */
    bool RefreshDiagnosticSnapshot() noexcept;

    /**
     * @brief Copy the cached snapshot out (zero-copy not provided to keep
     *        the API thread-safe).
     */
    bool ReadDiagnosticSnapshot(pf1550::DiagnosticSnapshot& out) noexcept;

    /**
     * @brief Run boot-time self-test (uses internal snapshot path).
     *
     * On success @c out.worst_severity == @c FaultSeverity::kInfo. Failures
     * are classified into Warning / Critical / McuKill via
     * @ref pf1550::FaultSeverityPortentaH7 (board-profile severity table).
     */
    bool RunPowerSelfTest(pf1550::SelfTestResult& out) noexcept;

    /// @brief Clear every RW1C latched fault status register.
    bool ClearLatchedFaults() noexcept;

    /**
     * @brief Convenience: are any currently-latched faults of severity
     *        `kCritical` or `kMcuKill`?
     */
    bool HasMcuAffectingFault() noexcept;

    /// @brief Expose raw driver (advanced use only — does not lock).
    Pf1550Driver* GetDriver() noexcept;
    /// @copydoc GetDriver
    const Pf1550Driver* GetDriver() const noexcept;

private:
    bool ensureInitializedLocked() noexcept;

    BaseI2c& i2c_;
    BaseGpio* standby_gpio_;
    BaseGpio* usb_vbus_en_gpio_;
    BaseGpio* usb_otg_en_gpio_;
    std::unique_ptr<HalPf1550Comm> comm_;
    std::unique_ptr<Pf1550Driver> driver_;
    bool initialized_;
    pf1550::DiagnosticSnapshot cached_snapshot_;
    mutable RtosMutex handler_mutex_;
};

#endif  // COMPONENT_HANDLER_PF1550_HANDLER_H_
