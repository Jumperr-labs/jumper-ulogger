#ifndef ULOGGER
#define ULOGGER

#include <stdint.h>
#include <stdlib.h>

typedef enum {
    NO_EVENT = 0,
    START_ADVERTISING = 1,
    STOP_ADVERTISING = 2,
    START_RADIO = 3,
    STOP_RADIO = 4
} EventType;

typedef struct {
    EventType event_type;
    uint32_t time;
} LoggingEvent;

typedef struct {
    LoggingEvent base_event;
    uint8_t channel;
} LoggingEventStartRadio;

typedef uint32_t timestamp;

typedef void (*handler_func)(EventType event_type, timestamp time, ...);

typedef struct {
    handler_func* handlers;
    size_t num_handlers;
} uLogger;

void get_timestamp(timestamp *data);

void ulogger_init(uLogger *logger, handler_func *handlers, size_t num_handlers);

void ulogger_log(uLogger *logger, EventType event_type, ...);


#endif // ULOGGER
