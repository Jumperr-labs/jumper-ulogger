#include "json_formatter.h"
#include "json_encoding_helper.h"

#include <stdio.h>
#include <string.h>


#define PACK_EVENT_TYPE(buf, type) \
        (buf) += sprintf((buf), "\"event\": \"%s\", ", #type)

int json_formatter_format(void * formatter_context, uLoggerEventHeader * event,
                          uint8_t ** formatted_event, size_t * formatted_event_length) {
    json_formatter_context * context = (json_formatter_context *) formatter_context;
    uint8_t * additional_data = ((uint8_t*)event) + sizeof(uLoggerEventHeader);
    memset(context->buffer, 0, context->buffer_length);
    char * buf = context->buffer;

    START_OBJECT(buf);
    START_ARRAY(buf, "default");
    START_OBJECT(buf);
    switch (event->event_type) {
        case DEVICE_STARTED_EVENT:
            PACK_EVENT_TYPE(buf, DEVICE_STARTED_EVENT);
            break;
        case WLAN_EVENT:
        {
            wlan_event_t * event_metadata = (wlan_event_t *) additional_data;
            PACK_EVENT_TYPE(buf, WLAN_EVENT);
            PACK_NAME_AND_INT(buf, event_metadata, is_connected);
            PACK_NAME_AND_STRING(buf, event_metadata, bssid);
        }
        break;
        default:
        {
            return -1;
        }
    }
    END_OBJECT(buf)
    END_ARRAY(buf);
    END_OBJECT(buf)
    *formatted_event =  context->buffer;
    *formatted_event_length = buf - context->buffer;
    return 0;
}
