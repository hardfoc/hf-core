/**
 * @file As5047uHandler.cpp
 * @brief Implementation of unified AS5047U magnetic rotary position sensor handler.
 *
 * This file implements the comprehensive AS5047U handler with CRTP SPI integration,
 * lazy initialization, shared pointer management, and complete exception-free design.
 * 
 * Key implementation features:
 * - As5047uSpiAdapter CRTP connecting BaseSpi to as5047u::SpiInterface<As5047uSpiAdapter>
 * - Lazy initialization pattern with deferred object creation
 * - Shared pointer management for safe cross-component access
 * - Thread-safe operations with RtosMutex protection
 * - Comprehensive error handling and validation
 * - High-level sensor abstraction methods
 * - Advanced AS5047U feature implementations
 * - Diagnostic and health monitoring
 *
 * @author HardFOC Team
 * @version 1.0
 * @date 2025
 * @copyright HardFOC
 */

#include "As5047uHandler.h"
#include <cstring>
#include <algorithm>
#include <cmath>
#include "handlers/logger/Logger.h"

//======================================================//
// AS5047U SPI ADAPTER IMPLEMENTATION
//======================================================//

As5047uSpiAdapter::As5047uSpiAdapter(BaseSpi& spi_interface) noexcept
    : spi_interface_(spi_interface) {}

void As5047uSpiAdapter::transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept {
    // Handle null pointer cases gracefully
    if (len == 0) return;
    
    // Perform SPI transfer through BaseSpi interface
    // Note: BaseSpi implementations should handle CS assertion/deassertion
    hf_spi_err_t result = spi_interface_.Transfer(
        const_cast<uint8_t*>(tx),  // BaseSpi expects non-const tx buffer
        rx,
        static_cast<uint16_t>(len),
        1000  // 1 second timeout
    );
    
    // AS5047U::spiBus::transfer doesn't return error codes, so we handle errors internally
    // The AS5047U driver will detect communication issues through CRC and frame validation
    (void)result; // Suppress unused variable warning
}

//======================================================//
// AS5047U HANDLER IMPLEMENTATION
//======================================================//

As5047uHandler::As5047uHandler(BaseSpi& spi_interface, const As5047uConfig& config) noexcept
    : spi_ref_(spi_interface),
      spi_adapter_(nullptr),
      as5047u_sensor_(nullptr),
      config_(config),
      initialized_(false),
      last_error_(AS5047U_Error::None),
      diagnostics_{} {
    
    // Generate description string
    snprintf(description_, sizeof(description_), "AS5047U_Handler_SPI");
    
    // Initialize diagnostics structure
    memset(&diagnostics_, 0, sizeof(diagnostics_));
}

bool As5047uHandler::Initialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    
    // Already initialized - return success
    if (initialized_ && as5047u_sensor_) {
        last_error_ = AS5047U_Error::None;
        return true;
    }
    
    // Create SPI adapter (CRTP pattern)
    spi_adapter_ = std::make_unique<As5047uSpiAdapter>(spi_ref_);
    if (!spi_adapter_) {
        last_error_ = AS5047U_Error::None;
        return false;
    }
    
    // Create AS5047U sensor instance (lazy initialization)
    as5047u_sensor_ = std::make_unique<as5047u::AS5047U<As5047uSpiAdapter>>(*spi_adapter_, config_.frame_format);
    if (!as5047u_sensor_) {
        spi_adapter_.reset();
        last_error_ = AS5047U_Error::None;
        return false;
    }
    
    // Apply initial configuration
    if (!ApplyConfiguration(config_)) {
        as5047u_sensor_.reset();
        spi_adapter_.reset();
        last_error_ = AS5047U_Error::None;
        return false;
    }
    
    // Test basic sensor communication (direct driver call, no recursive lock)
    uint16_t test_angle = as5047u_sensor_->GetAngle(config_.crc_retries);
    auto sticky = as5047u_sensor_->GetStickyErrorFlags();
    if (static_cast<uint16_t>(sticky) & (static_cast<uint16_t>(AS5047U_Error::CrcError) |
                                         static_cast<uint16_t>(AS5047U_Error::FramingError))) {
        as5047u_sensor_.reset();
        spi_adapter_.reset();
        last_error_ = sticky;
        return false;
    }
    (void)test_angle;
    
    // Initialize diagnostics
    UpdateDiagnostics();
    
    initialized_ = true;
    last_error_ = AS5047U_Error::None;
    return true;
}

bool As5047uHandler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = AS5047U_Error::None;
        return false;
    }
    return EnsureInitializedLocked();
}

bool As5047uHandler::Deinitialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    
    // Reset shared pointer (safe automatic cleanup)
    as5047u_sensor_.reset();
    spi_adapter_.reset();
    
    initialized_ = false;
    last_error_ = AS5047U_Error::None;
    return true;
}

bool As5047uHandler::IsInitialized() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return initialized_ && as5047u_sensor_ && spi_adapter_;
}

bool As5047uHandler::IsSensorReady() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return initialized_ && as5047u_sensor_ && spi_adapter_;
}

as5047u::AS5047U<As5047uSpiAdapter>* As5047uHandler::GetSensor() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = AS5047U_Error::None;
        return nullptr;
    }
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return as5047u_sensor_.get();
}

as5047u::AS5047U<As5047uSpiAdapter>* As5047uHandler::GetDriver() noexcept {
    return GetSensor();
}

const as5047u::AS5047U<As5047uSpiAdapter>* As5047uHandler::GetDriver() const noexcept {
    auto* self = const_cast<As5047uHandler*>(this);
    return self->GetDriver();
}

//======================================================//
// UTILITY METHODS
//======================================================//

As5047uConfig As5047uHandler::GetDefaultConfig() noexcept {
    As5047uConfig config;
    config.frame_format = FrameFormat::SPI_16;
    config.crc_retries = 2;
    config.enable_daec = true;
    config.enable_adaptive_filter = true;
    config.zero_position = 0;
    config.enable_abi_output = false;
    config.enable_uvw_output = false;
    config.enable_pwm_output = false;
    config.abi_resolution_bits = 14;
    config.uvw_pole_pairs = 1;
    config.high_temperature_mode = false;
    return config;
}

const char* As5047uHandler::GetDescription() const noexcept {
    return description_;
}

AS5047U_Error As5047uHandler::GetLastError() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return last_error_;
}

//======================================================//
// PRIVATE HELPER METHODS
//======================================================//

bool As5047uHandler::ValidateSensor() noexcept {
    return EnsureInitializedLocked();
}

bool As5047uHandler::EnsureInitializedLocked() noexcept {
    if (initialized_ && as5047u_sensor_ && spi_adapter_) {
        return true;
    }

    return Initialize();
}

void As5047uHandler::HandleSensorErrors(uint16_t sensor_errors) noexcept {
    diagnostics_.last_error_flags = sensor_errors;
    
    // Update specific diagnostic flags
    diagnostics_.agc_warning = (sensor_errors & static_cast<uint16_t>(AS5047U_Error::AgcWarning)) != 0;
    diagnostics_.cordic_overflow = (sensor_errors & static_cast<uint16_t>(AS5047U_Error::CordicOverflow)) != 0;
    diagnostics_.offset_compensation_ok = (sensor_errors & static_cast<uint16_t>(AS5047U_Error::OffCompError)) == 0;
    diagnostics_.communication_ok = (sensor_errors & (static_cast<uint16_t>(AS5047U_Error::CrcError) |
                                                     static_cast<uint16_t>(AS5047U_Error::FramingError) |
                                                     static_cast<uint16_t>(AS5047U_Error::CommandError))) == 0;
    
    // Check magnetic field status
    diagnostics_.magnetic_field_ok = (sensor_errors & static_cast<uint16_t>(AS5047U_Error::MagHalf)) == 0;
}

void As5047uHandler::UpdateDiagnostics() noexcept {
    if (!as5047u_sensor_) return;
    
    // Read current error flags (driver handles retries internally)
    uint16_t error_flags = as5047u_sensor_->GetErrorFlags(config_.crc_retries);
    HandleSensorErrors(error_flags);
}

bool As5047uHandler::ApplyConfiguration(const As5047uConfig& config) noexcept {
    if (!as5047u_sensor_) return false;
    
    bool success = true;
    
    // Set frame format (no return value, always succeeds)
    as5047u_sensor_->SetFrameFormat(config.frame_format);
    
    // Configure DAEC (driver returns true on success, false on failure)
    success &= as5047u_sensor_->SetDynamicAngleCompensation(config.enable_daec, config.crc_retries);
    
    // Configure adaptive filter (driver returns true on success, false on failure)
    success &= as5047u_sensor_->SetAdaptiveFilter(config.enable_adaptive_filter, config.crc_retries);
    
    // Set zero position (driver returns true on success, false on failure)
    success &= as5047u_sensor_->SetZeroPosition(config.zero_position, config.crc_retries);
    
    // Configure interfaces (driver returns true on success, false on failure)
    success &= as5047u_sensor_->ConfigureInterface(config.enable_abi_output, 
                                                   config.enable_uvw_output, 
                                                   config.enable_pwm_output, 
                                                   config.crc_retries);
    
    // Set ABI resolution (driver returns true on success, false on failure)
    if (config.enable_abi_output) {
        success &= as5047u_sensor_->SetABIResolution(config.abi_resolution_bits, config.crc_retries);
    }
    
    // Set UVW pole pairs (driver returns true on success, false on failure)
    if (config.enable_uvw_output) {
        success &= as5047u_sensor_->SetUVWPolePairs(config.uvw_pole_pairs, config.crc_retries);
    }
    
    // Set temperature mode (driver returns true on success, false on failure)
    success &= as5047u_sensor_->Set150CTemperatureMode(config.high_temperature_mode, config.crc_retries);
    
    return success;
}

//======================================================//
// FACTORY METHODS
//======================================================//

std::unique_ptr<As5047uHandler> CreateAs5047uHandler(BaseSpi& spi_interface, const As5047uConfig& config) noexcept {
    return std::make_unique<As5047uHandler>(spi_interface, config);
}

void As5047uHandler::DumpDiagnostics() const noexcept {
    static constexpr const char* TAG = "As5047uHandler";
    
    Logger::GetInstance().Info(TAG, "=== AS5047U HANDLER DIAGNOSTICS ===");
    
    MutexLockGuard lock(handler_mutex_);
    
    // System Health
    Logger::GetInstance().Info(TAG, "System Health:");
    Logger::GetInstance().Info(TAG, "  Initialized: %s", initialized_ ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Last Driver Error Flags: 0x%04X", static_cast<uint16_t>(last_error_));
    
    // Sensor Status
    Logger::GetInstance().Info(TAG, "Sensor Status:");
    if (as5047u_sensor_) {
        Logger::GetInstance().Info(TAG, "  Driver Instance: ACTIVE");
        Logger::GetInstance().Info(TAG, "  Description: %s", description_);
    } else {
        Logger::GetInstance().Info(TAG, "  Driver Instance: NOT_INITIALIZED");
    }
    
    // Configuration
    Logger::GetInstance().Info(TAG, "Configuration:");
    Logger::GetInstance().Info(TAG, "  Frame Format: %s",
        config_.frame_format == FrameFormat::SPI_16 ? "16-bit" :
        config_.frame_format == FrameFormat::SPI_24 ? "24-bit" :
        config_.frame_format == FrameFormat::SPI_32 ? "32-bit" : "Unknown");
    Logger::GetInstance().Info(TAG, "  CRC Retries: %d", config_.crc_retries);
    Logger::GetInstance().Info(TAG, "  DAEC Enabled: %s", config_.enable_daec ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Adaptive Filter: %s", config_.enable_adaptive_filter ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Zero Position: %d", config_.zero_position);
    Logger::GetInstance().Info(TAG, "  ABI Output: %s", config_.enable_abi_output ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  UVW Output: %s", config_.enable_uvw_output ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  PWM Output: %s", config_.enable_pwm_output ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  ABI Resolution: %d bits", config_.abi_resolution_bits);
    Logger::GetInstance().Info(TAG, "  UVW Pole Pairs: %d", config_.uvw_pole_pairs);
    Logger::GetInstance().Info(TAG, "  High Temp Mode: %s", config_.high_temperature_mode ? "YES" : "NO");
    
    // Diagnostics Information
    Logger::GetInstance().Info(TAG, "Sensor Diagnostics:");
    Logger::GetInstance().Info(TAG, "  Magnetic Field OK: %s", diagnostics_.magnetic_field_ok ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  AGC Warning: %s", diagnostics_.agc_warning ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  CORDIC Overflow: %s", diagnostics_.cordic_overflow ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Offset Compensation OK: %s", diagnostics_.offset_compensation_ok ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Communication OK: %s", diagnostics_.communication_ok ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Last Error Flags: 0x%04X", diagnostics_.last_error_flags);
    Logger::GetInstance().Info(TAG, "  Communication Errors: %d", diagnostics_.communication_errors);
    Logger::GetInstance().Info(TAG, "  Total Measurements: %d", diagnostics_.total_measurements);
    
    // SPI Interface Status
    Logger::GetInstance().Info(TAG, "SPI Interface:");
    if (spi_adapter_) {
        Logger::GetInstance().Info(TAG, "  SPI Adapter: ACTIVE");
    } else {
        Logger::GetInstance().Info(TAG, "  SPI Adapter: NOT_INITIALIZED");
    }
    
    // Performance Metrics
    if (diagnostics_.total_measurements > 0) {
        float error_rate = (float)diagnostics_.communication_errors / diagnostics_.total_measurements * 100.0f;
        Logger::GetInstance().Info(TAG, "Performance Metrics:");
        Logger::GetInstance().Info(TAG, "  Error Rate: %.2f%%", error_rate);
        Logger::GetInstance().Info(TAG, "  Success Rate: %.2f%%", 100.0f - error_rate);
    }
    
    // Memory Usage
    Logger::GetInstance().Info(TAG, "Memory Usage:");
    size_t estimated_memory = sizeof(*this);
    if (as5047u_sensor_) estimated_memory += sizeof(as5047u::AS5047U<As5047uSpiAdapter>);
    if (spi_adapter_) estimated_memory += sizeof(As5047uSpiAdapter);
    Logger::GetInstance().Info(TAG, "  Estimated Total: %d bytes", static_cast<int>(estimated_memory));
    
    // System Status Summary
    bool system_healthy = initialized_ && 
                         (last_error_ == AS5047U_Error::None) &&
                         diagnostics_.magnetic_field_ok &&
                         diagnostics_.communication_ok &&
                         !diagnostics_.cordic_overflow;
    
    Logger::GetInstance().Info(TAG, "System Status: %s", system_healthy ? "HEALTHY" : "DEGRADED");
    
    Logger::GetInstance().Info(TAG, "=== END AS5047U HANDLER DIAGNOSTICS ===");
}
