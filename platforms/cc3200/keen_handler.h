#ifndef KEEN_HANDLER_H
#define KEEN_HANDLER_H

#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"
#include "network_log_handler.h"
#include "json_formatter.h"

int keen_handler_init(network_log_config * config, uint8_t * event_buffer, size_t event_buffer_size, json_formatter_context * context, uint8_t * encoding_buffer, size_t encoding_buffer_size);

HandlerReturnType keen_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

#endif //KEEN_HANDLER_H
