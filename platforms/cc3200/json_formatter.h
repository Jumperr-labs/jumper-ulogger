#ifndef JSON_FORMATTER_H
#define JSON_FORMATTER_H

#include <stdint.h>
#include <stdlib.h>

#include "network_log_handler.h"

typedef struct {
    uint8_t * buffer;
    size_t buffer_length;
} json_formatter_context;


int json_formatter_format(void * formatter_context, uLoggerEventHeader * event,
                          uint8_t ** formatted_event, size_t * formatted_event_length);

#endif //JSON_FORMATTER_H
