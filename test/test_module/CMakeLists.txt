include_directories(../../ulogger)
link_directories(../../ulogger)

#set(SRC_FILES test_logger.c tests_runner.c)

add_executable(all_tests tests_runner.c test_logger.c test_buffer.c)

target_link_libraries(all_tests Unity ulogger)

#add_test(tests all_tests)
#
#add_custom_command(
#        TARGET all_tests
#        COMMENT "Run tests"
#        POST_BUILD
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
#        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures
#)