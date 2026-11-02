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
