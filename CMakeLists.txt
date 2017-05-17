cmake_minimum_required(VERSION 3.6)
project(jumper_ulogger)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)


include(CTest)
enable_testing()

add_subdirectory(external)
add_subdirectory(ulogger)
add_subdirectory(test/test_module)
add_subdirectory(platforms/nrf52)
add_subdirectory(samples/nrf52-ble-sample-project)
