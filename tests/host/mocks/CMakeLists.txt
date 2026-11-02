# hf-core host fakes.
#
#   hf_core_host_clock  FakeClock: backs the HF_RTOS_NONE_EXTERNAL_CLOCK hooks
#                       that hf-utils-rtos-wrap calls for time and delays.
#   hf_core_host_mocks  Mock{I2c,Spi,Gpio,Adc,Can,Nvs,Uart,Temperature}.
#
# A board HAL's host tests link hf_core_host_mocks (which brings hf_core and
# the clock) and include "HfCoreMocks.h". See docs/host-build.md.

add_library(hf_core_host_clock STATIC FakeClock.cpp)
target_include_directories(hf_core_host_clock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(hf_core_host_clock PRIVATE ${HF_CORE_STRICT_WARNINGS})

add_library(hf_core_host_mocks STATIC
    MockAdc.cpp
    MockCan.cpp
    MockGpio.cpp
    MockI2c.cpp
    MockNvs.cpp
    MockSpi.cpp
    MockTemperature.cpp
    MockUart.cpp
)
target_include_directories(hf_core_host_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hf_core_host_mocks PUBLIC hf_core hf_core_host_clock)
target_compile_options(hf_core_host_mocks PRIVATE ${HF_CORE_STRICT_WARNINGS})
