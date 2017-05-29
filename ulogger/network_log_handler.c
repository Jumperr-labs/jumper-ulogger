#include "network_log_handler.h"
#include "ubuffer.h"
#include <string.h>
#include <stdbool.h>
#include "network_log_handler.h"

HandlerReturnType network_handler_log(void * context, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length) {
    uLoggerEventHeader * stored_event = NULL;
    network_log_config * config = (network_log_config*) context;
    if (ubuffer_allocate_next(&config->buffer, (void **)&stored_event, sizeof(uLoggerEventHeader) + data_length)) {
              return HANDLER_FAIL;
    }

    stored_event->version = NETWORK_LOGGER_VERSION;
    stored_event->time = time;
    stored_event->event_type = event_type;
    stored_event->data_length = data_length;
    //TODO check that length matches with body length
    if (data_length > 0) {
        memcpy((void*)(((uint8_t*)stored_event) + sizeof(uLoggerEventHeader)), log_data, data_length);
    }
    
    return HANDLER_SUCCESS;
}

void netowork_logger_periodic_callback(void * data) {
    int err_code;
    uLoggerEventHeader * event;
    network_log_config * config = (network_log_config *) data;

    while (config->can_send(config->context) && 
            ubuffer_peek_first(&config->buffer, (void**)&event, sizeof(uLoggerEventHeader)) == UBUFFER_SUCCESS) {
        size_t len = sizeof(uLoggerEventHeader) + event->data_length;
        size_t formatted_length;
        if (config->format_method != NULL) {
            uint8_t * formatted_event;
            err_code = config->format_method(config->formatter_context, event, &formatted_event, &formatted_length);
            if (err_code) { //Can't format event, throw it away
                //TODO add a log event that we threw away an event
                err_code = 0;
            } else {
                err_code = config->send(config->context, (void *) formatted_event, formatted_length);
            }
        } else {
            err_code = config->send(config->context, (void *) event, len);
        }
        
        if (err_code) {
            return;
        }
        ubuffer_free_first(&config->buffer, (void**)&event, len);
    }
}

int network_logger_init(network_log_config * config, uint8_t * buffer, size_t buffer_size) {
    ubuffer_init(&config->buffer, (char *)buffer, buffer_size);
    return 0;
}
