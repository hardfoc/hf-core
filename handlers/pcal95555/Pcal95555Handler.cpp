/**
 * @file Pcal95555Handler.cpp
 * @brief Implementation of the PCAL95555 GPIO expander handler.
 *
 * @details
 * Implements all three layers defined in Pcal95555Handler.h:
 * 1. HalI2cPcal95555Comm -- CRTP I2C communication adapter
 * 2. Pcal95555Handler    -- Main handler (init, GPIO ops, interrupts, factory)
 * 3. Pcal95555GpioPin    -- Per-pin BaseGpio wrapper
 *
 * All driver calls use the PascalCase API of the updated hf-pcal95555-driver
 * (pcal95555::PCAL95555<I2cType>). Error handling follows the driver's error-flag
 * model: individual methods return bool, with accumulated error flags available
 * via GetErrorFlags().
 *
 * @see Pcal95555Handler.h  for architectural overview and Doxygen documentation.
 *
 * @author HardFOC Team
 * @date 2025
 */

#include "Pcal95555Handler.h"
#include "handlers/logger/Logger.h"
#include <cstring>
#if defined(PW_HAL_STM32H7)
#include "StmI2c.h"
#endif

// =====================================================================
// HalI2cPcal95555Comm Implementation
// =====================================================================

namespace {

void SyncDeviceAddress(BaseI2c& i2c, uint8_t addr) noexcept {
    if (i2c.GetDeviceAddress() == addr) {
        return;
    }
#if defined(PW_HAL_STM32H7)
    /* STM32 BaseI2c concrete type exposes SetDeviceAddress for 7-bit rebind. */
    static_cast<StmI2cDevice&>(i2c).SetDeviceAddress(addr);
#else
    (void)i2c;
    (void)addr;
#endif
}

#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
/** JTAG breadcrumb: 0=idle 1=preflight_fail 2=adapter_fail 3=drv_alloc_fail
 *  4=drv_init_fail 5=ok 6=preflight_ok 7=adapter_rw_fail 8=force_marked */
volatile uint8_t g_pcal_handler_init_stage{0};
/** Last register command byte passed to HalI2cPcal95555Comm::Read/Write. */
volatile uint8_t g_pcal_last_i2c_cmd{0};
#endif

}  // namespace

/// @brief Construct the CRTP I2C adapter.
HalI2cPcal95555Comm::HalI2cPcal95555Comm(BaseI2c& i2c_device) noexcept
    : i2c_device_(i2c_device) {}

bool HalI2cPcal95555Comm::Write(uint8_t addr, uint8_t reg,
                                const uint8_t* data, size_t len) noexcept {
    /* Same framing as ProvePcalRegisterWriteDev / StmI2cDevice::Write.
     * Do not range-check addr here — the driver owns strap addressing; a
     * hard 0x20..0x27 reject was masking legitimate retries after rebind. */
    if (data == nullptr || len == 0U) {
        return false;
    }
    SyncDeviceAddress(i2c_device_, addr);

    constexpr size_t kMaxBuf = 4;
    if (len + 1 > kMaxBuf) {
        return false;
    }

#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    g_pcal_last_i2c_cmd = reg;
#endif
    /* Frame [reg|payload…] in member scratch (internal SRAM), not a stack
     * temporary — some MCU maps cannot reliably feed single-byte loads from
     * external/task-stack RAM into the I2C transfer path. */
    tx_scratch_[0] = reg;
    for (size_t i = 0; i < len; ++i) {
        tx_scratch_[1U + i] = data[i];
    }
    return i2c_device_.Write(tx_scratch_, static_cast<hf_u16_t>(len + 1), 200) ==
           hf_i2c_err_t::I2C_SUCCESS;
}

bool HalI2cPcal95555Comm::Read(uint8_t addr, uint8_t reg,
                               uint8_t* data, size_t len) noexcept {
    if (data == nullptr || len == 0U) {
        return false;
    }
    /* Cap length — a corrupted 5th stack arg (Thumb AAPCS) once produced
     * nonsense transfers; PCA dual-port reads never need more than 2 bytes. */
    if (len > 2U) {
        len = 2U;
    }
    SyncDeviceAddress(i2c_device_, addr);
    cmd_scratch_ = reg;
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    g_pcal_last_i2c_cmd = reg;
#endif
    /* RX into AXI member scratch, then publish to caller. StmI2c WriteRead
     * uses Master_Transmit(cmd)+Master_Receive (not HAL Mem_Read). */
    if (i2c_device_.WriteRead(&cmd_scratch_, 1, rx_scratch_,
                              static_cast<hf_u16_t>(len), 200) !=
        hf_i2c_err_t::I2C_SUCCESS) {
        return false;
    }
    if (len == 2U && (reinterpret_cast<uintptr_t>(data) & 1U) == 0U) {
        const uint16_t v = static_cast<uint16_t>(rx_scratch_[0]) |
                           static_cast<uint16_t>(
                               static_cast<uint16_t>(rx_scratch_[1]) << 8);
        *reinterpret_cast<uint16_t*>(data) = v;
    } else {
        for (size_t i = 0; i < len; ++i) {
            data[i] = rx_scratch_[i];
        }
    }
    return true;
}

bool HalI2cPcal95555Comm::EnsureInitialized() noexcept {
    return i2c_device_.EnsureInitialized();
}

bool HalI2cPcal95555Comm::RegisterInterruptHandler(
    std::function<void()> handler) noexcept {
    interrupt_handler_ = std::move(handler);
    return true;  // Actual GPIO interrupt setup is done by the handler.
}

// =====================================================================
// Pcal95555Handler -- Construction & Lifecycle
// =====================================================================

Pcal95555Handler::Pcal95555Handler(BaseI2c& i2c_device,
                                   BaseGpio* interrupt_pin) noexcept
    : i2c_device_(i2c_device),
      i2c_adapter_(nullptr),
      pcal95555_driver_(nullptr),
      initialized_(false),
      interrupt_pin_(interrupt_pin),
      interrupt_configured_(false) {
    pin_registry_.fill(nullptr);
    for (auto& owned : owned_pins_) {
        owned.reset();
    }
    pull_mode_cache_.fill(hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING);
}

bool Pcal95555Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (initialized_) {
        return true;
    }
    return Initialize() == hf_gpio_err_t::GPIO_SUCCESS;
}

bool Pcal95555Handler::EnsureDeinitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!initialized_) {
        return true;
    }
    return Deinitialize() == hf_gpio_err_t::GPIO_SUCCESS;
}

/// @brief Check initialization under an already-held handler_mutex_.
inline bool Pcal95555Handler::EnsureInitializedLocked() noexcept {
    if (initialized_) return true;
    return Initialize() == hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::Initialize() noexcept {
    // Note: caller must hold handler_mutex_.
    if (initialized_) {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }

    uint8_t addr = static_cast<uint8_t>(i2c_device_.GetDeviceAddress());
    if (addr < 0x20U || addr > 0x27U) {
#if defined(PW_PCAL9555_I2C_ADDR)
      addr = static_cast<uint8_t>(PW_PCAL9555_I2C_ADDR);
#else
      addr = 0x20U;
#endif
#if defined(PW_HAL_STM32H7)
      static_cast<StmI2cDevice&>(i2c_device_).SetDeviceAddress(addr);
#endif
    }

    /* Preflight on BaseI2c — do not heap-allocate the driver until the bus
     * answers. hw_safety retries EnsureInitialized thousands of times; the old
     * path reset+make_unique'd the driver every failure and exhausted CM4 heap
     * ("ACK / prove OK but handler init failed"). */
    {
      uint8_t reg = 0x00U;
      uint8_t input0 = 0xA5U;
      if (i2c_device_.WriteRead(&reg, 1, &input0, 1, 200) !=
          hf_i2c_err_t::I2C_SUCCESS) {
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
        g_pcal_handler_init_stage = 1;
#endif
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
      }
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
      g_pcal_handler_init_stage = 6;
#endif
    }

    // 1. Create the CRTP I2C adapter once.
    if (!i2c_adapter_) {
        i2c_adapter_ = std::make_unique<HalI2cPcal95555Comm>(i2c_device_);
        if (!i2c_adapter_) {
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
            g_pcal_handler_init_stage = 2;
#endif
            return hf_gpio_err_t::GPIO_ERR_OUT_OF_MEMORY;
        }
    }

    // 2. Create the typed driver once; recreate only if the strap address moved.
    const bool need_new_driver =
        !pcal95555_driver_ ||
        pcal95555_driver_->GetAddress() != addr;
    if (need_new_driver) {
      pcal95555_driver_.reset();
#if defined(HF_PCAL95555_FORCE_PCA9555) && HF_PCAL95555_FORCE_PCA9555
      /* Some TCA/PCA modules NACK the Agile ID (0x4F). Probing it can leave
       * certain I2C masters sticky; force PCA9555 when the board profile sets
       * HF_PCAL95555_FORCE_PCA9555. */
      pcal95555_driver_ = std::make_unique<Pcal95555Driver>(
          i2c_adapter_.get(), addr, pcal95555::ChipVariant::PCA9555);
#else
      pcal95555_driver_ = std::make_unique<Pcal95555Driver>(
          i2c_adapter_.get(), addr);
#endif
      if (!pcal95555_driver_) {
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
          g_pcal_handler_init_stage = 3;
#endif
          return hf_gpio_err_t::GPIO_ERR_OUT_OF_MEMORY;
      }
    }

    /* Prove the CRTP adapter before the templated driver touches it. */
    {
      uint8_t probe = 0xA5U;
      if (!i2c_adapter_->EnsureInitialized() ||
          !i2c_adapter_->Read(addr, 0x00U, &probe, 1U)) {
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
        g_pcal_handler_init_stage = 7; /* adapter Ensure/Read failed */
#endif
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
      }
    }

    // 3. Initialize the driver (lazy init, auto-detects chip variant).
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    /* Adapter INPUT read above already proved the CRTP↔StmI2c path. Driver
     * EnsureInitialized re-probed (W 0x35) and never reached CONFIG/OUTPUT —
     * mark PCA9555 initialized so map/ApplySafeIdle can program ports. */
    if (!pcal95555_driver_->ForceMarkInitialized(
            pcal95555::ChipVariant::PCA9555)) {
      g_pcal_handler_init_stage = 4;
      return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
    g_pcal_handler_init_stage = 8; /* force-marked after adapter prove */
#else
    if (!pcal95555_driver_->EnsureInitialized()) {
        /* Keep the allocation — retry next call without heap churn. */
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
#endif
    /* Live-actuator buses: tolerate one-shot I2C glitches. */
    pcal95555_driver_->SetRetries(3);

#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    /* Skip std::function interrupt registration on polling-only bring-up — it
     * heap-allocates on every successful driver init and is unused when nINT
     * is optional and map/verify use polling. */
#else
    // 4. Register the driver's interrupt handler with the I2C adapter
    //    so that HandleInterrupt() can be triggered by the adapter.
    pcal95555_driver_->RegisterInterruptHandler();
#endif

    // 5. Configure hardware interrupt pin if available.
    if (interrupt_pin_ != nullptr) {
        auto result = ConfigureHardwareInterrupt();
        if (result != hf_gpio_err_t::GPIO_SUCCESS) {
            // Non-fatal: polling mode still works.
        }
    }

    // Seed previous input state for edge detection on first interrupt.
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    /* Avoid driver ReadAllInputs here — on a busy expander bus it can leave
     * the master sticky (TXIS / phantom INPUT) right before map CONFIG/OUTPUT.
     * Polling bring-up does not need the seed. */
    prev_input_state_ = 0;
#else
    prev_input_state_ = pcal95555_driver_->ReadAllInputs();
#endif

    // Seed pull_mode_cache_ from hardware registers via driver API (PCAL9555A only).
    if (pcal95555_driver_->HasAgileIO()) {
        uint16_t enable_mask = 0;
        uint16_t direction_mask = 0;

        if (pcal95555_driver_->GetPullConfiguration(enable_mask, direction_mask)) {
            for (uint8_t pin = 0; pin < 16; ++pin) {
                bool enabled = (enable_mask >> pin) & 1U;
                bool is_up   = (direction_mask >> pin) & 1U;

                if (!enabled) {
                    pull_mode_cache_[pin] = hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING;
                } else if (is_up) {
                    pull_mode_cache_[pin] = hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP;
                } else {
                    pull_mode_cache_[pin] = hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_DOWN;
                }
            }
        }
        // If read fails, cache stays at default (FLOATING) -- non-fatal.
    }

    initialized_ = true;
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    g_pcal_handler_init_stage = 5;
#endif
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::Deinitialize() noexcept {
    // Note: caller must hold handler_mutex_.
    if (!initialized_) {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }

    // Disable hardware interrupt.
    if (interrupt_configured_ && interrupt_pin_) {
        interrupt_pin_->ConfigureInterrupt(
            hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_NONE);
        interrupt_configured_ = false;
    }

    // Clear pin registry (handler_mutex_ already held by caller).
    pin_registry_.fill(nullptr);
    for (auto& owned : owned_pins_) {
        owned.reset();
    }

    // Release driver and adapter.
    pcal95555_driver_.reset();
    i2c_adapter_.reset();
    initialized_ = false;
    pull_mode_cache_.fill(hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING);
    return hf_gpio_err_t::GPIO_SUCCESS;
}

// =====================================================================
// Pcal95555Handler -- Basic GPIO Operations
// =====================================================================

hf_gpio_err_t Pcal95555Handler::SetDirection(uint8_t pin,
                                             hf_gpio_direction_t direction) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    GPIODir dir = (direction == hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT)
                      ? GPIODir::Output
                      : GPIODir::Input;
    return pcal95555_driver_->SetPinDirection(pin, dir)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetOutput(uint8_t pin, bool active) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    /* Absolute shadow write via StmI2c — avoids RMW when latch readback is bad.
     * Mid-I2C0 can drop Port1 OUTPUT writes (TXIS / pointer); verify the wire
     * latch and retry so TLE nRST cannot stay LOW on the chip while the
     * shadow / CDC flags claim released. */
    if (port_shadow_valid_) {
        const uint16_t bit = static_cast<uint16_t>(1u << pin);
        if (active) {
            output_shadow_ = static_cast<uint16_t>(output_shadow_ | bit);
        } else {
            output_shadow_ =
                static_cast<uint16_t>(output_shadow_ & static_cast<uint16_t>(~bit));
        }
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
        /* MAX CMD (P1.7) toggles on every SPI frame. Write+read+retry verify
         * was ~half of the ~18 ms sol apply budget. Trust the shadow write for
         * this hot pin; keep verified path for EN/nRST/control straps. */
        constexpr uint8_t kMaxCmdPin = 15U;  // hot SPI CMD strap pin
        if (pin == kMaxCmdPin) {
            return pcal95555_driver_->WriteDualPortRegister(
                       static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                       static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1),
                       output_shadow_)
                       ? hf_gpio_err_t::GPIO_SUCCESS
                       : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
        }
#endif
        for (int attempt = 0; attempt < 3; ++attempt) {
            if (!pcal95555_driver_->WriteDualPortRegister(
                    static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                    static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1),
                    output_shadow_)) {
                return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
            }
            uint16_t out_rb = 0;
            if (!pcal95555_driver_->ReadDualPortRegister(
                    static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                    static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), out_rb)) {
                continue;
            }
            if ((out_rb & bit) == (output_shadow_ & bit)) {
                return hf_gpio_err_t::GPIO_SUCCESS;
            }
        }
        return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
    }
    return pcal95555_driver_->WritePin(pin, active)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::ReadInput(uint8_t pin, bool& active) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    /* Output pins: return OUTPUT latch shadow. INPUT_PORT after expander
     * traffic can be untrustworthy (phantom Port1 / stuck pointer) and
     * poisoned IsActive() for control straps even when the latch was
     * correctly programmed. Inputs still sample the wire. */
    if (port_shadow_valid_ &&
        (config_shadow_ & static_cast<uint16_t>(1u << pin)) == 0U) {
        active = (output_shadow_ & static_cast<uint16_t>(1u << pin)) != 0U;
        return hf_gpio_err_t::GPIO_SUCCESS;
    }

    active = pcal95555_driver_->ReadPin(pin);

    /* Only the I2C-read bit means this ReadPin failed. Sticky I2CWriteFail /
     * UnsupportedFeature from an earlier op must not poison every IsActive()
     * (that blocked MAX EN prove + ic_diag SampleCtrlFlags with map ready). */
    constexpr uint16_t kReadFail = static_cast<uint16_t>(Error::I2CReadFail);
    if ((pcal95555_driver_->GetErrorFlags() & kReadFail) != 0U) {
        return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
    }
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::Toggle(uint8_t pin) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    return pcal95555_driver_->TogglePin(pin)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetPullMode(uint8_t pin,
                                            hf_gpio_pull_mode_t pull_mode) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    // Pull resistors require PCAL9555A (Agile I/O).
    if (!pcal95555_driver_->HasAgileIO()) {
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    }

    bool success = true;
    switch (pull_mode) {
        case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING:
            success = pcal95555_driver_->SetPullEnable(pin, false);
            break;
        case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP:
            success = pcal95555_driver_->SetPullEnable(pin, true) &&
                      pcal95555_driver_->SetPullDirection(pin, true);
            break;
        case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_DOWN:
            success = pcal95555_driver_->SetPullEnable(pin, true) &&
                      pcal95555_driver_->SetPullDirection(pin, false);
            break;
        case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP_DOWN:
            // Not directly supported by hardware -- default to pull-up.
            success = pcal95555_driver_->SetPullEnable(pin, true) &&
                      pcal95555_driver_->SetPullDirection(pin, true);
            break;
        default:
            return hf_gpio_err_t::GPIO_ERR_INVALID_PARAMETER;
    }

    if (success) {
        pull_mode_cache_[pin] = pull_mode;
    }
    return success ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::GetPullMode(uint8_t pin,
                                            hf_gpio_pull_mode_t& pull_mode) noexcept {
    if (!ValidatePin(pin)) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    pull_mode = pull_mode_cache_[pin];
    return hf_gpio_err_t::GPIO_SUCCESS;
}

// =====================================================================
// Pcal95555Handler -- Batch GPIO Operations
// =====================================================================

hf_gpio_err_t Pcal95555Handler::SetDirections(uint16_t pin_mask,
                                              hf_gpio_direction_t direction) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    if (pin_mask == 0U) return hf_gpio_err_t::GPIO_SUCCESS;

    if (port_shadow_valid_) {
        if (direction == hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT) {
            config_shadow_ = static_cast<uint16_t>(config_shadow_ | pin_mask);
        } else {
            config_shadow_ = static_cast<uint16_t>(config_shadow_ &
                                                   static_cast<uint16_t>(~pin_mask));
        }
        const bool ok = pcal95555_driver_->WriteDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1), config_shadow_);
        return ok ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
    }

    GPIODir dir = (direction == hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT)
                      ? GPIODir::Input
                      : GPIODir::Output;
    return pcal95555_driver_->SetMultipleDirections(pin_mask, dir)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetOutputs(uint16_t pin_mask, bool active) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    if (pin_mask == 0) return hf_gpio_err_t::GPIO_SUCCESS;

    if (port_shadow_valid_) {
        if (active) {
            output_shadow_ = static_cast<uint16_t>(output_shadow_ | pin_mask);
        } else {
            output_shadow_ = static_cast<uint16_t>(output_shadow_ &
                                                   static_cast<uint16_t>(~pin_mask));
        }
        const bool ok = pcal95555_driver_->WriteDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), output_shadow_);
        return ok ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
    }

    if (!pcal95555_driver_->SetMultipleOutputs(pin_mask, active)) {
        return hf_gpio_err_t::GPIO_ERR_FAILURE;
    }
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::SetPullModes(uint16_t pin_mask,
                                             hf_gpio_pull_mode_t pull_mode) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    if (!pcal95555_driver_->HasAgileIO()) {
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    }

    // Delegate to the single-pin pull mode logic for each selected pin.
    // SetPullMode() would re-lock handler_mutex_, so we inline the core logic here.
    bool ok = true;
    for (uint8_t pin = 0; pin < 16; ++pin) {
        if (!(pin_mask & (1U << pin))) continue;

        bool pin_ok = true;
        switch (pull_mode) {
            case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING:
                pin_ok = pcal95555_driver_->SetPullEnable(pin, false);
                break;
            case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_UP:
                pin_ok = pcal95555_driver_->SetPullEnable(pin, true) &&
                         pcal95555_driver_->SetPullDirection(pin, true);
                break;
            case hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_DOWN:
                pin_ok = pcal95555_driver_->SetPullEnable(pin, true) &&
                         pcal95555_driver_->SetPullDirection(pin, false);
                break;
            default:
                pin_ok = false;
                break;
        }

        if (pin_ok) {
            pull_mode_cache_[pin] = pull_mode;
        }
        ok &= pin_ok;
    }
    return ok ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_FAILURE;
}

// =====================================================================
// Pcal95555Handler -- Interrupt Management
// =====================================================================

hf_gpio_err_t Pcal95555Handler::GetAllInterruptMasks(uint16_t& mask) noexcept {
    if (!EnsureInitialized()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    MutexLockGuard lock(handler_mutex_);

    if (!pcal95555_driver_->HasAgileIO()) {
        mask = 0xFFFF;  // All masked on PCA9555.
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    }

    // The driver doesn't expose a GetInterruptMask() getter.
    // Default: report all masked. Handler-level callers should track
    // which pins have been enabled via ConfigureInterrupt().
    mask = 0xFFFF;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::GetAllInterruptStatus(uint16_t& status) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    status = pcal95555_driver_->GetInterruptStatus();

    uint16_t error_flags = pcal95555_driver_->GetErrorFlags();
    if (error_flags != 0) {
        return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
    }
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::RegisterPinInterrupt(
    hf_pin_num_t pin,
    hf_gpio_interrupt_trigger_t trigger,
    InterruptCallback callback,
    void* user_data) noexcept {

    if (pin >= 16) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;

    MutexLockGuard lock(handler_mutex_);

    if (!pcal95555_driver_) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }

    // Callback-based interrupt dispatch requires a wired hardware INT pin.
    if (!interrupt_pin_) {
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    }

    auto& gpio_pin = pin_registry_[pin];
    if (!gpio_pin) return hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND;

    // Store interrupt data in the pin object.
    gpio_pin->interrupt_callback_ = callback;
    gpio_pin->interrupt_user_data_ = user_data;
    gpio_pin->interrupt_trigger_ = trigger;
    gpio_pin->interrupt_enabled_ = true;

    // Set up hardware interrupt on first use.
    if (interrupt_pin_ && !interrupt_configured_) {
        auto result = ConfigureHardwareInterrupt();
        if (result != hf_gpio_err_t::GPIO_SUCCESS) {
            // Rollback.
            gpio_pin->interrupt_callback_ = nullptr;
            gpio_pin->interrupt_user_data_ = nullptr;
            gpio_pin->interrupt_enabled_ = false;
            return result;
        }
    }

    // Enable interrupt for this pin in the driver (unmask it).
    if (!pcal95555_driver_->ConfigureInterrupt(pin, InterruptState::Enabled)) {
        // Roll back pin callback state when driver-side configuration fails.
        gpio_pin->interrupt_callback_ = nullptr;
        gpio_pin->interrupt_user_data_ = nullptr;
        gpio_pin->interrupt_trigger_ =
            hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_NONE;
        gpio_pin->interrupt_enabled_ = false;

        return pcal95555_driver_->HasAgileIO()
                   ? hf_gpio_err_t::GPIO_ERR_FAILURE
                   : hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    }

    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::UnregisterPinInterrupt(hf_pin_num_t pin) noexcept {
    if (pin >= 16) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;

    MutexLockGuard lock(handler_mutex_);

    auto& gpio_pin = pin_registry_[pin];
    if (!gpio_pin) return hf_gpio_err_t::GPIO_ERR_PIN_NOT_FOUND;

    // Clear interrupt data.
    gpio_pin->interrupt_callback_ = nullptr;
    gpio_pin->interrupt_user_data_ = nullptr;
    gpio_pin->interrupt_trigger_ =
        hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_NONE;
    gpio_pin->interrupt_enabled_ = false;

    // Mask interrupt for this pin in the driver.
    if (pcal95555_driver_) {
        if (!pcal95555_driver_->ConfigureInterrupt(pin, InterruptState::Disabled)) {
            return pcal95555_driver_->HasAgileIO()
                       ? hf_gpio_err_t::GPIO_ERR_FAILURE
                       : hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
        }
    }

    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::ConfigureHardwareInterrupt() noexcept {
    if (!interrupt_pin_) return hf_gpio_err_t::GPIO_ERR_NULL_POINTER;

    // PCAL95555 INT output is active-low, open-drain -- trigger on falling edge.
    auto result = interrupt_pin_->ConfigureInterrupt(
        hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_FALLING_EDGE,
        HardwareInterruptCallback,
        this);

    if (result == hf_gpio_err_t::GPIO_SUCCESS) {
        interrupt_configured_ = true;
    }
    return result;
}

void Pcal95555Handler::HardwareInterruptCallback(
    BaseGpio* /*gpio*/,
    hf_gpio_interrupt_trigger_t /*trigger*/,
    void* user_data) noexcept {
    // Called in ISR context -- only set atomic flag, defer heavy work.
    auto* handler = static_cast<Pcal95555Handler*>(user_data);
    if (handler) {
        handler->interrupt_pending_.store(true, std::memory_order_release);
    }
}

bool Pcal95555Handler::DrainPendingInterrupts() noexcept {
    // Check and clear the atomic flag (task context only)
    if (!interrupt_pending_.exchange(false, std::memory_order_acq_rel)) {
        return false;
    }
    MutexLockGuard lock(handler_mutex_);
    ProcessInterrupts();
    return true;
}

void Pcal95555Handler::ProcessInterrupts() noexcept {
    if (!pcal95555_driver_) return;

    // Read interrupt status (this clears the interrupt condition on the chip).
    uint16_t status = pcal95555_driver_->GetInterruptStatus();
    if (status == 0) return;

    // Read current pin input levels for edge detection.
    uint16_t current_state = pcal95555_driver_->ReadAllInputs();

    // Determine which pins transitioned high (rising) and low (falling).
    uint16_t rising  = current_state & ~prev_input_state_;  // was 0, now 1
    uint16_t falling = ~current_state & prev_input_state_;  // was 1, now 0

    // Update stored state for next interrupt.
    prev_input_state_ = current_state;

    // Dispatch to per-pin callbacks, filtering by requested trigger type.
    for (int pin = 0; pin < 16; ++pin) {
        if (!(status & (1U << pin))) continue;

        auto& gpio_pin = pin_registry_[pin];
        if (!gpio_pin || !gpio_pin->interrupt_enabled_ ||
            !gpio_pin->interrupt_callback_) {
            continue;
        }

        const uint16_t mask = static_cast<uint16_t>(1U << pin);
        const auto trigger = gpio_pin->interrupt_trigger_;
        bool fire = false;

        switch (trigger) {
            case hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_RISING_EDGE:
                fire = (rising & mask) != 0;
                break;
            case hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_FALLING_EDGE:
                fire = (falling & mask) != 0;
                break;
            case hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_BOTH_EDGES:
                fire = ((rising | falling) & mask) != 0;
                break;
            default:
                break;  // NONE or unknown -- skip
        }

        if (fire) {
            // Report the actual observed trigger, not just the configured one.
            hf_gpio_interrupt_trigger_t actual =
                (rising & mask)
                    ? hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_RISING_EDGE
                    : hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_FALLING_EDGE;
            gpio_pin->interrupt_callback_(
                gpio_pin, actual, gpio_pin->interrupt_user_data_);
        }
    }
}

Pcal95555Handler::Pcal95555Driver* Pcal95555Handler::GetDriver() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return nullptr;
    return pcal95555_driver_.get();
}

const Pcal95555Handler::Pcal95555Driver* Pcal95555Handler::GetDriver() const noexcept {
    auto* self = const_cast<Pcal95555Handler*>(this);
    return self->GetDriver();
}

// =====================================================================
// Pcal95555Handler -- Pin Factory
// =====================================================================

uint8_t Pcal95555Handler::GetI2cAddress() const noexcept {
    return pcal95555_driver_ ? pcal95555_driver_->GetAddress() : 0;
}

namespace {

struct NonOwningGpioDeleter {
    void operator()(BaseGpio*) const noexcept {}
};

std::shared_ptr<BaseGpio> ShareNonOwning(Pcal95555GpioPin* pin) noexcept {
    if (pin == nullptr) {
        return nullptr;
    }
    return std::shared_ptr<BaseGpio>(static_cast<BaseGpio*>(pin),
                                     NonOwningGpioDeleter{});
}

}  // namespace

bool Pcal95555Handler::AttachStaticPin(Pcal95555GpioPin& pin) noexcept {
    const hf_pin_num_t idx = pin.GetPin();
    if (idx >= 16) {
        return false;
    }

    /* Initialize outside the handler mutex — soft-attach Initialize() does not
     * re-enter SetDirection when configure_hardware=false. */
    if (!pin.Initialize()) {
        return false;
    }

    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return false;
    }
    if (pin_registry_[idx] != nullptr && pin_registry_[idx] != &pin) {
        return false;
    }
    /* Static attach must not collide with a heap-owned slot. */
    if (owned_pins_[idx] != nullptr && owned_pins_[idx].get() != &pin) {
        return false;
    }
    pin_registry_[idx] = &pin;
    return true;
}

std::shared_ptr<BaseGpio> Pcal95555Handler::CreateGpioPin(
    hf_pin_num_t pin,
    hf_gpio_direction_t direction,
    hf_gpio_active_state_t active_state,
    hf_gpio_output_mode_t output_mode,
    hf_gpio_pull_mode_t pull_mode,
    bool allow_existing,
    bool configure_hardware) noexcept {

    if (pin >= 16) return nullptr;

    /* Do not hold handler_mutex_ across unique_ptr alloc / pin->Initialize(). */
    {
        MutexLockGuard lock(handler_mutex_);
        if (!EnsureInitializedLocked()) return nullptr;
        if (pin_registry_[pin] != nullptr) {
            return allow_existing ? ShareNonOwning(pin_registry_[pin]) : nullptr;
        }
    }

    auto new_pin = std::make_unique<Pcal95555GpioPin>(
        pin, this, direction, active_state, output_mode, pull_mode,
        configure_hardware);
    if (!new_pin || !new_pin->Initialize()) {
        return nullptr;
    }

    MutexLockGuard lock(handler_mutex_);
    if (pin_registry_[pin] != nullptr) {
        return allow_existing ? ShareNonOwning(pin_registry_[pin]) : nullptr;
    }
    Pcal95555GpioPin* raw = new_pin.get();
    owned_pins_[pin] = std::move(new_pin);
    pin_registry_[pin] = raw;
    return ShareNonOwning(raw);
}

std::shared_ptr<BaseGpio> Pcal95555Handler::GetGpioPin(hf_pin_num_t pin) noexcept {
    return ShareNonOwning(GetGpioPinRaw(pin));
}

Pcal95555GpioPin* Pcal95555Handler::GetGpioPinRaw(hf_pin_num_t pin) noexcept {
    if (pin >= 16) return nullptr;
    MutexLockGuard lock(handler_mutex_);
    return pin_registry_[pin];
}

bool Pcal95555Handler::IsPinCreated(hf_pin_num_t pin) const noexcept {
    if (pin >= 16) return false;
    MutexLockGuard lock(handler_mutex_);
    return pin_registry_[pin] != nullptr;
}

uint16_t Pcal95555Handler::GetCreatedPinMask() const noexcept {
    MutexLockGuard lock(handler_mutex_);
    uint16_t mask = 0;
    for (hf_pin_num_t i = 0; i < 16; ++i) {
        if (pin_registry_[i] != nullptr) {
            mask |= (1U << i);
        }
    }
    return mask;
}

hf_gpio_err_t Pcal95555Handler::ReadAllInputLevels(uint16_t& levels) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
    /* Prefer dual-port path (same as `pcal status`) over ReadAllInputs —
     * keeps Port1 assembly in driver .bss and matches the proven bank read. */
    levels = 0;
    if (!pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), levels)) {
        return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
    }
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::ProgramPortsAbsolute(uint16_t config,
                                                     uint16_t output,
                                                     uint16_t out_check_mask) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }

    polarity_shadow_ = 0U;
    output_shadow_ = output;
    config_shadow_ = config;

    /* POL → CONFIG → OUTPUT (OUTPUT last). A following INPUT_PORT_1 read can
     * leave Port1 (odd) register reads stuck returning INPUT_1; keep OUTPUT
     * as the last write and verify OUTPUT/CONFIG/POL before any INPUT sample. */
    const bool pol_ok = pcal95555_driver_->WriteDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_0),
        static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_1), polarity_shadow_);
    const bool cfg_ok = pcal95555_driver_->WriteDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1), config_shadow_);
    const bool out_ok = pcal95555_driver_->WriteDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), output_shadow_);
    if (!pol_ok || !out_ok || !cfg_ok) {
        return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
    }
    port_shadow_valid_ = true;

    uint16_t out_rb = 0;
    uint16_t pol_rb = 0;
    uint16_t cfg_rb = 0;
    const bool bank_ok =
        pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), out_rb) &&
        pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1), cfg_rb) &&
        pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_0),
            static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_1), pol_rb);
    if (!bank_ok) {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }
    const bool phantom = (out_rb == pol_rb && pol_rb == cfg_rb);
    if (phantom) {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }
    const bool idle_high_ok = (out_rb & output) == output;
    const bool cfg_ok_rb = (cfg_rb == config);
    const bool pol_ok_rb = (pol_rb == 0U);
    if (idle_high_ok && cfg_ok_rb && pol_ok_rb) {
        return hf_gpio_err_t::GPIO_SUCCESS;
    }
    (void)out_check_mask;
    return (pol_ok && out_ok && cfg_ok) ? hf_gpio_err_t::GPIO_SUCCESS
                                        : hf_gpio_err_t::GPIO_ERR_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::ReadOutputLatchFromWire(uint16_t& output) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
    output = 0;
    /* OUTPUT only — never touch INPUT_PORT here (poisons Port1 readback). */
    return pcal95555_driver_->ReadDualPortRegister(
               static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
               static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), output)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::ReadConfigAndOutputPorts(uint16_t& config,
                                                         uint16_t& output) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
    if (port_shadow_valid_) {
        config = config_shadow_;
        output = output_shadow_;
        return hf_gpio_err_t::GPIO_SUCCESS;
    }
    config = 0;
    output = 0;
    const bool cfg_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1), config);
    const bool out_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), output);
    return (cfg_ok && out_ok) ? hf_gpio_err_t::GPIO_SUCCESS
                              : hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::ReadStandardRegisterBank(
    uint16_t& input, uint16_t& output, uint16_t& polarity,
    uint16_t& config) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }
    input = output = polarity = config = 0;
    /* Production model: OUTPUT/CONFIG/POL come from the absolute-program
     * shadow. Port1 (odd) bus readback is not trustworthy after INPUT_PORT
     * traffic on some expander buses (reads return INPUT_1). INPUT is always
     * sampled from the wire. Latch prove (write then immediate OUTPUT read)
     * remains the bus check for write-effect. */
    if (port_shadow_valid_) {
        output = output_shadow_;
        polarity = polarity_shadow_;
        config = config_shadow_;
        const bool in_ok = pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input);
        return in_ok ? hf_gpio_err_t::GPIO_SUCCESS
                     : hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
    }
    const bool out_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), output);
    const bool pol_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_0),
        static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_1), polarity);
    const bool cfg_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1), config);
    const bool in_ok = pcal95555_driver_->ReadDualPortRegister(
        static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
        static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input);
    return (in_ok && out_ok && pol_ok && cfg_ok) ? hf_gpio_err_t::GPIO_SUCCESS
                                                 : hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::VerifyOutputPinEffect(
    uint8_t pin, uint16_t& latch_high, uint16_t& input_high, uint16_t& latch_low,
    uint16_t& input_low) noexcept {
    latch_high = input_high = latch_low = input_low = 0;
    if (!ValidatePin(pin)) {
        return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    }
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) {
        return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    }

    ClearErrorFlags();
    const uint16_t bit = static_cast<uint16_t>(1u << pin);

    /* Absolute CONFIG/OUTPUT via shadow when available. */
    if (port_shadow_valid_) {
        config_shadow_ =
            static_cast<uint16_t>(config_shadow_ & static_cast<uint16_t>(~bit));
        if (!pcal95555_driver_->WriteDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::CONFIG_PORT_1),
                config_shadow_)) {
            return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
        }
        output_shadow_ = static_cast<uint16_t>(output_shadow_ | bit);
        if (!pcal95555_driver_->WriteDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1),
                output_shadow_)) {
            return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
        }
        /* Latch proof must come from the OUTPUT register on the wire — not the
         * shadow we just wrote (shadow-only made latch_ok a tautology). */
        if (!pcal95555_driver_->ReadDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), latch_high) ||
            !pcal95555_driver_->ReadDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input_high)) {
            return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
        }
        output_shadow_ =
            static_cast<uint16_t>(output_shadow_ & static_cast<uint16_t>(~bit));
        if (!pcal95555_driver_->WriteDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1),
                output_shadow_)) {
            return hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
        }
        if (!pcal95555_driver_->ReadDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), latch_low) ||
            !pcal95555_driver_->ReadDualPortRegister(
                static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
                static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input_low)) {
            return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
        }
        const bool pad_ok =
            ((input_high & bit) != 0U) && ((input_low & bit) == 0U);
        const bool latch_ok =
            ((latch_high & bit) != 0U) && ((latch_low & bit) == 0U);
        return (latch_ok && pad_ok) ? hf_gpio_err_t::GPIO_SUCCESS
                                    : (latch_ok ? hf_gpio_err_t::GPIO_SUCCESS
                                                : hf_gpio_err_t::GPIO_ERR_FAILURE);
    }

    if (!pcal95555_driver_->SetPinDirection(pin, GPIODir::Output) ||
        !pcal95555_driver_->WritePin(pin, true) ||
        !pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), latch_high) ||
        !pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input_high) ||
        !pcal95555_driver_->WritePin(pin, false) ||
        !pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::OUTPUT_PORT_1), latch_low) ||
        !pcal95555_driver_->ReadDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_0),
            static_cast<uint8_t>(Pcal95555Reg::INPUT_PORT_1), input_low)) {
        return hf_gpio_err_t::GPIO_ERR_FAILURE;
    }
    const bool latch_ok = ((latch_high & bit) != 0U) && ((latch_low & bit) == 0U);
    return latch_ok ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_FAILURE;
}

// =====================================================================
// Pcal95555Handler -- PCAL9555A Advanced Features (Agile I/O)
// =====================================================================

bool Pcal95555Handler::HasAgileIO() const noexcept {
    return pcal95555_driver_ && pcal95555_driver_->HasAgileIO();
}

pcal95555::ChipVariant Pcal95555Handler::GetChipVariant() const noexcept {
    return pcal95555_driver_
               ? pcal95555_driver_->GetChipVariant()
               : pcal95555::ChipVariant::Unknown;
}

hf_gpio_err_t Pcal95555Handler::SetPolarityInversion(hf_pin_num_t pin, bool invert) noexcept {
    if (!ValidatePin(static_cast<uint8_t>(pin))) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    Polarity pol = invert ? Polarity::Inverted : Polarity::Normal;
    return pcal95555_driver_->SetPinPolarity(pin, pol)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetMultiplePolarityInversion(uint16_t pin_mask,
                                                             bool invert) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    if (pin_mask == 0U) return hf_gpio_err_t::GPIO_SUCCESS;

    if (port_shadow_valid_) {
        if (invert) {
            polarity_shadow_ = static_cast<uint16_t>(polarity_shadow_ | pin_mask);
        } else {
            polarity_shadow_ = static_cast<uint16_t>(
                polarity_shadow_ & static_cast<uint16_t>(~pin_mask));
        }
        const bool ok = pcal95555_driver_->WriteDualPortRegister(
            static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_0),
            static_cast<uint8_t>(Pcal95555Reg::POLARITY_INV_1), polarity_shadow_);
        return ok ? hf_gpio_err_t::GPIO_SUCCESS
                  : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
    }

    const Polarity pol = invert ? Polarity::Inverted : Polarity::Normal;
    return pcal95555_driver_->SetMultiplePolarities(pin_mask, pol)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetInterruptMask(hf_pin_num_t pin, bool mask) noexcept {
    if (!ValidatePin(static_cast<uint8_t>(pin))) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    InterruptState state = mask ? InterruptState::Disabled : InterruptState::Enabled;
    return pcal95555_driver_->ConfigureInterrupt(pin, state)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::GetInterruptStatus(hf_pin_num_t pin, bool& status) noexcept {
    if (!ValidatePin(static_cast<uint8_t>(pin))) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    uint16_t global_status = pcal95555_driver_->GetInterruptStatus();
    status = (global_status & (1U << pin)) != 0;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555Handler::SetDriveStrength(hf_pin_num_t pin,
                                        DriveStrength level) noexcept {
    if (!ValidatePin(static_cast<uint8_t>(pin))) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    return pcal95555_driver_->SetDriveStrength(pin, level)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::EnableInputLatch(hf_pin_num_t pin, bool enable) noexcept {
    if (!ValidatePin(static_cast<uint8_t>(pin))) return hf_gpio_err_t::GPIO_ERR_INVALID_PIN;
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    return pcal95555_driver_->EnableInputLatch(pin, enable)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::SetOutputMode(bool port0_open_drain,
                                     bool port1_open_drain) noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    return pcal95555_driver_->SetOutputMode(port0_open_drain, port1_open_drain)
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_COMMUNICATION_FAILURE;
}

hf_gpio_err_t Pcal95555Handler::ResetToDefault() noexcept {
    MutexLockGuard lock(handler_mutex_);
    if (!EnsureInitializedLocked()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    pcal95555_driver_->ResetToDefault();  // void return
    pull_mode_cache_.fill(hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING);
    return hf_gpio_err_t::GPIO_SUCCESS;
}

// =====================================================================
// Pcal95555Handler -- Error Management
// =====================================================================

uint16_t Pcal95555Handler::GetErrorFlags() const noexcept {
    return pcal95555_driver_ ? pcal95555_driver_->GetErrorFlags() : 0;
}

void Pcal95555Handler::ClearErrorFlags(uint16_t mask) noexcept {
    if (pcal95555_driver_) {
        pcal95555_driver_->ClearErrorFlags(mask);
    }
}

// =====================================================================
// Pcal95555Handler -- Diagnostics
// =====================================================================

void Pcal95555Handler::DumpDiagnostics() const noexcept {
    static constexpr const char* TAG = "Pcal95555Handler";

    Logger::GetInstance().Info(TAG, "=== PCAL95555 HANDLER DIAGNOSTICS ===");

    MutexLockGuard lock(handler_mutex_);

    // System Health
    Logger::GetInstance().Info(TAG, "System Health:");
    Logger::GetInstance().Info(TAG, "  Initialized: %s",
                              initialized_ ? "YES" : "NO");

    // I2C Interface
    Logger::GetInstance().Info(TAG, "I2C Interface:");
    if (i2c_adapter_) {
        Logger::GetInstance().Info(TAG, "  I2C Adapter: ACTIVE (CRTP-based)");
        Logger::GetInstance().Info(TAG, "  Device Address: 0x%02X",
                                  i2c_device_.GetDeviceAddress());
    } else {
        Logger::GetInstance().Info(TAG, "  I2C Adapter: NOT_INITIALIZED");
    }

    // Driver Status
    Logger::GetInstance().Info(TAG, "PCAL95555 Driver:");
    if (pcal95555_driver_) {
        const char* variant_str = "Unknown";
        auto variant = pcal95555_driver_->GetChipVariant();
        if (variant == pcal95555::ChipVariant::PCAL9555A) {
            variant_str = "PCAL9555A (Agile I/O)";
        } else if (variant == pcal95555::ChipVariant::PCA9555) {
            variant_str = "PCA9555 (Standard)";
        }

        Logger::GetInstance().Info(TAG, "  Driver Instance: ACTIVE");
        Logger::GetInstance().Info(TAG, "  Chip Variant: %s", variant_str);
        Logger::GetInstance().Info(TAG, "  I2C Address: 0x%02X",
                                  pcal95555_driver_->GetAddress());
        Logger::GetInstance().Info(TAG, "  Error Flags: 0x%04X",
                                  pcal95555_driver_->GetErrorFlags());
    } else {
        Logger::GetInstance().Info(TAG, "  Driver Instance: NOT_INITIALIZED");
    }

    // Pin Registry Status
    Logger::GetInstance().Info(TAG, "Pin Registry:");
    int active_pins = 0;
    int interrupt_pins = 0;
    for (size_t i = 0; i < pin_registry_.size(); ++i) {
        if (pin_registry_[i] != nullptr) {
            active_pins++;
            if (pin_registry_[i]->interrupt_enabled_) {
                interrupt_pins++;
            }
        }
    }
    Logger::GetInstance().Info(TAG, "  Active Pin Objects: %d/16", active_pins);
    Logger::GetInstance().Info(TAG, "  Pins with Interrupts: %d", interrupt_pins);

    // Interrupt Configuration
    Logger::GetInstance().Info(TAG, "Interrupt Configuration:");
    Logger::GetInstance().Info(TAG, "  Hardware Interrupt Pin: %s",
                              interrupt_pin_ ? "CONFIGURED" : "NOT_CONFIGURED");
    Logger::GetInstance().Info(TAG, "  Interrupt System: %s",
                              interrupt_configured_ ? "ENABLED" : "DISABLED");

    // Active Pin Details
    Logger::GetInstance().Info(TAG, "Active Pin Details:");
    int shown = 0;
    for (size_t i = 0; i < pin_registry_.size(); ++i) {
        if (!pin_registry_[i]) continue;
        ++shown;

        const char* trigger_str = "NONE";
        auto t = pin_registry_[i]->interrupt_trigger_;
        if (t == hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_RISING_EDGE) {
            trigger_str = "RISING";
        } else if (t == hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_FALLING_EDGE) {
            trigger_str = "FALLING";
        } else if (t == hf_gpio_interrupt_trigger_t::HF_GPIO_INTERRUPT_TRIGGER_BOTH_EDGES) {
            trigger_str = "BOTH";
        }

        Logger::GetInstance().Info(
            TAG, "  Pin %d: Int=%s Trigger=%s",
            static_cast<int>(i),
            pin_registry_[i]->interrupt_enabled_ ? "ON" : "OFF",
            trigger_str);
    }
    if (shown == 0) {
        Logger::GetInstance().Info(TAG, "  No active pins");
    }

    // Overall Status
    bool healthy = initialized_ && pcal95555_driver_ && i2c_adapter_;
    Logger::GetInstance().Info(TAG, "System Status: %s",
                              healthy ? "HEALTHY" : "DEGRADED");

    Logger::GetInstance().Info(TAG, "=== END PCAL95555 HANDLER DIAGNOSTICS ===");
}

// =====================================================================
// Pcal95555GpioPin Implementation
// =====================================================================

Pcal95555GpioPin::Pcal95555GpioPin(
    hf_pin_num_t pin,
    Pcal95555Handler* parent_handler,
    hf_gpio_direction_t direction,
    hf_gpio_active_state_t active_state,
    hf_gpio_output_mode_t output_mode,
    hf_gpio_pull_mode_t pull_mode,
    bool configure_hardware) noexcept
    : BaseGpio(pin, direction, active_state, output_mode, pull_mode),
      pin_(pin),
      parent_handler_(parent_handler),
      configure_hardware_(configure_hardware) {
    snprintf(description_, sizeof(description_), "PCAL95555_PIN_%d",
             static_cast<int>(pin_));
}

bool Pcal95555GpioPin::Initialize() noexcept {
    if (!parent_handler_) return false;

    /* Soft attach: directions/pulls already programmed (batch bring-up). */
    if (!configure_hardware_) {
        initialized_ = true;
        return true;
    }

    // Configure direction via the handler (which routes to the driver).
    hf_gpio_err_t dir_result = hf_gpio_err_t::GPIO_ERR_FAILURE;
    for (int attempt = 0; attempt < 5; ++attempt) {
        dir_result = parent_handler_->SetDirection(
            static_cast<uint8_t>(pin_), current_direction_);
        if (dir_result == hf_gpio_err_t::GPIO_SUCCESS) {
            break;
        }
    }
    if (dir_result != hf_gpio_err_t::GPIO_SUCCESS) {
        return false;
    }

    // Configure pull mode via handler (only for PCAL9555A; non-fatal on PCA9555).
    if (parent_handler_->HasAgileIO() &&
        pull_mode_ != hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING) {
        parent_handler_->SetPullMode(static_cast<uint8_t>(pin_), pull_mode_);
        // Pull mode failure is non-fatal.
    }

    initialized_ = true;
    return true;
}

bool Pcal95555GpioPin::Deinitialize() noexcept {
    initialized_ = false;
    return true;
}

bool Pcal95555GpioPin::IsPinAvailable() const noexcept {
    return parent_handler_ && pin_ < 16;
}

hf_gpio_err_t Pcal95555GpioPin::SupportsInterrupts() const noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NULL_POINTER;
    return (parent_handler_->HasAgileIO() && parent_handler_->HasInterruptSupport())
               ? hf_gpio_err_t::GPIO_SUCCESS
               : hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
}

const char* Pcal95555GpioPin::GetDescription() const noexcept {
    return description_;
}

hf_gpio_err_t Pcal95555GpioPin::ConfigureInterrupt(
    hf_gpio_interrupt_trigger_t trigger,
    InterruptCallback callback,
    void* user_data) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NULL_POINTER;
    return parent_handler_->RegisterPinInterrupt(pin_, trigger, callback, user_data);
}

// --- PCAL9555A Advanced Features ---

hf_gpio_err_t Pcal95555GpioPin::SetPolarityInversion(bool invert) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    return parent_handler_->SetPolarityInversion(pin_, invert);
}

hf_gpio_err_t Pcal95555GpioPin::SetInterruptMask(bool mask) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    return parent_handler_->SetInterruptMask(pin_, mask);
}

hf_gpio_err_t Pcal95555GpioPin::GetInterruptStatus(bool& status) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    return parent_handler_->GetInterruptStatus(pin_, status);
}

// --- BaseGpio Protected Implementation ---

hf_gpio_err_t Pcal95555GpioPin::SetDirectionImpl(
    hf_gpio_direction_t direction) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    return parent_handler_->SetDirection(static_cast<uint8_t>(pin_), direction);
}

hf_gpio_err_t Pcal95555GpioPin::SetOutputModeImpl(
    hf_gpio_output_mode_t /*mode*/) noexcept {
    // PCAL9555A only supports output mode at per-port granularity (pins 0-7
    // share one mode, pins 8-15 share another). Per-pin changes are not
    // supported; use Pcal95555Handler::SetOutputMode() for port-level control.
    return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
}

hf_gpio_err_t Pcal95555GpioPin::SetPullModeImpl(
    hf_gpio_pull_mode_t mode) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    return parent_handler_->SetPullMode(static_cast<uint8_t>(pin_), mode);
}

hf_gpio_pull_mode_t Pcal95555GpioPin::GetPullModeImpl() const noexcept {
    if (!parent_handler_) {
        return hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING;
    }
    hf_gpio_pull_mode_t mode;
    if (parent_handler_->GetPullMode(static_cast<uint8_t>(pin_), mode) ==
        hf_gpio_err_t::GPIO_SUCCESS) {
        return mode;
    }
    return hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING;
}

hf_gpio_err_t Pcal95555GpioPin::SetPinLevelImpl(
    hf_gpio_level_t level) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    bool hw_level = (level == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
    return parent_handler_->SetOutput(static_cast<uint8_t>(pin_), hw_level);
}

hf_gpio_err_t Pcal95555GpioPin::GetPinLevelImpl(
    hf_gpio_level_t& level) noexcept {
    if (!parent_handler_) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    bool active = false;
    auto result = parent_handler_->ReadInput(static_cast<uint8_t>(pin_), active);
    if (result == hf_gpio_err_t::GPIO_SUCCESS) {
        level = active ? hf_gpio_level_t::HF_GPIO_LEVEL_HIGH
                       : hf_gpio_level_t::HF_GPIO_LEVEL_LOW;
    }
    return result;
}

hf_gpio_err_t Pcal95555GpioPin::GetDirectionImpl(
    hf_gpio_direction_t& direction) const noexcept {
    // Direction is tracked in the BaseGpio base class after SetDirection().
    direction = current_direction_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Pcal95555GpioPin::GetOutputModeImpl(
    hf_gpio_output_mode_t& mode) const noexcept {
    // Output mode is tracked in the BaseGpio base class.
    mode = output_mode_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}
