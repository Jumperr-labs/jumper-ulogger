# THIS FILE IS ON:Y USED FOR IDE INTEGRATION AND NOT FOR BUILDING THE PROJECT
# Use make instead

include_directories($ENV{NRF52_SDK} ../../ulogger)
link_directories($ENV{NRF52_SDK} ../../ulogger)

set(SRC_FILES gatt_handler.c gatt_handler.h logging_config.h trace_nrf52.c trace_nrf52.h ulogger_nrf52.c ulogger_nrf52.c)

add_library(nrf52 STATIC ${SRC_FILES})
target_link_libraries(nrf52 ulogger NRF_SDK)