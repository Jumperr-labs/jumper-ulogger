#include "ulogger.h"

void ulogger_init(uLogger *logger, handler_func *handlers, size_t num_handlers) {
    logger->handlers = handlers;
    logger->num_handlers = num_handlers;
}

void ulogger_log(uLogger *logger, EventType event_type, ...) {
    int i;
    handler_func handler;
    timestamp time;

    get_timestamp(&time);
    for (i=0; i < logger->num_handlers; ++i) {
        handler = logger->handlers[i];
        (*handler)(event_type, time);
    }
}
