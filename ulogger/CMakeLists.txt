set(SOURCE_FILES
        ubuffer.c
        ubuffer.h
        ulogger.c
        ulogger.h
        ulogger_events.h
        network_log_handler.c
        network_log_handler.h
        compilers.h)

add_library(ulogger ${SOURCE_FILES})
