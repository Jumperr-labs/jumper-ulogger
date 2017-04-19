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

//typedef struct {
//    EventType event_type;
//    uint32_t time;
//} LoggingEvent;

//typedef struct {
//    LoggingEvent base_event;
//    uint8_t channel;
//} LoggingEventStartRadio;

typedef uint32_t timestamp;

typedef void (*handler_func)(EventType event_type, timestamp time, void* handler_data, ...);

typedef struct {
    handler_func* handlers;
    void** handlers_data;
    size_t num_handlers;
} uLogger;

void get_timestamp(timestamp *data);

void ulogger_init(uLogger *ulogger, handler_func *handlers, void** handlers_data, size_t num_handlers);

void ulogger_log(uLogger *ulogger, EventType event_type, ...);

#endif // ULOGGER
