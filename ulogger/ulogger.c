#include "ulogger.h"

typedef struct {
    EventType event_type;
    uint32_t time;
} LoggingEvent;

typedef struct {
    LoggingEvent base_event;
    uint8_t channel;
} LoggingEventRadioOn;

void logger_init(Logger *logger, handler_func *handlers, size_t num_handlers) {
    logger->handlers = handlers;
    logger->num_handlers = num_handlers;
}

void logger_log(Logger *logger, EventType event_type, ...) {
    int i;
    handler_func handler;
    timestamp time;

    get_timestamp(&time);
    for (i=0; i < logger->num_handlers; ++i) {
        handler = logger->handlers[i];
        (*handler)(event_type, time);
    }
}
