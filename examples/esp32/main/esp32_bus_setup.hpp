/**
 * @file esp32_bus_setup.hpp
 * @brief Shared ESP32 bus initialization for hf-core handler tests
 *
 * Provides factory functions to create and configure ESP32 I2C, SPI, GPIO,
 * and ADC peripherals using the internal interface wrap (EspI2c, EspSpi,
 * EspGpio, EspAdc). These are shared across all handler tests.
 *
 * Each factory returns a unique_ptr or a reference to a static instance.
 * Bus objects persist for the lifetime of the application.
 *
 * @note Include this file AFTER esp32_test_config.hpp (for pin definitions).
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#pragma once

#include "esp32_test_config.hpp"

// ESP32 internal interface implementations
#include "mcu/esp32/EspAdc.h"
#include "mcu/esp32/EspGpio.h"
#include "mcu/esp32/EspI2c.h"
#include "mcu/esp32/EspSpi.h"

#include <memory>

// ESP-IDF headers (C linkage)
#ifdef __cplusplus
extern "C" {
#endif
#include "esp_log.h"
#ifdef __cplusplus
}
#endif

static const char* BUS_TAG = "BusSetup";

// ═══════════════════════════════════════════════════════════════════════════
// I2C BUS — Shared by BNO08x, PCA9685, PCAL95555 handlers
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Create and initialize the shared I2C bus.
 *
 * Returns an EspI2cBus configured for the pins and speed in esp32_test_config.
 * Multiple I2C devices can be attached to this bus.
 *
 * @return Pointer to persistent EspI2cBus instance, or nullptr on failure.
 */
inline EspI2cBus* get_shared_i2c_bus() noexcept {
    static std::unique_ptr<EspI2cBus> bus;
    if (!bus) {
        hf_i2c_master_bus_config_t bus_cfg = {};
        bus_cfg.i2c_port = static_cast<i2c_port_t>(I2C_PORT_NUM);
        bus_cfg.sda_io_num = static_cast<hf_pin_num_t>(PIN_I2C_SDA);
        bus_cfg.scl_io_num = static_cast<hf_pin_num_t>(PIN_I2C_SCL);
        bus_cfg.flags.enable_internal_pullup = true;

        bus = std::make_unique<EspI2cBus>(bus_cfg);
        if (!bus->Initialize()) {
            ESP_LOGE(BUS_TAG, "I2C bus initialization failed (SDA=%d, SCL=%d)",
                     PIN_I2C_SDA, PIN_I2C_SCL);
            bus.reset();
            return nullptr;
        }
        ESP_LOGI(BUS_TAG, "I2C bus initialized (SDA=%d, SCL=%d, %d Hz)",
                 PIN_I2C_SDA, PIN_I2C_SCL, I2C_CLOCK_HZ);
    }
    return bus.get();
}

/**
 * @brief Get an I2C device handle for a specific address on the shared bus.
 * @param address 7-bit I2C device address.
 * @return Pointer to BaseI2c device, or nullptr if bus isn't initialized.
 */
inline BaseI2c* get_i2c_device(uint8_t address) noexcept {
    auto* bus = get_shared_i2c_bus();
    if (!bus) return nullptr;

    // Create device config and add to bus using CreateDevice/GetDevice pattern
    hf_i2c_device_config_t dev_cfg = {};
    dev_cfg.device_address = address;
    dev_cfg.scl_speed_hz = I2C_CLOCK_HZ;
    int dev_index = bus->CreateDevice(dev_cfg);
    if (dev_index < 0) {
        ESP_LOGE(BUS_TAG, "Failed to create I2C device at 0x%02X", address);
        return nullptr;
    }
    auto* dev = bus->GetDevice(dev_index);
    if (!dev || !dev->EnsureInitialized()) {
        ESP_LOGE(BUS_TAG, "Failed to initialize I2C device at 0x%02X", address);
        return nullptr;
    }
    ESP_LOGI(BUS_TAG, "I2C device added at 0x%02X", address);
    return dev;
}

// ═══════════════════════════════════════════════════════════════════════════
// SPI BUS — Shared by AS5047U, TMC9660 handlers
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Create and initialize the shared SPI bus.
 * @return Pointer to persistent EspSpiBus instance, or nullptr on failure.
 */
inline EspSpiBus* get_shared_spi_bus() noexcept {
    static std::unique_ptr<EspSpiBus> spi;
    if (!spi) {
        hf_spi_bus_config_t spi_cfg = {};
        spi_cfg.host = static_cast<hf_host_id_t>(SPI_HOST_ID);
        spi_cfg.mosi_pin = static_cast<hf_pin_num_t>(PIN_SPI_MOSI);
        spi_cfg.miso_pin = static_cast<hf_pin_num_t>(PIN_SPI_MISO);
        spi_cfg.sclk_pin = static_cast<hf_pin_num_t>(PIN_SPI_SCLK);
        spi_cfg.dma_channel = 0; // 0 = auto DMA channel selection

        spi = std::make_unique<EspSpiBus>(spi_cfg);
        if (!spi->Initialize()) {
            ESP_LOGE(BUS_TAG, "SPI bus initialization failed");
            spi.reset();
            return nullptr;
        }
        ESP_LOGI(BUS_TAG, "SPI bus initialized (MOSI=%d, MISO=%d, SCLK=%d)",
                 PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCLK);
    }
    return spi.get();
}

// ═══════════════════════════════════════════════════════════════════════════
// GPIO — Factory for individual pins
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Create an EspGpio for a specific pin.
 * @param pin_num GPIO pin number.
 * @param direction Initial direction (input/output).
 * @param active_state Active-high or active-low.
 * @return unique_ptr to EspGpio instance.
 */
inline std::unique_ptr<EspGpio> create_gpio(
    int pin_num,
    hf_gpio_direction_t direction = hf_gpio_direction_t::HF_GPIO_DIRECTION_INPUT,
    hf_gpio_active_state_t active_state = hf_gpio_active_state_t::HF_GPIO_ACTIVE_HIGH) noexcept {

    auto gpio = std::make_unique<EspGpio>(
        static_cast<hf_pin_num_t>(pin_num), direction, active_state);

    if (!gpio->Initialize()) {
        ESP_LOGE(BUS_TAG, "GPIO%d initialization failed", pin_num);
        return nullptr;
    }
    return gpio;
}

// ═══════════════════════════════════════════════════════════════════════════
// ADC — For NTC thermistor handler
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Create and initialize the shared ADC instance.
 * @return Pointer to persistent EspAdc instance, or nullptr on failure.
 */
inline EspAdc* get_shared_adc() noexcept {
    static std::unique_ptr<EspAdc> adc;
    if (!adc) {
        hf_adc_unit_config_t adc_cfg = {};
        adc_cfg.unit_id = static_cast<uint8_t>(NTC_ADC_UNIT);
        adc_cfg.bit_width = hf_adc_bitwidth_t::WIDTH_12BIT;

        adc = std::make_unique<EspAdc>(adc_cfg);
        if (!adc->Initialize()) {
            ESP_LOGE(BUS_TAG, "ADC initialization failed (unit=%d)", NTC_ADC_UNIT);
            adc.reset();
            return nullptr;
        }
        ESP_LOGI(BUS_TAG, "ADC initialized (unit=%d, 12-bit)", NTC_ADC_UNIT);
    }
    return adc.get();
}

// ═══════════════════════════════════════════════════════════════════════════
// I2C BUS SCAN UTILITY
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Scan the I2C bus for responding devices.
 *
 * Probes addresses 0x08–0x77 and logs a grid of found devices.
 * Useful for verifying hardware connections before running handler tests.
 *
 * @param bus Pointer to initialized EspI2cBus.
 * @return Number of devices found.
 */
inline int scan_i2c_bus(EspI2cBus* bus) noexcept {
    if (!bus) return 0;

    int found = 0;
    ESP_LOGI(BUS_TAG, "── I2C Bus Scan ─────────────────────────");
    ESP_LOGI(BUS_TAG, "     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

    for (uint8_t row = 0; row < 8; ++row) {
        char line[64];
        int pos = snprintf(line, sizeof(line), "%02X: ", row << 4);
        for (uint8_t col = 0; col < 16; ++col) {
            uint8_t addr = (row << 4) | col;
            if (addr < 0x08 || addr > 0x77) {
                pos += snprintf(line + pos, sizeof(line) - pos, "   ");
                continue;
            }
            // Probe the address using the bus's built-in probe method
            if (bus->ProbeDevice(addr)) {
                pos += snprintf(line + pos, sizeof(line) - pos, "%02X ", addr);
                found++;
            } else {
                pos += snprintf(line + pos, sizeof(line) - pos, "-- ");
            }
        }
        ESP_LOGI(BUS_TAG, "%s", line);
    }
    ESP_LOGI(BUS_TAG, "Found %d device(s)", found);
    ESP_LOGI(BUS_TAG, "─────────────────────────────────────────");
    return found;
}
