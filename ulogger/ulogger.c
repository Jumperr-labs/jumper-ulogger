#include "ulogger.h"

uLoggerErrorCode ulogger_init(uLogger *ulogger, handler_func *handlers, void **handlers_data, size_t num_handlers) {
    ulogger->handlers = handlers;
    ulogger->handlers_data = handlers_data;
    ulogger->num_handlers = num_handlers;

    return ULOGGER_SUCCESS;
}

uLoggerErrorCode ulogger_log(uLogger *ulogger, LogLevel level, EventType event_type, void * log_data, size_t data_length) {
    handler_func log_handle;
    timestamp time;
    HandlerReturnType err_code;
    int i;
    uLoggerErrorCode return_value = ULOGGER_SUCCESS;

    get_timestamp(&time);
    for (i=0; i < ulogger->num_handlers; ++i) {
        log_handle = ulogger->handlers[i];
        err_code = (*log_handle)(ulogger->handlers_data[i], level, event_type, time, log_data, data_length);
        if (err_code) {
            return_value = ULOGGER_FAIL;
        }
    }

    return return_value;
}
