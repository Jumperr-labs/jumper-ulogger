/**
 * @file
 * @brief Helper module for creating buffered network loggers.
 * The idea behind the network handler is that in an embedded environment all network logs will be buffered and sent in bursts
 * to reduce power usage and improve performance.
 */
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

/**
 * @brief Initializes a network_log_config struct with the given buffer.
 * @param config The config struct, to be passed to the logger as handler context data
 * @param buffer
 * @param buffer_size
 * @return 0 on success, error code otherwise
 */
int network_logger_init(network_log_config * config, uint8_t * buffer, size_t buffer_size);

/**
 * Log handler method for the ulogger log handler list.
 * @param config
 * @param level
 * @param event_type
 * @param time
 * @param log_data
 * @param data_length
 * @return
 */
HandlerReturnType network_handler_log(network_log_config * config, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);

#endif