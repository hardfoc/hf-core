# The reference handler: what tools/new_handler.py generates for a real driver, built and tested on the
# host against hf_core_host_mocks. Regenerate it with the command in the docstring of tools/new_handler.py.
#
# It is an example, not a handler: it is not part of hf_core, it lives in namespace hf, and
# handlers/bmp581/ stays the BMP581 handler the HALs use until that one is migrated.
#
#   bmp581/Bmp581Handler.{h,cpp}, HalI2cBmp581Comm.h, test_bmp581.cpp   generated; handler_template_fresh
#                                                                      fails when they drift from the template
#   bmp581/test_bmp581_example.cpp                                     written by hand: the chip's own cases

if(NOT HF_CORE_ENABLE_BMP581)
    return()
endif()

add_library(hf_core_example_bmp581 STATIC bmp581/Bmp581Handler.cpp)
target_link_libraries(hf_core_example_bmp581 PUBLIC hf_core)
target_compile_options(hf_core_example_bmp581 PRIVATE ${HF_CORE_STRICT_WARNINGS})

add_executable(test_example_bmp581 bmp581/test_bmp581.cpp bmp581/test_bmp581_example.cpp)
target_link_libraries(test_example_bmp581 PRIVATE hf_core_example_bmp581 hf_core_host_test_main hf_core_host_mocks)
target_compile_options(test_example_bmp581 PRIVATE ${HF_CORE_STRICT_WARNINGS})
add_test(NAME test_example_bmp581 COMMAND test_example_bmp581)
set_tests_properties(test_example_bmp581 PROPERTIES TIMEOUT 60)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME handler_template_fresh
             COMMAND ${Python3_EXECUTABLE} "${HF_CORE_ROOT}/tools/new_handler.py"
                     --chip Bmp581 --bus i2c --driver-repo hf-bmp581-driver --driver-header bmp581.hpp
                     --driver-class Driver --port-base BusInterface --error-enum DriverError
                     --result-type DriverResult --init-method Init
                     --example-dir templates/handler/example --check)
    add_test(NAME new_handler_tool
             COMMAND ${Python3_EXECUTABLE} -m unittest discover -s "${HF_CORE_ROOT}/tools/tests" -v)
endif()
