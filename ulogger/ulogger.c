#include "ulogger.h"

void ulogger_init(uLogger *ulogger, handler_func *handlers, void** handlers_data, size_t num_handlers) {
    ulogger->handlers = handlers;
    ulogger->handlers_data = handlers_data;
    ulogger->num_handlers = num_handlers;
}

void ulogger_log(uLogger *ulogger, EventType event_type, ...) {
    LoggingEvent event;

    event.event_type = event_type;
    get_timestamp(&(event.time));
    ulogger_log_existing_event(ulogger, &event);
}

void ulogger_log_existing_event(uLogger *ulogger, LoggingEvent* event, ...) {
    handler_func handler;
    for (int i=0; i < ulogger->num_handlers; ++i) {
        handler = ulogger->handlers[i];
        (*handler)(event->event_type, event->time, ulogger->handlers_data[i]);
    }
}