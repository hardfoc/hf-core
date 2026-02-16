# ===========================================================================
# main/CMakeLists.txt — APP_TYPE-driven source selection for hf-core tests
# ===========================================================================
#
# This CMakeLists supports a multi-directory layout where each test category
# has its own subdirectory under main/:
#
#   main/
#   ├── handler_tests/     10 handler comprehensive tests
#   ├── utils_tests/       General utils, RTOS wrappers, Logger, CANopen
#   └── integration_tests/ Full-system cross-component tests
#
# The APP_SOURCE_FILE env var (set by build_app.sh from app_config.yml)
# selects which .cpp file to compile.
#
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2025-2026 HardFOC Team
# ===========================================================================

# ── Phase 1: Requirements declaration (before project is fully resolved) ──
# During this phase, use safe defaults. The real values come from env vars
# set by build_app.sh.

# ── Phase 1: Source file resolution ───────────────────────────────────────
# build_app.sh sets both env vars AND -D CMake variables. Check all sources.

if(DEFINED ENV{APP_SOURCE_FILE} AND NOT "$ENV{APP_SOURCE_FILE}" STREQUAL "")
    # Priority 1: env var (set by build_app.sh export)
    set(ACTIVE_SOURCE_FILE "$ENV{APP_SOURCE_FILE}")
elseif(DEFINED APP_SOURCE_FILE AND NOT "${APP_SOURCE_FILE}" STREQUAL "")
    # Priority 2: CMake -D variable (set by build_app.sh -D flag)
    set(ACTIVE_SOURCE_FILE "${APP_SOURCE_FILE}")
elseif(DEFINED ENV{APP_TYPE} AND NOT "$ENV{APP_TYPE}" STREQUAL "")
    # Fallback: derive from APP_TYPE env var
    set(ACTIVE_SOURCE_FILE "$ENV{APP_TYPE}.cpp")
elseif(DEFINED APP_TYPE AND NOT "${APP_TYPE}" STREQUAL "")
    # Fallback: derive from APP_TYPE CMake var
    set(ACTIVE_SOURCE_FILE "${APP_TYPE}.cpp")
else()
    # Ultimate fallback for direct idf.py invocation
    set(ACTIVE_SOURCE_FILE "utils_tests/general_utils_comprehensive_test.cpp")
endif()

message(STATUS "── main/CMakeLists.txt ──")
message(STATUS "  APP_SOURCE_FILE: ${ACTIVE_SOURCE_FILE}")

# ── Include directories ───────────────────────────────────────────────────
# Paths relative to this CMakeLists.txt (examples/esp32/main/)
set(MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Handler headers — relative paths up to the handler sources
set(HANDLER_ROOT "${MAIN_DIR}/../../../handlers")
set(DRIVER_EXT_ROOT "${MAIN_DIR}/../../../hf-core-drivers/external")
set(DRIVER_INT_ROOT "${MAIN_DIR}/../../../hf-core-drivers/internal")
set(UTILS_GENERAL_ROOT "${MAIN_DIR}/../../../hf-core-utils/hf-utils-general")
set(UTILS_RTOS_ROOT "${MAIN_DIR}/../../../hf-core-utils/hf-utils-rtos-wrap")
set(UTILS_CANOPEN_ROOT "${MAIN_DIR}/../../../hf-core-utils/hf-utils-canopen")

# ── Source file registration ──────────────────────────────────────────────
# The hf_core component exports all driver/handler/utils include paths.
# main only needs "." for local test headers (TestFramework.h, esp32_test_config.hpp, etc.)
idf_component_register(
    SRCS "${ACTIVE_SOURCE_FILE}"
    INCLUDE_DIRS
        "."
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
        hf_core
)

# ── C++20 standard ────────────────────────────────────────────────────────────
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON)

# ── Build-type flags ──────────────────────────────────────────────────────
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${COMPONENT_LIB} PRIVATE -O0 -g3 -DHFCORE_DEBUG=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${COMPONENT_LIB} PRIVATE -O2 -DNDEBUG)
endif()

# ── Inject APP_TYPE define ────────────────────────────────────────────────
if(DEFINED APP_TYPE)
    string(TOUPPER "${APP_TYPE}" APP_TYPE_UPPER)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE
        "EXAMPLE_TYPE_${APP_TYPE_UPPER}=1"
    )
endif()

# ── Warnings ──────────────────────────────────────────────────────────────
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall -Wextra -Wpedantic
    -Wno-unused-parameter
    -Wno-missing-field-initializers
)
