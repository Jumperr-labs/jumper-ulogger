include_directories(../../ulogger ./)
link_directories(../../ulogger)

#set(SRC_FILES test_logger.c tests_runner.c)

add_executable(ulogger_tests tests_runner.c test_ulogger.c test_ubuffer.c mock_user_handlers.c mock_critical_section.c
        logging_config.h)

target_include_directories(ulogger PUBLIC ./)

target_link_libraries(ulogger_tests Unity ulogger)


#add_test(tests all_tests)
#
#add_custom_command(
#        TARGET all_tests
#        COMMENT "Run tests"
#        POST_BUILD
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
#        COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures
#)