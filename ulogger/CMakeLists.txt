set(SOURCE_FILES
        ubuffer.c
        ubuffer.h
        ulogger.c
        ulogger.h
        ulogger_events.h
        handlers/network_log_handler.c
        handlers/network_log_handler.h
        )

add_library(ulogger ${SOURCE_FILES})
