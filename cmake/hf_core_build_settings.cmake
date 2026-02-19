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
#   # 1. Platform selection (MCU + RTOS)
#   set(HF_CORE_MCU  "ESP32")          # MCU family: ESP32 (STM32, RP2040 future)
#   set(HF_CORE_RTOS "FREERTOS")       # RTOS: FREERTOS (THREADX, ZEPHYR future)
#
#   # 2. Driver/handler selection
#   set(HF_CORE_ENABLE_TMC9660 ON)
#   set(HF_CORE_ENABLE_PCAL95555 ON)
#   set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
#
#   # 3. Include this file
#   include(path/to/core/cmake/hf_core_build_settings.cmake)
#
#   # 4. Use the outputs:
#   #    HF_CORE_SOURCES, HF_CORE_INCLUDE_DIRS,
#   #    HF_CORE_IDF_REQUIRES (ESP32 only), HF_CORE_COMPILE_DEFINITIONS
#
# Outputs:
#   HF_CORE_SOURCES             — All collected source files
#   HF_CORE_INCLUDE_DIRS        — All collected include directories
#   HF_CORE_COMPILE_DEFINITIONS — Feature-based preprocessor definitions
#   HF_CORE_IDF_REQUIRES        — Build-system component requirements (ESP-IDF)
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
# ██  PLATFORM SELECTION — MCU Family + RTOS
# ===========================================================================
#
# These two variables determine which low-level implementations are compiled.
# Everything above the platform layer (handlers, drivers, utilities) is
# hardware-agnostic and compiled unconditionally when enabled.
#
# Supported values (current + planned):
#   HF_CORE_MCU:   ESP32
#   HF_CORE_RTOS:  FREERTOS
#
# The HAL CMakeLists.txt sets these BEFORE including this file.
# If not set, they default to the only currently-implemented platform.
# ===========================================================================

# ── MCU Family ────────────────────────────────────────────────────────────
if(NOT DEFINED HF_CORE_MCU)
    set(HF_CORE_MCU "ESP32")
endif()
string(TOUPPER "${HF_CORE_MCU}" HF_CORE_MCU)

set(_HF_SUPPORTED_MCUS "ESP32")   # extend: "ESP32;STM32;RP2040"
if(NOT HF_CORE_MCU IN_LIST _HF_SUPPORTED_MCUS)
    message(FATAL_ERROR
        "[hf-core] Unsupported MCU '${HF_CORE_MCU}'. "
        "Supported: ${_HF_SUPPORTED_MCUS}")
endif()

# Derive lowercase directory name from MCU family
string(TOLOWER "${HF_CORE_MCU}" _HF_MCU_DIR)  # "esp32", "stm32", ...

# ── RTOS Selection ────────────────────────────────────────────────────────
if(NOT DEFINED HF_CORE_RTOS)
    set(HF_CORE_RTOS "FREERTOS")
endif()
string(TOUPPER "${HF_CORE_RTOS}" HF_CORE_RTOS)

set(_HF_SUPPORTED_RTOS "FREERTOS")   # extend: "FREERTOS;THREADX;ZEPHYR"
if(NOT HF_CORE_RTOS IN_LIST _HF_SUPPORTED_RTOS)
    message(FATAL_ERROR
        "[hf-core] Unsupported RTOS '${HF_CORE_RTOS}'. "
        "Supported: ${_HF_SUPPORTED_RTOS}")
endif()

# ===========================================================================
# Feature Toggles — Set these BEFORE including this file
# ===========================================================================

# ── Foundation (always-on, cannot be disabled) ────────────────────────────
# Base abstract classes, MCU-selected implementations, RTOS wrappers,
# and general utilities are always included based on platform selection.

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
# ██  MCU-SPECIFIC DRIVER COMPATIBILITY GATES
# ===========================================================================
#
# Some external drivers are tied to MCU-specific peripherals (e.g. WS2812
# requires ESP32 RMT). If the user enables such a driver but the selected
# MCU doesn't support it, we emit a clear fatal error at configure time
# rather than letting it fail cryptically during compilation.
#
# Portable drivers (communicate via Base* interfaces like SPI, I2C, ADC)
# work on any MCU and have no gate.
#
# To add a new MCU-gated driver:
#   1. Add an entry to the MCU_REQUIRED table below
#   2. Document which peripheral it needs and why
# ===========================================================================

# Table: DRIVER_NAME -> REQUIRED_MCU ; PERIPHERAL_REASON
# (Only drivers with MCU-specific peripheral dependencies need entries)
set(_HF_MCU_GATED_DRIVERS
    "WS2812:ESP32:RMT peripheral"
    # Future examples:
    # "SOME_STM32_DRIVER:STM32:DMA2D peripheral"
)

foreach(_gate IN LISTS _HF_MCU_GATED_DRIVERS)
    # Parse "DRIVER:MCU:REASON"
    string(REPLACE ":" ";" _gate_parts "${_gate}")
    list(GET _gate_parts 0 _gate_driver)
    list(GET _gate_parts 1 _gate_mcu)
    list(GET _gate_parts 2 _gate_reason)

    # Check: is this driver enabled AND is the MCU wrong?
    if(HF_CORE_ENABLE_${_gate_driver} AND NOT HF_CORE_MCU STREQUAL "${_gate_mcu}")
        message(FATAL_ERROR
            "[hf-core] Driver '${_gate_driver}' requires MCU=${_gate_mcu} "
            "(${_gate_reason}), but HF_CORE_MCU=${HF_CORE_MCU}. "
            "Either disable HF_CORE_ENABLE_${_gate_driver} or change HF_CORE_MCU.")
    endif()
endforeach()

# ===========================================================================
# ██  FOUNDATION SOURCES — MCU-selected interface implementations
# ===========================================================================

# ── MCU Interface Implementations (selected by HF_CORE_MCU) ──────────────
# Core set: SPI, I2C, GPIO, ADC, NVS, Logger, Temperature, PeriodicTimer
# These source paths use _HF_MCU_DIR to select the correct mcu/ subfolder.

set(_HF_MCU_SRC "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/mcu/${_HF_MCU_DIR}")
set(_HF_MCU_INC "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/mcu/${_HF_MCU_DIR}")

if(HF_CORE_MCU STREQUAL "ESP32")
    # ESP32 family — EspSpi, EspI2c, EspGpio, EspAdc, etc.
    set(HF_CORE_INTERFACE_SOURCES
        "${_HF_MCU_SRC}/EspSpi.cpp"
        "${_HF_MCU_SRC}/EspI2c.cpp"
        "${_HF_MCU_SRC}/EspGpio.cpp"
        "${_HF_MCU_SRC}/EspAdc.cpp"
        "${_HF_MCU_SRC}/EspNvs.cpp"
        "${_HF_MCU_SRC}/EspLogger.cpp"
        "${_HF_MCU_SRC}/EspTemperature.cpp"
        "${_HF_MCU_SRC}/EspPeriodicTimer.cpp"
    )

    # Optional interfaces (ESP32)
    if(HF_CORE_ENABLE_UART)
        list(APPEND HF_CORE_INTERFACE_SOURCES "${_HF_MCU_SRC}/EspUart.cpp")
    endif()
    if(HF_CORE_ENABLE_CAN)
        list(APPEND HF_CORE_INTERFACE_SOURCES "${_HF_MCU_SRC}/EspCan.cpp")
    endif()
    if(HF_CORE_ENABLE_PWM)
        list(APPEND HF_CORE_INTERFACE_SOURCES "${_HF_MCU_SRC}/EspPwm.cpp")
    endif()
    if(HF_CORE_ENABLE_PIO)
        list(APPEND HF_CORE_INTERFACE_SOURCES "${_HF_MCU_SRC}/EspPio.cpp")
    endif()

# elseif(HF_CORE_MCU STREQUAL "STM32")
#     set(HF_CORE_INTERFACE_SOURCES
#         "${_HF_MCU_SRC}/StmSpi.cpp"
#         "${_HF_MCU_SRC}/StmI2c.cpp"
#         ...
#     )
endif()

# Shared utilities from internal interface wrap (MCU-agnostic)
list(APPEND HF_CORE_INTERFACE_SOURCES
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/utils/AsciiArtGenerator.cpp"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/src/utils/DigitalOutputGuard.cpp"
)

# Note: EspBluetooth and EspWifi are NOT included by default.
# They require esp_bt/esp_wifi components and are rarely needed.

# ===========================================================================
# ██  RTOS SOURCES — Selected by HF_CORE_RTOS
# ===========================================================================
#
# The RTOS wrapper provides OS-agnostic primitives (threads, mutexes,
# semaphores, timers, queues) via a common API. The source files in the
# wrapper use OsAbstraction.h which maps to the selected RTOS.
#
# Currently all wrapper sources are in a single directory. If a second RTOS
# is added, the structure would become:
#   hf-utils-rtos-wrap/src/freertos/   — FreeRTOS implementations
#   hf-utils-rtos-wrap/src/threadx/    — ThreadX implementations
#   hf-utils-rtos-wrap/src/common/     — shared code
#
# For now, the single directory covers FreeRTOS. The key abstraction point
# is OsAbstraction.h which would be swapped per RTOS selection.
# ===========================================================================

if(HF_CORE_RTOS STREQUAL "FREERTOS")
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

# elseif(HF_CORE_RTOS STREQUAL "THREADX")
#     set(HF_CORE_RTOS_SOURCES
#         "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/threadx/BaseThread.cpp"
#         ...
#     )
endif()

# ── General Utilities (always included, hardware-agnostic) ────────────────
set(HF_CORE_GENERAL_UTILS_SOURCES
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/SoftwareVersion.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/TimestampedVariable.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/CrcCalculator.c"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/Utility.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/VariableAnomalyMonitor.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-general/src/VariableMonitor.cpp"
)

# ── CANopen Utilities (optional, hardware-agnostic) ───────────────────────
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
#
# Driver portability classification:
#   PORTABLE  — Uses Base* interfaces (SPI, I2C, ADC). Works on any MCU.
#   MCU-GATED — Requires MCU-specific peripheral. Gated above.

# ── AS5047U (SPI encoder — PORTABLE, header-only driver, Style A) ────────
if(HF_CORE_ENABLE_AS5047U)
    include("${HF_CORE_DRIVER_EXT}/hf-as5047u-driver/cmake/hf_as5047u_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/as5047u/As5047uHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_AS5047U_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_AS5047U_SOURCE_FILES})
endif()

# ── BNO08x (I2C/SPI IMU — PORTABLE, vendor C sources, Style A) ───────────
if(HF_CORE_ENABLE_BNO08X)
    include("${HF_CORE_DRIVER_EXT}/hf-bno08x-driver/cmake/hf_bno08x_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/bno08x/Bno08xHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_BNO08X_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_BNO08X_SOURCE_FILES})
endif()

# ── MAX22200 (SPI solenoid driver — PORTABLE, header-only, Style A) ──────
if(HF_CORE_ENABLE_MAX22200)
    include("${HF_CORE_DRIVER_EXT}/hf-max22200-driver/cmake/hf_max22200_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/max22200/Max22200Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_MAX22200_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_MAX22200_SOURCE_FILES})
endif()

# ── NTC Thermistor (ADC — PORTABLE, has lookup table sources, Style B) ───
if(HF_CORE_ENABLE_NTC_THERMISTOR)
    set(HF_NTC_THERMISTOR_ROOT "${HF_CORE_DRIVER_EXT}/hf-ntc-thermistor-driver")
    include("${HF_NTC_THERMISTOR_ROOT}/cmake/hf_ntc_thermistor_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/ntc/NtcTemperatureHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_NTC_THERMISTOR_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_NTC_THERMISTOR_SOURCES})
endif()

# ── PCA9685 (I2C PWM driver — PORTABLE, header-only with .ipp, Style A) ──
if(HF_CORE_ENABLE_PCA9685)
    include("${HF_CORE_DRIVER_EXT}/hf-pca9685-driver/cmake/hf_pca9685_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/pca9685/Pca9685Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_PCA9685_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_PCA9685_SOURCE_FILES})
endif()

# ── PCAL95555 (I2C GPIO expander — PORTABLE, header-only, Style A) ───────
if(HF_CORE_ENABLE_PCAL95555)
    include("${HF_CORE_DRIVER_EXT}/hf-pcal95555-driver/cmake/hf_pcal95555_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/pcal95555/Pcal95555Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_PCAL95555_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_PCAL95555_SOURCE_FILES})
endif()

# ── TLE92466ED (SPI solenoid driver — PORTABLE, header-only, Style A) ────
if(HF_CORE_ENABLE_TLE92466ED)
    include("${HF_CORE_DRIVER_EXT}/hf-tle92466ed-driver/cmake/hf_tle92466ed_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/tle92466ed/Tle92466edHandler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_TLE92466ED_PUBLIC_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_TLE92466ED_SOURCE_FILES})
endif()

# ── TMC5160 (SPI/UART stepper — PORTABLE, header-only, Style A) ──────────
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

# ── TMC9660 (SPI/UART BLDC — PORTABLE, has bootloader, Style B) ──────────
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

# ── WS2812 (RMT LED — MCU-GATED:ESP32, mixed C/C++, Style B) ─────────────
# Requires ESP32 RMT peripheral. Gated by MCU compatibility check above.
if(HF_CORE_ENABLE_WS2812)
    set(HF_WS2812_RMT_ROOT "${HF_CORE_DRIVER_EXT}/hf-ws2812-rmt-driver")
    include("${HF_WS2812_RMT_ROOT}/cmake/hf_ws2812_rmt_build_settings.cmake")
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/ws2812/Ws2812Handler.cpp")
    list(APPEND HF_CORE_EXT_DRIVER_INCLUDE_DIRS ${HF_WS2812_RMT_INCLUDE_DIRS})
    list(APPEND HF_CORE_EXT_DRIVER_SOURCES      ${HF_WS2812_RMT_SOURCES})
endif()

# ── Logger Handler (hardware-agnostic via ConsolePort abstraction) ────────
if(HF_CORE_ENABLE_LOGGER)
    list(APPEND HF_CORE_HANDLER_SOURCES
        "${HF_CORE_HANDLER_ROOT}/logger/Logger.cpp")
endif()

# ===========================================================================
# ██  INCLUDE DIRECTORIES — MCU-aware path selection
# ===========================================================================

# ── Foundation Include Dirs (always included) ─────────────────────────────
set(HF_CORE_INCLUDE_DIRS
    # Parent of core/ — allows #include "core/handlers/xxx/Xxx.h" paths
    "${HF_CORE_ROOT}/.."
    # Core root — allows #include "handlers/xxx/Xxx.h" paths
    "${HF_CORE_ROOT}"
    # Handler common utilities
    "${HF_CORE_HANDLER_ROOT}/common"
    # Internal interface — base abstract classes (hardware-agnostic)
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/base"
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/utils"
    # Internal interface — MCU-selected implementations
    "${HF_CORE_DRIVER_INT}/hf-internal-interface-wrap/inc/mcu"
    "${_HF_MCU_INC}"
    "${_HF_MCU_INC}/utils"
    # Pin configuration
    "${HF_CORE_DRIVER_INT}/hf-pincfg"
    "${HF_CORE_DRIVER_INT}/hf-pincfg/src"
    # General utilities (hardware-agnostic)
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
# ██  BUILD-SYSTEM REQUIREMENTS — MCU-dependent
# ===========================================================================
#
# These are the build-system component dependencies required by the selected
# platform. For ESP-IDF builds these become idf_component_register(REQUIRES).
# For other build systems (e.g. STM32 HAL + CMake) this section would
# export different variables.
# ===========================================================================

if(HF_CORE_MCU STREQUAL "ESP32")
    # ── ESP-IDF Component Requirements ────────────────────────────────────
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

    # ── Conditional Requirements ──────────────────────────────────────────
    if(HF_CORE_ENABLE_UART)
        list(APPEND HF_CORE_IDF_REQUIRES esp_driver_uart)
    endif()
    if(HF_CORE_ENABLE_CAN)
        list(APPEND HF_CORE_IDF_REQUIRES esp_driver_twai)
    endif()
    if(HF_CORE_ENABLE_ADC)
        list(APPEND HF_CORE_IDF_REQUIRES esp_adc)
    endif()
    if(HF_CORE_ENABLE_RMT)
        list(APPEND HF_CORE_IDF_REQUIRES esp_driver_rmt)
    endif()

    # Deduplicate
    list(REMOVE_DUPLICATES HF_CORE_IDF_REQUIRES)

# elseif(HF_CORE_MCU STREQUAL "STM32")
#     # STM32 HAL — no IDF requires, but maybe CMSIS/HAL paths
#     set(HF_CORE_STM32_HAL_MODULES GPIO SPI I2C ADC TIM)
#     if(HF_CORE_ENABLE_UART)
#         list(APPEND HF_CORE_STM32_HAL_MODULES USART)
#     endif()
#     if(HF_CORE_ENABLE_CAN)
#         list(APPEND HF_CORE_STM32_HAL_MODULES FDCAN)
#     endif()
endif()

# ===========================================================================
# ██  COMPILE DEFINITIONS — Platform + Feature flags
# ===========================================================================

set(HF_CORE_COMPILE_DEFINITIONS "")

# ── MCU Family Definition ─────────────────────────────────────────────────
# Exactly one HF_MCU_FAMILY_* is defined, allowing #ifdef guards in code.
if(HF_CORE_MCU STREQUAL "ESP32")
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HF_MCU_FAMILY_ESP32=1)
# elseif(HF_CORE_MCU STREQUAL "STM32")
#     list(APPEND HF_CORE_COMPILE_DEFINITIONS HF_MCU_FAMILY_STM32=1)
endif()

# ── RTOS Definition ───────────────────────────────────────────────────────
if(HF_CORE_RTOS STREQUAL "FREERTOS")
    list(APPEND HF_CORE_COMPILE_DEFINITIONS HF_RTOS_FREERTOS=1)
# elseif(HF_CORE_RTOS STREQUAL "THREADX")
#     list(APPEND HF_CORE_COMPILE_DEFINITIONS HF_RTOS_THREADX=1)
endif()

list(APPEND HF_CORE_COMPILE_DEFINITIONS HF_THREAD_SAFE=1)

# ── Feature Definitions ───────────────────────────────────────────────────
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
message(STATUS "  MCU:           ${HF_CORE_MCU}")
message(STATUS "  RTOS:          ${HF_CORE_RTOS}")
message(STATUS "  Features:      ${_hf_enabled_features}")
message(STATUS "  Interfaces:    SPI I2C GPIO ADC NVS Timer${_hf_enabled_ifaces}")
message(STATUS "  Sources:       ${_hf_src_count} total (${_hf_handler_count} handlers, ${_hf_driver_count} driver)")
message(STATUS "  Include dirs:  ${_hf_inc_count}")
if(HF_CORE_MCU STREQUAL "ESP32")
    message(STATUS "  IDF requires:  ${HF_CORE_IDF_REQUIRES}")
endif()
message(STATUS "")
