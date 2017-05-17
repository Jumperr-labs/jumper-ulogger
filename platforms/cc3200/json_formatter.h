/**@file
 * @brief Json formatter.
 * A json formatter used to format events before sending them to the events api.
 * @ingroup cc3200_platform
 * @{
 */
#ifndef JSON_FORMATTER_H
#define JSON_FORMATTER_H

#include <stdint.h>
#include <stdlib.h>

#include "network_log_handler.h"

typedef struct {
    uint8_t * buffer;
    size_t buffer_length;
} json_formatter_context;

/**
 * @brief A formatter implementation as defined in network_log_handler.h, to format your custom events
 * you must implement a case in the formatter for each event.
 */
int json_formatter_format(void * formatter_context, uLoggerEventHeader * event,
                          uint8_t ** formatted_event, size_t * formatted_event_length);

#endif //JSON_FORMATTER_H
/**
 * @}
 */
