#ifndef ULOGGER
#define ULOGGER

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    ULOGGER_SUCCESS = 0,
    ULOGGER_FAIL
} uLoggerErrorCode;

typedef uint32_t EventType;
#define GENERIC_EVENTS_START    0
#define START_ADVERTISING       1
#define STOP_ADVERTISING        2
#define START_RADIO             3
#define STOP_RADIO              4
#define ULOGGER_BATTERY_EVENT   5


#define PLATFORM_EVENTS_START   10000

#define USER_EVENTS_START       1000000000

#define ULOGGER_LOG(logger, level, event) ulogger_log(logger, level, event, 0, 0);

typedef enum {
    ULOGGER_DEBUG = 0,
    ULOGGER_INFO,
    ULOGGER_WARNING,
    ULOGGER_ERROR,
    ULOGGER_FATAL
} LogLevel;

typedef enum {
    HANDLER_SUCCESS = 0,
    HANDLER_FAIL
} HandlerReturnType;

typedef uint32_t timestamp;

typedef HandlerReturnType (*handler_func)(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);

typedef struct {
    handler_func* handlers;
    void** handlers_data;
    size_t num_handlers;
} uLogger;

#define SIZEOF_ULOGGER sizeof(uLogger)

void get_timestamp(timestamp *data);

uLoggerErrorCode ulogger_init(void *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers);

uLoggerErrorCode ulogger_log(void *ulogger, LogLevel level, EventType event_type, void * log_data, size_t data_length);

#endif // ULOGGER
