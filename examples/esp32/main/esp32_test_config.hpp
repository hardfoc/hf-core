/**
 * @file esp32_test_config.hpp
 * @brief Centralized hardware pin and address configuration for hf-core tests
 *
 * All pin assignments, I2C addresses, SPI configurations, and ADC channels
 * are defined here. Modify this single file to match your hardware setup.
 *
 * Default pin assignments target the HardFOC Vortex v1 board with an
 * ESP32-S3 module. Override any pin at compile time with -D flags:
 *   -D PIN_I2C_SDA=21 -D PIN_I2C_SCL=22
 *
 * @author HardFOC Team
 * @date 2025-2026
 * @copyright GPL-3.0-or-later
 */

#pragma once

#include <cstdint>

// ═══════════════════════════════════════════════════════════════════════════
// I2C BUS CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 8
#endif
#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 9
#endif
#ifndef I2C_PORT_NUM
#define I2C_PORT_NUM 0
#endif
#ifndef I2C_CLOCK_HZ
#define I2C_CLOCK_HZ 400000  // 400 kHz (Fast Mode)
#endif

// ═══════════════════════════════════════════════════════════════════════════
// SPI BUS CONFIGURATION
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_SPI_MOSI
#define PIN_SPI_MOSI 11
#endif
#ifndef PIN_SPI_MISO
#define PIN_SPI_MISO 13
#endif
#ifndef PIN_SPI_SCLK
#define PIN_SPI_SCLK 12
#endif
#ifndef SPI_HOST_ID
#define SPI_HOST_ID 1  // SPI2_HOST on ESP32-S3
#endif

// ═══════════════════════════════════════════════════════════════════════════
// AS5047U ENCODER (SPI)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_AS5047U_CS
#define PIN_AS5047U_CS 10
#endif
#ifndef AS5047U_SPI_CLOCK_HZ
#define AS5047U_SPI_CLOCK_HZ 1000000  // 1 MHz
#endif

// ═══════════════════════════════════════════════════════════════════════════
// BNO08x IMU (I2C)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef BNO08X_I2C_ADDR
#define BNO08X_I2C_ADDR 0x4A  // 0x4A (SA0=LOW) or 0x4B (SA0=HIGH)
#endif
#ifndef PIN_BNO08X_INT
#define PIN_BNO08X_INT 4
#endif
#ifndef PIN_BNO08X_RST
#define PIN_BNO08X_RST 5
#endif

// ═══════════════════════════════════════════════════════════════════════════
// PCA9685 PWM CONTROLLER (I2C)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PCA9685_I2C_ADDR
#define PCA9685_I2C_ADDR 0x40  // Default (all address pins LOW)
#endif

// ═══════════════════════════════════════════════════════════════════════════
// PCAL95555 GPIO EXPANDER (I2C)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PCAL95555_I2C_ADDR
#define PCAL95555_I2C_ADDR 0x20  // Default (A0=A1=A2=LOW)
#endif
#ifndef PIN_PCAL95555_INT
#define PIN_PCAL95555_INT 6  // Interrupt output (active-low, optional)
#endif

// ═══════════════════════════════════════════════════════════════════════════
// NTC THERMISTOR (ADC)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef NTC_ADC_CHANNEL
#define NTC_ADC_CHANNEL 0  // ADC1_CH0
#endif
#ifndef NTC_ADC_UNIT
#define NTC_ADC_UNIT 0  // ADC_UNIT_1
#endif

// ═══════════════════════════════════════════════════════════════════════════
// TMC9660 MOTOR CONTROLLER (SPI)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_TMC9660_CS
#define PIN_TMC9660_CS 15
#endif
#ifndef PIN_TMC9660_RST
#define PIN_TMC9660_RST 16
#endif
#ifndef PIN_TMC9660_DRV_EN
#define PIN_TMC9660_DRV_EN 17
#endif
#ifndef PIN_TMC9660_FAULTN
#define PIN_TMC9660_FAULTN 18
#endif
#ifndef PIN_TMC9660_WAKE
#define PIN_TMC9660_WAKE 21
#endif
#ifndef TMC9660_DEVICE_ADDR
#define TMC9660_DEVICE_ADDR 1
#endif
#ifndef TMC9660_SPI_CLOCK_HZ
#define TMC9660_SPI_CLOCK_HZ 4000000  // 4 MHz
#endif

// ═══════════════════════════════════════════════════════════════════════════
// TMC5160 STEPPER MOTOR DRIVER (SPI)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_TMC5160_CS
#define PIN_TMC5160_CS 37
#endif
#ifndef PIN_TMC5160_DRV_ENN
#define PIN_TMC5160_DRV_ENN 38
#endif
#ifndef PIN_TMC5160_DIAG0
#define PIN_TMC5160_DIAG0 39
#endif
#ifndef PIN_TMC5160_DIAG1
#define PIN_TMC5160_DIAG1 40
#endif
#ifndef TMC5160_SPI_CLOCK_HZ
#define TMC5160_SPI_CLOCK_HZ 4000000  // 4 MHz
#endif

// ═══════════════════════════════════════════════════════════════════════════
// TLE92466ED SOLENOID DRIVER (SPI)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_TLE92466ED_CS
#define PIN_TLE92466ED_CS 41
#endif
#ifndef PIN_TLE92466ED_RESN
#define PIN_TLE92466ED_RESN 42
#endif
#ifndef PIN_TLE92466ED_EN
#define PIN_TLE92466ED_EN 2
#endif
#ifndef PIN_TLE92466ED_FAULTN
#define PIN_TLE92466ED_FAULTN 1
#endif
#ifndef TLE92466ED_SPI_CLOCK_HZ
#define TLE92466ED_SPI_CLOCK_HZ 5000000  // 5 MHz
#endif

// ═══════════════════════════════════════════════════════════════════════════
// MAX22200 SOLENOID / MOTOR DRIVER (SPI)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_MAX22200_CS
#define PIN_MAX22200_CS 36
#endif
#ifndef PIN_MAX22200_ENABLE
#define PIN_MAX22200_ENABLE 35
#endif
#ifndef PIN_MAX22200_CMD
#define PIN_MAX22200_CMD 45
#endif
#ifndef PIN_MAX22200_FAULT
#define PIN_MAX22200_FAULT 48
#endif
#ifndef MAX22200_SPI_CLOCK_HZ
#define MAX22200_SPI_CLOCK_HZ 5000000  // 5 MHz
#endif

// ═══════════════════════════════════════════════════════════════════════════
// WS2812 ADDRESSABLE LED STRIP (RMT)
// ═══════════════════════════════════════════════════════════════════════════

#ifndef PIN_WS2812_DATA
#define PIN_WS2812_DATA 48
#endif
#ifndef WS2812_NUM_LEDS
#define WS2812_NUM_LEDS 8
#endif
#ifndef WS2812_RMT_CHANNEL
#define WS2812_RMT_CHANNEL 0
#endif
