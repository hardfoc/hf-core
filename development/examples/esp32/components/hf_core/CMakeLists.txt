# ===========================================================================
# hf_core ESP-IDF Component Wrapper
# ===========================================================================
#
# Wraps the entire hf-core platform (handlers + driver sources + utils) as
# a single ESP-IDF component so that example/test applications can depend
# on it with a simple REQUIRES hf_core.
#
# This wrapper enables ALL features — core examples test every handler.
# HAL-level CMakeLists.txt files enable only the subset they need.
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2025-2026 HardFOC Team
# ===========================================================================

cmake_minimum_required(VERSION 3.16)

# ===========================================================================
# Enable ALL features — core examples test every driver/handler
# ===========================================================================
set(HF_CORE_ENABLE_AS5047U        ON)
set(HF_CORE_ENABLE_BNO08X         ON)
set(HF_CORE_ENABLE_MAX22200       ON)
set(HF_CORE_ENABLE_NTC_THERMISTOR ON)
set(HF_CORE_ENABLE_PCA9685        ON)
set(HF_CORE_ENABLE_PCAL95555      ON)
set(HF_CORE_ENABLE_TLE92466ED     ON)
set(HF_CORE_ENABLE_TMC5160        ON)
set(HF_CORE_ENABLE_TMC9660        ON)
set(HF_CORE_ENABLE_WS2812         ON)
set(HF_CORE_ENABLE_LOGGER         ON)
# Optional utilities
set(HF_CORE_ENABLE_UTILS_CANOPEN  ON)
# Optional interfaces (auto-enabled by drivers, but explicit for clarity)
set(HF_CORE_ENABLE_UART           ON)
set(HF_CORE_ENABLE_CAN            ON)
set(HF_CORE_ENABLE_PWM            ON)
set(HF_CORE_ENABLE_PIO            ON)

# ===========================================================================
# Include the shared build settings (collects sources, includes, IDF deps)
# ===========================================================================
include("${CMAKE_CURRENT_SOURCE_DIR}/../../../../cmake/hf_core_build_settings.cmake")

# ── Register component using collected build settings ─────────────────────
idf_component_register(
    SRCS
        ${HF_CORE_SOURCES}
    INCLUDE_DIRS
        ${HF_CORE_INCLUDE_DIRS}
    REQUIRES
        ${HF_CORE_IDF_REQUIRES}
)

# ── C++20 for all component sources ───────────────────────────────────────────
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

# ── Apply feature-based compile definitions from build settings ───────────
target_compile_definitions(${COMPONENT_LIB} PUBLIC ${HF_CORE_COMPILE_DEFINITIONS})

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

# ── MCU target definitions based on ESP-IDF target ───────────────────────
if(CONFIG_IDF_TARGET_ESP32)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32=1
        HF_MCU_ESP32=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32=1
    )
elseif(CONFIG_IDF_TARGET_ESP32S3)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32S3=1
        HF_MCU_ESP32S3=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32S3=1
    )
elseif(CONFIG_IDF_TARGET_ESP32C6)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32C6=1
        HF_MCU_ESP32C6=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32C6=1
    )
elseif(CONFIG_IDF_TARGET_ESP32S2)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32S2=1
        HF_MCU_ESP32S2=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32S2=1
    )
elseif(CONFIG_IDF_TARGET_ESP32C3)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32C3=1
        HF_MCU_ESP32C3=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32C3=1
    )
elseif(CONFIG_IDF_TARGET_ESP32H2)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_TARGET_MCU_ESP32H2=1
        HF_MCU_ESP32H2=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
        ESP32H2=1
    )
else()
    target_compile_definitions(${COMPONENT_LIB} PUBLIC
        HF_MCU_ESP32=1
        HF_MCU_FAMILY_ESP32=1
        HF_THREAD_SAFE=1
    )
endif()

# ── Suppress warnings in third-party/generated code ──────────────────────
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-volatile
)
