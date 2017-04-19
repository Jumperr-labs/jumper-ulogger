#include "ubuffer.h"
#include "ulogger.h"

#ifndef ULOGGER_ASYNC
#define ULOGGER_ASYNC

// Currently the same as uBufferErrorCode in ubuffer.h
typedef enum {
    uLogger_Async_Sucess = 0,
    uLogger_Async_Buffer_Full = 1,
    uLogger_Async_Buffer_Empty = 2
} uLoggerAsyncErrorCode;

typedef struct {
    uLogger* ulogger;
    uBuffer* ubuffer;
    handler_func* handlers;
    size_t num_handlers;
} uLoggerAsync;

uLoggerAsyncErrorCode ulogger_async_init(uLoggerAsync *ulogger_async, char *start, size_t buffer_capacity, handler_func *handlers,
                                         void** handlers_data, size_t num_handlers);

uLoggerAsyncErrorCode async_logger_log_event(uLoggerAsync *ulogger_async, EventType event_type, timestamp time, ...);

uLoggerAsyncErrorCode async_logger_flush(uLoggerAsync *ulogger_async);


#endif //ULOGGER_ASYNC
