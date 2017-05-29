/**@file
 * @brief Events api handler.
 * The events log handler forwards logging events directly to jumper's logging api.
 * @ingroup cc3200_platform
 * @{
 */
#ifndef EVENTS_API_HANDLER_H
#define EVENTS_API_HANDLER_H

#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"
#include "network_log_handler.h"
#include "json_formatter.h"

/**
 * @brief Initializes the event logging handler
 * @param config A pointer to an instance of network_log_config
 * @param event_buffer A buffer for storing the data before sending it in large chunks to the server. Increasing the buffer size means you can wait
 * more between bursts.
 * @param event_buffer_size The size of the given buffer.
 * @param context A pointer to an instance of json_formatter_context
 * @param encoding_buffer A buffer to be used when encoding events to json. The buffer must be large enough to accomedate the longest json string that can be created
 * from encoding the events.
 * @param encoding_buffer_size the size of the given encoding buffer.
 * @return 0 on success, error code otherwise. */
int events_api_handler_init(network_log_config * config, uint8_t * event_buffer, size_t event_buffer_size, json_formatter_context * context, uint8_t * encoding_buffer, size_t encoding_buffer_size);

/**
 * @brief Handler implementation for the events api handler (as defined in network_log_handler.h)
 */
HandlerReturnType events_api_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

#endif //EVENTS_API_HANDLER_H
/**
 * @}
 */
