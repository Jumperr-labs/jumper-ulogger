set(SOURCE_FILES
#        circular_buffer.c
#        circular_buffer.h
        ulogger.c
        ulogger.h
        )

if(${NRF52})
    set(SOURCE_FILES "${SOURCE_FILES} nrf52.c")
endif()

add_library(ulogger ulogger.c)
