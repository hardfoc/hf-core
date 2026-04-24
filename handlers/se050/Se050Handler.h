/**
 * @file Se050Handler.h
 * @brief HAL-level handler for the NXP SE050 / SE050A secure element (I²C, T=1oI2C).
 *
 * @details Bridges `BaseI2c` (and optional `BaseGpio` for `SE_RESET`) to the header-only
 *          `se050::Device<TransportT>` from `hf-se050-driver`, mirroring the `Fdo2Handler` /
 *          `Pca9685Handler` pattern:
 *
 *            - `HalI2cSe050Comm` implements `se050::I2cTransceiveInterface<HalI2cSe050Comm>`
 *              using `BaseI2c::Write` / `Read` / `WriteRead` with RTOS-safe delays,
 *            - `Se050Handler` owns the adapter plus `se050::Device<HalI2cSe050Comm>`,
 *            - all public entry points are serialized with `RtosMutex` (private mutex by
 *              default; optional shared mutex when several devices share one I²C bus).
 *
 *          After `EnsureInitialized()` succeeds, use `GetDevice()` for applet commands
 *          (`GetVersion`, `GetRandom`, key lifecycle, etc.) exactly like the standalone
 *          ESP32 examples under `hf-se050-driver/examples/esp32/main/`.
 *
 * @copyright Copyright (c) 2026 HardFOC. All rights reserved.
 */

#ifndef COMPONENT_HANDLER_SE050_HANDLER_H_
#define COMPONENT_HANDLER_SE050_HANDLER_H_

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "RtosMutex.h"
#include "base/BaseGpio.h"
#include "base/BaseI2c.h"

#include "core/hf-core-drivers/external/hf-se050-driver/inc/se050_device.hpp"
#include "core/hf-core-drivers/external/hf-se050-driver/inc/se050_i2c_transport_interface.hpp"

//==============================================================================
// CRTP I2C ADAPTER (BaseI2c → se050 transport)
//==============================================================================

/**
 * @brief CRTP adapter mapping `BaseI2c` primitives to `se050::I2cTransceiveInterface`.
 *
 * @par Reset GPIO
 * When @p reset_gpio is non-null, `HardwareReset()` asserts reset with **`SetInactive()`**,
 * waits 2 ms, then releases with **`SetActive()`** and waits 10 ms (same cadence as the
 * ESP-IDF reference transport). Configure **`BaseGpio`** polarity so this sequence matches
 * your `SE_RESET` wiring (typical nRESET: assert = drive low = logical **inactive** on an
 * active-high GPIO).
 */
class HalI2cSe050Comm : public se050::I2cTransceiveInterface<HalI2cSe050Comm> {
public:
    HalI2cSe050Comm(BaseI2c& i2c, BaseGpio* reset_gpio) noexcept : i2c_(i2c), reset_gpio_(reset_gpio) {}

    [[nodiscard]] bool EnsureInitialized() noexcept { return i2c_.EnsureInitialized(); }

    [[nodiscard]] se050::Error I2cWrite(const std::uint8_t* tx, std::size_t tx_len,
                                        std::uint32_t timeout_ms) noexcept;

    [[nodiscard]] se050::Error I2cRead(std::uint8_t* rx, std::size_t rx_len,
                                        std::uint32_t timeout_ms) noexcept;

    [[nodiscard]] se050::Error Transceive(const std::uint8_t* tx, std::size_t tx_len, std::uint8_t* rx,
                                          std::size_t rx_cap, std::size_t* rx_len_out,
                                          std::uint32_t timeout_ms) noexcept;

    [[nodiscard]] se050::Error HardwareReset() noexcept;

    void delay_ms_impl(std::uint32_t ms) noexcept;

private:
    BaseI2c&   i2c_;
    BaseGpio* reset_gpio_;
};

//==============================================================================
// Handler configuration
//==============================================================================

/// Runtime tuning for one SE050 instance on a `BaseI2c` device.
struct Se050HandlerConfig {
    /// Timeout for management / crypto APDUs (`GetVersion`, `GetRandom`, …).
    std::uint32_t apdu_timeout_ms{300};
    /// T=1 warm-reset S-block exchange timeout.
    std::uint32_t warm_reset_timeout_ms{200};
    /// Inter-frame delay before reading a T=1 response (ms).
    std::uint32_t t1_inter_frame_delay_ms{2};
};

//==============================================================================
// Handler
//==============================================================================

/**
 * @brief Thread-safe façade around `se050::Device<HalI2cSe050Comm>`.
 *
 * Typical flow:
 * @code
 * BaseI2c& se_i2c = *bus.GetDeviceByAddress(0x48);
 * Se050Handler se(se_i2c, Se050HandlerConfig{}, nullptr);
 * if (!se.EnsureInitialized()) { ... }
 * se050::cmd::VersionInfo v{};
 * (void)se.GetDevice().GetVersion(&v, se.Config().apdu_timeout_ms);
 * @endcode
 */
class Se050Handler {
public:
    using DeviceType = se050::Device<HalI2cSe050Comm>;

    /**
     * @param i2c        I²C device whose 7-bit address matches the SE050 (usually `0x48`).
     * @param config     Timeouts and T=1 tuning.
     * @param reset_gpio Optional `SE_RESET` / power-style reset line (nullptr if unused).
     * @param bus_mutex  Optional shared mutex when multiple devices share the bus.
     */
    explicit Se050Handler(BaseI2c& i2c, const Se050HandlerConfig& config = Se050HandlerConfig{},
                           BaseGpio* reset_gpio = nullptr, RtosMutex* bus_mutex = nullptr) noexcept;

    Se050Handler(const Se050Handler&)            = delete;
    Se050Handler& operator=(const Se050Handler&) = delete;
    Se050Handler(Se050Handler&&)                 = delete;
    Se050Handler& operator=(Se050Handler&&)      = delete;

    [[nodiscard]] const Se050HandlerConfig& Config() const noexcept { return config_; }

    /**
     * @brief Initialize I²C, pulse optional reset GPIO, run T=1 warm reset, `SELECT` applet,
     *        and verify with `GetVersion`.
     */
    bool EnsureInitialized() noexcept;

    [[nodiscard]] bool IsPresent() const noexcept {
        return initialized_.load(std::memory_order_acquire);
    }

    /// Direct access to the full `se050::Device` API (lock externally if needed).
    [[nodiscard]] DeviceType&       GetDevice() noexcept { return device_; }
    [[nodiscard]] const DeviceType& GetDevice() const noexcept { return device_; }

private:
    bool EnsureInitializedLocked() noexcept;

    Se050HandlerConfig config_;
    HalI2cSe050Comm     comm_;
    DeviceType          device_;

    std::atomic<bool> initialized_{false};

    RtosMutex  private_mutex_{};
    RtosMutex* bus_mutex_;
};

#endif  // COMPONENT_HANDLER_SE050_HANDLER_H_
