#include "network_log_handler.h"
#include "ubuffer.h"
#include <string.h>
#include <stdbool.h>

#include "nrf_log.h"
#define NETWORK_LOGGER_VERSION 1

typedef struct {
    uint32_t version;
    EventType event_type;
    timestamp time;
    size_t data_length;
} uLoggerEventHeader;

HandlerReturnType network_handler_log(network_log_config * config, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length) {
    uLoggerEventHeader * stored_event = NULL;

    NRF_LOG_INFO("network_handler_log %d\n", data_length);
    if (ubuffer_allocate_next(&config->buffer, (void **)&stored_event, sizeof(uLoggerEventHeader) + data_length)) {
        
        NRF_LOG_INFO("Failed to allocate\n");
        return HANDLER_FAIL;
    }

    NRF_LOG_INFO("After alloc head is %d, empty is %d\n", config->buffer.head, config->buffer.num_empty_bytes_at_end);
    stored_event->version = NETWORK_LOGGER_VERSION;
    stored_event->time = time;
    stored_event->event_type = event_type;
    stored_event->data_length = data_length;
    
    if (data_length > 0) {
        memcpy((void*)(((uint8_t*)stored_event) + sizeof(uLoggerEventHeader)), log_data, data_length);
    }
    
    return HANDLER_SUCCESS;
}

static void periodic_callback(void * data) {
    int err_code;
    
    uLoggerEventHeader * event;
    network_log_config * config = (network_log_config *) data;
    NRF_LOG_INFO("Periodic callbaack %d %d\n", config->can_send(config->context), ubuffer_peek_first(&config->buffer, (void**)&event, sizeof(uLoggerEventHeader)));
    NRF_LOG_INFO("Buffer size %d\n", config->buffer.size);
    while (config->can_send(config->context) && 
            ubuffer_peek_first(&config->buffer, (void**)&event, sizeof(uLoggerEventHeader)) == UBUFFER_SUCCESS) {
        uint16_t len = sizeof(uLoggerEventHeader) + event->data_length;
        err_code = config->send(config->context, (void *) event, len);
    
        NRF_LOG_INFO("Got result %d\n", err_code);
        if (err_code) {
            return;
        }
        ubuffer_free_first(&config->buffer, (void**)&event, len);
        NRF_LOG_INFO("After free head: %d, empty %d\n", config->buffer.head, config->buffer.num_empty_bytes_at_end);
    }
}

int network_logger_init(network_log_config * config, uint8_t * buffer, size_t buffer_size) {
    int res;
    ubuffer_init(&config->buffer, (char *)buffer, buffer_size);
    res = config->timer_init(config->log_send_period, periodic_callback);
    if (res) {
        return res;
    }

    return 0;
}