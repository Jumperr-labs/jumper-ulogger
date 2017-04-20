#ifndef ULOGGER
#define ULOGGER

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    ULOGGER_SUCCESS = 0,
    ULOGGER_FAIL
} uLoggerErrorCode;

typedef enum {
    NO_EVENT = 0,
    START_ADVERTISING = 1,
    STOP_ADVERTISING = 2,
    START_RADIO = 3,
    STOP_RADIO = 4
} EventType;

typedef enum {
    HANDLER_SUCCESS = 0,
    HANDLER_FAIL
} HandlerReturnType;

typedef uint32_t timestamp;

typedef HandlerReturnType (*handler_func)(EventType event_type, timestamp time, void *handler_data, ...);

typedef struct {
    handler_func* handlers;
    void** handlers_data;
    size_t num_handlers;
} uLogger;

#define SIZEOF_ULOGGER sizeof(uLogger)

void get_timestamp(timestamp *data);

uLoggerErrorCode ulogger_init(void *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers);

uLoggerErrorCode ulogger_log(void *ulogger, EventType event_type, ...);

#endif // ULOGGER
