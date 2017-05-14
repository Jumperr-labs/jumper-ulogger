#include "keen_handler.h"
#include "ulogger.h"
#include "network_log_handler.h"
#include "http_client.h"
#include "keen_client.h"
#include "logging_config.h"
#include "common.h"

char *api_version = KEEN_API_VERSION;
char *project_id = KEEN_PROJECT_ID;
char *write_key = KEEN_WRITE_KEY;

extern uint32_t g_ulStatus;

#undef send

static int keen_handler_send(void * network_context, uint8_t * data, uint32_t length);
static bool keen_handler_can_send(void * network_context);
static int keen_handler_timer_start(network_log_config * config);

int keen_handler_init(network_log_config * config, uint8_t * buffer, size_t buffer_size) {
    config->log_send_period = KEEN_HANDLER_LOG_SEND_PERIOD;
    config->context = (void *) NULL;
    config->send = keen_handler_send;
    config->can_send = keen_handler_can_send;
    network_logger_init(config, buffer, buffer_size);
    keen_handler_timer_start(config); 
    
    return 0;
}

static bool keen_handler_can_send(void * network_context) {
    return IS_CONNECTED(g_ulStatus) && IS_IP_ACQUIRED(g_ulStatus);
}

static int keen_handler_send(void * network_context, uint8_t * data, uint32_t length) {
  uLoggerEventHeader * event = (uLoggerEventHeader *) data;
  uint32_t error_code = 0;
  uint8_t * additional_data = data + sizeof(uLoggerEventHeader); 
  //error_code = add_event("jumper_event", "{\"event_id\": 1}");
  UART_PRINT("Sending data....... %d\n", error_code);
  return 0;
}

static void periodic_caller(void * parameters) {
    network_log_config * config = (network_log_config *) parameters;
    while (1) {
        osi_Sleep(config->log_send_period);
        config->callback(parameters);
    }
}

HandlerReturnType keen_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length) {
    network_handler_log((network_log_config *)handler_data, level, event_type, time, data, data_length);
    return HANDLER_SUCCESS;
}


static int keen_handler_timer_start(network_log_config * config) {
    osi_TaskCreate(periodic_caller, (const signed char*)"Send Buffer Task",KEEN_HANDLER_STACK_SIZE ,(void*)config, 1, NULL);
    return 0;
}