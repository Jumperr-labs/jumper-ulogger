#include "ulogger.h"

void ulogger_init(uLogger *ulogger, handler_func *handlers, void** handlers_data, size_t num_handlers) {
    ulogger->handlers = handlers;
    ulogger->handlers_data = handlers_data;
    ulogger->num_handlers = num_handlers;
}

void ulogger_log(uLogger *ulogger, EventType event_type, ...) {
    handler_func handler;
    timestamp time;

    get_timestamp(&time);
    for (int i=0; i < ulogger->num_handlers; ++i) {
        handler = ulogger->handlers[i];
        (*handler)(event_type, time, ulogger->handlers_data[i]);
    }
}