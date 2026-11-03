# hf-core host unit tests. One executable per file, one ctest entry each.

function(hf_core_host_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE hf_core_host_test_main hf_core_host_mocks)
    target_compile_options(${name} PRIVATE ${HF_CORE_STRICT_WARNINGS})
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES TIMEOUT 60)
endfunction()

hf_core_host_test(test_mocks            test_mocks.cpp)
hf_core_host_test(test_rtos_host        test_rtos_host.cpp)
hf_core_host_test(test_utils_general    test_utils_general.cpp)
hf_core_host_test(test_pcal95555        test_pcal95555.cpp)
hf_core_host_test(test_bno08x           test_bno08x.cpp)
hf_core_host_test(test_as5047u          test_as5047u.cpp)
hf_core_host_test(test_tmc9660          test_tmc9660.cpp)
hf_core_host_test(test_ntc              test_ntc.cpp)
hf_core_host_test(test_fdo2             test_fdo2.cpp)
hf_core_host_test(test_pca9685          test_pca9685.cpp)
hf_core_host_test(test_pf1550           test_pf1550.cpp)

# ── CANopen driver and host ports (CO_driver_hardfoc.c, PipePort, ...) ──────
# CANopenNode itself lives outside hf-core (a HAL's third_party/), so the
# tests fetch the revision the HALs pin unless HF_CORE_CANOPENNODE_ROOT
# points at a checkout. Only its headers are needed: the tests drive the
# driver directly, without an object dictionary.
#
# HF_CORE_CANOPEN_SANITIZE=thread|address builds these tests (and the driver)
# with that sanitizer; CI runs the thread build for the loopback test.
set(HF_CORE_CANOPEN_SANITIZE "" CACHE STRING "Sanitizer for the CANopen host tests (thread, address or empty)")
if(NOT HF_CORE_CANOPENNODE_ROOT OR NOT EXISTS "${HF_CORE_CANOPENNODE_ROOT}/301/CO_driver.h")
    include(FetchContent)
    FetchContent_Declare(canopennode
        GIT_REPOSITORY https://github.com/CANopenNode/CANopenNode.git
        GIT_TAG        ac2140717c3c498d9b0351bce052bab630a74764  # v4, as pinned by hf-hal-vortex-v1
        GIT_SHALLOW    FALSE)
    FetchContent_GetProperties(canopennode)
    if(NOT canopennode_POPULATED)
        FetchContent_Populate(canopennode)
    endif()
    set(HF_CORE_CANOPENNODE_ROOT "${canopennode_SOURCE_DIR}")
endif()

set(HF_CORE_CANOPENNODE_OD_DIR "")
set(HF_CORE_CANOPENNODE_CONFIG_DIR "")
set(HF_CORE_CANOPENNODE_PLATFORM "POSIX")
set(HF_CORE_CANOPENNODE_RTOS "POSIX")
include("${HF_CORE_ROOT}/cmake/hf_core_canopennode.cmake")

find_package(Threads REQUIRED)
set(_hf_co_sanitize "")
if(HF_CORE_CANOPEN_SANITIZE)
    set(_hf_co_sanitize "-fsanitize=${HF_CORE_CANOPEN_SANITIZE}" -fno-omit-frame-pointer)
endif()

add_library(hf_core_canopen_host STATIC
    ${HF_CORE_CANOPENNODE_DRIVER_SOURCES}
    ${HF_CORE_CANOPENNODE_PORT_SOURCES})
target_include_directories(hf_core_canopen_host PUBLIC ${HF_CORE_CANOPENNODE_INCLUDE_DIRS})
target_compile_definitions(hf_core_canopen_host PUBLIC ${HF_CORE_CANOPENNODE_DEFINITIONS})
target_compile_options(hf_core_canopen_host PRIVATE ${HF_CORE_STRICT_WARNINGS} PUBLIC ${_hf_co_sanitize})
target_link_options(hf_core_canopen_host PUBLIC ${_hf_co_sanitize})
target_link_libraries(hf_core_canopen_host PUBLIC Threads::Threads)

function(hf_core_canopen_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE hf_core_host_test_main hf_core_canopen_host)
    target_compile_options(${name} PRIVATE ${HF_CORE_STRICT_WARNINGS})
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES TIMEOUT 300 LABELS canopen)
endfunction()

hf_core_canopen_test(test_co_driver       test_co_driver.cpp)
hf_core_canopen_test(test_frame_record    test_frame_record.cpp)
hf_core_canopen_test(test_pipe_loopback   test_pipe_loopback.cpp)
