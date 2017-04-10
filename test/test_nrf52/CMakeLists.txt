include_directories(../../ulogger)
link_directories(../../ulogger)
#

include_directories(../../external/nrf5-sdk)
link_directories(../../external/nrf5-sdk)
#
#
#
#
#set (NRF_SOURCE main.c sdk_config.h ulogger_config.h)
#include_directories(../../external/nrf5-sdk)
#link_directories(../../external/nrf5-sdk)
#link_libraries(nrf5_sdk)
add_executable(nrf_test main.c)

target_link_libraries(nrf_test NRF_SDK ulogger)

#
#target_link_libraries(nrf_test ulogger nrf5_sdk)
#
#add_test(tests my_test_ex)
#add_custom_command(
#        TARGET my_test_ex
#        COMMENT "Run tests"
#        POST_BUILD
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
#        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures
#)