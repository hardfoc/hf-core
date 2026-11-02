# ===========================================================================
# hf-core: CANopenNode stack + generic driver source lists
# ===========================================================================
#
# One place that lists what a CANopenNode build compiles, used by
# hf_core_build_settings.cmake (ESP-IDF / HAL builds) and by host builds that
# only want the CANopen pieces (hf-core tests, a product's host node binary).
#
# Inputs (set before include()):
#   HF_CORE_CANOPENNODE_ROOT        CANopenNode v4 checkout (CANopen.c at its root)
#   HF_CORE_CANOPENNODE_OD_DIR      directory with the application's OD.c / OD.h
#                                   (empty: driver-only build, no stack sources)
#   HF_CORE_CANOPENNODE_CONFIG_DIR  optional directory with hf_co_app_config.h
#                                   (CO_CONFIG_* stack configuration)
#   HF_CORE_CANOPENNODE_PLATFORM    ESP32 | POSIX | NONE   (port bindings to build)
#   HF_CORE_CANOPENNODE_RTOS        FREERTOS | POSIX | NONE (CO lock backend)
#   HF_CORE_CANOPENNODE_GATEWAY     ON to compile the CiA 309-3 ASCII gateway
#
# Outputs:
#   HF_CORE_CANOPENNODE_DRIVER_SOURCES  CO_driver_hardfoc.c
#   HF_CORE_CANOPENNODE_PORT_SOURCES    port bindings for the platform
#   HF_CORE_CANOPENNODE_STACK_SOURCES   CANopenNode modules + the application OD.c
#   HF_CORE_CANOPENNODE_INCLUDE_DIRS
#   HF_CORE_CANOPENNODE_DEFINITIONS
#
# The upstream example/OD.c and example/CO_storageBlank.c are never compiled:
# the OD comes from the application, and storage from its own backend.
#
# SPDX-License-Identifier: GPL-3.0-or-later
# ===========================================================================

set(_hf_co_dir "${CMAKE_CURRENT_LIST_DIR}/../handlers/common/canopen")
get_filename_component(_hf_co_dir "${_hf_co_dir}" ABSOLUTE)

if(NOT DEFINED HF_CORE_CANOPENNODE_PLATFORM)
    set(HF_CORE_CANOPENNODE_PLATFORM "NONE")
endif()
if(NOT DEFINED HF_CORE_CANOPENNODE_RTOS)
    set(HF_CORE_CANOPENNODE_RTOS "NONE")
endif()

set(HF_CORE_CANOPENNODE_DRIVER_SOURCES "${_hf_co_dir}/CO_driver_hardfoc.c")

set(HF_CORE_CANOPENNODE_PORT_SOURCES "")
if(HF_CORE_CANOPENNODE_PLATFORM STREQUAL "ESP32")
    list(APPEND HF_CORE_CANOPENNODE_PORT_SOURCES "${_hf_co_dir}/EspTwaiPort.cpp")
elseif(HF_CORE_CANOPENNODE_PLATFORM STREQUAL "POSIX")
    list(APPEND HF_CORE_CANOPENNODE_PORT_SOURCES "${_hf_co_dir}/PipePort.cpp")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        list(APPEND HF_CORE_CANOPENNODE_PORT_SOURCES "${_hf_co_dir}/LinuxSocketCanPort.cpp")
    endif()
endif()

set(HF_CORE_CANOPENNODE_STACK_SOURCES "")
if(NOT "${HF_CORE_CANOPENNODE_OD_DIR}" STREQUAL "")
    if(NOT EXISTS "${HF_CORE_CANOPENNODE_OD_DIR}/OD.c")
        message(FATAL_ERROR
            "[hf-core] HF_CORE_CANOPENNODE_OD_DIR=${HF_CORE_CANOPENNODE_OD_DIR} has no OD.c")
    endif()
    set(_r "${HF_CORE_CANOPENNODE_ROOT}")
    set(HF_CORE_CANOPENNODE_STACK_SOURCES
        "${_r}/301/CO_ODinterface.c"
        "${_r}/301/CO_NMT_Heartbeat.c"
        "${_r}/301/CO_HBconsumer.c"
        "${_r}/301/CO_Node_Guarding.c"
        "${_r}/301/CO_Emergency.c"
        "${_r}/301/CO_SDOserver.c"
        "${_r}/301/CO_SDOclient.c"
        "${_r}/301/CO_TIME.c"
        "${_r}/301/CO_SYNC.c"
        "${_r}/301/CO_PDO.c"
        "${_r}/301/CO_fifo.c"
        "${_r}/301/crc16-ccitt.c"
        "${_r}/303/CO_LEDs.c"
        "${_r}/305/CO_LSSslave.c"
        "${_r}/storage/CO_storage.c"
        "${_r}/CANopen.c"
        "${HF_CORE_CANOPENNODE_OD_DIR}/OD.c"
    )
    if(HF_CORE_CANOPENNODE_GATEWAY)
        list(APPEND HF_CORE_CANOPENNODE_STACK_SOURCES "${_r}/309/CO_gateway_ascii.c")
    endif()
endif()

# This directory first: its CO_driver_target.h is the one the stack includes.
set(HF_CORE_CANOPENNODE_INCLUDE_DIRS "${_hf_co_dir}")
if(NOT "${HF_CORE_CANOPENNODE_CONFIG_DIR}" STREQUAL "")
    list(APPEND HF_CORE_CANOPENNODE_INCLUDE_DIRS "${HF_CORE_CANOPENNODE_CONFIG_DIR}")
endif()
if(NOT "${HF_CORE_CANOPENNODE_OD_DIR}" STREQUAL "")
    list(APPEND HF_CORE_CANOPENNODE_INCLUDE_DIRS "${HF_CORE_CANOPENNODE_OD_DIR}")
endif()
list(APPEND HF_CORE_CANOPENNODE_INCLUDE_DIRS "${HF_CORE_CANOPENNODE_ROOT}")

set(HF_CORE_CANOPENNODE_DEFINITIONS "")
if(HF_CORE_CANOPENNODE_RTOS STREQUAL "FREERTOS")
    list(APPEND HF_CORE_CANOPENNODE_DEFINITIONS HF_CO_DRIVER_FREERTOS=1)
endif()
