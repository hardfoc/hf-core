/**
 * @file Ws2812Handler.h
 * @brief Unified handler for WS2812 addressable LED strips using RMT peripheral.
 *
 * @details
 * Provides HAL-level integration for WS2812/SK6812/NeoPixel LED strips.
 * Features:
 * - Direct pixel color control with brightness adjustment
 * - Built-in animation effects (rainbow, chase, breathe, sparkle, wave)
 * - Multi-segment support for independent animation zones
 * - Thread-safe operations with RtosMutex
 * - Lazy initialization pattern
 * - Full access to underlying strip and animator objects
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
 * if (leds.Initialize()) {
 *     leds.SetPixel(0, 255, 0, 0);  // Red
 *     leds.SetPixel(1, 0, 255, 0);  // Green
 *     leds.Show();
 *
 *     // Or use effects
 *     leds.SetEffect(WS2812Animator::Effect::Rainbow, 0xFFFFFF);
 *     while (true) {
 *         leds.Tick();
 *         vTaskDelay(pdMS_TO_TICKS(20));
 *     }
 * }
 * @endcode
 *
 * @note This handler wraps the ESP-IDF RMT-based WS2812 driver directly.
 *       No Base* interface injection is needed since the driver manages RMT internally.
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_WS2812_HANDLER_H_
#define COMPONENT_HANDLER_WS2812_HANDLER_H_

#include <cstdint>
#include <memory>
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

    //=========================================================================
    // Initialization
    //=========================================================================

    /** @brief Initialize the RMT channel and LED strip. */
    bool Initialize() noexcept;

    /** @brief Deinitialize and release RMT resources. */
    bool Deinitialize() noexcept;

    /** @brief Check if initialized. */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    //=========================================================================
    // Pixel Control
    //=========================================================================

    /**
     * @brief Set a single pixel color.
     * @param index LED index (0 to num_leds-1).
     * @param r Red component (0-255).
     * @param g Green component (0-255).
     * @param b Blue component (0-255).
     */
    bool SetPixel(uint32_t index, uint8_t r, uint8_t g, uint8_t b) noexcept;

    /**
     * @brief Set all pixels to the same color.
     */
    bool SetAllPixels(uint8_t r, uint8_t g, uint8_t b) noexcept;

    /**
     * @brief Clear all pixels (set to black).
     */
    bool Clear() noexcept;

    /**
     * @brief Transmit pixel data to the LED strip.
     * @return true if transmission succeeded.
     */
    bool Show() noexcept;

    /**
     * @brief Set global brightness.
     * @param brightness Brightness level (0-255).
     */
    bool SetBrightness(uint8_t brightness) noexcept;

    /**
     * @brief Get the number of LEDs.
     */
    [[nodiscard]] uint32_t GetNumLeds() const noexcept { return config_.num_leds; }

    //=========================================================================
    // Animation Effects
    //=========================================================================

    /**
     * @brief Set the animation effect.
     * @param effect Effect type.
     * @param color Base color for the effect (RGB packed as 0xRRGGBB).
     * @param speed Animation speed (smaller = faster).
     */
    bool SetEffect(WS2812Animator::Effect effect, uint32_t color = 0xFFFFFF) noexcept;

    /**
     * @brief Advance animation by time delta (call periodically).
     */
    bool Tick() noexcept;

    /**
     * @brief Advance animation by one step.
     */
    bool Step() noexcept;

    //=========================================================================
    // Direct Access
    //=========================================================================

    /**
     * @brief Get the underlying LED strip object.
     * @return Pointer to WS2812Strip, or nullptr if not initialized.
     */
    [[nodiscard]] WS2812Strip* GetStrip() noexcept {
        return strip_.get();
    }

    /**
     * @brief Get the animator object.
     * @return Pointer to WS2812Animator, or nullptr if not initialized.
     */
    [[nodiscard]] WS2812Animator* GetAnimator() noexcept {
        return animator_.get();
    }

    /** @brief Dump diagnostics to logger. */
    void DumpDiagnostics() noexcept;

private:
    Config config_;
    bool initialized_{false};
    mutable RtosMutex mutex_;
    std::unique_ptr<WS2812Strip> strip_;
    std::unique_ptr<WS2812Animator> animator_;
};

/// @}

#endif // COMPONENT_HANDLER_WS2812_HANDLER_H_
