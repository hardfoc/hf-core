# hf_core_host_test_main: the HF_TEST / CHECK harness (HostTest.h) and its
# main(). Built with the mocks, so a board HAL can write its host tests the
# same way hf-core does.

add_library(hf_core_host_test_main STATIC HostTestMain.cpp)
target_include_directories(hf_core_host_test_main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hf_core_host_test_main PUBLIC hf_core_host_clock)
target_compile_options(hf_core_host_test_main PRIVATE ${HF_CORE_STRICT_WARNINGS})
