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

esp_err_t Ws2812Handler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return ESP_OK;
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

    // Begin() returns esp_err_t — propagate directly
    esp_err_t err = strip_->Begin();
    if (err != ESP_OK) {
        Logger::GetInstance().Error(TAG, "Failed to init RMT channel: 0x%x", err);
        strip_.reset();
        return err;
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
    return ESP_OK;
}

bool Ws2812Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

bool Ws2812Handler::EnsureInitializedLocked() noexcept {
    if (initialized_ && strip_ && animator_) {
        return true;
    }
    return Initialize() == ESP_OK;
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

WS2812Strip* Ws2812Handler::GetStrip() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return strip_.get();
}

const WS2812Strip* Ws2812Handler::GetStrip() const noexcept {
    auto* self = const_cast<Ws2812Handler*>(this);
    return self->GetStrip();
}

WS2812Animator* Ws2812Handler::GetAnimator() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return animator_.get();
}

const WS2812Animator* Ws2812Handler::GetAnimator() const noexcept {
    auto* self = const_cast<Ws2812Handler*>(this);
    return self->GetAnimator();
}

void Ws2812Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    auto& log = Logger::GetInstance();
    (void)EnsureInitializedLocked();
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
