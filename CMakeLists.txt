cmake_minimum_required(VERSION 3.6)
project(jumper_ulogger)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(CTest)
enable_testing()

if(${NRF52})
    set(SOURCE_FILES "${SOURCE_FILES} nrf52.c")
endif()

add_subdirectory(external)
add_subdirectory(ulogger)
add_subdirectory(test/test_module)
