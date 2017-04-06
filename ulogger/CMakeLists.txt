set(SOURCE_FILES
#        circular_buffer.c
#        circular_buffer.h
        logger.c
        logger.h
        logger_config.h
        )

if(${NRF52})
    set(SOURCE_FILES "${SOURCE_FILES} nrf52.c")
endif()

add_library(ulogger logger.c)
