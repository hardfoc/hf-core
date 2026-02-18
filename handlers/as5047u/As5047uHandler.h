/**
 * @file As5047uHandler.h
 * @brief Unified handler for AS5047U magnetic rotary position sensor with SPI integration.
 *
 * This class provides a modern, unified interface for a single AS5047U device following
 * the same architectural excellence as TMC9660Handler and PCAL95555Handler:
 * - Bridge pattern for BaseSpi integration
 * - Lazy initialization with lightweight construction
 * - Shared pointer management for safe memory handling
 * - Complete exception-free design with noexcept methods
 * - Thread-safe operations with RtosMutex protection
 * - Comprehensive error handling and validation
 * - High-level sensor abstraction with position/velocity/diagnostics
 * - Advanced AS5047U features (DAEC, OTP programming, interface configuration)
 * - Factory method for creating multiple sensor instances
 *
 * Features:
 * - 14-bit absolute angle measurement (0-16383 counts per revolution)
 * - Velocity measurement with multiple unit conversions
 * - Dynamic Angle Error Compensation (DAEC)
 * - ABI/UVW/PWM interface configuration
 * - OTP programming for permanent settings
 * - Comprehensive diagnostics and error handling
 * - Multiple SPI frame formats (16/24/32-bit)
 * - Thread-safe concurrent access
 *
 * @author HardFOC Team
 * @version 1.0
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_AS5047U_HANDLER_H_
#define COMPONENT_HANDLER_AS5047U_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>
#include "core/hf-core-drivers/external/hf-as5047u-driver/inc/as5047u.hpp"
#include "base/BaseSpi.h"
#include "utils/RtosMutex.h"

//======================================================//
// AS5047U SPI BRIDGE ADAPTER
//======================================================//

/**
 * @brief CRTP adapter connecting BaseSpi interface to AS5047U SpiInterface.
 * 
 * This adapter implements the as5047u::SpiInterface<As5047uSpiAdapter> CRTP interface
 * using a HardFOC BaseSpi implementation, enabling the AS5047U driver to work with
 * any SPI controller that inherits from BaseSpi.
 * 
 * Thread Safety: This adapter is thread-safe when the underlying BaseSpi
 * implementation is thread-safe.
 */
class As5047uSpiAdapter : public as5047u::SpiInterface<As5047uSpiAdapter> {
public:
    /**
     * @brief Construct SPI adapter with BaseSpi interface
     * @param spi_interface Reference to BaseSpi implementation
     */
    explicit As5047uSpiAdapter(BaseSpi& spi_interface) noexcept;

    /**
     * @brief Perform full-duplex SPI transfer (CRTP dispatch target)
     * @param tx Transmit buffer (can be nullptr to send zeros)
     * @param rx Receive buffer (can be nullptr to discard received data)
     * @param len Number of bytes to transfer
     */
    void transfer(const uint8_t* tx, uint8_t* rx, std::size_t len) noexcept;

private:
    BaseSpi& spi_interface_;
};

//======================================================//
// AS5047U SENSOR DATA STRUCTURES
//======================================================//

/**
 * @brief Complete sensor measurement data structure
 */
struct As5047uMeasurement {
    uint16_t angle_raw;              ///< Raw angle (0-16383 LSB)
    uint16_t angle_compensated;      ///< DAEC compensated angle (0-16383 LSB)
    int16_t velocity_raw;            ///< Raw velocity (signed 14-bit)
    float velocity_deg_per_sec;      ///< Velocity in degrees per second
    float velocity_rad_per_sec;      ///< Velocity in radians per second
    float velocity_rpm;              ///< Velocity in revolutions per minute
    uint8_t agc_value;               ///< Automatic Gain Control value (0-255)
    uint16_t magnitude;              ///< Magnetic field magnitude (0-16383)
    uint16_t error_flags;            ///< Current error flags
    bool valid;                      ///< True if measurement is valid
};

/**
 * @brief Sensor diagnostic information
 */
struct As5047uDiagnostics {
    bool magnetic_field_ok;          ///< Magnetic field strength is adequate
    bool agc_warning;                ///< AGC at minimum or maximum
    bool cordic_overflow;            ///< CORDIC algorithm overflow
    bool offset_compensation_ok;     ///< Offset compensation completed
    bool communication_ok;           ///< SPI communication working
    uint16_t last_error_flags;       ///< Last error flags read
    uint32_t communication_errors;   ///< Count of communication errors
    uint32_t total_measurements;     ///< Total measurements taken
};

/**
 * @brief AS5047U configuration structure
 */
struct As5047uConfig {
    FrameFormat frame_format;        ///< SPI frame format (16/24/32-bit)
    uint8_t crc_retries;             ///< Number of CRC error retries
    bool enable_daec;                ///< Enable Dynamic Angle Error Compensation
    bool enable_adaptive_filter;     ///< Enable adaptive filtering
    uint16_t zero_position;          ///< Zero position offset (0-16383)
    bool enable_abi_output;          ///< Enable ABI incremental output
    bool enable_uvw_output;          ///< Enable UVW commutation output
    bool enable_pwm_output;          ///< Enable PWM output
    uint8_t abi_resolution_bits;     ///< ABI resolution in bits (10-14)
    uint8_t uvw_pole_pairs;          ///< UVW pole pairs (1-7)
    bool high_temperature_mode;      ///< Enable 150°C operation mode
};

//======================================================//
// AS5047U HANDLER CLASS
//======================================================//

/**
 * @brief Unified handler for AS5047U magnetic rotary position sensor.
 * 
 * This class provides a comprehensive interface for the AS5047U sensor with:
 * - Lazy initialization pattern for optimal memory usage
 * - Shared pointer management for safe cross-component sharing
 * - Complete exception-free design for embedded reliability
 * - Thread-safe operations with mutex protection
 * - CRTP adapter integration with BaseSpi
 * - High-level sensor abstraction
 * - Advanced AS5047U features and diagnostics
 * 
 * Architecture follows the same excellence as TMC9660Handler and PCAL95555Handler.
 */
class As5047uHandler {
public:
    //======================================================//
    // CONSTRUCTION AND LIFECYCLE
    //======================================================//

    /**
     * @brief Construct AS5047U handler with SPI interface
     * @param spi_interface Reference to BaseSpi implementation
     * @param config Initial sensor configuration
     * 
     * Note: Lightweight constructor following lazy initialization pattern.
     * Objects are created during Initialize() call.
     */
    explicit As5047uHandler(BaseSpi& spi_interface, 
                           const As5047uConfig& config = GetDefaultConfig()) noexcept;

    /**
     * @brief Destructor - automatically handles cleanup
     */
    ~As5047uHandler() noexcept = default;

    // Disable copy construction and assignment
    As5047uHandler(const As5047uHandler&) = delete;
    As5047uHandler& operator=(const As5047uHandler&) = delete;

    // Non-movable (holds mutex, unique_ptrs, and raw bus pointer)
    As5047uHandler(As5047uHandler&&) = delete;
    As5047uHandler& operator=(As5047uHandler&&) = delete;

    //======================================================//
    // INITIALIZATION AND STATUS
    //======================================================//

    /**
     * @brief Initialize the AS5047U sensor (lazy initialization)
        * @return true if successful, false otherwise
     * 
     * Creates AS5047U driver instance and performs sensor initialization.
     * Safe to call multiple times - subsequent calls return current status.
     */
    bool Initialize() noexcept;

    /**
     * @brief Ensure the handler is initialized (lazy init entrypoint).
     * @return true if initialized and ready.
     */
    bool EnsureInitialized() noexcept;

    /**
     * @brief Deinitialize the sensor and free resources
        * @return true if successful
     */
    bool Deinitialize() noexcept;

    /**
     * @brief Check if sensor is initialized and ready
     * @return True if sensor is ready for operations
     */
    bool IsInitialized() const noexcept;

    /**
     * @brief Check if sensor driver is ready (lazy initialization helper)
     * @return True if driver instance exists and is ready
     */
    bool IsSensorReady() const noexcept;

    /**
     * @brief Get pointer to AS5047U driver for advanced operations
     * @return Pointer to AS5047U driver or nullptr if not initialized
     *
     * @warning Raw pointer — NOT mutex-protected. Caller is responsible for
     *          external synchronization in multi-task environments.
     *          Prefer visitDriver() for thread-safe access.
     * 
     * Note: Caller must not delete the returned pointer; lifetime is owned by the handler.
     */
    as5047u::AS5047U<As5047uSpiAdapter>* GetSensor() noexcept;

    /**
     * @brief Naming-consistent alias of GetSensor().
     * @warning Raw pointer — NOT mutex-protected. Prefer visitDriver().
     */
    as5047u::AS5047U<As5047uSpiAdapter>* GetDriver() noexcept;
    const as5047u::AS5047U<As5047uSpiAdapter>* GetDriver() const noexcept;

    /**
     * @brief Visit the underlying AS5047U driver under handler mutex protection.
     * @return Callable result or default-constructed value when driver is unavailable.
     */
    template <typename Fn>
    auto visitDriver(Fn&& fn) noexcept -> decltype(fn(std::declval<as5047u::AS5047U<As5047uSpiAdapter>&>())) {
        using ReturnType = decltype(fn(std::declval<as5047u::AS5047U<As5047uSpiAdapter>&>()));
        MutexLockGuard lock(handler_mutex_);
        if (!EnsureInitializedLocked() || !as5047u_sensor_) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        return fn(*as5047u_sensor_);
    }

    //======================================================//
    // UTILITY METHODS
    //======================================================//

    /**
     * @brief Get default sensor configuration
     * @return Default As5047uConfig structure
     */
    static As5047uConfig GetDefaultConfig() noexcept;

    /**
     * @brief Get sensor description string
     * @return Description of the sensor
     */
    const char* GetDescription() const noexcept;

    /**
     * @brief Get last error code
     * @return Last error that occurred
     */
    AS5047U_Error GetLastError() const noexcept;
    
    /**
     * @brief Dump comprehensive diagnostics and statistics to log as INFO level.
     * Logs AS5047U sensor status, communication health, and measurement statistics.
     */
    void DumpDiagnostics() const noexcept;

private:
    //======================================================//
    // PRIVATE MEMBERS
    //======================================================//

    BaseSpi& spi_ref_;                               ///< Reference to SPI interface
    std::unique_ptr<As5047uSpiAdapter> spi_adapter_; ///< SPI CRTP adapter
    std::unique_ptr<as5047u::AS5047U<As5047uSpiAdapter>> as5047u_sensor_; ///< AS5047U driver instance
    As5047uConfig config_;                           ///< Sensor configuration
    mutable RtosMutex handler_mutex_;                ///< Thread safety mutex
    bool initialized_;                               ///< Initialization state
    mutable AS5047U_Error last_error_;               ///< Last driver-reported error flags
    mutable As5047uDiagnostics diagnostics_;         ///< Cached diagnostics
    char description_[64];                           ///< Sensor description

    //======================================================//
    // PRIVATE HELPER METHODS
    //======================================================//

    /**
     * @brief Validate sensor parameters
     * @return True if sensor is in valid state
     */
    bool ValidateSensor() noexcept;

    /**
     * @brief Ensure initialized while mutex is already held.
     * @return True if sensor is ready.
     */
    bool EnsureInitializedLocked() noexcept;

    /**
     * @brief Handle sensor errors and update diagnostics
     * @param sensor_errors Error flags from sensor
     */
    void HandleSensorErrors(uint16_t sensor_errors) noexcept;

    /**
     * @brief Update cached diagnostics
     */
    void UpdateDiagnostics() noexcept;

    /**
     * @brief Apply configuration to sensor
     * @param config Configuration to apply
     * @return True if successful
     */
    bool ApplyConfiguration(const As5047uConfig& config) noexcept;
};

//======================================================//
// FACTORY METHODS
//======================================================//

/**
 * @brief Create AS5047U handler instance
 * @param spi_interface Reference to SPI interface
 * @param config Sensor configuration
 * @return Unique pointer to AS5047U handler
 */
std::unique_ptr<As5047uHandler> CreateAs5047uHandler(
    BaseSpi& spi_interface,
    const As5047uConfig& config = As5047uHandler::GetDefaultConfig()) noexcept;

#endif // COMPONENT_HANDLER_AS5047U_HANDLER_H_
