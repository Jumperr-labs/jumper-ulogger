#include "ulogger.h"

void ulogger_init(void *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers) {
    uLogger* ulogger_handle = (uLogger*) ulogger;
    ulogger_handle->handlers = handlers;
    ulogger_handle->handlers_data = handlers_data;
    ulogger_handle->num_handlers = num_handlers;
}

void ulogger_log(void *ulogger, EventType event_type, ...) {
    uLogger* ulogger_handle = (uLogger*) ulogger;
    handler_func log_handle;
    timestamp time;

    get_timestamp(&time);
    for (int i=0; i < ulogger_handle->num_handlers; ++i) {
        log_handle = ulogger_handle->handlers[i];
        (*log_handle)(event_type, time, ulogger_handle->handlers_data[i]);
    }
}