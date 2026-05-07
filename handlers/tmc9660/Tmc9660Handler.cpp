#include "Tmc9660Handler.h"
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include "HandlerCommon.h"
#include "core/hf-core-drivers/external/hf-tmc9660-driver/inc/tmc9660_comm_interface.hpp"
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsAbstraction.h"
#include "core/hf-core-utils/hf-utils-rtos-wrap/include/OsUtility.h"

namespace {
// EspUart::Read returns UART_SUCCESS even on partial / 0-byte reads (it only
// distinguishes ESP-IDF "no error" from negative error codes). The TMC9660 caller
// passes an uninitialized 8/9-byte stack buffer and then runs CRC over it, which
// then trips "CRC mismatch" any time the chip is silent. The vendor ESP32
// reference (`examples/TMC9660/TMC9660.c` + `examples/esp32/main/esp32_tmc9660_bus.hpp`)
// only ever returns success when `bytes_read == expected`, so mirror that here.
static bool ReadExactBytesUart(BaseUart& uart, uint8_t* dst, size_t total,
                                uint32_t total_timeout_ms) noexcept {
    if (total == 0) {
        return true;
    }
    const int64_t deadline_us =
        handler_utils::MonotonicTimeUs() + static_cast<int64_t>(total_timeout_ms) * 1000;
    size_t got = 0;
    while (got < total) {
        if (handler_utils::MonotonicTimeUs() >= deadline_us) {
            return false;
        }
        const auto avail = uart.BytesAvailable();
        if (avail == 0) {
            handler_utils::DelayMs(1);
            continue;
        }
        const uint16_t to_read =
            static_cast<uint16_t>(std::min<size_t>(total - got, static_cast<size_t>(avail)));
        // Single-shot Read with a small timeout — at this point bytes are queued.
        const auto r = uart.Read(dst + got, to_read, /*timeout_ms=*/5);
        if (r != hf_uart_err_t::UART_SUCCESS) {
            return false;
        }
        got += to_read;
    }
    return got == total;
}

// Hex-dump a fixed-size byte array. When @p is_tmcl is true, output is gated by
// `TMC9660_LOG_TMCL_RAW_FRAMES` in `tmc9660_comm_interface.hpp` (default 0). Bootloader
// `[BL TX]` / `[BL RX]` lines stay on (they use `is_tmcl == false`).
template <size_t N>
static void LogHexDump(const char* tag, const char* prefix,
                        const std::array<uint8_t, N>& buf, bool is_tmcl) noexcept {
#if !TMC9660_LOG_TMCL_RAW_FRAMES
    if (is_tmcl) {
        (void)tag;
        (void)prefix;
        (void)buf;
        return;
    }
#endif
    auto& log = Logger::GetInstance();
    if (N == 8) {
        log.Info(tag, "%s %02X %02X %02X %02X %02X %02X %02X %02X", prefix, buf[0], buf[1], buf[2],
                 buf[3], buf[4], buf[5], buf[6], buf[7]);
    } else if (N == 9) {
        log.Info(tag, "%s %02X %02X %02X %02X %02X %02X %02X %02X %02X", prefix, buf[0], buf[1],
                 buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8]);
    }
}
} // namespace

//==============================================================================
// DEFAULT BOOTLOADER CONFIGURATION
//==============================================================================

/**
 * @brief Default TMC9660 bootloader configuration for the vendor three-phase reference evaluation kit.
 *
 * On-die options only; host RST/DRV_EN/WAKE/SPI CS wiring stays in the app (`BaseGpio` / `BaseSpi`).
 *
 * Matches `hf-tmc9660-driver` `bldc_comprehensive_test.cpp` when `use_flash == false`: same
 * GPIO pull profile, Hall, ABN1/2, Y2 brake chopper / mechanical brake, comms, clock, and
 * SPI flash off. Pass a custom `BootloaderConfig*` if your netlist differs.
 */
const tmc9660::BootloaderConfig Tmc9660Handler::kDefaultBootConfig = {
    // LDO Configuration
    {
        tmc9660::bootcfg::LDOVoltage::V5_0,      // vext1
        tmc9660::bootcfg::LDOVoltage::V3_3,      // vext2
        tmc9660::bootcfg::LDOSlope::Slope3ms,    // slope_vext1
        tmc9660::bootcfg::LDOSlope::Slope3ms,    // slope_vext2
        false                                     // ldo_short_fault
    },
    // Boot Configuration
    {
        tmc9660::bootcfg::BootMode::Parameter,    // boot_mode
        false,                                    // bl_ready_fault
        true,                                     // bl_exit_fault
        false,                                    // disable_selftest
        false,                                    // bl_config_fault
        true                                      // start_motor_control
    },
    // UART Configuration (reference kit: TMCL UART on GPIO6/7 alongside SPI0)
    {
        1,                                         // device_address
        255,                                       // host_address (broadcast)
        false,                                     // disable_uart (matches driver BLDC example)
        tmc9660::bootcfg::UartRxPin::GPIO7,       // rx_pin
        tmc9660::bootcfg::UartTxPin::GPIO6,       // tx_pin
        tmc9660::bootcfg::BaudRate::Auto16x       // baud_rate
    },
    // RS485 Configuration (disabled)
    {
        false,                                     // enable_rs485
        tmc9660::bootcfg::RS485TxEnPin::None,     // txen_pin
        0,                                         // txen_pre_delay
        0                                          // txen_post_delay
    },
    // SPI Boot Configuration (reference kit: SPI0 SCK on GPIO11; no external flash)
    {
        false,                                     // disable_spi
        tmc9660::bootcfg::SPIInterface::SPI0,     // boot_spi_iface
        tmc9660::bootcfg::SPI0SckPin::GPIO11      // spi0_sck_pin
    },
    // SPI Flash Configuration (disabled; enable + iface separation per datasheet if populated)
    {
        false,                                     // enable_flash
        tmc9660::bootcfg::SPIInterface::SPI0,     // flash_spi_iface (unused when flash off)
        tmc9660::bootcfg::SPI0SckPin::GPIO11,     // spi0_sck_pin
        12,                                        // cs_pin (unused when flash disabled)
        tmc9660::bootcfg::SPIFlashFreq::Div4      // freq_div (unused when flash disabled)
    },
    // I2C EEPROM Configuration (disabled)
    {
        false,                                     // enable_eeprom
        tmc9660::bootcfg::I2CSdaPin::GPIO5,       // sda_pin
        tmc9660::bootcfg::I2CSclPin::GPIO4,       // scl_pin
        0,                                         // address_bits
        tmc9660::bootcfg::I2CFreq::Freq100k       // freq_code
    },
    // Clock Configuration
    {
        tmc9660::bootcfg::ClockSource::External,       // use_external
        tmc9660::bootcfg::ExtSourceType::Oscillator,   // ext_source_type
        tmc9660::bootcfg::XtalDrive::Freq16MHz,        // xtal_drive
        false,                                         // xtal_boost
        tmc9660::bootcfg::SysClkSource::PLL,           // pll_selection
        15,                                            // rdiv (16 MHz ext: reference kit RDIV = MHz − 1)
        tmc9660::bootcfg::SysClkDiv::Div1              // sysclk_div
    },
    // GPIO Configuration (reference kit: Hall + ABN pull-ups, GPIO17–18 pulldowns, GPIO5 analog)
    {
        0x0000, 0x00,  // outputMask_0_15, outputMask_16_18
        0x0000, 0x00,  // directionMask_0_15, directionMask_16_18
        0xE11C, 0x01,  // pullUpMask_0_15 (2,3,4,8,13,14,15), pullUpMask_16_18 (GPIO16)
        0x0000, 0x06,  // pullDownMask_0_15, pullDownMask_16_18 (GPIO17, GPIO18)
        0x08           // analogMask_2_5 (GPIO5)
    },
    // Hall Configuration (reference kit)
    {
        true,
        tmc9660::bootcfg::HallUPin::GPIO2,
        tmc9660::bootcfg::HallVPin::GPIO3,
        tmc9660::bootcfg::HallWPin::GPIO4
    },
    // ABN Encoder 1 Configuration (reference kit)
    {
        true,
        tmc9660::bootcfg::ABN1APin::GPIO8,
        tmc9660::bootcfg::ABN1BPin::GPIO13,
        tmc9660::bootcfg::ABN1NPin::GPIO14
    },
    // ABN Encoder 2 Configuration (reference kit)
    {
        true,
        tmc9660::bootcfg::ABN2APin::GPIO15,
        tmc9660::bootcfg::ABN2BPin::GPIO16
    },
    // Reference Switches Configuration (disabled)
    {},
    // Step/Direction Configuration (disabled)
    {},
    // SPI Encoder Configuration (disabled)
    {},
    // Mechanical Brake Configuration (reference kit: Y2_LS)
    {
        true,
        tmc9660::bootcfg::MechBrakeOutput::Y2_LS
    },
    // Brake Chopper Configuration (reference kit: Y2_HS)
    {
        true,
        tmc9660::bootcfg::BrakeChopperOutput::Y2_HS
    },
    // Memory Storage Configuration (no external flash / EEPROM in this path)
    {
        tmc9660::bootcfg::MemStorage::Disabled,
        tmc9660::bootcfg::MemStorage::Disabled
    }
};

//==============================================================================
// COMMON HELPERS FOR HAL COMM INTERFACES
//==============================================================================

namespace {

/**
 * @brief Set a BaseGpio pin level based on a TMC9660 signal.
 *
 * @details The BaseGpio is already configured with the IC's natural polarity
 *          (`HF_GPIO_ACTIVE_LOW` for active-low signals like WAKE/FAULTN, normal
 *          for RST/DRV_EN). So `SetState(ACTIVE)` drives the physical line to
 *          whatever level *asserts* the signal at the IC. Applying `active_high`
 *          again here would double-invert and break WAKE: when the driver asks
 *          for `gpioSetActive(WAKE)`, the IC needs physical LOW, but a second
 *          inversion would call `SetState(INACTIVE)` → physical HIGH → chip
 *          stays in hibernate and never replies on UART. We deliberately ignore
 *          `active_high` here for that reason and let the BaseGpio handle it.
 */
static bool setGpioFromSignal(BaseGpio& gpio_pin, tmc9660::GpioSignal signal,
                              bool /*active_high - unused; BaseGpio handles polarity*/) noexcept {
    auto state = (signal == tmc9660::GpioSignal::ACTIVE)
                     ? hf_gpio_state_t::HF_GPIO_STATE_ACTIVE
                     : hf_gpio_state_t::HF_GPIO_STATE_INACTIVE;
    return gpio_pin.SetState(state) == hf_gpio_err_t::GPIO_SUCCESS;
}

/**
 * @brief Read a BaseGpio pin and convert to TMC9660 signal.
 *
 * @details Mirror of `setGpioFromSignal`: the BaseGpio's `IsActive()` already
 *          returns "is the IC signal asserted" (because its `active_state` is
 *          configured for the IC's natural polarity). Applying `active_high`
 *          again would invert FAULTN reads. Ignored on purpose.
 */
static bool readGpioToSignal(BaseGpio& gpio_pin, tmc9660::GpioSignal& signal,
                             bool /*active_high - unused; BaseGpio handles polarity*/) noexcept {
    bool is_active = false;
    if (gpio_pin.IsActive(is_active) != hf_gpio_err_t::GPIO_SUCCESS) {
        return false;
    }
    // BaseGpio::IsActive() already returns "is the IC signal asserted" (it knows its
    // own active_state). Map straight through.
    signal = is_active ? tmc9660::GpioSignal::ACTIVE : tmc9660::GpioSignal::INACTIVE;
    return true;
}

} // anonymous namespace

//==============================================================================
// HAL SPI COMM INTERFACE IMPLEMENTATION
//==============================================================================

HalSpiTmc9660Comm::HalSpiTmc9660Comm(BaseSpi& spi, BaseGpio& rst, BaseGpio& drv_en,
                                       BaseGpio& faultn, BaseGpio& wake,
                                       bool rst_active_high, bool drv_en_active_high,
                                       bool faultn_active_low, bool wake_active_low) noexcept
    : SpiCommInterface<HalSpiTmc9660Comm>(rst_active_high, drv_en_active_high,
                                           wake_active_low, faultn_active_low),
      spi_(spi), ctrl_pins_{rst, drv_en, faultn, wake} {
}

bool HalSpiTmc9660Comm::spiTransferTMCL(std::array<uint8_t, 8>& tx, std::array<uint8_t, 8>& rx) noexcept {
    if (!spi_.EnsureInitialized()) {
        return false;
    }
    hf_spi_err_t result = spi_.Transfer(tx.data(), rx.data(), hf_u16_t(8), hf_u32_t(0));
    // TMC9660 SPI TMCL pacing: the chip uses a two-transaction protocol where
    // TX1 carries the new command and TX2 (NO_OP) drains its reply. Back-to-
    // back transfers at typical SPI clocks (>=1 MHz) leave only ~80 µs between
    // CS edges, which is shorter than the chip needs to load the parameter-
    // mode command into the TMCL parser. The chip then returns SPI_STATUS=OK
    // (0xFF) but TMCL_STATUS=REPLY_INVALID_CMD (0x02) on the *second* SPI
    // transaction — an in-flight race that disappears when even one extra
    // ESP_LOG between transfers introduces a few hundred µs of pacing.
    // Adding a deterministic ~150 µs post-transfer delay gives the chip a
    // stable inter-frame gap close to the natural pacing of a 9-byte UART
    // frame at 115200 baud (~780 µs / frame). This single delay applies once
    // per host-side TMCL frame and is invisible to upper layers.
    handler_utils::DelayUs(150);
    return result == hf_spi_err_t::SPI_SUCCESS;
}

bool HalSpiTmc9660Comm::spiTransferBootloader(std::array<uint8_t, 5>& tx, std::array<uint8_t, 5>& rx) noexcept {
    if (!spi_.EnsureInitialized()) {
        return false;
    }
    hf_spi_err_t result = spi_.Transfer(tx.data(), rx.data(), hf_u16_t(5), hf_u32_t(0));
    return result == hf_spi_err_t::SPI_SUCCESS;
}

bool HalSpiTmc9660Comm::gpioSet(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept {
    return setGpioFromSignal(ctrl_pins_.get(pin), signal,
                             this->pinActiveLevels_[static_cast<int>(pin)]);
}

bool HalSpiTmc9660Comm::gpioRead(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal& signal) noexcept {
    return readGpioToSignal(ctrl_pins_.get(pin), signal,
                            this->pinActiveLevels_[static_cast<int>(pin)]);
}

void HalSpiTmc9660Comm::debugLog(int level, const char* tag, const char* format, va_list args) noexcept {
    handler_utils::RouteLogToLogger(level, tag, format, args);
}

void HalSpiTmc9660Comm::delayMs(uint32_t ms) noexcept { handler_utils::DelayMs(ms); }
void HalSpiTmc9660Comm::delayUs(uint32_t us) noexcept { handler_utils::DelayUs(us); }

//==============================================================================
// HAL UART COMM INTERFACE IMPLEMENTATION
//==============================================================================

HalUartTmc9660Comm::HalUartTmc9660Comm(BaseUart& uart, BaseGpio& rst, BaseGpio& drv_en,
                                         BaseGpio& faultn, BaseGpio& wake,
                                         bool rst_active_high, bool drv_en_active_high,
                                         bool faultn_active_low, bool wake_active_low) noexcept
    : UartCommInterface<HalUartTmc9660Comm>(rst_active_high, drv_en_active_high,
                                             wake_active_low, faultn_active_low),
      uart_(uart), ctrl_pins_{rst, drv_en, faultn, wake} {
}

bool HalUartTmc9660Comm::uartSendTMCL(const std::array<uint8_t, 9>& data) noexcept {
    if (!uart_.EnsureInitialized()) {
        return false;
    }
    // Drop any garbage so the following 9-byte read aligns with this transaction's reply.
    (void)uart_.FlushRx();
    LogHexDump("Tmc9660Uart", "[TMCL TX]", data, true);
    // Use a real timeout so EspUart::Write also waits for the TX FIFO to drain
    // (Vendor ESP32 UART reference waits for TX idle after each write.)
    hf_uart_err_t result = uart_.Write(data.data(), 9, /*timeout_ms=*/100);
    return result == hf_uart_err_t::UART_SUCCESS;
}

bool HalUartTmc9660Comm::uartReceiveTMCL(std::array<uint8_t, 9>& data) noexcept {
    if (!uart_.EnsureInitialized()) {
        return false;
    }
    // Pre-fill so a partial / silent read can never accidentally pass the address+checksum check.
    data.fill(0xFF);
    if (!ReadExactBytesUart(uart_, data.data(), data.size(), /*total_timeout_ms=*/200)) {
        return false;
    }
    LogHexDump("Tmc9660Uart", "[TMCL RX]", data, true);
    return true;
}

bool HalUartTmc9660Comm::uartTransferBootloader(const std::array<uint8_t, 8>& tx,
                                                  std::array<uint8_t, 8>& rx) noexcept {
    if (!uart_.EnsureInitialized()) {
        return false;
    }
    // Mirror vendor ESP32 UART reference: drain stale RX, send, then read EXACTLY 8 bytes.
    (void)uart_.FlushRx();
    LogHexDump("Tmc9660Uart", "[BL TX]", tx, false);
    if (uart_.Write(tx.data(), 8, /*timeout_ms=*/100) != hf_uart_err_t::UART_SUCCESS) {
        return false;
    }
    rx.fill(0xFF);
    if (!ReadExactBytesUart(uart_, rx.data(), rx.size(), /*total_timeout_ms=*/200)) {
        return false;
    }
    LogHexDump("Tmc9660Uart", "[BL RX]", rx, false);
    return true;
}

bool HalUartTmc9660Comm::gpioSet(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal signal) noexcept {
    return setGpioFromSignal(ctrl_pins_.get(pin), signal,
                             this->pinActiveLevels_[static_cast<int>(pin)]);
}

bool HalUartTmc9660Comm::gpioRead(tmc9660::TMC9660CtrlPin pin, tmc9660::GpioSignal& signal) noexcept {
    return readGpioToSignal(ctrl_pins_.get(pin), signal,
                            this->pinActiveLevels_[static_cast<int>(pin)]);
}

void HalUartTmc9660Comm::debugLog(int level, const char* tag, const char* format, va_list args) noexcept {
    handler_utils::RouteLogToLogger(level, tag, format, args);
}

void HalUartTmc9660Comm::delayMs(uint32_t ms) noexcept { handler_utils::DelayMs(ms); }
void HalUartTmc9660Comm::delayUs(uint32_t us) noexcept { handler_utils::DelayUs(us); }

//==============================================================================
// TMC9660 HANDLER - CONSTRUCTORS / DESTRUCTOR
//==============================================================================

Tmc9660Handler::Tmc9660Handler(BaseSpi& spi, BaseGpio& rst, BaseGpio& drv_en,
                                BaseGpio& faultn, BaseGpio& wake,
                                uint8_t address,
                                const tmc9660::BootloaderConfig* bootCfg)
    : use_spi_(true),
      bootCfg_(bootCfg),
      device_address_(address) {
    // Create SPI comm interface and driver (lazy - driver created in Initialize)
    spi_comm_ = std::make_unique<HalSpiTmc9660Comm>(spi, rst, drv_en, faultn, wake);
    // Eagerly create peripheral wrappers so accessors never return dangling refs.
    // The wrapper methods themselves guard on IsDriverReady().
    gpioWrappers_[0] = std::make_unique<Gpio>(*this, 17);
    gpioWrappers_[1] = std::make_unique<Gpio>(*this, 18);
    adcWrapper_         = std::make_unique<Adc>(*this);
    temperatureWrapper_ = std::make_unique<Temperature>(*this);
    std::snprintf(description_, sizeof(description_), "TMC9660 Motor Driver (SPI @0x%02X)", address);
}

Tmc9660Handler::Tmc9660Handler(BaseUart& uart, BaseGpio& rst, BaseGpio& drv_en,
                                BaseGpio& faultn, BaseGpio& wake,
                                uint8_t address,
                                const tmc9660::BootloaderConfig* bootCfg)
    : use_spi_(false),
      bootCfg_(bootCfg),
      device_address_(address) {
    // Create UART comm interface and driver (lazy - driver created in Initialize)
    uart_comm_ = std::make_unique<HalUartTmc9660Comm>(uart, rst, drv_en, faultn, wake);
    // Eagerly create peripheral wrappers so accessors never return dangling refs.
    gpioWrappers_[0] = std::make_unique<Gpio>(*this, 17);
    gpioWrappers_[1] = std::make_unique<Gpio>(*this, 18);
    adcWrapper_         = std::make_unique<Adc>(*this);
    temperatureWrapper_ = std::make_unique<Temperature>(*this);
    std::snprintf(description_, sizeof(description_), "TMC9660 Motor Driver (UART @0x%02X)", address);
}

Tmc9660Handler::~Tmc9660Handler() noexcept = default;

//==============================================================================
// INITIALIZATION
//==============================================================================

bool Tmc9660Handler::Initialize(bool performReset, bool retrieveBootloaderInfo,
                                 bool failOnVerifyError) {
    static constexpr const char* TAG = "Tmc9660Handler";

    if (IsDriverReady()) {
        return true;
    }

    // Create driver instance if not already created
    if (use_spi_) {
        if (!spi_comm_) {
            Logger::GetInstance().Error(TAG, "SPI comm interface not available");
            return false;
        }
        if (!spi_driver_) {
            spi_driver_ = std::make_unique<SpiDriver>(*spi_comm_, device_address_, bootCfg_);
        }
    } else {
        if (!uart_comm_) {
            Logger::GetInstance().Error(TAG, "UART comm interface not available");
            return false;
        }
        if (!uart_driver_) {
            uart_driver_ = std::make_unique<UartDriver>(*uart_comm_, device_address_, bootCfg_);
        }
    }

    // DRV_EN / WAKE before bootloader traffic: hf-tmc9660-driver's bootloaderInit() toggles RST
    // and polls FAULTN but does not assert DRV_EN or WAKE first. When those nets are off-chip,
    // expander-driven, or otherwise idle at boot, leaving them inactive can yield all-zero SPI/UART
    // until the die is fully enabled / out of hibernate (same reason reference BLDC flows assert
    // DRV_EN first).
    if (use_spi_ && spi_comm_) {
        (void)spi_comm_->gpioSetActive(tmc9660::TMC9660CtrlPin::DRV_EN);
        (void)spi_comm_->gpioSetActive(tmc9660::TMC9660CtrlPin::WAKE);
        spi_comm_->delayMs(2);
    } else if (!use_spi_ && uart_comm_) {
        (void)uart_comm_->gpioSetActive(tmc9660::TMC9660CtrlPin::DRV_EN);
        (void)uart_comm_->gpioSetActive(tmc9660::TMC9660CtrlPin::WAKE);
        uart_comm_->delayMs(2);
    }

    // Run bootloader initialization.
    // NOTE: visitDriver() cannot be used here because SpiDriver::BootloaderInitResult
    // and UartDriver::BootloaderInitResult are separate enum types from different
    // template instantiations, causing inconsistent auto return-type deduction.
    bool success = false;
    if (use_spi_) {
        if (spi_driver_) {
            auto result = spi_driver_->bootloaderInit(bootCfg_, performReset, retrieveBootloaderInfo, failOnVerifyError);
            success = (result == SpiDriver::BootloaderInitResult::Success);
        }
    } else {
        if (uart_driver_) {
            auto result = uart_driver_->bootloaderInit(bootCfg_, performReset, retrieveBootloaderInfo, failOnVerifyError);
            success = (result == UartDriver::BootloaderInitResult::Success);
        }
    }

    if (!success) {
        Logger::GetInstance().Error(TAG, "Bootloader initialization failed");
        // Drop driver so IsDriverReady() stays false and a later EnsureInitialized() can retry.
        spi_driver_.reset();
        uart_driver_.reset();
        return false;
    }

    // Peripheral wrappers (GPIO, ADC, Temperature) are created eagerly in the
    // constructor so that the accessors always return valid references.

    Logger::GetInstance().Info(TAG, "TMC9660 initialized successfully via %s",
                               use_spi_ ? "SPI" : "UART");
    return true;
}

bool Tmc9660Handler::EnsureInitialized() noexcept {
    MutexLockGuard lock(handler_mutex_);
    return EnsureInitializedLocked();
}

bool Tmc9660Handler::EnsureInitializedLocked() noexcept {
    if (IsDriverReady()) {
        return true;
    }
    return Initialize();
}

bool Tmc9660Handler::IsDriverReady() const noexcept {
    if (use_spi_) return spi_driver_ != nullptr;
    return uart_driver_ != nullptr;
}

//==============================================================================
// PERIPHERAL ACCESSORS
//==============================================================================

Tmc9660Handler::Gpio& Tmc9660Handler::gpio(uint8_t gpioNumber) {
    (void)EnsureInitialized();
    if (gpioNumber == 17) return *gpioWrappers_[0];
    if (gpioNumber == 18) return *gpioWrappers_[1];
    return *gpioWrappers_[0]; // Fallback
}

Tmc9660Handler::Adc& Tmc9660Handler::adc() {
    (void)EnsureInitialized();
    return *adcWrapper_;
}

Tmc9660Handler::Temperature& Tmc9660Handler::temperature() {
    (void)EnsureInitialized();
    return *temperatureWrapper_;
}

//==============================================================================
// COMMUNICATION INFO
//==============================================================================

tmc9660::CommMode Tmc9660Handler::GetCommMode() const noexcept {
    return use_spi_ ? tmc9660::CommMode::SPI : tmc9660::CommMode::UART;
}

Tmc9660Handler::SpiDriver* Tmc9660Handler::driverViaSpi() noexcept {
    if (!EnsureInitialized() || !use_spi_) {
        return nullptr;
    }
    return spi_driver_.get();
}

const Tmc9660Handler::SpiDriver* Tmc9660Handler::driverViaSpi() const noexcept {
    auto* self = const_cast<Tmc9660Handler*>(this);
    return self->driverViaSpi();
}

Tmc9660Handler::UartDriver* Tmc9660Handler::driverViaUart() noexcept {
    if (!EnsureInitialized() || use_spi_) {
        return nullptr;
    }
    return uart_driver_.get();
}

const Tmc9660Handler::UartDriver* Tmc9660Handler::driverViaUart() const noexcept {
    auto* self = const_cast<Tmc9660Handler*>(this);
    return self->driverViaUart();
}

//==============================================================================
// GPIO WRAPPER IMPLEMENTATION
//==============================================================================

Tmc9660Handler::Gpio::Gpio(Tmc9660Handler& parent, uint8_t gpioNumber)
    : BaseGpio(gpioNumber, hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT),
      parent_(parent), gpioNumber_(gpioNumber) {
    std::snprintf(description_, sizeof(description_), "TMC9660 GPIO%u", gpioNumber_);
}

bool Tmc9660Handler::Gpio::Initialize() noexcept {
    if (!parent_.EnsureInitialized()) return false;
    return parent_.visitDriver([&](auto& driver) {
        return driver.gpio.setMode(gpioNumber_, true, false, true);
    });
}

bool Tmc9660Handler::Gpio::Deinitialize() noexcept { return true; }

hf_gpio_err_t Tmc9660Handler::Gpio::SetPinLevelImpl(hf_gpio_level_t level) noexcept {
    if (!parent_.EnsureInitialized()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    if (direction_ != hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT)
        return hf_gpio_err_t::GPIO_ERR_INVALID_CONFIGURATION;
    bool pin_high = (level == hf_gpio_level_t::HF_GPIO_LEVEL_HIGH);
    bool ok = parent_.visitDriver([&](auto& driver) {
        return driver.gpio.writePin(gpioNumber_, pin_high);
    });
    return ok ? hf_gpio_err_t::GPIO_SUCCESS : hf_gpio_err_t::GPIO_ERR_WRITE_FAILURE;
}

hf_gpio_err_t Tmc9660Handler::Gpio::GetPinLevelImpl(hf_gpio_level_t& level) noexcept {
    if (!parent_.EnsureInitialized()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;
    bool pin_state = false;
    bool ok = parent_.visitDriver([&](auto& driver) {
        return driver.gpio.readDigital(gpioNumber_, pin_state);
    });
    if (!ok) return hf_gpio_err_t::GPIO_ERR_READ_FAILURE;
    level = pin_state ? hf_gpio_level_t::HF_GPIO_LEVEL_HIGH : hf_gpio_level_t::HF_GPIO_LEVEL_LOW;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Tmc9660Handler::Gpio::SetDirectionImpl(hf_gpio_direction_t direction) noexcept {
    if (!parent_.EnsureInitialized()) return hf_gpio_err_t::GPIO_ERR_NOT_INITIALIZED;

    bool is_output = (direction == hf_gpio_direction_t::HF_GPIO_DIRECTION_OUTPUT);
    bool ok = parent_.visitDriver([&](auto& driver) {
        return driver.gpio.setMode(gpioNumber_, is_output, false, true);
    });
    if (!ok) return hf_gpio_err_t::GPIO_ERR_FAILURE;

    direction_ = direction;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Tmc9660Handler::Gpio::SetOutputModeImpl(hf_gpio_output_mode_t mode) noexcept {
    if (mode != hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL)
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Tmc9660Handler::Gpio::SetPullModeImpl(hf_gpio_pull_mode_t mode) noexcept {
    if (mode != hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING)
        return hf_gpio_err_t::GPIO_ERR_UNSUPPORTED_OPERATION;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_pull_mode_t Tmc9660Handler::Gpio::GetPullModeImpl() const noexcept {
    return hf_gpio_pull_mode_t::HF_GPIO_PULL_MODE_FLOATING;
}

bool Tmc9660Handler::Gpio::IsPinAvailable() const noexcept {
    return gpioNumber_ == 17 || gpioNumber_ == 18;
}

hf_u8_t Tmc9660Handler::Gpio::GetMaxPins() const noexcept { return 2; }

const char* Tmc9660Handler::Gpio::GetDescription() const noexcept { return description_; }

hf_gpio_err_t Tmc9660Handler::Gpio::GetDirectionImpl(hf_gpio_direction_t& direction) const noexcept {
    direction = direction_;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

hf_gpio_err_t Tmc9660Handler::Gpio::GetOutputModeImpl(hf_gpio_output_mode_t& mode) const noexcept {
    mode = hf_gpio_output_mode_t::HF_GPIO_OUTPUT_MODE_PUSH_PULL;
    return hf_gpio_err_t::GPIO_SUCCESS;
}

//==============================================================================
// ADC WRAPPER IMPLEMENTATION
//==============================================================================

Tmc9660Handler::Adc::Adc(Tmc9660Handler& parent) : parent_(parent) {}

bool Tmc9660Handler::Adc::Initialize() noexcept { return true; }
bool Tmc9660Handler::Adc::Deinitialize() noexcept { return true; }
hf_u8_t Tmc9660Handler::Adc::GetMaxChannels() const noexcept { return 15; }

bool Tmc9660Handler::Adc::IsChannelAvailable(hf_channel_id_t channel_id) const noexcept {
    return ValidateChannelId(channel_id) == hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadChannelV(hf_channel_id_t channel_id, float& channel_reading_v,
                                                 hf_u8_t /*numOfSamplesToAvg*/,
                                                 hf_time_t /*timeBetweenSamples*/) noexcept {
    MutexLockGuard lock(mutex_);
    const uint64_t start_time_us = GetCurrentTimeUs();
    hf_u32_t raw = 0;
    hf_adc_err_t result = ReadChannelLocked(channel_id, raw, channel_reading_v);
    UpdateStatistics(result, start_time_us);
    return result;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadChannelCount(hf_channel_id_t channel_id,
                                                     hf_u32_t& channel_reading_count,
                                                     hf_u8_t /*numOfSamplesToAvg*/,
                                                     hf_time_t /*timeBetweenSamples*/) noexcept {
    MutexLockGuard lock(mutex_);
    const uint64_t start_time_us = GetCurrentTimeUs();
    float voltage = 0.0f;
    hf_adc_err_t result = ReadChannelLocked(channel_id, channel_reading_count, voltage);
    UpdateStatistics(result, start_time_us);
    return result;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadChannel(hf_channel_id_t channel_id,
                                                hf_u32_t& channel_reading_count,
                                                float& channel_reading_v,
                                                hf_u8_t /*numOfSamplesToAvg*/,
                                                hf_time_t /*timeBetweenSamples*/) noexcept {
    MutexLockGuard lock(mutex_);
    const uint64_t start_time_us = GetCurrentTimeUs();
    hf_adc_err_t result = ReadChannelLocked(channel_id, channel_reading_count, channel_reading_v);
    UpdateStatistics(result, start_time_us);
    return result;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadMultipleChannels(const hf_channel_id_t* channel_ids,
                                                         hf_u8_t num_channels,
                                                         hf_u32_t* readings, float* voltages) noexcept {
    if (!channel_ids || !readings || !voltages)
        return hf_adc_err_t::ADC_ERR_NULL_POINTER;

    MutexLockGuard lock(mutex_);
    for (hf_u8_t i = 0; i < num_channels; ++i) {
        hf_adc_err_t result = ReadChannelLocked(channel_ids[i], readings[i], voltages[i]);
        if (result != hf_adc_err_t::ADC_SUCCESS) return result;
    }
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::GetStatistics(hf_adc_statistics_t& statistics) noexcept {
    MutexLockGuard lock(mutex_);
    statistics = statistics_;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::GetDiagnostics(hf_adc_diagnostics_t& diagnostics) noexcept {
    MutexLockGuard lock(mutex_);
    diagnostics = diagnostics_;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ResetStatistics() noexcept {
    MutexLockGuard lock(mutex_);
    statistics_ = hf_adc_statistics_t{};
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ResetDiagnostics() noexcept {
    MutexLockGuard lock(mutex_);
    diagnostics_ = hf_adc_diagnostics_t{};
    last_error_.store(hf_adc_err_t::ADC_SUCCESS);
    return hf_adc_err_t::ADC_SUCCESS;
}

// Private ADC helpers
hf_adc_err_t Tmc9660Handler::Adc::ValidateChannelId(hf_channel_id_t channel_id) const noexcept {
    if (channel_id <= 3) return hf_adc_err_t::ADC_SUCCESS;
    if (channel_id >= 10 && channel_id <= 13) return hf_adc_err_t::ADC_SUCCESS;
    if (channel_id >= 20 && channel_id <= 21) return hf_adc_err_t::ADC_SUCCESS;
    if (channel_id >= 30 && channel_id <= 31) return hf_adc_err_t::ADC_SUCCESS;
    if (channel_id >= 40 && channel_id <= 42) return hf_adc_err_t::ADC_SUCCESS;
    return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadChannelLocked(hf_channel_id_t channel_id,
                                                      hf_u32_t& raw, float& voltage) noexcept {
    // Caller must hold mutex_.
    hf_adc_err_t validation_result = ValidateChannelId(channel_id);
    if (validation_result != hf_adc_err_t::ADC_SUCCESS) {
        UpdateDiagnostics(validation_result);
        return validation_result;
    }

    hf_adc_err_t result = hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    voltage = 0.0f;

    if (channel_id <= 3) {
        result = ReadAinChannel(channel_id, raw, voltage);
    } else if (channel_id >= 10 && channel_id <= 13) {
        result = ReadCurrentSenseChannel(channel_id - 10, raw, voltage);
    } else if (channel_id >= 20 && channel_id <= 21) {
        result = ReadVoltageChannel(channel_id - 20, raw, voltage);
    } else if (channel_id >= 30 && channel_id <= 31) {
        result = ReadTemperatureChannel(channel_id - 30, raw, voltage);
    } else if (channel_id >= 40 && channel_id <= 42) {
        result = ReadMotorDataChannel(channel_id - 40, raw, voltage);
    }

    if (result != hf_adc_err_t::ADC_SUCCESS) UpdateDiagnostics(result);
    return result;
}

hf_adc_err_t Tmc9660Handler::Adc::UpdateStatistics(hf_adc_err_t result, uint64_t start_time_us) noexcept {
    const uint64_t end_time_us = GetCurrentTimeUs();
    const uint32_t conversion_time_us = static_cast<uint32_t>(end_time_us - start_time_us);

    statistics_.totalConversions++;
    if (result == hf_adc_err_t::ADC_SUCCESS) {
        statistics_.successfulConversions++;
        if (statistics_.totalConversions == 1) {
            statistics_.minConversionTimeUs = conversion_time_us;
            statistics_.maxConversionTimeUs = conversion_time_us;
            statistics_.averageConversionTimeUs = conversion_time_us;
        } else {
            statistics_.minConversionTimeUs = std::min(statistics_.minConversionTimeUs, conversion_time_us);
            statistics_.maxConversionTimeUs = std::max(statistics_.maxConversionTimeUs, conversion_time_us);
            const uint32_t total_time = statistics_.averageConversionTimeUs * (statistics_.successfulConversions - 1) + conversion_time_us;
            statistics_.averageConversionTimeUs = total_time / statistics_.successfulConversions;
        }
    } else {
        statistics_.failedConversions++;
    }
    return result;
}

uint64_t Tmc9660Handler::Adc::GetCurrentTimeUs() const noexcept {
    OS_Ulong ticks = os_time_get();
    return static_cast<uint64_t>(ticks) * 1000000 / osTickRateHz;
}

void Tmc9660Handler::Adc::UpdateDiagnostics(hf_adc_err_t error) noexcept {
    last_error_.store(error);
    if (error != hf_adc_err_t::ADC_SUCCESS) {
        diagnostics_.consecutiveErrors++;
        diagnostics_.lastErrorCode = error;
        diagnostics_.lastErrorTimestamp = GetCurrentTimeUs();
        if (diagnostics_.consecutiveErrors > 10) diagnostics_.adcHealthy = false;
    } else {
        diagnostics_.consecutiveErrors = 0;
        diagnostics_.adcHealthy = true;
    }
}

// TMC9660-specific ADC channel methods
hf_adc_err_t Tmc9660Handler::Adc::ReadAinChannel(uint8_t ain_channel,
                                                    hf_u32_t& raw_value, float& voltage) noexcept {
    if (!parent_.EnsureInitialized()) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    uint16_t analog_value = 0;
    bool ok = parent_.visitDriver([&](auto& driver) {
        return driver.gpio.readAnalog(ain_channel, analog_value);
    });
    if (!ok) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    raw_value = static_cast<hf_u32_t>(analog_value);
    voltage = static_cast<float>(analog_value) * 3.3f / 65535.0f;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadCurrentSenseChannel(uint8_t current_channel,
                                                            hf_u32_t& raw_value, float& voltage) noexcept {
    if (!parent_.EnsureInitialized()) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    tmc9660::tmcl::Parameters param;
    switch (current_channel) {
        case 0: param = tmc9660::tmcl::Parameters::ADC_I0; break;
        case 1: param = tmc9660::tmcl::Parameters::ADC_I1; break;
        case 2: param = tmc9660::tmcl::Parameters::ADC_I2; break;
        case 3: param = tmc9660::tmcl::Parameters::ADC_I3; break;
        default: return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }

    uint32_t value = 0;
    if (!parent_.visitDriver([&](auto& driver) { return driver.readParameter(param, value); }))
        return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    raw_value = static_cast<hf_u32_t>(value);
    voltage = static_cast<float>(value) * 3.3f / 65535.0f;
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadVoltageChannel(uint8_t voltage_channel,
                                                       hf_u32_t& raw_value, float& voltage) noexcept {
    if (!parent_.EnsureInitialized()) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    switch (voltage_channel) {
        case 0: // Supply voltage
            voltage = parent_.visitDriver([](auto& driver) { return driver.telemetry.getSupplyVoltage(); });
            raw_value = static_cast<hf_u32_t>(voltage * 1000.0f);
            break;
        case 1: // Driver voltage (use supply as approximation)
            voltage = parent_.visitDriver([](auto& driver) { return driver.telemetry.getSupplyVoltage(); });
            raw_value = static_cast<hf_u32_t>(voltage * 1000.0f);
            break;
        default:
            return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadTemperatureChannel(uint8_t temp_channel,
                                                           hf_u32_t& raw_value, float& voltage) noexcept {
    if (!parent_.EnsureInitialized()) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    switch (temp_channel) {
        case 0: { // Chip temperature
            float temp_c = parent_.visitDriver([](auto& driver) { return driver.telemetry.getChipTemperature(); });
            raw_value = static_cast<hf_u32_t>(temp_c * 100.0f);
            voltage = temp_c;
            break;
        }
        case 1: { // External temperature
            uint16_t ext_temp_raw = parent_.visitDriver([](auto& driver) { return driver.telemetry.getExternalTemperature(); });
            raw_value = static_cast<hf_u32_t>(ext_temp_raw);
            voltage = static_cast<float>(ext_temp_raw) * 3.3f / 65535.0f;
            break;
        }
        default:
            return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }
    return hf_adc_err_t::ADC_SUCCESS;
}

hf_adc_err_t Tmc9660Handler::Adc::ReadMotorDataChannel(uint8_t motor_channel,
                                                         hf_u32_t& raw_value, float& voltage) noexcept {
    if (!parent_.EnsureInitialized()) return hf_adc_err_t::ADC_ERR_CHANNEL_READ_ERR;

    switch (motor_channel) {
        case 0: { // Motor current
            int16_t current_ma = parent_.visitDriver([](auto& driver) { return driver.telemetry.getMotorCurrent(); });
            raw_value = static_cast<hf_u32_t>(current_ma);
            voltage = static_cast<float>(current_ma) / 1000.0f;
            break;
        }
        case 1: { // Motor velocity
            int32_t vel = parent_.visitDriver([](auto& driver) { return driver.telemetry.getActualVelocity(); });
            raw_value = static_cast<hf_u32_t>(vel);
            voltage = static_cast<float>(vel);
            break;
        }
        case 2: { // Motor position
            int32_t pos = parent_.visitDriver([](auto& driver) { return driver.telemetry.getActualPosition(); });
            raw_value = static_cast<hf_u32_t>(pos);
            voltage = static_cast<float>(pos);
            break;
        }
        default:
            return hf_adc_err_t::ADC_ERR_INVALID_CHANNEL;
    }
    return hf_adc_err_t::ADC_SUCCESS;
}

const char* Tmc9660Handler::Adc::GetChannelTypeString(hf_channel_id_t channel_id) const noexcept {
    if (channel_id <= 3) return "AIN";
    if (channel_id >= 10 && channel_id <= 13) return "Current";
    if (channel_id >= 20 && channel_id <= 21) return "Voltage";
    if (channel_id >= 30 && channel_id <= 31) return "Temperature";
    if (channel_id >= 40 && channel_id <= 42) return "Motor";
    return "Unknown";
}

//==============================================================================
// TEMPERATURE WRAPPER IMPLEMENTATION
//==============================================================================

Tmc9660Handler::Temperature::Temperature(Tmc9660Handler& parent)
    : parent_(parent), last_error_(hf_temp_err_t::TEMP_SUCCESS) {
    statistics_ = hf_temp_statistics_t{};
    diagnostics_ = hf_temp_diagnostics_t{};
}

bool Tmc9660Handler::Temperature::Initialize() noexcept {
    static constexpr const char* TAG = "Tmc9660Handler::Temperature";
    if (IsInitialized()) return true;
    if (!parent_.EnsureInitialized()) {
        Logger::GetInstance().Error(TAG, "Parent TMC9660 driver not ready");
        return false;
    }
    Logger::GetInstance().Info(TAG, "Temperature sensor initialized");
    return true;
}

bool Tmc9660Handler::Temperature::Deinitialize() noexcept { return true; }

hf_temp_err_t Tmc9660Handler::Temperature::ReadTemperatureCelsiusImpl(float* temperature_celsius) noexcept {
    static constexpr const char* TAG = "Tmc9660Handler::Temperature";

    if (temperature_celsius == nullptr) {
        UpdateDiagnostics(hf_temp_err_t::TEMP_ERR_NULL_POINTER);
        return hf_temp_err_t::TEMP_ERR_NULL_POINTER;
    }

    MutexLockGuard lock(mutex_);
    uint64_t start_time_us = GetCurrentTimeUs();

    if (!parent_.EnsureInitialized()) {
        UpdateDiagnostics(hf_temp_err_t::TEMP_ERR_NOT_INITIALIZED);
        return hf_temp_err_t::TEMP_ERR_NOT_INITIALIZED;
    }

    float temp_c = parent_.visitDriver([](auto& driver) { return driver.telemetry.getChipTemperature(); });

    // Check for error condition (TMC9660 returns -273.0f on error)
    if (temp_c < -270.0f || std::isnan(temp_c)) {
        Logger::GetInstance().Error(TAG, "Failed to read chip temperature");
        UpdateDiagnostics(hf_temp_err_t::TEMP_ERR_READ_FAILED);
        return hf_temp_err_t::TEMP_ERR_READ_FAILED;
    }

    if (temp_c < -40.0f || temp_c > 150.0f) {
        Logger::GetInstance().Warn(TAG, "Temperature out of range: %.2f°C", temp_c);
        UpdateDiagnostics(hf_temp_err_t::TEMP_ERR_OUT_OF_RANGE);
        return hf_temp_err_t::TEMP_ERR_OUT_OF_RANGE;
    }

    *temperature_celsius = temp_c;
    UpdateStatistics(hf_temp_err_t::TEMP_SUCCESS, start_time_us);
    UpdateDiagnostics(hf_temp_err_t::TEMP_SUCCESS);
    return hf_temp_err_t::TEMP_SUCCESS;
}

hf_temp_err_t Tmc9660Handler::Temperature::GetSensorInfo(hf_temp_sensor_info_t* info) const noexcept {
    if (info == nullptr) return hf_temp_err_t::TEMP_ERR_NULL_POINTER;

    info->sensor_type = HF_TEMP_SENSOR_TYPE_INTERNAL;
    info->min_temp_celsius = -40.0f;
    info->max_temp_celsius = 150.0f;
    info->resolution_celsius = 0.1f;
    info->accuracy_celsius = 2.0f;
    info->response_time_ms = 100;
    info->capabilities = HF_TEMP_CAP_HIGH_PRECISION | HF_TEMP_CAP_FAST_RESPONSE;
    info->manufacturer = "Trinamic";
    info->model = "TMC9660";
    info->version = "Internal Chip Temperature Sensor";
    return hf_temp_err_t::TEMP_SUCCESS;
}

hf_u32_t Tmc9660Handler::Temperature::GetCapabilities() const noexcept {
    return HF_TEMP_CAP_HIGH_PRECISION | HF_TEMP_CAP_FAST_RESPONSE;
}

hf_temp_err_t Tmc9660Handler::Temperature::UpdateStatistics(hf_temp_err_t result,
                                                              uint64_t start_time_us) noexcept {
    uint64_t end_time_us = GetCurrentTimeUs();
    uint32_t operation_time_us = static_cast<uint32_t>(end_time_us - start_time_us);

    statistics_.total_operations++;
    if (result == hf_temp_err_t::TEMP_SUCCESS) {
        statistics_.successful_operations++;
        statistics_.temperature_readings++;
    } else {
        statistics_.failed_operations++;
    }

    if (statistics_.total_operations == 1) {
        statistics_.min_operation_time_us = operation_time_us;
        statistics_.max_operation_time_us = operation_time_us;
        statistics_.average_operation_time_us = operation_time_us;
    } else {
        statistics_.min_operation_time_us = std::min(statistics_.min_operation_time_us, operation_time_us);
        statistics_.max_operation_time_us = std::max(statistics_.max_operation_time_us, operation_time_us);
        statistics_.average_operation_time_us =
            (statistics_.average_operation_time_us * (statistics_.total_operations - 1) + operation_time_us) /
            statistics_.total_operations;
    }
    return result;
}

uint64_t Tmc9660Handler::Temperature::GetCurrentTimeUs() const noexcept {
    OS_Ulong ticks = os_time_get();
    return static_cast<uint64_t>(ticks) * 1000000 / osTickRateHz;
}

void Tmc9660Handler::Temperature::UpdateDiagnostics(hf_temp_err_t error) noexcept {
    diagnostics_.last_error_code = error;
    diagnostics_.last_error_timestamp = static_cast<hf_u32_t>(GetCurrentTimeUs() / 1000);

    if (error != hf_temp_err_t::TEMP_SUCCESS) {
        diagnostics_.consecutive_errors++;
        diagnostics_.sensor_healthy = false;
    } else {
        diagnostics_.consecutive_errors = 0;
        diagnostics_.sensor_healthy = true;
    }
    diagnostics_.sensor_available = parent_.IsDriverReady();
    last_error_.store(error);
}

//==============================================================================
// DIAGNOSTICS
//==============================================================================

void Tmc9660Handler::DumpDiagnostics() noexcept {
    static constexpr const char* TAG = "Tmc9660Handler";
    MutexLockGuard lock(handler_mutex_);
    const bool ready = EnsureInitializedLocked();

    Logger::GetInstance().Info(TAG, "=== TMC9660 HANDLER DIAGNOSTICS ===");
    Logger::GetInstance().Info(TAG, "Description: %s", description_);
    Logger::GetInstance().Info(TAG, "Driver Ready: %s", ready ? "YES" : "NO");
    Logger::GetInstance().Info(TAG, "Comm Mode: %s", use_spi_ ? "SPI" : "UART");
    Logger::GetInstance().Info(TAG, "Device Address: %d", device_address_);

    if (ready) {
        uint32_t status_flags = 0, error_flags = 0;
        float voltage = visitDriverInternal([](auto& driver) { return driver.telemetry.getSupplyVoltage(); });
        float temp = visitDriverInternal([](auto& driver) { return driver.telemetry.getChipTemperature(); });
        visitDriverInternal([&](auto& driver) { return driver.telemetry.getGeneralStatusFlags(status_flags); });
        visitDriverInternal([&](auto& driver) { return driver.telemetry.getGeneralErrorFlags(error_flags); });

        Logger::GetInstance().Info(TAG, "Supply Voltage: %.2fV", voltage);
        Logger::GetInstance().Info(TAG, "Chip Temperature: %.2f°C", temp);
        Logger::GetInstance().Info(TAG, "Status Flags: 0x%08X", status_flags);
        Logger::GetInstance().Info(TAG, "Error Flags: 0x%08X", error_flags);
    }

    // ADC diagnostics
    if (adcWrapper_) {
        Logger::GetInstance().Info(TAG, "ADC Wrapper: ACTIVE");
    }

    // GPIO diagnostics
    int active_gpio_count = 0;
    for (size_t i = 0; i < gpioWrappers_.size(); ++i) {
        if (gpioWrappers_[i]) active_gpio_count++;
    }
    Logger::GetInstance().Info(TAG, "Active GPIO Wrappers: %d/%d",
                               active_gpio_count, static_cast<int>(gpioWrappers_.size()));

    // Bootloader config
    if (bootCfg_) {
        Logger::GetInstance().Info(TAG, "Boot Mode: %s",
            bootCfg_->boot.boot_mode == tmc9660::bootcfg::BootMode::Parameter ? "Parameter" :
            bootCfg_->boot.boot_mode == tmc9660::bootcfg::BootMode::Register ? "Register" : "Unknown");
    }

    Logger::GetInstance().Info(TAG, "=== END TMC9660 HANDLER DIAGNOSTICS ===");
}

const char* Tmc9660Handler::GetDescription() const noexcept {
    return description_;
}
