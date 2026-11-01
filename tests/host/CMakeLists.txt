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
