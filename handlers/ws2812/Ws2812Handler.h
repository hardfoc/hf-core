/**
 * @file Ws2812Handler.h
 * @brief Unified handler for WS2812 addressable LED strips using RMT peripheral.
 *
 * @details
 * Provides HAL-level lifecycle management for WS2812/SK6812/NeoPixel LED strips.
 * Features:
 * - Thread-safe lifecycle management (Initialize, Deinitialize)
 * - Direct access to underlying WS2812Strip and WS2812Animator objects
 * - Lazy initialization pattern
 * - Comprehensive diagnostics
 *
 * All pixel operations and animation effects should be performed through
 * GetStrip() and GetAnimator() which expose the full driver API.
 *
 * ## Usage Example
 *
 * @code
 * Ws2812Handler leds(Ws2812Handler::Config{
 *     .gpio_pin = GPIO_NUM_48,
 *     .num_leds = 30,
 *     .led_type = LedType::RGB,
 *     .brightness = 50
 * });
 *
 * if (leds.Initialize() == ESP_OK) {
 *     auto* strip = leds.GetStrip();
 *     auto* anim  = leds.GetAnimator();
 *     strip->SetPixel(0, 0xFF0000);  // Red
 *     strip->Show();
 * }
 * @endcode
 *
 * @note This handler provides lifecycle management and direct access to the
 *       underlying WS2812Strip and WS2812Animator objects. All pixel operations
 *       and effects should be performed through GetStrip() and GetAnimator().
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_WS2812_HANDLER_H_
#define COMPONENT_HANDLER_WS2812_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "core/hf-core-drivers/external/hf-ws2812-rmt-driver/inc/ws2812_cpp.hpp"
#include "core/hf-core-drivers/external/hf-ws2812-rmt-driver/inc/ws2812_effects.hpp"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/utils/RtosMutex.h"

#if defined(ESP_PLATFORM)
#include "driver/gpio.h"
#else
using gpio_num_t = int;
#endif

///////////////////////////////////////////////////////////////////////////////
/// @defgroup WS2812_Handler WS2812 LED Strip Handler
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Ws2812Handler
 * @brief Unified handler for WS2812 addressable LED strips.
 *
 * Provides thread-safe access to LED strip control and animation effects.
 */
class Ws2812Handler {
public:
    /**
     * @brief Configuration structure for the WS2812 handler.
     */
    struct Config {
        gpio_num_t gpio_pin{};                ///< GPIO pin connected to LED data line
        uint32_t num_leds{1};                 ///< Number of LEDs in the strip
        LedType led_type{LedType::RGB};
        uint8_t brightness{255};              ///< Global brightness (0-255)
        int rmt_channel{0};                   ///< RMT channel number
        uint16_t t0h{400};                    ///< T0H timing in nanoseconds
        uint16_t t1h{800};                    ///< T1H timing in nanoseconds
        uint16_t t0l{850};                    ///< T0L timing in nanoseconds
        uint16_t t1l{450};                    ///< T1L timing in nanoseconds
    };

    //=========================================================================
    // Construction
    //=========================================================================

    /**
     * @brief Construct WS2812 handler with configuration.
     * @param config LED strip configuration.
     */
    explicit Ws2812Handler(const Config& config) noexcept;

    ~Ws2812Handler() noexcept;

    // Non-copyable
    Ws2812Handler(const Ws2812Handler&) = delete;
    Ws2812Handler& operator=(const Ws2812Handler&) = delete;

    // Non-movable
    Ws2812Handler(Ws2812Handler&&) = delete;
    Ws2812Handler& operator=(Ws2812Handler&&) = delete;

    //=========================================================================
    // Initialization
    //=========================================================================

    /**
     * @brief Initialize the RMT channel and LED strip.
     * @return ESP_OK on success, or esp_err_t error code from the RMT driver.
     */
    esp_err_t Initialize() noexcept;

    /**
     * @brief Ensure strip resources are initialized (lazy init entrypoint).
     * @return true if initialized and ready.
     */
    bool EnsureInitialized() noexcept;

    /** @brief Deinitialize and release RMT resources. */
    bool Deinitialize() noexcept;

    /** @brief Check if initialized. */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    //=========================================================================
    // Direct Access
    //=========================================================================

    /**
     * @brief Get the number of LEDs in the strip.
     */
    [[nodiscard]] uint32_t GetNumLeds() const noexcept { return config_.num_leds; }

    /**
     * @brief Get the underlying LED strip object.
     * @return Pointer to WS2812Strip, or nullptr if not initialized.
     */
    [[nodiscard]] WS2812Strip* GetStrip() noexcept;
    [[nodiscard]] const WS2812Strip* GetStrip() const noexcept;

    /** @brief Naming-consistent alias of GetStrip(). */
    [[nodiscard]] WS2812Strip* GetDriver() noexcept;
    [[nodiscard]] const WS2812Strip* GetDriver() const noexcept;

    /**
     * @brief Get the animator object.
     * @return Pointer to WS2812Animator, or nullptr if not initialized.
     */
    [[nodiscard]] WS2812Animator* GetAnimator() noexcept;
    [[nodiscard]] const WS2812Animator* GetAnimator() const noexcept;

    /**
     * @brief Visit strip driver with a callable.
     * @return Callable result or default-constructed value if unavailable.
     */
    template <typename Fn>
    auto visitDriver(Fn&& fn) noexcept -> decltype(fn(std::declval<WS2812Strip&>())) {
        using ReturnType = decltype(fn(std::declval<WS2812Strip&>()));
        MutexLockGuard lock(mutex_);
        if (!EnsureInitializedLocked() || !strip_) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        return fn(*strip_);
    }

    /**
     * @brief Visit animator object with a callable.
     * @return Callable result or default-constructed value if unavailable.
     */
    template <typename Fn>
    auto visitAnimator(Fn&& fn) noexcept -> decltype(fn(std::declval<WS2812Animator&>())) {
        using ReturnType = decltype(fn(std::declval<WS2812Animator&>()));
        MutexLockGuard lock(mutex_);
        if (!EnsureInitializedLocked() || !animator_) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        return fn(*animator_);
    }

    /** @brief Dump diagnostics to logger. */
    void DumpDiagnostics() noexcept;

private:
    bool EnsureInitializedLocked() noexcept;

    Config config_;
    bool initialized_{false};
    mutable RtosMutex mutex_;
    std::unique_ptr<WS2812Strip> strip_;
    std::unique_ptr<WS2812Animator> animator_;
};

/// @}

#endif // COMPONENT_HANDLER_WS2812_HANDLER_H_
