#include "ulogger_async.h"


uLoggerAsyncErrorCode ulogger_async_init(uLoggerAsync *ulogger_async, char *start, size_t buffer_capacity, handler_func *handlers,
                                         void** handlers_data, size_t num_handlers) {
    ulogger_init(ulogger_async->ulogger, handlers, handlers_data, num_handlers);
    ubuffer_init(ulogger_async->ubuffer, start, buffer_capacity);
}

uLoggerAsyncErrorCode async_logger_log_event(uLoggerAsync *ulogger_async, EventType event_type, timestamp time,
                                                  ...) {
    LoggingEvent* event;
    uBufferErrorCode err_code;

    err_code = ubuffer_push(ulogger_async->ubuffer, (void**) &event, sizeof(LoggingEvent));
    if (err_code) return (uLoggerAsyncErrorCode) err_code;

    event->event_type = event_type;
    event->time = time;

    return uLogger_Async_Sucess;
}

uLoggerAsyncErrorCode async_logger_flush(uLoggerAsync *ulogger_async) {
    uBufferErrorCode err_code;
    LoggingEvent* event;
    while (ulogger_async->ubuffer->size) {
        err_code = ubuffer_pop(ulogger_async->ubuffer, (void**) &event, sizeof(LoggingEvent));
        if (err_code) return (uLoggerAsyncErrorCode) err_code;
        ulogger_log_existing_event(ulogger_async->ulogger, event);
    }
}