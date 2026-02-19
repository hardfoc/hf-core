# ===========================================================================
# hf-core Shared Build Settings — Configurable Feature-Based Source Collection
# ===========================================================================
#
# This file is the central configuration point for the hf-core platform.
# Consumers (HAL layers, core examples, standalone projects) include this
# file after setting feature toggles to control which drivers, handlers,
# utilities, and interface implementations are compiled.
#
# Usage:
#   set(HF_CORE_ENABLE_TMC9660 ON)
#   set(HF_CORE_ENABLE_PCAL95555 ON)
#   set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
#   include(path/to/core/cmake/hf_core_build_settings.cmake)
#   # Then use: HF_CORE_SOURCES, HF_CORE_INCLUDE_DIRS, HF_CORE_IDF_REQUIRES
#
# Outputs:
#   HF_CORE_SOURCES             — All collected source files
#   HF_CORE_INCLUDE_DIRS        — All collected include directories
#   HF_CORE_IDF_REQUIRES        — ESP-IDF component requirements
#   HF_CORE_COMPILE_DEFINITIONS — Feature-based preprocessor definitions
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2025-2026 HardFOC Team
# ===========================================================================

include_guard(GLOBAL)

# ===========================================================================
# Path Roots (resolved from this file's location: core/cmake/)
# ===========================================================================
set(HF_CORE_ROOT          "${CMAKE_CURRENT_LIST_DIR}/..")
set(HF_CORE_HANDLER_ROOT  "${HF_CORE_ROOT}/handlers")
set(HF_CORE_DRIVER_EXT    "${HF_CORE_ROOT}/hf-core-drivers/external")
set(HF_CORE_DRIVER_INT    "${HF_CORE_ROOT}/hf-core-drivers/internal")
set(HF_CORE_UTILS_ROOT    "${HF_CORE_ROOT}/hf-core-utils")

# ===========================================================================
# Feature Toggles — Set these BEFORE including this file
# ===========================================================================

# ── Foundation (always-on, cannot be disabled) ────────────────────────────
# Internal interface base classes, ESP32 core implementations,
# RTOS wrappers, and general utilities are always included.

# ── Optional Utility Groups ───────────────────────────────────────────────
if(NOT DEFINED HF_CORE_ENABLE_UTILS_CANOPEN)
    set(HF_CORE_ENABLE_UTILS_CANOPEN OFF)
endif()

# ── Optional Interface Implementations ────────────────────────────────────
# These are auto-enabled by driver selections but can also be set manually.
if(NOT DEFINED HF_CORE_ENABLE_UART)
    set(HF_CORE_ENABLE_UART OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_CAN)
    set(HF_CORE_ENABLE_CAN OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_PWM)
    set(HF_CORE_ENABLE_PWM OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_PIO)
    set(HF_CORE_ENABLE_PIO OFF)
endif()

# ── External Drivers + Handlers (opt-in) ──────────────────────────────────
if(NOT DEFINED HF_CORE_ENABLE_AS5047U)
    set(HF_CORE_ENABLE_AS5047U OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_BNO08X)
    set(HF_CORE_ENABLE_BNO08X OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_MAX22200)
    set(HF_CORE_ENABLE_MAX22200 OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_NTC_THERMISTOR)
    set(HF_CORE_ENABLE_NTC_THERMISTOR OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_PCA9685)
    set(HF_CORE_ENABLE_PCA9685 OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_PCAL95555)
    set(HF_CORE_ENABLE_PCAL95555 OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_TLE92466ED)
    set(HF_CORE_ENABLE_TLE92466ED OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_TMC5160)
    set(HF_CORE_ENABLE_TMC5160 OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_TMC9660)
    set(HF_CORE_ENABLE_TMC9660 OFF)
endif()
if(NOT DEFINED HF_CORE_ENABLE_WS2812)
    set(HF_CORE_ENABLE_WS2812 OFF)
endif()

# ── Logger (almost always needed) ─────────────────────────────────────────
if(NOT DEFINED HF_CORE_ENABLE_LOGGER)
    set(HF_CORE_ENABLE_LOGGER ON)
endif()

# ===========================================================================
# Auto-Enable Dependencies
# ===========================================================================

# Drivers that use UART need the UART interface implementation
if(HF_CORE_ENABLE_TMC5160 OR HF_CORE_ENABLE_TMC9660)
    set(HF_CORE_ENABLE_UART ON)
endif()

# WS2812 needs the RMT peripheral
if(HF_CORE_ENABLE_WS2812)
    set(HF_CORE_ENABLE_RMT ON)
else()
    if(NOT DEFINED HF_CORE_ENABLE_RMT)
        set(HF_CORE_ENABLE_RMT OFF)
    endif()
endif()

# NTC thermistor needs the ADC peripheral
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    set(HF_CORE_ENABLE_ADC ON)
else()
    if(NOT DEFINED HF_CORE_ENABLE_ADC)
        set(HF_CORE_ENABLE_ADC OFF)
    endif()
endif()

# ===========================================================================
# Foundation Sources (always included)
# ===========================================================================

# ── ESP32 Interface Implementations (core set) ───────────────────────────
set(HF_CORE_INTERFACE_SOURCES
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspSpi.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspI2c.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspGpio.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspAdc.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspNvs.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspLogger.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspTemperature.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspPeriodicTimer.cpp"
    # Shared utilities from internal interface wrap
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/utils/AsciiArtGenerator.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/utils/DigitalOutputGuard.cpp"
)

# ── Optional Interface Implementations ────────────────────────────────────
if(HF_CORE_ENABLE_UART)
    list(APPEND HF_CORE_INTERFACE_SOURCES
        "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspUart.cpp")
endif()
if(HF_CORE_ENABLE_CAN)
    list(APPEND HF_CORE_INTERFACE_SOURCES
        "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspCan.cpp")
endif()
if(HF_CORE_ENABLE_PWM)
    list(APPEND HF_CORE_INTERFACE_SOURCES
        "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspPwm.cpp")
endif()
if(HF_CORE_ENABLE_PIO)
    list(APPEND HF_CORE_INTERFACE_SOURCES
        "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspPio.cpp")
endif()
# Note: EspBluetooth and EspWifi are NOT included by default.
# They require esp_bt/esp_wifi components and are rarely needed.

# ── RTOS Wrapper Utilities (always included) ──────────────────────────────
set(HF_CORE_RTOS_SOURCES
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/BaseThread.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/CriticalSection.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/FreeRTOSUtils.c"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/Mutex.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/MutexGuard.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/OsUtility.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/PeriodicTimer.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/SignalSemaphore.cpp"
)

# ── General Utilities (always included) ───────────────────────────────────
set(HF_CORE_GENERAL_UTILS_SOURCES
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/SoftwareVersion.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/TimestampedVariable.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/CrcCalculator.c"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/Utility.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/VariableAnomalyMonitor.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/VariableMonitor.cpp"
)

# ── CANopen Utilities (optional) ──────────────────────────────────────────
set(HF_CORE_CANOPEN_SOURCES "")
if(HF_CORE_ENABLE_UTILS_CANOPEN)
    set(HF_CORE_CANOPEN_SOURCES
        "${HF_CORE_UTILS_ROOT}/hf-utils-canopen/src/CanOpenUtils.cpp"
        "${HF_CORE_UTILS_ROOT}/hf-utils-canopen/src/CanOpenMotorUtils.cpp"
    )
endif()

# ===========================================================================
# External Driver Sources + Handler Sources (opt-in per feature)
# ===========================================================================

set(HF_CORE_HANDLER_SOURCES "")
set(HF_CORE_EXT_DRIVER_SOURCES "")
set(HF_CORE_EXT_DRIVER_INCLUDE_DIRS "")

# ── Include each driver's build_settings for version header generation ────
# Each driver's build_settings.cmake generates a version header and exposes
# sources and include directories. We delegate to these files completely to
# avoid duplicating source/include lists.
#
# Two driver styles exist:
#   Style A: Uses include_guard(GLOBAL) + CMAKE_CURRENT_LIST_DIR. Exports:
#            HF_<NAME>_PUBLIC_INCLUDE_DIRS, HF_<NAME>_SOURCE_FILES
#   Style B: Requires HF_<NAME>_ROOT set before inclusion.     Exports:
#            HF_<NAME>_INCLUDE_DIRS,        HF_<NAME>_SOURCES

# ── AS5047U (SPI encoder — header-only driver, Style A) ──────────────────
if(HF_CORE_ENABLE_AS5047U)
    include("${HF_CORE_DRIVER_EXT}/hf-as5047u-driver/cmake/hf_as5047u_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/as5047u/As5047uHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_AS5047U_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_AS5047U_SOURCE_FILES})
endif()

# ── BNO08x (I2C/SPI IMU — vendor C sources, Style A) ─────────────────────
if(HF_CORE_ENABLE_BNO08X)
    include("${HF_CORE_DRIVER_EXT}/hf-bno08x-driver/cmake/hf_bno08x_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/bno08x/Bno08xHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_BNO08X_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_BNO08X_SOURCE_FILES})
endif()

# ── MAX22200 (SPI solenoid driver — header-only, Style A) ────────────────
if(HF_CORE_ENABLE_MAX22200)
    include("${HF_CORE_DRIVER_EXT}/hf-max22200-driver/cmake/hf_max22200_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/max22200/Max22200Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_MAX22200_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_MAX22200_SOURCE_FILES})
endif()

# ── NTC Thermistor (ADC — has lookup table sources, Style B) ─────────────
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    set(HF_NTC_THERMISTOR_ROOT "${HF_CORE_DRIVER_EXT}/hf-ntc-thermistor-driver")
    include("${HF_NTC_THERMISTOR_ROOT}/cmake/hf_ntc_thermistor_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/ntc/NtcTemperatureHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_NTC_THERMISTOR_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_NTC_THERMISTOR_SOURCES})
endif()

# ── PCA9685 (I2C PWM driver — header-only with .ipp, Style A) ────────────
if(HF_CORE_ENABLE_PCA9685)
    include("${HF_CORE_DRIVER_EXT}/hf-pca9685-driver/cmake/hf_pca9685_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/pca9685/Pca9685Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_PCA9685_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_PCA9685_SOURCE_FILES})
endif()

# ── PCAL95555 (I2C GPIO expander — header-only with .ipp, Style A) ───────
if(HF_CORE_ENABLE_PCAL95555)
    include("${HF_CORE_DRIVER_EXT}/hf-pcal95555-driver/cmake/hf_pcal95555_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/pcal95555/Pcal95555Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_PCAL95555_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_PCAL95555_SOURCE_FILES})
endif()

# ── TLE92466ED (SPI solenoid driver — header-only, Style A) ──────────────
if(HF_CORE_ENABLE_TLE92466ED)
    include("${HF_CORE_DRIVER_EXT}/hf-tle92466ed-driver/cmake/hf_tle92466ed_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/tle92466ed/Tle92466edHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_TLE92466ED_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_TLE92466ED_SOURCE_FILES})
endif()

# ── TMC5160 (SPI/UART stepper driver — header-only, Style A) ─────────────
# Note: tmc51x0_register_defs.cpp excluded by design — has duplicate case
# values from X-macro expansion (IOIN/OUTPUT share address 0x04).
# The driver's build_settings already reports empty SOURCE_FILES.
if(HF_CORE_ENABLE_TMC5160)
    include("${HF_CORE_DRIVER_EXT}/hf-tmc5160-driver/cmake/hf_tmc51x0_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/tmc5160/Tmc5160Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_TMC51X0_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_TMC51X0_SOURCE_FILES})
endif()

# ── TMC9660 (SPI/UART BLDC/stepper driver — has bootloader, Style B) ─────
if(HF_CORE_ENABLE_TMC9660)
    set(HF_TMC9660_ROOT "${HF_CORE_DRIVER_EXT}/hf-tmc9660-driver")
    include("${HF_TMC9660_ROOT}/cmake/hf_tmc9660_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/tmc9660/Tmc9660Handler.cpp"
        "${HF_CORE_HANDLER_ROOT}/tmc9660/Tmc9660AdcWrapper.cpp"
    )
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_TMC9660_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_TMC9660_SOURCES})
endif()

# ── WS2812 (RMT LED strip — mixed C/C++ sources, Style B) ────────────────
if(HF_CORE_ENABLE_WS2812)
    set(HF_WS2812_RMT_ROOT "${HF_CORE_DRIVER_EXT}/hf-ws2812-rmt-driver")
    include("${HF_WS2812_RMT_ROOT}/cmake/hf_ws2812_rmt_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/ws2812/Ws2812Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_WS2812_RMT_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_WS2812_RMT_SOURCES})
endif()

# ── Logger Handler ────────────────────────────────────────────────────────
if(HF_CORE_ENABLE_LOGGER)
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/logger/Logger.cpp")
endif()

# ===========================================================================
# Include Directories
# ===========================================================================

# ── Foundation Include Dirs (always included) ─────────────────────────────
set(HF_CORE_INCLUDE_DIRS
    # Parent of core/ — allows #include "core/handlers/xxx/Xxx.h" paths
    "${HF_CORE_ROOT}/.."
    # Core root — allows #include "handlers/xxx/Xxx.h" paths
    "${HF_CORE_ROOT}"
    # Handler common utilities
    "${HF_CORE_HANDLER_ROOT}/common"
    # Internal interface — base abstract classes
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/base"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/utils"
    # Internal interface — ESP32 MCU implementations
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/mcu"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/mcu/esp32"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/mcu/esp32/utils"
    # Pin configuration
    "${HF_CORE_DRIVER_INT}/hf-pincfg"
    "${HF_CORE_DRIVER_INT}/hf-pincfg/src"
    # General utilities
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/include"
    # RTOS wrapper utilities
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/include"
)

# ── CANopen Utilities Include Dir ─────────────────────────────────────────
if(HF_CORE_ENABLE_UTILS_CANOPEN)
    list(APPEND HF_CORE_INCLUDE_DIRS
        "${HF_CORE_UTILS_ROOT}/hf-utils-canopen/include")
endif()

# ── Per-Handler Include Dirs (add handler dir for direct includes) ────────
if(HF_CORE_ENABLE_AS5047U)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/as5047u")
endif()
if(HF_CORE_ENABLE_BNO08X)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/bno08x")
endif()
if(HF_CORE_ENABLE_MAX22200)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/max22200")
endif()
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/ntc")
endif()
if(HF_CORE_ENABLE_PCA9685)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/pca9685")
endif()
if(HF_CORE_ENABLE_PCAL95555)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/pcal95555")
endif()
if(HF_CORE_ENABLE_TLE92466ED)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/tle92466ed")
endif()
if(HF_CORE_ENABLE_TMC5160)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/tmc5160")
endif()
if(HF_CORE_ENABLE_TMC9660)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/tmc9660")
endif()
if(HF_CORE_ENABLE_WS2812)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/ws2812")
endif()
if(HF_CORE_ENABLE_LOGGER)
    list(APPEND HF_CORE_INCLUDE_DIRS "${HF_CORE_HANDLER_ROOT}/logger")
endif()

# ── External Driver Include Dirs (added by per-driver sections above) ─────
list(APPEND HF_CORE_INCLUDE_DIRS ${HF_CORE_EXT_DRIVER_INCLUDE_DIRS})

# ===========================================================================
# ESP-IDF Component Requirements
# ===========================================================================

# ── Base Requirements (always needed) ─────────────────────────────────────
set(HF_CORE_IDF_REQUIRES
    driver
    freertos
    nvs_flash
    esp_timer
    hal
    soc
    log
    esp_driver_spi
    esp_driver_i2c
    esp_driver_gpio
)

# ── Conditional Requirements ──────────────────────────────────────────────
if(HF_CORE_ENABLE_UART)
    list(APPEND HF_CORE_IDF_REQUIRES esp_driver_uart)
endif()
if(HF_CORE_ENABLE_ADC)
    list(APPEND HF_CORE_IDF_REQUIRES esp_adc)
endif()
if(HF_CORE_ENABLE_RMT)
    list(APPEND HF_CORE_IDF_REQUIRES esp_driver_rmt)
endif()

# Deduplicate
list(REMOVE_DUPLICATES HF_CORE_IDF_REQUIRES)

# ===========================================================================
# Compile Definitions
# ===========================================================================

set(HF_CORE_COMPILE_DEFINITIONS
    HF_MCU_FAMILY_ESP32=1
    HF_THREAD_SAFE=1
)

if(HF_CORE_ENABLE_AS5047U)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_AS5047U_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_BNO08X)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_BNO08X_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_MAX22200)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_MAX22200_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_NTC_THERMISTOR_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_PCA9685)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_PCA9685_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_PCAL95555)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_PCAL95555_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_TLE92466ED)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_TLE92466_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_TMC5160)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_TMC5160_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_TMC9660)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_TMC9660_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_WS2812)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_WS2812_SUPPORT=1)
endif()
if(HF_CORE_ENABLE_LOGGER)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_LOGGER=1)
endif()
if(HF_CORE_ENABLE_UTILS_CANOPEN)
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HARDFOC_CANOPEN_UTILS=1)
endif()
list(APPEND HF_CORE_COMPILE_DEFINITIONS
    HARDFOC_RTOS_WRAP=1
    HARDFOC_CORE_UTILS=1
    HARDFOC_CORE_DRIVERS=1
)

# ===========================================================================
# Final Aggregation — These are the outputs consumers use
# ===========================================================================

set(HF_CORE_SOURCES
    ${HF_CORE_INTERFACE_SOURCES}
    ${HF_CORE_RTOS_SOURCES}
    ${HF_CORE_GENERAL_UTILS_SOURCES}
    ${HF_CORE_CANOPEN_SOURCES}
    ${HF_CORE_HANDLER_SOURCES}
    ${HF_CORE_EXT_DRIVER_SOURCES}
)

# ===========================================================================
# Build Summary (printed during configuration)
# ===========================================================================

list(LENGTH HF_CORE_SOURCES          _hf_src_count)
list(LENGTH HF_CORE_INCLUDE_DIRS     _hf_inc_count)
list(LENGTH HF_CORE_IDF_REQUIRES     _hf_req_count)
list(LENGTH HF_CORE_HANDLER_SOURCES  _hf_handler_count)
list(LENGTH HF_CORE_EXT_DRIVER_SOURCES _hf_driver_count)

# Build enabled-features string for compact display
set(_hf_enabled_features "")
if(HF_CORE_ENABLE_AS5047U)
    string(APPEND _hf_enabled_features " AS5047U")
endif()
if(HF_CORE_ENABLE_BNO08X)
    string(APPEND _hf_enabled_features " BNO08x")
endif()
if(HF_CORE_ENABLE_MAX22200)
    string(APPEND _hf_enabled_features " MAX22200")
endif()
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    string(APPEND _hf_enabled_features " NTC")
endif()
if(HF_CORE_ENABLE_PCA9685)
    string(APPEND _hf_enabled_features " PCA9685")
endif()
if(HF_CORE_ENABLE_PCAL95555)
    string(APPEND _hf_enabled_features " PCAL95555")
endif()
if(HF_CORE_ENABLE_TLE92466ED)
    string(APPEND _hf_enabled_features " TLE92466ED")
endif()
if(HF_CORE_ENABLE_TMC5160)
    string(APPEND _hf_enabled_features " TMC5160")
endif()
if(HF_CORE_ENABLE_TMC9660)
    string(APPEND _hf_enabled_features " TMC9660")
endif()
if(HF_CORE_ENABLE_WS2812)
    string(APPEND _hf_enabled_features " WS2812")
endif()
if(HF_CORE_ENABLE_LOGGER)
    string(APPEND _hf_enabled_features " Logger")
endif()
if(HF_CORE_ENABLE_UTILS_CANOPEN)
    string(APPEND _hf_enabled_features " CANopen")
endif()
if(NOT _hf_enabled_features)
    set(_hf_enabled_features " (foundation only)")
endif()

set(_hf_enabled_ifaces "")
if(HF_CORE_ENABLE_UART)
    string(APPEND _hf_enabled_ifaces " UART")
endif()
if(HF_CORE_ENABLE_CAN)
    string(APPEND _hf_enabled_ifaces " CAN")
endif()
if(HF_CORE_ENABLE_PWM)
    string(APPEND _hf_enabled_ifaces " PWM")
endif()
if(HF_CORE_ENABLE_PIO)
    string(APPEND _hf_enabled_ifaces " PIO")
endif()
if(NOT _hf_enabled_ifaces)
    set(_hf_enabled_ifaces " (core set only)")
endif()

message(STATUS "")
message(STATUS "╔══════════════════════════════════════════════════════╗")
message(STATUS "║           hf-core Build Configuration               ║")
message(STATUS "╚══════════════════════════════════════════════════════╝")
message(STATUS "  Features:      ${_hf_enabled_features}")
message(STATUS "  Interfaces:    SPI I2C GPIO ADC NVS Timer${_hf_enabled_ifaces}")
message(STATUS "  Sources:       ${_hf_src_count} total (${_hf_handler_count} handlers, ${_hf_driver_count} driver)")
message(STATUS "  Include dirs:  ${_hf_inc_count}")
message(STATUS "  IDF requires:  ${HF_CORE_IDF_REQUIRES}")
message(STATUS "")
