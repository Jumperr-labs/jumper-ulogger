#include "ulogger.h"

uLoggerErrorCode ulogger_init(void *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers) {
    uLogger* ulogger_handle = (uLogger*) ulogger;
    ulogger_handle->handlers = handlers;
    ulogger_handle->handlers_data = handlers_data;
    ulogger_handle->num_handlers = num_handlers;

    return ULOGGER_SUCCESS;
}

uLoggerErrorCode ulogger_log(void *ulogger, EventType event_type, ...) {
    uLogger* ulogger_handle = (uLogger*) ulogger;
    handler_func log_handle;
    timestamp time;
    HandlerReturnType err_code;
    uLoggerErrorCode return_value = ULOGGER_SUCCESS;

    get_timestamp(&time);
    for (int i=0; i < ulogger_handle->num_handlers; ++i) {
        log_handle = ulogger_handle->handlers[i];
        err_code = (*log_handle)(event_type, time, ulogger_handle->handlers_data[i]);
        if (err_code) return_value = ULOGGER_FAIL;
    }

    return return_value;
}