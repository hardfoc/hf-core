# ===========================================================================
# hf-core host build (Linux / macOS) — libraries, mocks and unit tests
# ===========================================================================
#
# This file is only the host entry point. On-target builds do not use it:
# ESP-IDF projects include cmake/hf_core_build_settings.cmake from their own
# component (see examples/esp32/components/hf_core), and HAL repositories do
# the same from their CMake.
#
# The host build selects HF_CORE_MCU=NONE and HF_CORE_RTOS=NONE, so no
# Esp*/Stm* interface implementation and no RTOS kernel is compiled. What is
# left is the MCU-agnostic part of hf-core: handlers/, the external chip
# drivers, hf-core-utils and the Base* interfaces. See docs/host-build.md.
#
#   cmake --preset host-debug
#   cmake --build --preset host-debug
#   ctest --preset host-debug
#
# Targets:
#   hf_core_deps        submodule sources (drivers, utils, rtos-wrap host stub)
#   hf_core             hf-core's own handlers, built with -Wall -Wextra -Werror
#   hf_core_host_clock  FakeClock behind the no-RTOS time hooks (tests/host/mocks)
#   hf_core_host_mocks  scriptable Base* fakes for unit tests (tests/host/mocks)
#   hf_core_host_test_main  HF_TEST / CHECK harness and main() (tests/host/support)
#
# SPDX-License-Identifier: GPL-3.0-or-later
# ===========================================================================

cmake_minimum_required(VERSION 3.22)
project(hf_core LANGUAGES C CXX)

if(CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR
        "[hf-core] The top-level CMakeLists.txt is the host build. Cross builds "
        "include cmake/hf_core_build_settings.cmake from their own project.")
endif()

# Tests default on for a standalone checkout and off when a HAL pulls hf-core
# in with add_subdirectory(); the mocks are built either way.
option(HF_CORE_BUILD_TESTS "Build the host unit tests under tests/host" ${PROJECT_IS_TOP_LEVEL})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ── Platform: no MCU, no RTOS ─────────────────────────────────────────────
set(HF_CORE_MCU  "NONE")
set(HF_CORE_RTOS "NONE")

# ── Features: every MCU-agnostic handler and driver ───────────────────────
# WS2812 needs the ESP32 RMT peripheral. Dynamixel and CANopenNode need
# third-party checkouts that live outside hf-core; they stay off here.
foreach(_feature
        AS5047U ADS7952 ADS9324 AT25040 BNO08X MAX22200 NTC_THERMISTOR
        MCP9700 ALICAT_BASIS2 FDO2 SFM BMP581 SE050 PCA9685 PF1550
        PCAL95555 TLE92466ED TMC5160 TMC9660 ISF15ACP4 VORTEX_DRIVER
        LOGGER UART CAN UTILS_CANOPEN)
    if(NOT DEFINED HF_CORE_ENABLE_${_feature})
        set(HF_CORE_ENABLE_${_feature} ON)
    endif()
endforeach()
foreach(_feature WS2812 DYNAMIXEL CANOPENNODE USB_SERIAL_JTAG PWM PIO)
    if(NOT DEFINED HF_CORE_ENABLE_${_feature})
        set(HF_CORE_ENABLE_${_feature} OFF)
    endif()
endforeach()

include(cmake/hf_core_build_settings.cmake)

# ── Warning policy ────────────────────────────────────────────────────────
# hf-core's own sources (handlers/, tests/host/) build warning-clean.
# Submodule sources are separate repositories with their own CI; their
# headers are included as SYSTEM so their warnings do not fail this build.
set(HF_CORE_STRICT_WARNINGS -Wall -Wextra -Werror)

# Sources include "core/handlers/...", "core/hf-core-drivers/..." and
# "core/hf-core-utils/...". hf_core_build_settings.cmake resolves all of
# them through one "core" root, which would make submodule headers look
# like hf-core's own. Split that root in two: "core/handlers" through a
# normal include directory, the submodule trees through a SYSTEM one.
get_filename_component(_hf_root "${HF_CORE_ROOT}" REALPATH)
set(_hf_own_root "${CMAKE_CURRENT_BINARY_DIR}/hf_core_own_root")
set(_hf_dep_root "${CMAKE_CURRENT_BINARY_DIR}/hf_core_dep_root")
foreach(_link
        "${_hf_own_root}/core/handlers|${_hf_root}/handlers"
        "${_hf_dep_root}/core/hf-core-drivers|${_hf_root}/hf-core-drivers"
        "${_hf_dep_root}/core/hf-core-utils|${_hf_root}/hf-core-utils")
    string(REPLACE "|" ";" _pair "${_link}")
    list(GET _pair 0 _from)
    list(GET _pair 1 _to)
    if(NOT EXISTS "${_from}")
        get_filename_component(_from_dir "${_from}" DIRECTORY)
        file(MAKE_DIRECTORY "${_from_dir}")
        file(CREATE_LINK "${_to}" "${_from}" SYMBOLIC)
    endif()
endforeach()

# Include directories that belong to hf-core itself, not to a submodule.
set(HF_CORE_OWN_INCLUDE_DIRS "${_hf_own_root}" "${_hf_root}")
set(HF_CORE_DEP_INCLUDE_DIRS "${_hf_dep_root}")
get_filename_component(_hf_parent_inc "${HF_CORE_PARENT_INCLUDE_DIR}" REALPATH)
foreach(_dir IN LISTS HF_CORE_INCLUDE_DIRS)
    get_filename_component(_abs "${_dir}" REALPATH)
    string(FIND "${_abs}/" "${_hf_root}/handlers/" _pos)
    if(_abs STREQUAL _hf_parent_inc OR _abs STREQUAL _hf_root)
        continue()
    elseif(_pos EQUAL 0)
        list(APPEND HF_CORE_OWN_INCLUDE_DIRS "${_abs}")
    else()
        list(APPEND HF_CORE_DEP_INCLUDE_DIRS "${_abs}")
    endif()
endforeach()
list(REMOVE_DUPLICATES HF_CORE_OWN_INCLUDE_DIRS)
list(REMOVE_DUPLICATES HF_CORE_DEP_INCLUDE_DIRS)

# ── hf_core_deps: submodule sources ───────────────────────────────────────
set(_hf_dep_sources
    ${HF_CORE_INTERFACE_SOURCES}
    ${HF_CORE_RTOS_SOURCES}
    ${HF_CORE_GENERAL_UTILS_SOURCES}
    ${HF_CORE_CANOPEN_SOURCES}
    ${HF_CORE_EXT_DRIVER_SOURCES}
    # BaseThread on the host: the common members, plus the four members that
    # need a scheduler, replaced by BaseThreadHostStub.cpp (Setup() runs
    # inline; the test drives Step()).
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/BaseThread.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/SignalSemaphore.cpp"
    "${HF_CORE_UTILS_ROOT}/hf-utils-rtos-wrap/src/BaseThreadHostStub.cpp"
)
add_library(hf_core_deps STATIC ${_hf_dep_sources})
target_include_directories(hf_core_deps SYSTEM PUBLIC ${HF_CORE_DEP_INCLUDE_DIRS})
target_include_directories(hf_core_deps PUBLIC ${HF_CORE_OWN_INCLUDE_DIRS})
target_compile_definitions(hf_core_deps PUBLIC
    ${HF_CORE_COMPILE_DEFINITIONS}
    # Time on the host comes from FakeClock (tests/host/mocks), not a tick.
    HF_RTOS_NONE_EXTERNAL_CLOCK=1
)
target_compile_options(hf_core_deps PRIVATE -w)
# hf_os_none_clock_us() / hf_os_none_delay_us() live in hf_core_host_clock.
target_link_libraries(hf_core_deps PUBLIC hf_core_host_clock)

# ── hf_core: hf-core's own handlers ───────────────────────────────────────
add_library(hf_core STATIC
    ${HF_CORE_HANDLER_SOURCES}
    "${_hf_root}/handlers/logger/factory/NoBackendLoggerFactory.cpp"
)
target_link_libraries(hf_core PUBLIC hf_core_deps)
target_compile_options(hf_core PRIVATE ${HF_CORE_STRICT_WARNINGS})

# ── Host fakes and test harness (always built: board HALs link them) ─────
add_subdirectory(tests/host/mocks)
add_subdirectory(tests/host/support)

if(HF_CORE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/host)
endif()
