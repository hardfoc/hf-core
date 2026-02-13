/**
 * @file Ws2812Handler.cpp
 * @brief Implementation of WS2812 LED strip handler.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Ws2812Handler.h"
#include "Logger.h"

static constexpr const char* TAG = "WS2812";

///////////////////////////////////////////////////////////////////////////////
// Ws2812Handler Implementation
///////////////////////////////////////////////////////////////////////////////

Ws2812Handler::Ws2812Handler(const Config& config) noexcept
    : config_(config) {
    Logger::GetInstance().Info(TAG, "WS2812 handler created (GPIO=%d, LEDs=%lu)",
                              static_cast<int>(config_.gpio_pin),
                              static_cast<unsigned long>(config_.num_leds));
}

Ws2812Handler::~Ws2812Handler() noexcept {
    if (initialized_) {
        Deinitialize();
    }
}

bool Ws2812Handler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return true;
    }

    // Create the LED strip (all types in global scope)
    strip_ = std::make_unique<WS2812Strip>(
        config_.gpio_pin,
        config_.rmt_channel,
        config_.num_leds,
        config_.led_type,
        config_.t0h,
        config_.t1h,
        config_.t0l,
        config_.t1l,
        config_.brightness
    );

    // Begin() returns esp_err_t
    esp_err_t err = strip_->Begin();
    if (err != ESP_OK) {
        Logger::GetInstance().Error(TAG, "Failed to init RMT channel: %d", err);
        strip_.reset();
        return false;
    }

    // Create the animator (global scope class)
    animator_ = std::make_unique<WS2812Animator>(*strip_);

    // Clear all LEDs on init
    for (uint32_t i = 0; i < config_.num_leds; ++i) {
        strip_->SetPixel(i, 0);  // SetPixel(index, uint32_t rgbw)
    }
    strip_->Show();

    initialized_ = true;
    Logger::GetInstance().Info(TAG, "WS2812 initialized (%lu LEDs)",
                              static_cast<unsigned long>(config_.num_leds));
    return true;
}

bool Ws2812Handler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return true;

    // Turn off all LEDs
    if (strip_) {
        for (uint32_t i = 0; i < config_.num_leds; ++i) {
            strip_->SetPixel(i, 0);
        }
        strip_->Show();
    }

    animator_.reset();
    strip_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "WS2812 deinitialized");
    return true;
}

bool Ws2812Handler::SetPixel(uint32_t index, uint8_t r, uint8_t g, uint8_t b) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !strip_) return false;
    if (index >= config_.num_leds) return false;

    // Pack RGB into uint32_t for the driver (0x00RRGGBB)
    uint32_t color = (static_cast<uint32_t>(r) << 16) |
                     (static_cast<uint32_t>(g) << 8)  |
                     static_cast<uint32_t>(b);
    strip_->SetPixel(index, color);
    return true;
}

bool Ws2812Handler::SetAllPixels(uint8_t r, uint8_t g, uint8_t b) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !strip_) return false;

    uint32_t color = (static_cast<uint32_t>(r) << 16) |
                     (static_cast<uint32_t>(g) << 8)  |
                     static_cast<uint32_t>(b);
    for (uint32_t i = 0; i < config_.num_leds; ++i) {
        strip_->SetPixel(i, color);
    }
    return true;
}

bool Ws2812Handler::Clear() noexcept {
    return SetAllPixels(0, 0, 0);
}

bool Ws2812Handler::Show() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !strip_) return false;
    esp_err_t err = strip_->Show();
    return (err == ESP_OK);
}

bool Ws2812Handler::SetBrightness(uint8_t brightness) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !strip_) return false;
    strip_->SetBrightness(brightness);
    return true;
}

bool Ws2812Handler::SetEffect(WS2812Animator::Effect effect, uint32_t color) noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !animator_) return false;
    animator_->SetEffect(effect, color);
    return true;
}

bool Ws2812Handler::Tick() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !animator_ || !strip_) return false;
    animator_->Tick();
    esp_err_t err = strip_->Show();
    return (err == ESP_OK);
}

bool Ws2812Handler::Step() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_ || !animator_ || !strip_) return false;
    // Step() returns current step as uint16_t, but we ignore it here
    animator_->Step();
    esp_err_t err = strip_->Show();
    return (err == ESP_OK);
}

void Ws2812Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    auto& log = Logger::GetInstance();
    log.Info(TAG, "=== WS2812 Diagnostics ===");
    log.Info(TAG, "  GPIO: %d", static_cast<int>(config_.gpio_pin));
    log.Info(TAG, "  LEDs: %lu", static_cast<unsigned long>(config_.num_leds));
    log.Info(TAG, "  RMT channel: %d", config_.rmt_channel);
    log.Info(TAG, "  Brightness: %u", static_cast<unsigned>(config_.brightness));
    log.Info(TAG, "  Initialized: %s", initialized_ ? "yes" : "no");
    if (strip_) {
        log.Info(TAG, "  Strip Length: %lu", static_cast<unsigned long>(strip_->Length()));
    }
    log.Info(TAG, "=== End WS2812 Diagnostics ===");
}
