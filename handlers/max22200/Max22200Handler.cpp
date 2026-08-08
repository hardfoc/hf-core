/**
 * @file Max22200Handler.cpp
 * @brief Implementation of MAX22200 handler with SPI communication adapter.
 * @copyright Copyright (c) 2024-2025 HardFOC. All rights reserved.
 */

#include "Max22200Handler.h"
#include "Logger.h"
#include "HandlerCommon.h"

#include <cstdint>
#include <cstring>

namespace {
/* Fixed SPI staging buffers in TU static storage (internal SRAM).
 * The MAX protocol builds short frames that must not be handed to the SPI
 * peripheral from a task stack or external RAM on MCUs with restricted
 * single-byte access outside tightly-coupled memory. */
uint8_t g_max_spi_tx[8]{};
uint8_t g_max_spi_rx[8]{};
}  // namespace
#include "OsUtility.h"

static constexpr const char* TAG = "MAX22200";

///////////////////////////////////////////////////////////////////////////////
// HalSpiMax22200Comm Implementation
///////////////////////////////////////////////////////////////////////////////

HalSpiMax22200Comm::HalSpiMax22200Comm(
    BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
    BaseGpio* fault) noexcept
    : spi_(spi), enable_(enable), cmd_(cmd), fault_(fault) {}

bool HalSpiMax22200Comm::Initialize() noexcept {
    auto& log = Logger::GetInstance();

    if (!spi_.EnsureInitialized()) {
        log.Error(TAG, "comm.Init: SPI EnsureInitialized failed");
        initialized_ = false;
        return false;
    }

    /* Map already programmed directions — polarity + park only.
     * Keep ENABLE HIGH: forcing it low here then relying on the driver's
     * 0.5 ms post-rise delay is too short for PCAL-backed bench stand-in and
     * left STATUS/ACTIVE dead while a raw CS ping still saw COMER (0x04). */
    enable_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!enable_.EnsureInitialized()) {
        log.Error(TAG, "comm.Init: ENABLE.EnsureInitialized failed");
        initialized_ = false;
        return false;
    }
    auto err = enable_.SetActive();
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        log.Error(TAG, "comm.Init: ENABLE.SetActive failed (%d)", static_cast<int>(err));
        initialized_ = false;
        return false;
    }

    cmd_.SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH);
    if (!cmd_.EnsureInitialized()) {
        log.Error(TAG, "comm.Init: CMD.EnsureInitialized failed");
        initialized_ = false;
        return false;
    }
    /* Park CMD HIGH (command-write phase); driver toggles per transaction. */
    err = cmd_.SetActive();
    if (err != hf_gpio_err_t::GPIO_SUCCESS) {
        log.Error(TAG, "comm.Init: CMD.SetActive failed (%d)", static_cast<int>(err));
        initialized_ = false;
        return false;
    }

    if (fault_) {
        fault_->SetActiveState(hf_gpio_active_state_t::HF_GPIO_ACTIVE_LOW);
        if (!fault_->EnsureInitialized()) {
            log.Error(TAG, "comm.Init: FAULT.EnsureInitialized failed");
            initialized_ = false;
            return false;
        }
    }

    /* PCAL + VM wake margin before first STATUS (driver adds another 0.5 ms). */
    DelayUs(5000);

    log.Info(TAG, "comm.Init: OK (EN=HIGH, CMD=HIGH, FAULT=input)");
    initialized_ = true;
    return true;
}

bool HalSpiMax22200Comm::Transfer(const uint8_t* tx_data, uint8_t* rx_data, size_t length) noexcept {
    if (!IsReady() || tx_data == nullptr || rx_data == nullptr || length == 0) {
        return false;
    }
    if (length > sizeof(g_max_spi_tx)) {
        return false;
    }
    /* Copy TX via aligned 32-bit loads into internal SRAM staging.
     * Plain byte loads from external/task-stack buffers are unsafe on some
     * MCU memory maps; word loads keep the SPI path coherent. */
    for (size_t i = 0; i < length; ++i) {
        const auto addr = reinterpret_cast<uintptr_t>(tx_data + i);
        const auto aligned = addr & ~static_cast<uintptr_t>(3U);
        const uint32_t word =
            *reinterpret_cast<const volatile uint32_t*>(aligned);
        const unsigned shift = static_cast<unsigned>((addr & 3U) * 8U);
        g_max_spi_tx[i] = static_cast<uint8_t>((word >> shift) & 0xFFU);
    }
    auto err = spi_.Transfer(g_max_spi_tx, g_max_spi_rx,
                             static_cast<hf_u16_t>(length), hf_u32_t{0});
    if (err != hf_spi_err_t::SPI_SUCCESS) {
        return false;
    }
    for (size_t i = 0; i < length; ++i) {
        const auto addr = reinterpret_cast<uintptr_t>(rx_data + i);
        const auto aligned = addr & ~static_cast<uintptr_t>(3U);
        volatile uint32_t* cell = reinterpret_cast<volatile uint32_t*>(aligned);
        const unsigned shift = static_cast<unsigned>((addr & 3U) * 8U);
        const uint32_t mask = static_cast<uint32_t>(0xFFU) << shift;
        const uint32_t old = *cell;
        *cell = (old & ~mask) | (static_cast<uint32_t>(g_max_spi_rx[i]) << shift);
    }
    return true;
}

bool HalSpiMax22200Comm::SetChipSelect(bool /*state*/) noexcept {
    /* Soft-CS is owned by StmSpiDevice (SoftChipSelectGuard per Transfer).
     * External MAX driver may call this — ignore; never drive CS here. */
    return true;
}

bool HalSpiMax22200Comm::Configure(uint32_t /*speed_hz*/, uint8_t /*mode*/, bool /*msb_first*/) noexcept {
    // BaseSpi is pre-configured
    return true;
}

bool HalSpiMax22200Comm::IsReady() const noexcept {
    if (!initialized_) return false;
    if (!spi_.IsInitialized()) return false;
    if (!enable_.IsInitialized() || !cmd_.IsInitialized()) return false;
    if (fault_ && !fault_->IsInitialized()) return false;
    return true;
}

void HalSpiMax22200Comm::DelayUs(uint32_t us) noexcept {
    handler_utils::DelayUs(us);
}

void HalSpiMax22200Comm::GpioSet(max22200::CtrlPin pin, max22200::GpioSignal signal) noexcept {
    if (!initialized_) return;

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case max22200::CtrlPin::ENABLE:  gpio = &enable_; break;
        case max22200::CtrlPin::CMD:     gpio = &cmd_;    break;
        case max22200::CtrlPin::FAULT:   gpio = fault_;   break;
        default: return;
    }

    if (gpio == nullptr) return;

    hf_gpio_err_t gpio_err = hf_gpio_err_t::GPIO_SUCCESS;
    if (signal == max22200::GpioSignal::ACTIVE) {
        gpio_err = gpio->SetActive();
    } else {
        gpio_err = gpio->SetInactive();
    }

    if (gpio_err != hf_gpio_err_t::GPIO_SUCCESS) {
        /* Do not clear initialized_ — a single PCAL I2C glitch during CMD
         * toggle must not permanently kill IsReady()/Transfer mid-init. */
        Logger::GetInstance().Error(TAG, "GPIO control failed for MAX22200 control pin");
        return;
    }

    /* EN is still PCAL I2C on live-actuator bring-up; CMD is MCU PD5 (no settle). */
    if (pin == max22200::CtrlPin::ENABLE) {
        DelayUs(2000);
    } else if (pin == max22200::CtrlPin::CMD) {
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
        /* MCU GPIO: datasheet tCMS is 20 ns — no software settle. */
#else
        DelayUs(200);
#endif
    }
}

bool HalSpiMax22200Comm::GpioRead(max22200::CtrlPin pin, max22200::GpioSignal& signal) noexcept {
    if (!IsReady()) return false;

    BaseGpio* gpio = nullptr;

    switch (pin) {
        case max22200::CtrlPin::ENABLE:  gpio = &enable_; break;
        case max22200::CtrlPin::CMD:     gpio = &cmd_;    break;
        case max22200::CtrlPin::FAULT:   gpio = fault_;   break;
        default: return false;
    }

    if (gpio == nullptr) return false;

    bool is_active = false;
    auto err = gpio->IsActive(is_active);
    if (err != hf_gpio_err_t::GPIO_SUCCESS) return false;

    signal = is_active ? max22200::GpioSignal::ACTIVE
                       : max22200::GpioSignal::INACTIVE;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Max22200Handler Implementation
///////////////////////////////////////////////////////////////////////////////

Max22200Handler::Max22200Handler(
    BaseSpi& spi, BaseGpio& enable, BaseGpio& cmd,
    BaseGpio* fault) noexcept {
    comm_ = std::make_unique<HalSpiMax22200Comm>(spi, enable, cmd, fault);
    Logger::GetInstance().Info(TAG, "MAX22200 handler created");
}

Max22200Handler::~Max22200Handler() noexcept {
    if (initialized_) {
        Deinitialize();
    }
}

max22200::DriverStatus Max22200Handler::Initialize() noexcept {
    MutexLockGuard lock(mutex_);
    return InitializeLocked();
}

max22200::DriverStatus Max22200Handler::InitializeLocked() noexcept {
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return max22200::DriverStatus::OK;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return max22200::DriverStatus::INITIALIZATION_ERROR;
    }

    driver_ = std::make_unique<DriverType>(*comm_);
    auto status = driver_->Initialize();
    if (status != max22200::DriverStatus::OK) {
        last_fault_byte_ = driver_->GetLastFaultByte();
        Logger::GetInstance().Error(TAG, "Driver init failed: %s fault=0x%02X",
                                   max22200::DriverStatusToStr(status),
                                   last_fault_byte_);
        driver_.reset();
        return status;
    }

    if (!WaitForActiveAndDrainFaults()) {
        last_fault_byte_ = driver_ ? driver_->GetLastFaultByte() : last_fault_byte_;
        driver_.reset();
        return max22200::DriverStatus::INITIALIZATION_ERROR;
    }

    last_fault_byte_ = driver_->GetLastFaultByte();
    initialized_ = true;
    Logger::GetInstance().Info(TAG, "MAX22200 initialized successfully");
    return max22200::DriverStatus::OK;
}

bool Max22200Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(mutex_);
    return EnsureInitializedLocked();
}

max22200::DriverStatus Max22200Handler::Initialize(const max22200::BoardConfig& board_config) noexcept {
    MutexLockGuard lock(mutex_);
    if (initialized_) {
        Logger::GetInstance().Warn(TAG, "Already initialized");
        return max22200::DriverStatus::OK;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return max22200::DriverStatus::INITIALIZATION_ERROR;
    }

    driver_ = std::make_unique<DriverType>(*comm_);

    // Set board config (void return)
    driver_->SetBoardConfig(board_config);

    // Then initialize
    auto status = driver_->Initialize();
    if (status != max22200::DriverStatus::OK) {
        last_fault_byte_ = driver_->GetLastFaultByte();
        Logger::GetInstance().Error(TAG, "Driver init failed: %s fault=0x%02X",
                                   max22200::DriverStatusToStr(status),
                                   last_fault_byte_);
        driver_.reset();
        return status;
    }

    if (!WaitForActiveAndDrainFaults()) {
        last_fault_byte_ = driver_ ? driver_->GetLastFaultByte() : last_fault_byte_;
        driver_.reset();
        return max22200::DriverStatus::INITIALIZATION_ERROR;
    }

    last_fault_byte_ = driver_->GetLastFaultByte();
    initialized_ = true;
    Logger::GetInstance().Info(TAG, "MAX22200 initialized with board config");
    return max22200::DriverStatus::OK;
}

bool Max22200Handler::EnsureInitializedLocked() noexcept {
    if (initialized_ && driver_) {
        return true;
    }
    if (!comm_) {
        Logger::GetInstance().Error(TAG, "Comm adapter not created");
        return false;
    }
    return InitializeLocked() == max22200::DriverStatus::OK;
}

uint8_t Max22200Handler::GetLastFaultByte() const noexcept {
    return driver_ ? driver_->GetLastFaultByte() : last_fault_byte_;
}

bool Max22200Handler::WaitForActiveAndDrainFaults() noexcept {
    // Proven ESP / troubleshooting pattern: bare STATUS=0x1 ACTIVE poke after
    // Initialize(), then ReadStatus until ACTIVE latches (t_WU + slow V18).
    // See hf-max22200-driver/docs/troubleshooting.md.
    constexpr uint32_t kPostInitWaitMs = 50;
    constexpr uint32_t kPollIntervalMs = 25;
#if defined(PW_FEATURE_LIVE_ACTUATORS) && PW_FEATURE_LIVE_ACTUATORS
    constexpr uint32_t kPollTimeoutMs = 4000;
#else
    constexpr uint32_t kPollTimeoutMs = 2000;
#endif

    auto& log = Logger::GetInstance();

    os_thread_sleep(os_convert_msec_to_delay_ticks(kPostInitWaitMs));

    max22200::StatusConfig st{};
    uint32_t waited_ms = 0;
    uint32_t status_ok_polls = 0;
    while (waited_ms < kPollTimeoutMs) {
        (void)driver_->WriteRegister32(max22200::RegBank::STATUS, 0x00000001U);
        const auto rs = driver_->ReadStatus(st);
        last_fault_byte_ = driver_->GetLastFaultByte();
        if (rs == max22200::DriverStatus::OK) {
            ++status_ok_polls;
        }
        /* ACTIVE=1 is the bring-up gate. UVM alone is logged but not fatal —
         * bench stand-in rails can leave a sticky UVM bit while SPI is healthy. */
        if (st.active) {
            if (st.undervoltage) {
                log.Warn(TAG,
                         "ACTIVE=1 after %u ms with UVM still set (fault=0x%02X) — continuing",
                         static_cast<unsigned>(kPostInitWaitMs + waited_ms),
                         last_fault_byte_);
            } else {
                log.Info(TAG,
                         "Chip awake after %u ms — ACTIVE=1, UVM=0",
                         static_cast<unsigned>(kPostInitWaitMs + waited_ms));
            }
            break;
        }
        os_thread_sleep(os_convert_msec_to_delay_ticks(kPollIntervalMs));
        waited_ms += kPollIntervalMs;
    }

    if (!st.active) {
        last_fault_byte_ = driver_->GetLastFaultByte();
        /* Only called after driver_->Initialize() succeeded (COMER cleared).
         * ACTIVE may stay clear on marginal VM — continue when STATUS polls OK. */
        if (status_ok_polls >= 3U) {
            log.Warn(TAG,
                     "ACTIVE not latched after %u ms (UVM=%d fault=0x%02X) after "
                     "%u OK STATUS polls — accepting SPI-live (check VM/ACTIVE)",
                     static_cast<unsigned>(kPostInitWaitMs + waited_ms),
                     st.undervoltage ? 1 : 0, last_fault_byte_,
                     static_cast<unsigned>(status_ok_polls));
        } else {
            log.Error(TAG,
                      "Chip never reached ACTIVE=1 after %u ms (last STATUS: "
                      "ACTIVE=%d UVM=%d fault=0x%02X ok=%u). See "
                      "hf-max22200-driver troubleshooting guide.",
                      static_cast<unsigned>(kPostInitWaitMs + waited_ms),
                      st.active, st.undervoltage, last_fault_byte_,
                      static_cast<unsigned>(status_ok_polls));
            return false;
        }
    }

    max22200::FaultStatus drain{};
    (void)driver_->ReadFaultRegister(drain);
    last_fault_byte_ = driver_->GetLastFaultByte();
    log.Debug(TAG,
              "Drained POR fault bits: OCP=0x%02X OLF=0x%02X HHF=0x%02X DPM=0x%02X",
              drain.overcurrent_channel_mask,
              drain.open_load_fault_channel_mask,
              drain.hit_not_reached_channel_mask,
              drain.plunger_movement_fault_channel_mask);

    return true;
}

max22200::DriverStatus Max22200Handler::Deinitialize() noexcept {
    MutexLockGuard lock(mutex_);
    if (!initialized_) return max22200::DriverStatus::OK;

    if (driver_) {
        driver_->Deinitialize();
    }
    driver_.reset();
    initialized_ = false;
    Logger::GetInstance().Info(TAG, "MAX22200 deinitialized");
    return max22200::DriverStatus::OK;
}

max22200::DriverStatus Max22200Handler::ConfigureChannel(uint8_t channel,
                                        const max22200::ChannelConfig& config) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        return drv.ConfigureChannel(channel, config);
    });
}

max22200::DriverStatus Max22200Handler::SetupCdrChannel(uint8_t channel, uint16_t hit_ma,
                                       uint16_t hold_ma, float hit_time_ms) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        auto s = drv.SetHitCurrentMa(channel, hit_ma);
        if (s != max22200::DriverStatus::OK) return s;
        s = drv.SetHoldCurrentMa(channel, hold_ma);
        if (s != max22200::DriverStatus::OK) return s;
        return drv.SetHitTimeMs(channel, hit_time_ms);
    });
}

max22200::DriverStatus Max22200Handler::SetupVdrChannel(uint8_t channel, float hit_duty_pct,
                                       float hold_duty_pct, float hit_time_ms) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        auto s = drv.SetHitDutyPercent(channel, hit_duty_pct);
        if (s != max22200::DriverStatus::OK) return s;
        s = drv.SetHoldDutyPercent(channel, hold_duty_pct);
        if (s != max22200::DriverStatus::OK) return s;
        return drv.SetHitTimeMs(channel, hit_time_ms);
    });
}

max22200::DriverStatus Max22200Handler::EnableChannel(uint8_t channel) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        return drv.EnableChannel(channel);
    });
}

max22200::DriverStatus Max22200Handler::DisableChannel(uint8_t channel) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        return drv.DisableChannel(channel);
    });
}

max22200::DriverStatus Max22200Handler::EnableAllChannels() noexcept {
    return withDriver([](auto& drv) -> max22200::DriverStatus {
        return drv.EnableAllChannels();
    });
}

max22200::DriverStatus Max22200Handler::DisableAllChannels() noexcept {
    return withDriver([](auto& drv) -> max22200::DriverStatus {
        return drv.DisableAllChannels();
    });
}

bool Max22200Handler::IsChannelEnabled(uint8_t channel) noexcept {
    return withDriver([&](auto& drv) -> bool {
        if (channel >= kNumChannels) return false;
        max22200::StatusConfig status{};
        if (drv.ReadStatus(status) != max22200::DriverStatus::OK) return false;
        return (status.channels_on_mask & (1u << channel)) != 0;
    });
}

max22200::DriverStatus Max22200Handler::SetChannelsMask(uint8_t mask) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        return drv.SetChannelsOn(mask);
    });
}

max22200::DriverStatus Max22200Handler::GetStatus(max22200::StatusConfig& status) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        return drv.ReadStatus(status);
    });
}

max22200::DriverStatus Max22200Handler::GetChannelFaults(uint8_t channel,
                                        max22200::FaultStatus& faults) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        if (channel >= kNumChannels) return max22200::DriverStatus::INVALID_PARAMETER;
        (void)channel; // FaultStatus is device-wide
        return drv.ReadFaultRegister(faults);
    });
}

bool Max22200Handler::HasFault() noexcept {
    return withDriver([](auto& drv) -> bool {
        max22200::StatusConfig status{};
        if (drv.ReadFaultFlags(status) != max22200::DriverStatus::OK) return false;
        return status.hasFault();
    });
}

max22200::DriverStatus Max22200Handler::ClearFaults() noexcept {
    return withDriver([](auto& drv) -> max22200::DriverStatus {
        return drv.ClearAllFaults();
    });
}

max22200::DriverStatus Max22200Handler::ReadFaultRegister(max22200::FaultStatus& faults) noexcept {
    return withDriver([&](auto& drv) -> max22200::DriverStatus {
        return drv.ReadFaultRegister(faults);
    });
}

Max22200Handler::DriverType* Max22200Handler::GetDriver() noexcept {
    MutexLockGuard lock(mutex_);
    if (!EnsureInitializedLocked()) {
        return nullptr;
    }
    return driver_.get();
}

const Max22200Handler::DriverType* Max22200Handler::GetDriver() const noexcept {
    auto* self = const_cast<Max22200Handler*>(this);
    return self->GetDriver();
}

void Max22200Handler::DumpDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    auto& log = Logger::GetInstance();
    if (!EnsureInitializedLocked() || !driver_) {
        log.Warn(TAG, "Not initialized — cannot dump diagnostics");
        return;
    }

    log.Info(TAG, "=== MAX22200 Diagnostics ===");

    // Status
    max22200::StatusConfig status{};
    if (driver_->ReadStatus(status) == max22200::DriverStatus::OK) {
        log.Info(TAG, "  Active: %s", status.active ? "yes" : "no");
        log.Info(TAG, "  Channels ON: 0x%02x", status.channels_on_mask);
        log.Info(TAG, "  Has fault: %s", status.hasFault() ? "YES" : "no");
    }

    // Fault register
    max22200::FaultStatus faults{};
    if (driver_->ReadFaultRegister(faults) == max22200::DriverStatus::OK) {
        log.Info(TAG, "  Faults: hasFault=%s", faults.hasFault() ? "YES" : "no");
        log.Info(TAG, "  OLF=0x%02x OCP=0x%02x HHF=0x%02x DPM=0x%02x",
                 faults.open_load_fault_channel_mask,
                 faults.overcurrent_channel_mask,
                 faults.hit_not_reached_channel_mask,
                 faults.plunger_movement_fault_channel_mask);
    }

    // Statistics
    auto stats = driver_->GetStatistics();
    log.Info(TAG, "  Transfers: %lu, Success rate: %.1f%%",
             static_cast<unsigned long>(stats.total_transfers),
             static_cast<double>(stats.getSuccessRate()));

    log.Info(TAG, "=== End MAX22200 Diagnostics ===");
}
