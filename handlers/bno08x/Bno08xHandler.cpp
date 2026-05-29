/**
 * @file Bno08xHandler.cpp
 * @brief Implementation of unified BNO08x IMU sensor handler.
 *
 * This implementation provides:
 * - CRTP communication adapters bridging BaseI2c/BaseSpi to bno08x::CommInterface
 * - Complete handler lifecycle (Initialize/Deinitialize)
 * - SH-2 sensor data reading for all IMU axes, quaternions, Euler angles
 * - Activity/gesture detection (tap, step, shake, pickup, stability)
 * - Calibration status monitoring
 * - Sensor enable/disable with configurable intervals
 * - Hardware control (reset, boot, wake pins)
 * - Thread-safe operations with recursive mutex
 * - Comprehensive diagnostics
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#include "Bno08xHandler.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include "handlers/logger/Logger.h"
#include "OsUtility.h"

// SH-2 error codes for mapping
extern "C" {
#include "core/hf-core-drivers/external/hf-bno08x-driver/src/sh2/sh2_err.h"
}

// ============================================================================
//  I2C CRTP ADAPTER IMPLEMENTATION
// ============================================================================

bool HalI2cBno08xComm::Open() noexcept {
    // Initialize GPIO pins if provided
    if (reset_gpio_) {
        reset_gpio_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
        // RSTN is active-low: ACTIVE = assert reset (drive LOW)
        reset_gpio_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        reset_gpio_->EnsureInitialized();
        // Start with reset released (INACTIVE = HIGH)
        reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
    }

    if (int_gpio_) {
        int_gpio_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT);
        int_gpio_->SetPullMode(hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP);
        // INT is active-low: ACTIVE = data available (pin LOW)
        int_gpio_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        int_gpio_->EnsureInitialized();
    }

    // Ensure the I2C bus is initialized
    return i2c_.EnsureInitialized();
}

void HalI2cBno08xComm::Close() noexcept {
    // Don't deinitialize the I2C bus - it may be shared with other devices
}

int HalI2cBno08xComm::Write(const uint8_t* data, uint32_t length) noexcept {
    if (!data || length == 0) return -1;

    hf_i2c_err_t result = i2c_.Write(data, static_cast<hf_u16_t>(length));
    if (result == hf_i2c_err_t::I2C_SUCCESS) {
        return static_cast<int>(length);
    }
    return -1;
}

int HalI2cBno08xComm::Read(uint8_t* data, uint32_t length) noexcept {
    if (!data || length == 0) return -1;

    // Check INT pin first if available (configured active-low: IsActive() = data ready)
    if (int_gpio_) {
        bool is_active = false;
        if (int_gpio_->IsActive(is_active) == hf_gpio_err_t::GPIO_SUCCESS) {
            if (!is_active) {
                return 0;  // No data ready
            }
        }
    }

    hf_i2c_err_t result = i2c_.Read(data, static_cast<hf_u16_t>(length));
    if (result == hf_i2c_err_t::I2C_SUCCESS) {
        return static_cast<int>(length);
    }
    return -1;
}

bool HalI2cBno08xComm::DataAvailable() noexcept {
    if (!int_gpio_) return true;  // Assume data available if no INT pin

    // INT is active-low, configured as such: IsActive() = data available
    bool is_active = false;
    if (int_gpio_->IsActive(is_active) == hf_gpio_err_t::GPIO_SUCCESS) {
        return is_active;
    }
    return true;  // Assume available on read error
}

void HalI2cBno08xComm::Delay(uint32_t ms) noexcept {
    os_delay_msec(static_cast<uint16_t>(ms));
}

uint32_t HalI2cBno08xComm::GetTimeUs() noexcept {
    return static_cast<uint32_t>(RtosTime::GetCurrentTimeUs());
}

void HalI2cBno08xComm::GpioSet(bno08x::CtrlPin pin, bno08x::GpioSignal signal) noexcept {
    switch (pin) {
        case bno08x::CtrlPin::RSTN:
            if (!reset_gpio_) return;
            // RSTN is configured as active-low in Open()
            // ACTIVE -> SetState(ACTIVE) -> drives GPIO LOW (assert reset)
            // INACTIVE -> SetState(INACTIVE) -> drives GPIO HIGH (release)
            if (signal == bno08x::GpioSignal::ACTIVE) {
                reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_ACTIVE);
            } else {
                reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
            }
            break;
        case bno08x::CtrlPin::BOOTN:
        case bno08x::CtrlPin::WAKE:
        case bno08x::CtrlPin::PS0:
        case bno08x::CtrlPin::PS1:
            break; // Not wired for I2C
    }
}

// ============================================================================
//  SPI CRTP ADAPTER IMPLEMENTATION
// ============================================================================

bool HalSpiBno08xComm::Open() noexcept {
    // Initialize GPIO pins if provided
    if (reset_gpio_) {
        reset_gpio_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
        reset_gpio_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        reset_gpio_->EnsureInitialized();
        reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
    }

    if (int_gpio_) {
        int_gpio_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT);
        int_gpio_->SetPullMode(hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP);
        int_gpio_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        int_gpio_->EnsureInitialized();
    }

    if (wake_gpio_) {
        wake_gpio_->SetDirection(hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
        // WAKE is active-low: ACTIVE = assert wake (drive LOW)
        wake_gpio_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        wake_gpio_->EnsureInitialized();
        // Start with wake released (INACTIVE = HIGH)
        wake_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
    }

    // Ensure the SPI bus is initialized
    return spi_.EnsureInitialized();
}

void HalSpiBno08xComm::Close() noexcept {
    // Don't deinitialize the SPI bus - it may be shared
}

int HalSpiBno08xComm::Write(const uint8_t* data, uint32_t length) noexcept {
    if (!data || length == 0) return -1;

    hf_spi_err_t result = spi_.Write(data, static_cast<hf_u16_t>(length), 100);
    if (result == hf_spi_err_t::SPI_SUCCESS) {
        return static_cast<int>(length);
    }
    return -1;
}

int HalSpiBno08xComm::Read(uint8_t* data, uint32_t length) noexcept {
    if (!data || length == 0) return -1;

    // Check INT pin first if available
    if (int_gpio_) {
        bool is_active = false;
        if (int_gpio_->IsActive(is_active) == hf_gpio_err_t::GPIO_SUCCESS) {
            if (!is_active) {
                return 0;
            }
        }
    }

    hf_spi_err_t result = spi_.Read(data, static_cast<hf_u16_t>(length), 100);
    if (result == hf_spi_err_t::SPI_SUCCESS) {
        return static_cast<int>(length);
    }
    return -1;
}

bool HalSpiBno08xComm::DataAvailable() noexcept {
    if (!int_gpio_) return true;

    bool is_active = false;
    if (int_gpio_->IsActive(is_active) == hf_gpio_err_t::GPIO_SUCCESS) {
        return is_active;
    }
    return true;
}

void HalSpiBno08xComm::Delay(uint32_t ms) noexcept {
    os_delay_msec(static_cast<uint16_t>(ms));
}

uint32_t HalSpiBno08xComm::GetTimeUs() noexcept {
    return static_cast<uint32_t>(RtosTime::GetCurrentTimeUs());
}

void HalSpiBno08xComm::GpioSet(bno08x::CtrlPin pin, bno08x::GpioSignal signal) noexcept {
    switch (pin) {
        case bno08x::CtrlPin::RSTN:
            if (!reset_gpio_) return;
            if (signal == bno08x::GpioSignal::ACTIVE) {
                reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_ACTIVE);
            } else {
                reset_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
            }
            break;
        case bno08x::CtrlPin::WAKE:
            if (!wake_gpio_) return;
            // WAKE is configured as active-low in Open()
            if (signal == bno08x::GpioSignal::ACTIVE) {
                wake_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_ACTIVE);
            } else {
                wake_gpio_->SetState(hf_gpio_state_t::HF_GPIO_STATE_INACTIVE);
            }
            break;
        case bno08x::CtrlPin::BOOTN:
        case bno08x::CtrlPin::PS0:
        case bno08x::CtrlPin::PS1:
            break; // Not wired in this SPI implementation
    }
}

// ============================================================================
//  BNO08X HANDLER IMPLEMENTATION
// ============================================================================

// --- Constructors ---

Bno08xHandler::Bno08xHandler(BaseI2c& i2c_device,
                             const Bno08xConfig& config,
                             BaseGpio* reset_gpio,
                             BaseGpio* int_gpio) noexcept
    : driver_ops_(std::make_unique<Bno08xDriverImpl<HalI2cBno08xComm>>(
          HalI2cBno08xComm(i2c_device, reset_gpio, int_gpio)))
    , config_(config)
    , interface_type_(BNO085Interface::I2C) {
    std::snprintf(description_, sizeof(description_),
                  "BNO08x IMU (I2C @0x%02X)",
                  static_cast<unsigned>(i2c_device.GetDeviceAddress()));
}

Bno08xHandler::Bno08xHandler(BaseSpi& spi_device,
                             const Bno08xConfig& config,
                             BaseGpio* reset_gpio,
                             BaseGpio* int_gpio,
                             BaseGpio* wake_gpio) noexcept
    : driver_ops_(std::make_unique<Bno08xDriverImpl<HalSpiBno08xComm>>(
          HalSpiBno08xComm(spi_device, reset_gpio, int_gpio, wake_gpio)))
    , config_(config)
    , interface_type_(BNO085Interface::SPI) {
    std::snprintf(description_, sizeof(description_),
                  "BNO08x IMU (SPI)");
}

// --- Initialization ---

Bno08xError Bno08xHandler::Initialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = Bno08xError::MUTEX_LOCK_FAILED;
        return last_error_;
    }

    if (initialized_) {
        last_error_ = Bno08xError::SUCCESS;
        return last_error_;
    }

    if (!driver_ops_) {
        last_error_ = Bno08xError::INITIALIZATION_FAILED;
        return last_error_;
    }

    // Perform hardware reset via the driver (uses RSTN pin if wired)
    driver_ops_->HardwareReset(10);

    // Initialize the SH-2 protocol
    if (!driver_ops_->Begin()) {
        last_error_ = Bno08xError::SENSOR_NOT_RESPONDING;
        return last_error_;
    }

    // Set internal callback that forwards to user callback
    driver_ops_->SetCallback([this](const SensorEvent& event) {
        // Forward to user callback (no extra lock needed - called from Update()
        // which already holds the mutex, and the recursive mutex allows re-entry)
        if (user_callback_) {
            user_callback_(event);
        }
    });

    if (!applyConfigLocked()) {
        last_error_ = Bno08xError::INITIALIZATION_FAILED;
        return last_error_;
    }

    initialized_ = true;
    last_error_ = Bno08xError::SUCCESS;
    return last_error_;
}

bool Bno08xHandler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = Bno08xError::MUTEX_LOCK_FAILED;
        return false;
    }
    return ensureInitializedLocked();
}

Bno08xError Bno08xHandler::Deinitialize() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = Bno08xError::MUTEX_LOCK_FAILED;
        return last_error_;
    }

    if (!initialized_) {
        last_error_ = Bno08xError::SUCCESS;
        return last_error_;
    }

    // Clear the driver callback
    if (driver_ops_) {
        driver_ops_->SetCallback(nullptr);
    }

    user_callback_ = nullptr;
    initialized_ = false;
    last_error_ = Bno08xError::SUCCESS;
    return last_error_;
}

bool Bno08xHandler::IsInitialized() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return lock.IsLocked() && initialized_;
}

bool Bno08xHandler::ensureInitializedLocked() noexcept {
    if (initialized_ && driver_ops_) {
        last_error_ = Bno08xError::SUCCESS;
        return true;
    }
    if (!driver_ops_) {
        last_error_ = Bno08xError::INITIALIZATION_FAILED;
        return false;
    }

    const Bno08xError init_result = Initialize();
    if (init_result == Bno08xError::SUCCESS) {
        return true;
    }

    // Initialize() can return a non-fatal communication error after partial
    // sensor enables while the driver itself is ready for use.
    return initialized_ && driver_ops_;
}

bool Bno08xHandler::applyConfigLocked() noexcept {
    if (!driver_ops_) {
        return false;
    }

    const auto enable_if_configured =
        [this](BNO085Sensor sensor, bool enabled, uint32_t interval_ms) noexcept {
            if (!enabled) {
                return true;
            }
            return driver_ops_->EnableSensor(sensor, interval_ms, 0.0f);
        };

    return enable_if_configured(BNO085Sensor::Accelerometer,
                                config_.enable_accelerometer,
                                config_.accelerometer_interval_ms)
        && enable_if_configured(BNO085Sensor::Gyroscope,
                                config_.enable_gyroscope,
                                config_.gyroscope_interval_ms)
        && enable_if_configured(BNO085Sensor::Magnetometer,
                                config_.enable_magnetometer,
                                config_.magnetometer_interval_ms)
        && enable_if_configured(BNO085Sensor::RotationVector,
                                config_.enable_rotation_vector,
                                config_.rotation_interval_ms)
        && enable_if_configured(BNO085Sensor::LinearAcceleration,
                                config_.enable_linear_acceleration,
                                config_.linear_accel_interval_ms)
        && enable_if_configured(BNO085Sensor::Gravity,
                                config_.enable_gravity,
                                config_.gravity_interval_ms)
        && enable_if_configured(BNO085Sensor::GameRotationVector,
                                config_.enable_game_rotation,
                                config_.game_rotation_interval_ms)
        && enable_if_configured(BNO085Sensor::TapDetector,
                                config_.enable_tap_detector,
                                0)
        && enable_if_configured(BNO085Sensor::StepCounter,
                                config_.enable_step_counter,
                                0)
        && enable_if_configured(BNO085Sensor::StepDetector,
                                config_.enable_step_detector,
                                0)
        && enable_if_configured(BNO085Sensor::ShakeDetector,
                                config_.enable_shake_detector,
                                0)
        && enable_if_configured(BNO085Sensor::PickupDetector,
                                config_.enable_pickup_detector,
                                0)
        && enable_if_configured(BNO085Sensor::SignificantMotion,
                                config_.enable_significant_motion,
                                0)
        && enable_if_configured(BNO085Sensor::StabilityClassifier,
                                config_.enable_stability_classifier,
                                0);
}

// ============================================================================
//  SERVICE LOOP
// ============================================================================

Bno08xError Bno08xHandler::Update() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = Bno08xError::MUTEX_LOCK_FAILED;
        return last_error_;
    }

    if (!ensureInitializedLocked()) {
        return last_error_;
    }

    // Pump the SH-2 service loop (dispatches callbacks internally)
    driver_ops_->Update();

    // Check for driver errors
    int driver_error = driver_ops_->GetLastError();
    if (driver_error != 0) {
        last_error_ = mapDriverError(driver_error);
    } else {
        last_error_ = Bno08xError::SUCCESS;
    }

    return last_error_;
}

// ============================================================================
//  CALLBACK MANAGEMENT
// ============================================================================

void Bno08xHandler::SetSensorCallback(SensorCallback callback) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (lock.IsLocked()) {
        user_callback_ = std::move(callback);
    }
}

void Bno08xHandler::ClearSensorCallback() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (lock.IsLocked()) {
        user_callback_ = nullptr;
    }
}

// ============================================================================
//  UTILITY METHODS
// ============================================================================

void Bno08xHandler::QuaternionToEuler(const Bno08xQuaternion& quaternion,
                                       Bno08xEulerAngles& euler_angles) noexcept {
    if (!quaternion.valid) {
        euler_angles.valid = false;
        return;
    }

    float w = quaternion.w;
    float x = quaternion.x;
    float y = quaternion.y;
    float z = quaternion.z;

    // Roll (x-axis rotation)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    euler_angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    float sinp = 2.0f * (w * y - z * x);
    if (std::abs(sinp) >= 1.0f) {
        euler_angles.pitch = std::copysign(static_cast<float>(M_PI) / 2.0f, sinp);
    } else {
        euler_angles.pitch = std::asin(sinp);
    }

    // Yaw (z-axis rotation)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    euler_angles.yaw = std::atan2(siny_cosp, cosy_cosp);

    euler_angles.accuracy = quaternion.accuracy;
    euler_angles.timestamp_us = quaternion.timestamp_us;
    euler_angles.valid = true;
}

BNO085Interface Bno08xHandler::GetInterfaceType() const noexcept {
    return interface_type_;
}

IBno08xDriverOps* Bno08xHandler::GetSensor() noexcept {
    if (!EnsureInitialized()) {
        return nullptr;
    }

    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        last_error_ = Bno08xError::MUTEX_LOCK_FAILED;
        return nullptr;
    }
    return driver_ops_.get();
}

const IBno08xDriverOps* Bno08xHandler::GetSensor() const noexcept {
    auto* self = const_cast<Bno08xHandler*>(this);
    return self->GetSensor();
}

Bno08xError Bno08xHandler::GetLastError() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    return lock.IsLocked() ? last_error_ : Bno08xError::MUTEX_LOCK_FAILED;
}

int Bno08xHandler::GetLastDriverError() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) return -1;
    auto* self = const_cast<Bno08xHandler*>(this);
    if (!self->ensureInitializedLocked() || !driver_ops_) return -1;
    return driver_ops_->GetLastError();
}

const char* Bno08xHandler::GetDescription() const noexcept {
    return description_;
}

Bno08xError Bno08xHandler::mapDriverError(int sh2_error) noexcept {
    switch (sh2_error) {
        case 0:  return Bno08xError::SUCCESS;            // SH2_OK
        case -2: return Bno08xError::INVALID_PARAMETER;  // SH2_ERR_BAD_PARAM
        case -4: return Bno08xError::COMMUNICATION_FAILED; // SH2_ERR_IO
        case -5: return Bno08xError::SENSOR_NOT_RESPONDING; // SH2_ERR_HUB
        case -6: return Bno08xError::TIMEOUT;            // SH2_ERR_TIMEOUT
        default: return Bno08xError::COMMUNICATION_FAILED; // SH2_ERR
    }
}

// ============================================================================
//  DIAGNOSTICS
// ============================================================================

void Bno08xHandler::DumpDiagnostics() const noexcept {
    static constexpr const char* TAG = "Bno08xHandler";

    Logger::GetInstance().Info(TAG, "=== BNO08X HANDLER DIAGNOSTICS ===");

    MutexLockGuard lock(handler_mutex_);
    if (!lock.IsLocked()) {
        Logger::GetInstance().Info(TAG, "  Failed to acquire mutex for diagnostics");
        return;
    }

    // System Health
    Logger::GetInstance().Info(TAG, "System Health:");
    Logger::GetInstance().Info(TAG, "  Initialized: %s", initialized_ ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "  Last Error: %s", Bno08xErrorToString(last_error_));
    Logger::GetInstance().Info(TAG, "  Description: %s", description_);

    // Interface
    const char* iface_str = "UNKNOWN";
    switch (interface_type_) {
        case BNO085Interface::I2C:     iface_str = "I2C"; break;
        case BNO085Interface::SPI:     iface_str = "SPI"; break;
        case BNO085Interface::UART:    iface_str = "UART"; break;
        case BNO085Interface::UARTRVC: iface_str = "UART-RVC"; break;
    }
    Logger::GetInstance().Info(TAG, "  Interface: %s", iface_str);

    // Driver status
    Logger::GetInstance().Info(TAG, "Driver:");
    if (driver_ops_) {
        Logger::GetInstance().Info(TAG, "  Instance: ACTIVE");
        Logger::GetInstance().Info(TAG, "  Last SH2 Error: %d", driver_ops_->GetLastError());
    } else {
        Logger::GetInstance().Info(TAG, "  Instance: NOT_CREATED");
    }

    // Sensor Configuration
    Logger::GetInstance().Info(TAG, "Sensor Configuration:");
    Logger::GetInstance().Info(TAG, "  Accelerometer: %s (%lu ms)",
        config_.enable_accelerometer ? "ON" : "OFF",
        static_cast<unsigned long>(config_.accelerometer_interval_ms));
    Logger::GetInstance().Info(TAG, "  Gyroscope: %s (%lu ms)",
        config_.enable_gyroscope ? "ON" : "OFF",
        static_cast<unsigned long>(config_.gyroscope_interval_ms));
    Logger::GetInstance().Info(TAG, "  Magnetometer: %s (%lu ms)",
        config_.enable_magnetometer ? "ON" : "OFF",
        static_cast<unsigned long>(config_.magnetometer_interval_ms));
    Logger::GetInstance().Info(TAG, "  Rotation Vector: %s (%lu ms)",
        config_.enable_rotation_vector ? "ON" : "OFF",
        static_cast<unsigned long>(config_.rotation_interval_ms));
    Logger::GetInstance().Info(TAG, "  Linear Accel: %s (%lu ms)",
        config_.enable_linear_acceleration ? "ON" : "OFF",
        static_cast<unsigned long>(config_.linear_accel_interval_ms));
    Logger::GetInstance().Info(TAG, "  Gravity: %s (%lu ms)",
        config_.enable_gravity ? "ON" : "OFF",
        static_cast<unsigned long>(config_.gravity_interval_ms));
    Logger::GetInstance().Info(TAG, "  Game Rotation: %s (%lu ms)",
        config_.enable_game_rotation ? "ON" : "OFF",
        static_cast<unsigned long>(config_.game_rotation_interval_ms));

    // Activity sensors
    Logger::GetInstance().Info(TAG, "Activity Sensors:");
    Logger::GetInstance().Info(TAG, "  Tap Detector: %s",
        config_.enable_tap_detector ? "ON" : "OFF");
    Logger::GetInstance().Info(TAG, "  Step Counter: %s",
        config_.enable_step_counter ? "ON" : "OFF");
    Logger::GetInstance().Info(TAG, "  Shake Detector: %s",
        config_.enable_shake_detector ? "ON" : "OFF");
    Logger::GetInstance().Info(TAG, "  Pickup Detector: %s",
        config_.enable_pickup_detector ? "ON" : "OFF");
    Logger::GetInstance().Info(TAG, "  Significant Motion: %s",
        config_.enable_significant_motion ? "ON" : "OFF");
    Logger::GetInstance().Info(TAG, "  Stability Classifier: %s",
        config_.enable_stability_classifier ? "ON" : "OFF");

    // Calibration status (if initialized)
    if (initialized_ && driver_ops_) {
        SensorEvent accel = driver_ops_->GetLatest(BNO085Sensor::Accelerometer);
        SensorEvent gyro = driver_ops_->GetLatest(BNO085Sensor::Gyroscope);
        SensorEvent mag = driver_ops_->GetLatest(BNO085Sensor::Magnetometer);
        SensorEvent rv = driver_ops_->GetLatest(BNO085Sensor::RotationVector);

        Logger::GetInstance().Info(TAG, "Calibration Accuracy:");
        Logger::GetInstance().Info(TAG, "  Accelerometer: %u/3", accel.vector.accuracy);
        Logger::GetInstance().Info(TAG, "  Gyroscope: %u/3", gyro.vector.accuracy);
        Logger::GetInstance().Info(TAG, "  Magnetometer: %u/3", mag.vector.accuracy);
        Logger::GetInstance().Info(TAG, "  Rotation Vector: %u/3", rv.rotation.accuracy);
    }

    // Callback status
    Logger::GetInstance().Info(TAG, "Callback: %s",
        user_callback_ ? "REGISTERED" : "NONE");

    // Memory estimate
    size_t estimated_memory = sizeof(*this);
    Logger::GetInstance().Info(TAG, "Estimated Memory: %u bytes",
        static_cast<unsigned>(estimated_memory));

    Logger::GetInstance().Info(TAG, "=== END BNO08X HANDLER DIAGNOSTICS ===");
}
