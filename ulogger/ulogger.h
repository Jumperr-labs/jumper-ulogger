#ifndef LOGGER
#define LOGGER

#include <stdint.h>
#include <stdlib.h>

typedef enum EventType {
    START_ADVERTISING = 0,
    STOP_ADVERTISING = 1,
    START_RADIO = 2,
    STOP_RADIO = 3
} EventType;

typedef struct {
    EventType event_type;
    uint32_t time;
} LoggingEvent;

typedef struct {
    LoggingEvent base_event;
    uint8_t channel;
} LoggingEventStartRadio;

typedef uint32_t timestamp;

typedef void (*handler_func)(EventType event_type, timestamp time, ...);

typedef struct {
    handler_func* handlers;
    size_t num_handlers;
} uLogger;

void get_timestamp(timestamp *data);

void ulogger_init(uLogger *logger, handler_func *handlers, size_t num_handlers);

void ulogger_log(uLogger *logger, EventType event_type, ...);


#endif

//typedef struct {
//    handler_func* handler;
//} HandlerConfig;
//
//typedef struct {
//    HandlerConfig base_config;
//} NrfBleHandlerConfig;
//
//typedef struct {
//    HandlerConfig base_config;
//} NrfUartHandlerConfig;
//
//typedef union {
//    NrfBleHandlerConfig;
//} HandlerPolymorphicConfig;
