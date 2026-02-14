# ===========================================================================
# HardFOC Core (hf-core) — ESP32 Examples & Tests
# ===========================================================================
#
# Root CMakeLists.txt for the ESP32 example/test project.
# Uses the APP_TYPE mechanism to select which test application to build.
#
# Usage:
#   ./scripts/build_app.sh <app_type> <build_type> [idf_version]
#
# Example:
#   ./scripts/build_app.sh bno08x_handler_test Debug
#   ./scripts/build_app.sh general_utils_test Release
#   ./scripts/build_app.sh full_integration_test Debug
#
# ---------------------------------------------------------------------------
# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (c) 2025-2026 HardFOC Team
# ===========================================================================

cmake_minimum_required(VERSION 3.16)

# ── Default configuration ─────────────────────────────────────────────────
set(DEFAULT_APP_TYPE "general_utils_test")
set(DEFAULT_BUILD_TYPE "Release")
set(DEFAULT_SOURCE_FILE "utils_tests/general_utils_comprehensive_test.cpp")

# ── Resolve APP_TYPE ──────────────────────────────────────────────────────
if(DEFINED ENV{APP_TYPE} AND NOT "$ENV{APP_TYPE}" STREQUAL "")
    set(APP_TYPE "$ENV{APP_TYPE}")
elseif(NOT DEFINED APP_TYPE OR "${APP_TYPE}" STREQUAL "")
    set(APP_TYPE "${DEFAULT_APP_TYPE}")
endif()

# ── Resolve BUILD_TYPE ────────────────────────────────────────────────────
# build_app.sh passes -D BUILD_TYPE=xxx, so check that first
if(DEFINED ENV{BUILD_TYPE} AND NOT "$ENV{BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "$ENV{BUILD_TYPE}")
elseif(DEFINED BUILD_TYPE AND NOT "${BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "${BUILD_TYPE}")
elseif(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
endif()

# ── Validate configuration ────────────────────────────────────────────────
if("${APP_TYPE}" STREQUAL "")
    message(FATAL_ERROR
        "APP_TYPE is not set.\n"
        "Usage: ./scripts/build_app.sh <app_type> <build_type>\n"
        "  e.g. ./scripts/build_app.sh bno08x_handler_test Debug\n"
        "  e.g. ./scripts/build_app.sh general_utils_test Release\n"
        "Run './scripts/build_app.sh list' for available applications."
    )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(FATAL_ERROR
        "CMAKE_BUILD_TYPE is not set.\n"
        "Valid values: Debug, Release\n"
        "Usage: ./scripts/build_app.sh <app_type> <build_type>"
    )
endif()

message(STATUS "╔══════════════════════════════════════════════════════╗")
message(STATUS "║  HardFOC Core — ESP32 Test Build                    ║")
message(STATUS "╠══════════════════════════════════════════════════════╣")
message(STATUS "║  APP_TYPE:    ${APP_TYPE}")
message(STATUS "║  BUILD_TYPE:  ${CMAKE_BUILD_TYPE}")
message(STATUS "╚══════════════════════════════════════════════════════╝")

# ── Include ESP-IDF build system ──────────────────────────────────────────
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_hf_core_${APP_TYPE}_app)
