#ifndef NETWORK_LOG_HANDLER
#define NETWORK_LOG_HANDLER

#include "ubuffer.h"
#include <stdbool.h>

typedef void (*periodic_callback_function)(void * config);

typedef int (*send_to_network)(void * network_context, uint8_t * data, uint32_t length);
typedef bool (*can_send_to_network)(void * network_context);
typedef int (*periodic_callback_init)(uint32_t time_in_ms, periodic_callback_function func);

typedef struct {
    uint32_t log_send_period; 
    void * context;
    send_to_network send;
    can_send_to_network can_send;
    periodic_callback_init timer_init;
    uBuffer buffer;
} network_log_config;

int network_logger_init(network_log_config * config, uint8_t * buffer, size_t buffer_size);

HandlerReturnType network_handler_log(network_log_config * config, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);

#endif