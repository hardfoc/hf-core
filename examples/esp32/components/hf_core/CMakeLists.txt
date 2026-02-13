# ===========================================================================
# hf_core ESP-IDF Component Wrapper
# ===========================================================================
#
# Wraps the entire hf-core platform (handlers + driver sources + utils) as
# a single ESP-IDF component so that example/test applications can depend
# on it with a simple REQUIRES hf_core.
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2025-2026 HardFOC Team
# ===========================================================================

cmake_minimum_required(VERSION 3.16)

# ── Path roots ────────────────────────────────────────────────────────────
set(CORE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set(LIB_ROOT "${CORE_ROOT}/..") # Parent of core/ — resolves "core/..." includes
set(HANDLER_ROOT "${CORE_ROOT}/handlers")
set(DRIVER_EXT "${CORE_ROOT}/hf-core-drivers/external")
set(DRIVER_INT "${CORE_ROOT}/hf-core-drivers/internal")
set(UTILS_ROOT "${CORE_ROOT}/hf-core-utils")

# ── Collect handler sources ───────────────────────────────────────────────
file(GLOB HANDLER_SRCS
    "${HANDLER_ROOT}/as5047u/*.cpp"
    "${HANDLER_ROOT}/bno08x/*.cpp"
    "${HANDLER_ROOT}/max22200/*.cpp"
    "${HANDLER_ROOT}/pca9685/*.cpp"
    "${HANDLER_ROOT}/pcal95555/*.cpp"
    "${HANDLER_ROOT}/ntc/*.cpp"
    "${HANDLER_ROOT}/tle92466ed/*.cpp"
    "${HANDLER_ROOT}/tmc5160/*.cpp"
    "${HANDLER_ROOT}/tmc9660/*.cpp"
    "${HANDLER_ROOT}/ws2812/*.cpp"
    "${HANDLER_ROOT}/logger/*.cpp"
)

# ── Collect driver implementation sources (non-header-only parts) ─────────
# BNO08x — SH2 vendor library + DFU firmware (C sources)
file(GLOB_RECURSE BNO08X_SRCS
    "${DRIVER_EXT}/hf-bno08x-driver/src/*.c"
    "${DRIVER_EXT}/hf-bno08x-driver/src/*.cpp"
)

# NTC thermistor — lookup table + conversion (C++ sources)
file(GLOB NTC_DRIVER_SRCS
    "${DRIVER_EXT}/hf-ntc-thermistor-driver/src/*.cpp"
    "${DRIVER_EXT}/hf-ntc-thermistor-driver/src/*.c"
)

# TMC5160 — register definitions (C++ source)
# Note: tmc51x0_register_defs.cpp excluded — has duplicate case values
# from X-macro expansion (IOIN/OUTPUT share address 0x04)
file(GLOB TMC5160_SRCS
    "${DRIVER_EXT}/hf-tmc5160-driver/src/*.cpp"
    "${DRIVER_EXT}/hf-tmc5160-driver/src/*.c"
)
list(REMOVE_ITEM TMC5160_SRCS "${DRIVER_EXT}/hf-tmc5160-driver/src/tmc51x0_register_defs.cpp")

# TMC9660 — bootloader (C++ source in subdirectory)
file(GLOB_RECURSE TMC9660_SRCS
    "${DRIVER_EXT}/hf-tmc9660-driver/src/*.cpp"
    "${DRIVER_EXT}/hf-tmc9660-driver/src/*.c"
)

# WS2812 — LED strip encoder + RMT wrapper + effects (C/C++ sources)
file(GLOB WS2812_SRCS
    "${DRIVER_EXT}/hf-ws2812-rmt-driver/src/*.cpp"
    "${DRIVER_EXT}/hf-ws2812-rmt-driver/src/*.c"
)

set(DRIVER_EXT_SRCS
    ${BNO08X_SRCS}
    ${NTC_DRIVER_SRCS}
    ${TMC5160_SRCS}
    ${TMC9660_SRCS}
    ${WS2812_SRCS}
)

# ── Collect utils sources ─────────────────────────────────────────────────
file(GLOB UTILS_GENERAL_SRCS
    "${UTILS_ROOT}/hf-utils-general/src/*.cpp"
    "${UTILS_ROOT}/hf-utils-general/src/*.c"
)
file(GLOB UTILS_RTOS_SRCS
    "${UTILS_ROOT}/hf-utils-rtos-wrap/src/*.cpp"
    "${UTILS_ROOT}/hf-utils-rtos-wrap/src/*.c"
)
file(GLOB UTILS_CANOPEN_SRCS
    "${UTILS_ROOT}/hf-utils-canopen/src/*.cpp"
    "${UTILS_ROOT}/hf-utils-canopen/src/*.c"
)

# ── Collect ESP32 interface implementations ───────────────────────────────
file(GLOB ESP32_IMPL_SRCS
    "${DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/*.cpp"
    "${DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/*.c"
    "${DRIVER_INT}/hf-internal-interface-wrap/src/utils/*.cpp"
)
# Exclude EspBluetooth — requires esp_bt component not needed for handler tests
list(REMOVE_ITEM ESP32_IMPL_SRCS "${DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspBluetooth.cpp")
# Exclude EspWifi — requires esp_event/esp_wifi components not needed for handler tests
list(REMOVE_ITEM ESP32_IMPL_SRCS "${DRIVER_INT}/hf-internal-interface-wrap/src/mcu/esp32/EspWifi.cpp")

# ── Register component ────────────────────────────────────────────────────
idf_component_register(
    SRCS
        ${HANDLER_SRCS}
        ${DRIVER_EXT_SRCS}
        ${UTILS_GENERAL_SRCS}
        ${UTILS_RTOS_SRCS}
        ${UTILS_CANOPEN_SRCS}
        ${ESP32_IMPL_SRCS}
    INCLUDE_DIRS
        # Parent of core/ — allows handlers to use #include "core/..." paths
        "${LIB_ROOT}"
        # Core root — allows #include "handlers/xxx/Xxx.h" paths
        "${CORE_ROOT}"
        # Handler includes (each handler directory + parent for includes like "handlers/xxx/Xxx.h")
        "${HANDLER_ROOT}/as5047u"
        "${HANDLER_ROOT}/bno08x"
        "${HANDLER_ROOT}/max22200"
        "${HANDLER_ROOT}/pca9685"
        "${HANDLER_ROOT}/pcal95555"
        "${HANDLER_ROOT}/ntc"
        "${HANDLER_ROOT}/tle92466ed"
        "${HANDLER_ROOT}/tmc5160"
        "${HANDLER_ROOT}/tmc9660"
        "${HANDLER_ROOT}/ws2812"
        "${HANDLER_ROOT}/logger"
        # Internal interface — base abstract classes
        "${DRIVER_INT}/hf-internal-interface-wrap/inc"
        "${DRIVER_INT}/hf-internal-interface-wrap/inc/base"
        "${DRIVER_INT}/hf-internal-interface-wrap/inc/utils"
        # Internal interface — ESP32 MCU implementations
        "${DRIVER_INT}/hf-internal-interface-wrap/inc/mcu/esp32"
        "${DRIVER_INT}/hf-internal-interface-wrap/inc/mcu/esp32/utils"
        # Pin configuration (headers live in src/)
        "${DRIVER_INT}/hf-pincfg"
        "${DRIVER_INT}/hf-pincfg/src"
        # ── External driver includes ──────────────────────────────────────
        # AS5047U (header-only driver)
        "${DRIVER_EXT}/hf-as5047u-driver/inc"
        # BNO08x (SH2 vendor headers in src/)
        "${DRIVER_EXT}/hf-bno08x-driver/inc"
        "${DRIVER_EXT}/hf-bno08x-driver/src"
        "${DRIVER_EXT}/hf-bno08x-driver/src/sh2"
        "${DRIVER_EXT}/hf-bno08x-driver/src/dfu"
        # MAX22200 (header-only driver)
        "${DRIVER_EXT}/hf-max22200-driver/inc"
        # NTC thermistor
        "${DRIVER_EXT}/hf-ntc-thermistor-driver/inc"
        # PCA9685 (header-only driver + .ipp in src/)
        "${DRIVER_EXT}/hf-pca9685-driver/inc"
        # PCAL95555 (header-only driver + .ipp in src/)
        "${DRIVER_EXT}/hf-pcal95555-driver/inc"
        # TLE92466ED (header-only driver)
        "${DRIVER_EXT}/hf-tle92466ed-driver/inc"
        # TMC5160 (has sub-directories for features + registers)
        "${DRIVER_EXT}/hf-tmc5160-driver/inc"
        "${DRIVER_EXT}/hf-tmc5160-driver/inc/features"
        "${DRIVER_EXT}/hf-tmc5160-driver/inc/registers"
        # TMC9660 (has sub-directories for bootloader, parameter_mode, register_mode)
        "${DRIVER_EXT}/hf-tmc9660-driver/inc"
        "${DRIVER_EXT}/hf-tmc9660-driver/inc/bootloader"
        "${DRIVER_EXT}/hf-tmc9660-driver/inc/parameter_mode"
        "${DRIVER_EXT}/hf-tmc9660-driver/inc/register_mode"
        # WS2812 LED strip
        "${DRIVER_EXT}/hf-ws2812-rmt-driver/inc"
        # ── Utility includes ─────────────────────────────────────────────
        "${UTILS_ROOT}/hf-utils-general/include"
        "${UTILS_ROOT}/hf-utils-rtos-wrap/include"
        "${UTILS_ROOT}/hf-utils-canopen/include"
    REQUIRES
        driver
        esp_timer
        freertos
        nvs_flash
        esp_adc
        esp_driver_i2c
        esp_driver_spi
        esp_driver_gpio
        esp_driver_uart
        esp_driver_rmt
)

# ── C++23 for all component sources (required for std::expected in TLE driver) ─
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

# ── WS2812 Kconfig defaults (PUBLIC so consumers that include WS2812 headers get them) ─
target_compile_definitions(${COMPONENT_LIB} PUBLIC
    CONFIG_WS2812_NUM_LEDS=1
    CONFIG_WS2812_LED_TYPE_RGB=1
    CONFIG_WS2812_T0H=350
    CONFIG_WS2812_T1H=900
    CONFIG_WS2812_T0L=900
    CONFIG_WS2812_T1L=350
    CONFIG_WS2812_DEFAULT_BRIGHTNESS=25
    CONFIG_WS2812_LED_RMT_TX_GPIO=48
    CONFIG_WS2812_LED_RMT_TX_CHANNEL=0
)

# ── Suppress warnings in third-party/generated code ──────────────────────
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-volatile
)
