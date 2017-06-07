#include "events_api_handler.h"
#include "ulogger.h"
#include "network_log_handler.h"
#include "http_client.h"
#include "logging_config.h"
#include "common.h"
#include "json_encoding_helper.h"

extern uint32_t g_ulStatus;

#undef send
static jumper_http_client_context_t http_context;
static int events_api_handler_send(void * network_context, uint8_t * data, uint32_t length);
static bool events_api_handler_can_send(void * network_context);
static int events_api_handler_timer_start(network_log_config * config);

int events_api_handler_init(network_log_config * config, uint8_t * event_buffer, size_t event_buffer_size, json_formatter_context * context, uint8_t * encoding_buffer, size_t encoding_buffer_size) {
    context->buffer = encoding_buffer;
    context->buffer_length = encoding_buffer_size;

    config->log_send_period = API_HANDLER_LOG_SEND_PERIOD;
    config->context = (void *) &http_context;
    config->send = events_api_handler_send;
    config->can_send = events_api_handler_can_send;
    config->callback = netowork_logger_periodic_callback;
    config->formatter_context = context;
    config->format_method = json_formatter_format;
    network_logger_init(config, event_buffer, event_buffer_size);
    
    if (events_api_handler_timer_start(config)) {
        return -1;
    }
    
    return 0;
}

static bool events_api_handler_can_send(void * network_context) {
    return IS_CONNECTED(g_ulStatus) && IS_IP_ACQUIRED(g_ulStatus);
}

static int events_api_handler_send(void * network_context, uint8_t * data, uint32_t length) {

    jumper_http_client_context_t * config = (jumper_http_client_context_t *) network_context;
    http_client_result_t error_code = http_client_send_event(config, data, length);

//http_client_result_t http_client_send_event(jumper_http_client_context_t * context, char * data, uint32_t data_length);
    UART_PRINT("Sending data....... %d\n", error_code);
    return error_code;
}

static void periodic_caller(void * parameters) {
    network_log_config * config = (network_log_config *) parameters;
    while (1) {
        osi_Sleep(config->log_send_period);
        config->callback(parameters);
    }
}

HandlerReturnType events_api_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length) {
    network_handler_log((network_log_config *)handler_data, level, event_type, time, data, data_length);
    return HANDLER_SUCCESS;
}


static int events_api_handler_timer_start(network_log_config * config) {
    if (osi_TaskCreate(periodic_caller, (const signed char*)"Send Buffer Task",EVENTS_API_HANDLER_STACK_SIZE ,(void*)config, 1, NULL) == OSI_OK) {
        return 0;
    }
    return -1;
}
