/**
 * @file Pf1550Handler.h
 * @brief HAL handler for NXP PF1550 PMIC (I2C + optional strap GPIOs).
 *
 * Bridges the **portable** `pf1550::PF1550<HalPf1550Comm>` driver to HardFOC
 * `BaseI2c` / `BaseGpio` abstractions used inside `pw-controller-sw`. Adds:
 *
 *  - **Thread-safe** access via @c RtosMutex for both the I²C transport and
 *    the handler-level state (init flag, last snapshot).
 *  - **Diagnostic-snapshot caching** for non-blocking reads from monitor
 *    threads (see @ref Pf1550Handler::ReadDiagnosticSnapshot).
 *  - **Boot self-test** entry point used by the system boot recipe
 *    (see @ref Pf1550Handler::RunPowerSelfTest).
 *  - **Severity classification** through @ref pf1550::FaultSeverityPortentaH7.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */
#ifndef COMPONENT_HANDLER_PF1550_HANDLER_H_
#define COMPONENT_HANDLER_PF1550_HANDLER_H_

#include <cstdint>
#include <memory>

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
    /// @brief Drive a `CtrlPin` strap (PJ0 / PJ4 / PJ6 on Portenta) Active/Inactive.
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
 *  3. (Optional) call @ref ApplyPortentaH7CarrierProfile in early boot.
 *  4. Periodically call @ref RefreshDiagnosticSnapshot from a monitor thread.
 */
class Pf1550Handler {
public:
    /// @brief Alias for the templated driver type.
    using Pf1550Driver = pf1550::PF1550<HalPf1550Comm>;

    /// @brief Construct with required I²C and optional strap pins.
    Pf1550Handler(BaseI2c& i2c, BaseGpio* standby_gpio = nullptr,
                  BaseGpio* usb_vbus_en_gpio = nullptr,
                  BaseGpio* usb_otg_en_gpio = nullptr) noexcept;

    Pf1550Handler(const Pf1550Handler&) = delete;
    Pf1550Handler& operator=(const Pf1550Handler&) = delete;

    /// @brief Idempotent — verify DEVICE_ID and prime the bus adapter.
    bool EnsureInitialized() noexcept;
    /// @brief Has @ref EnsureInitialized ever succeeded?
    bool IsInitialized() const noexcept { return initialized_; }

    /// @brief Apply VFR-heritage default profile (legacy).
    bool ApplyPortentaH7Profile() noexcept;
    /// @brief Apply carrier-aware profile (SW1-first, full SW2 enable).
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
     * are classified into Warning / Critical / McuKill following the
     * Portenta H7 wiring (see @ref pf1550::FaultSeverityPortentaH7).
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
