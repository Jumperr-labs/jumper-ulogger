# THIS FILE IS ON:Y USED FOR IDE INTEGRATION AND NOT FOR BUILDING THE PROJECT
# Use make instead

include_directories(../../ulogger)
link_directories(../../ulogger)
add_executable(nrf-sample-project config/sdk_config.h logging_config.h main.c)
target_link_libraries(nrf-sample-project ulogger nrf52)
