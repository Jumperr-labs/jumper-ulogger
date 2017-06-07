#include "json_formatter.h"
#include "json_encoding_helper.h"
#include "logging_config.h"
#include "netcfg.h"
#include <stdio.h>
#include <string.h>


#define PACK_EVENT_TYPE(buf, type) \
        (buf) += sprintf((buf), "\"type\": \"%s\", ", #type)


int json_formatter_format(void * formatter_context, uLoggerEventHeader * event,
                          uint8_t ** formatted_event, size_t * formatted_event_length) {

    json_formatter_context * context = (json_formatter_context *) formatter_context;
    uint8_t * additional_data = ((uint8_t*)event) + sizeof(uLoggerEventHeader);
    _u8 mac_address[SL_MAC_ADDR_LEN];
    _u8 macAddressLen = SL_MAC_ADDR_LEN;
    sl_NetCfgGet(SL_MAC_ADDRESS_GET,(_u8 *)NULL,&macAddressLen, mac_address);

    memset(context->buffer, 0, context->buffer_length);
    uint8_t * buf = context->buffer;
    START_ARRAY(buf);
    START_OBJECT(buf);
    PACK_NAME_AND_MAC_ADDRESS(buf, "device_id", mac_address);
    switch (event->event_type) {
        case DEVICE_BOOT_EVENT:
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
    *formatted_event =  context->buffer;
    *formatted_event_length = buf - context->buffer;
    return 0;
}
