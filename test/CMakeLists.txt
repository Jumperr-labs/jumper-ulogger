include_directories(../ulogger)
link_directories(../ulogger)

add_executable(my_test_ex test_logger.c)

target_link_libraries(my_test_ex Unity ulogger)

add_test(tests my_test_ex)
add_custom_command(
        TARGET my_test_ex
        COMMENT "Run tests"
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures
)