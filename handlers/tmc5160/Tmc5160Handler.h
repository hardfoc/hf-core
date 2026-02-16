/**
 * @file Tmc5160Handler.h
 * @brief Unified handler for TMC5160/TMC5130 stepper motor driver with SPI/UART integration.
 *
 * @details
 * This file provides the complete HAL-level integration for a single TMC5160 stepper motor
 * driver device. It bridges the HardFOC base interfaces (BaseSpi, BaseUart, BaseGpio) with
 * the templated tmc51x0::TMC51x0<CommType> driver from the hf-tmc5160-driver library.
 *
 * ## Architecture Overview
 *
 * The file contains three layers:
 *
 * 1. **CRTP Communication Adapters** (HalSpiTmc5160Comm, HalUartTmc5160Comm):
 *    Bridge HardFOC BaseSpi/BaseUart/BaseGpio to the TMC5160 driver's CRTP-based
 *    communication interfaces (tmc51x0::SpiCommInterface, tmc51x0::UartCommInterface).
 *
 * 2. **Tmc5160Handler** (main class):
 *    Non-templated facade that owns one typed driver instance (SpiDriver or UartDriver).
 *    Uses the visitDriver() template to route calls to the active driver, keeping the
 *    public API free of template parameters. Provides:
 *    - Full driver initialization with DriverConfig or ConfigBuilder
 *    - Direct access to all 15 driver subsystems (rampControl, motorControl, etc.)
 *    - Convenience methods for common operations
 *    - Thread-safe operations with RtosMutex protection
 *    - Lazy initialization pattern
 *
 * 3. **Subsystem Access**:
 *    All TMC5160 subsystems are accessible through the visitDriver() template or
 *    typed driver pointers for advanced users.
 *
 * ## Supported Subsystems
 *
 * | Subsystem        | Access via driver.           | Description                           |
 * |------------------|------------------------------|---------------------------------------|
 * | rampControl      | driver.rampControl           | Motion profile control                |
 * | motorControl     | driver.motorControl          | Current/chopper/stealthChop           |
 * | thresholds       | driver.thresholds            | Velocity threshold config             |
 * | switches         | driver.switches              | Reference switch control              |
 * | encoder          | driver.encoder               | ABN encoder interface                 |
 * | stallGuard       | driver.stallGuard            | StallGuard2 detection                 |
 * | tuning           | driver.tuning                | Auto-tuning routines                  |
 * | homing           | driver.homing                | Sensorless/switch/encoder homing      |
 * | status           | driver.status                | Status/diagnostics                    |
 * | powerStage       | driver.powerStage            | Short/overcurrent protection          |
 * | communication    | driver.communication         | Register read/write                   |
 * | io               | driver.io                    | Pin/mode helpers                      |
 * | events           | driver.events                | XCompare / ramp events                |
 * | printer          | driver.printer               | Debug printing                        |
 * | uartConfig       | driver.uartConfig            | UART node addressing                  |
 *
 * ## Usage Example
 *
 * @code
 * // 1. Obtain SPI interface and EN GPIO
 * Tmc5160Handler handler(spi, enable_gpio);
 *
 * // 2. Build configuration
 * auto config = tmc51x0::Tmc5160ConfigBuilder()
 *     .WithMotorMa(1500)
 *     .WithRunCurrentMa(1200)
 *     .WithHoldCurrentMa(400)
 *     .WithStealthChop(true)
 *     .Build();
 *
 * // 3. Initialize
 * if (handler.Initialize(config)) {
 *     // 4. Use motor control
 *     handler.visitDriver([](auto& drv) {
 *         drv.rampControl.SetRampMode(tmc51x0::RampMode::POSITIONING);
 *         drv.rampControl.SetMaxSpeed(tmc51x0::Speed::FromRPM(100));
 *         drv.rampControl.SetAcceleration(tmc51x0::Acceleration::FromRPMS2(50));
 *         drv.rampControl.SetTargetPosition(51200); // 1 revolution at 256 usteps
 *     });
 * }
 * @endcode
 *
 * @author HardFOC Team
 * @date 2025
 * @copyright HardFOC
 */

#ifndef COMPONENT_HANDLER_TMC5160_HANDLER_H_
#define COMPONENT_HANDLER_TMC5160_HANDLER_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <cstdarg>
#include "core/hf-core-drivers/external/hf-tmc5160-driver/inc/tmc51x0.hpp"
#include "core/hf-core-drivers/external/hf-tmc5160-driver/inc/tmc51x0_comm_interface.hpp"
#include "core/hf-core-drivers/external/hf-tmc5160-driver/inc/tmc51x0_types.hpp"
#include "core/hf-core-drivers/external/hf-tmc5160-driver/inc/tmc51x0_result.hpp"
#include "base/BaseSpi.h"
#include "base/BaseUart.h"
#include "base/BaseGpio.h"
#include "core/hf-core-drivers/internal/hf-internal-interface-wrap/inc/utils/RtosMutex.h"

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TMC5160_HAL_CommAdapters HAL Communication Adapters
/// @brief CRTP communication classes bridging HardFOC interfaces to TMC5160.
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @struct Tmc5160CtrlPins
 * @brief Shared helper holding TMC5160 host-side control pin references.
 *
 * @details
 * The TMC5160 requires at minimum an EN (enable) pin. Additional optional
 * pins include DIAG0, DIAG1 for diagnostics, and STEP/DIR for external
 * step/dir mode.
 */
struct Tmc5160CtrlPins {
    BaseGpio& enable;                ///< DRV_ENN enable pin (active LOW disables driver)
    BaseGpio* diag0{nullptr};        ///< Optional DIAG0 diagnostic pin
    BaseGpio* diag1{nullptr};        ///< Optional DIAG1 diagnostic pin

    /**
     * @brief Resolve a TMC51x0CtrlPin enum to the corresponding BaseGpio pointer.
     * @return Pointer to BaseGpio, or nullptr if pin not configured.
     */
    BaseGpio* get(tmc51x0::TMC51x0CtrlPin pin) noexcept {
        switch (pin) {
            case tmc51x0::TMC51x0CtrlPin::EN:    return &enable;
            case tmc51x0::TMC51x0CtrlPin::DIAG0: return diag0;
            case tmc51x0::TMC51x0CtrlPin::DIAG1: return diag1;
            default:                               return nullptr;
        }
    }
};

/**
 * @class HalSpiTmc5160Comm
 * @brief Concrete SPI communication adapter for TMC5160 using BaseSpi and BaseGpio.
 *
 * @details
 * Implements all methods required by tmc51x0::SpiCommInterface<HalSpiTmc5160Comm>
 * through the CRTP pattern. This class bridges BaseSpi to the TMC5160 driver's
 * SPI protocol (40-bit datagrams, Mode 3).
 */
class HalSpiTmc5160Comm : public tmc51x0::SpiCommInterface<HalSpiTmc5160Comm> {
public:
    /**
     * @brief Construct the SPI communication adapter.
     * @param spi    Reference to a pre-configured BaseSpi implementation.
     * @param enable BaseGpio connected to TMC5160 DRV_ENN (pin 28, active LOW).
     * @param diag0  Optional BaseGpio connected to DIAG0 (pin 26).
     * @param diag1  Optional BaseGpio connected to DIAG1 (pin 27).
     * @param active_levels Pin active level configuration.
     */
    HalSpiTmc5160Comm(BaseSpi& spi, BaseGpio& enable,
                       BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
                       const tmc51x0::PinActiveLevels& active_levels = {}) noexcept;

    /// @name CRTP-Required Methods
    /// @{

    /** @brief Low-level SPI transfer (5 bytes per TMC5160 datagram). */
    tmc51x0::Result<void> SpiTransfer(const uint8_t* tx, uint8_t* rx, size_t length) noexcept;

    /** @brief Set a TMC5160 control pin to a given logical signal state. */
    tmc51x0::Result<void> GpioSet(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept;

    /** @brief Read a TMC5160 control pin's current logical signal state. */
    tmc51x0::Result<tmc51x0::GpioSignal> GpioRead(tmc51x0::TMC51x0CtrlPin pin) noexcept;

    /** @brief Debug logging — routes to HardFOC Logger. */
    void DebugLog(int level, const char* tag, const char* format, va_list args) noexcept;

    /** @brief Delay in milliseconds. */
    void DelayMs(uint32_t ms) noexcept;

    /** @brief Delay in microseconds. */
    void DelayUs(uint32_t us) noexcept;

    /** @brief Get communication mode (always SPI). */
    [[nodiscard]] tmc51x0::CommMode GetMode() const noexcept { return tmc51x0::CommMode::SPI; }

    /// @}

private:
    BaseSpi&         spi_;
    Tmc5160CtrlPins  ctrl_pins_;
    tmc51x0::PinActiveLevels active_levels_;
};

/**
 * @class HalUartTmc5160Comm
 * @brief Concrete UART communication adapter for TMC5160 using BaseUart and BaseGpio.
 *
 * @details
 * Implements all methods required by tmc51x0::UartCommInterface<HalUartTmc5160Comm>
 * through the CRTP pattern.
 */
class HalUartTmc5160Comm : public tmc51x0::UartCommInterface<HalUartTmc5160Comm> {
public:
    /**
     * @brief Construct the UART communication adapter.
     * @param uart   Reference to a pre-configured BaseUart implementation.
     * @param enable BaseGpio connected to TMC5160 DRV_ENN (pin 28, active LOW).
     * @param diag0  Optional BaseGpio connected to DIAG0.
     * @param diag1  Optional BaseGpio connected to DIAG1.
     * @param active_levels Pin active level configuration.
     */
    HalUartTmc5160Comm(BaseUart& uart, BaseGpio& enable,
                        BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
                        const tmc51x0::PinActiveLevels& active_levels = {}) noexcept;

    /// @name CRTP-Required Methods
    /// @{

    /** @brief Send data over UART. */
    tmc51x0::Result<void> UartSend(const uint8_t* data, size_t length) noexcept;

    /** @brief Receive data over UART. */
    tmc51x0::Result<void> UartReceive(uint8_t* data, size_t length) noexcept;

    /** @brief Set a TMC5160 control pin. */
    tmc51x0::Result<void> GpioSet(tmc51x0::TMC51x0CtrlPin pin, tmc51x0::GpioSignal signal) noexcept;

    /** @brief Read a TMC5160 control pin. */
    tmc51x0::Result<tmc51x0::GpioSignal> GpioRead(tmc51x0::TMC51x0CtrlPin pin) noexcept;

    /** @brief Debug logging. */
    void DebugLog(int level, const char* tag, const char* format, va_list args) noexcept;

    /** @brief Delay in milliseconds. */
    void DelayMs(uint32_t ms) noexcept;

    /** @brief Delay in microseconds. */
    void DelayUs(uint32_t us) noexcept;

    /** @brief Get communication mode (always UART). */
    [[nodiscard]] tmc51x0::CommMode GetMode() const noexcept { return tmc51x0::CommMode::UART; }

    /// @}

private:
    BaseUart&        uart_;
    Tmc5160CtrlPins  ctrl_pins_;
    tmc51x0::PinActiveLevels active_levels_;
};

/// @}

///////////////////////////////////////////////////////////////////////////////
/// @defgroup TMC5160_Handler Main Handler Class
/// @brief Non-templated facade for TMC5160 stepper motor control.
/// @{
///////////////////////////////////////////////////////////////////////////////

/**
 * @class Tmc5160Handler
 * @brief Unified handler for TMC5160/TMC5130 stepper motor driver.
 *
 * @details
 * Non-templated facade that owns one typed driver instance (SPI or UART).
 * Provides convenience methods for common operations and direct access to
 * all driver subsystems through visitDriver().
 */
class Tmc5160Handler {
public:
    /// @brief SPI driver type alias
    using SpiDriver  = tmc51x0::TMC51x0<HalSpiTmc5160Comm>;
    /// @brief UART driver type alias
    using UartDriver = tmc51x0::TMC51x0<HalUartTmc5160Comm>;

    //=========================================================================
    // Construction
    //=========================================================================

    /**
     * @brief Construct a TMC5160 handler with SPI communication.
     * @param spi    Reference to pre-configured BaseSpi (Mode 3, MSB first).
     * @param enable BaseGpio connected to DRV_ENN (active LOW).
     * @param diag0  Optional DIAG0 pin.
     * @param diag1  Optional DIAG1 pin.
     * @param daisy_chain_position Position in SPI daisy chain (0 = single/first).
     * @param active_levels Pin polarity configuration.
     */
    Tmc5160Handler(BaseSpi& spi, BaseGpio& enable,
                   BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
                   uint8_t daisy_chain_position = 0,
                   const tmc51x0::PinActiveLevels& active_levels = {}) noexcept;

    /**
     * @brief Construct a TMC5160 handler with UART communication.
     * @param uart   Reference to pre-configured BaseUart.
     * @param enable BaseGpio connected to DRV_ENN (active LOW).
     * @param diag0  Optional DIAG0 pin.
     * @param diag1  Optional DIAG1 pin.
     * @param uart_node_address UART node address (0-254).
     * @param active_levels Pin polarity configuration.
     */
    Tmc5160Handler(BaseUart& uart, BaseGpio& enable,
                   BaseGpio* diag0 = nullptr, BaseGpio* diag1 = nullptr,
                   uint8_t uart_node_address = 0,
                   const tmc51x0::PinActiveLevels& active_levels = {}) noexcept;

    /// @brief Destructor.
    ~Tmc5160Handler() noexcept;

    // Non-copyable, non-movable
    Tmc5160Handler(const Tmc5160Handler&) = delete;
    Tmc5160Handler& operator=(const Tmc5160Handler&) = delete;
    Tmc5160Handler(Tmc5160Handler&&) = delete;
    Tmc5160Handler& operator=(Tmc5160Handler&&) = delete;

    //=========================================================================
    // Initialization
    //=========================================================================

    /**
     * @brief Initialize the TMC5160 driver with full configuration.
     * @param config Driver configuration (motor, chopper, ramp, etc.).
     * @param verbose Print config summary if true.
     * @return true if initialization succeeded.
     */
    bool Initialize(const tmc51x0::DriverConfig& config, bool verbose = true) noexcept;

    /**
     * @brief Ensure driver is initialized (lazy init entrypoint).
     * @return true if initialized and ready.
     */
    bool EnsureInitialized() noexcept;

    /**
     * @brief Deinitialize — disable motor and release resources.
     * @return true if deinitialization succeeded.
     */
    bool Deinitialize() noexcept;

    /**
     * @brief Check if the handler is initialized.
     */
    [[nodiscard]] bool IsInitialized() const noexcept { return initialized_; }

    /**
     * @brief Check if SPI is being used.
     */
    [[nodiscard]] bool IsSpi() const noexcept { return is_spi_; }

    //=========================================================================
    // Direct Driver Access (visitDriver pattern)
    //=========================================================================

    /**
     * @brief Visit the typed driver with a callable.
     *
     * This is the primary way to access all 15 subsystems of the TMC5160 driver.
     * The callable receives a reference to either SpiDriver or UartDriver.
     *
     * @tparam Fn Callable type accepting auto& (SpiDriver& or UartDriver&)
     * @param fn  Callable to execute with the driver reference
     * @return The return value of fn, or a default-constructed value if not initialized
     *
     * @code
     * handler.visitDriver([](auto& drv) {
     *     drv.motorControl.Enable();
     *     drv.rampControl.SetTargetPosition(51200);
     * });
     *
     * auto pos = handler.visitDriver([](auto& drv) -> int32_t {
     *     auto result = drv.rampControl.GetCurrentPosition();
     *     return result ? result.Value() : 0;
     * });
     * @endcode
     */
    template <typename Fn>
    auto visitDriver(Fn&& fn) noexcept -> decltype(fn(std::declval<SpiDriver&>())) {
        using ReturnType = decltype(fn(std::declval<SpiDriver&>()));
        MutexLockGuard lock(mutex_);
        if (!EnsureInitialized()) {
            if constexpr (std::is_void_v<ReturnType>) {
                return;
            } else {
                return ReturnType{};
            }
        }
        if (is_spi_ && spi_driver_) {
            return fn(*spi_driver_);
        } else if (!is_spi_ && uart_driver_) {
            return fn(*uart_driver_);
        }
        if constexpr (std::is_void_v<ReturnType>) {
            return;
        } else {
            return ReturnType{};
        }
    }

    /**
     * @brief Get typed SPI driver pointer (null if UART mode or not initialized).
     */
    [[nodiscard]] SpiDriver* driverViaSpi() noexcept;
    [[nodiscard]] const SpiDriver* driverViaSpi() const noexcept;

    /**
     * @brief Get typed UART driver pointer (null if SPI mode or not initialized).
     */
    [[nodiscard]] UartDriver* driverViaUart() noexcept;
    [[nodiscard]] const UartDriver* driverViaUart() const noexcept;

    //=========================================================================
    // Convenience Methods — Motor Control
    //=========================================================================

    /** @brief Enable the motor driver power stage. */
    bool EnableMotor() noexcept;

    /** @brief Disable the motor driver power stage. */
    bool DisableMotor() noexcept;

    /** @brief Check if motor is enabled. */
    [[nodiscard]] bool IsMotorEnabled() noexcept;

    /**
     * @brief Set run and hold currents (raw register values).
     * @param irun  Run current register value (0-31, where 31 = full scale).
     * @param ihold Hold current register value (0-31, where 31 = full scale).
     */
    bool SetCurrent(uint8_t irun, uint8_t ihold) noexcept;

    //=========================================================================
    // Convenience Methods — Motion Control
    //=========================================================================

    /**
     * @brief Set target position (requires POSITIONING mode).
     * @param position Target position in microsteps.
     */
    bool SetTargetPosition(int32_t position) noexcept;

    /**
     * @brief Set target velocity (requires VELOCITY_POSITIVE or VELOCITY_NEGATIVE mode).
     * @param velocity Target velocity in internal units.
     */
    bool SetTargetVelocity(int32_t velocity) noexcept;

    /**
     * @brief Set maximum speed for ramp generator.
     * @param speed Maximum speed value.
     * @param unit  Unit for the speed value (default: Steps).
     */
    bool SetMaxSpeed(float speed, tmc51x0::Unit unit = tmc51x0::Unit::Steps) noexcept;

    /**
     * @brief Set acceleration for ramp generator.
     * @param accel Acceleration value.
     * @param unit  Unit for the acceleration value (default: Steps).
     */
    bool SetAcceleration(float accel, tmc51x0::Unit unit = tmc51x0::Unit::Steps) noexcept;

    /**
     * @brief Stop motor immediately.
     */
    bool Stop() noexcept;

    /**
     * @brief Get current position in microsteps.
     */
    int32_t GetCurrentPosition() noexcept;

    /**
     * @brief Get current velocity.
     */
    int32_t GetCurrentVelocity() noexcept;

    /**
     * @brief Check if target position has been reached.
     */
    bool IsTargetReached() noexcept;

    //=========================================================================
    // Convenience Methods — Status & Diagnostics
    //=========================================================================

    /**
     * @brief Check if the motor is at standstill.
     */
    bool IsStandstill() noexcept;

    /**
     * @brief Check for overtemperature condition.
     */
    bool IsOvertemperature() noexcept;

    /**
     * @brief Check for stall detection.
     */
    bool IsStallDetected() noexcept;

    /**
     * @brief Get StallGuard result value.
     * @return StallGuard value, or -1 on error.
     */
    int32_t GetStallGuardResult() noexcept;

    /**
     * @brief Get the chip version info.
     */
    uint32_t GetChipVersion() noexcept;

    /**
     * @brief Dump diagnostic information to logger.
     */
    void DumpDiagnostics() noexcept;

    //=========================================================================
    // Configuration Snapshot
    //=========================================================================

    /**
     * @brief Get the driver configuration used during initialization.
     */
    [[nodiscard]] const tmc51x0::DriverConfig& GetDriverConfig() const noexcept { return config_; }

private:
    bool EnsureInitializedLocked() noexcept;

    /// @brief Communication mode flag
    bool is_spi_{true};

    /// @brief Initialization state
    bool initialized_{false};

    /// @brief Thread safety mutex
    mutable RtosMutex mutex_;

    /// @brief SPI communication adapter (owned, null if UART)
    std::unique_ptr<HalSpiTmc5160Comm> spi_comm_;

    /// @brief UART communication adapter (owned, null if SPI)
    std::unique_ptr<HalUartTmc5160Comm> uart_comm_;

    /// @brief SPI driver instance (owned, null if UART)
    std::unique_ptr<SpiDriver> spi_driver_;

    /// @brief UART driver instance (owned, null if SPI)
    std::unique_ptr<UartDriver> uart_driver_;

    /// @brief Daisy chain position (SPI) or node address (UART)
    uint8_t address_{0};

    /// @brief Configuration snapshot
    tmc51x0::DriverConfig config_{};

    /// @brief Helper: execute a visitor on the active driver (no lock, internal use)
    template <typename Fn>
    auto visitDriverInternal(Fn&& fn) noexcept -> decltype(fn(std::declval<SpiDriver&>())) {
        using ReturnType = decltype(fn(std::declval<SpiDriver&>()));
        if (is_spi_ && spi_driver_) {
            return fn(*spi_driver_);
        } else if (!is_spi_ && uart_driver_) {
            return fn(*uart_driver_);
        }
        if constexpr (std::is_void_v<ReturnType>) {
            return;
        } else {
            return ReturnType{};
        }
    }
};

/// @}

#endif // COMPONENT_HANDLER_TMC5160_HANDLER_H_
