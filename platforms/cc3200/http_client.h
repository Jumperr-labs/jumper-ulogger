#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

// HTTP Client lib
#include <http/client/httpcli.h>
#include <http/client/common.h>

#include "logging_config.h"

#define JUMPER_HTTP_HOST_PORT 443
#define JUMPER_HTTP_API_VERSION "1.0"
#define JUMPER_HTTP_EVENTS_URI "/"JUMPER_HTTP_API_VERSION"/projects/"JUMPER_PROJECT_ID"/events"
#define JUMPER_HTTP_HOST_NAME "eventsapi2.jumper.io"

typedef enum {
    HTTP_CLIENT_OK = 0,
    HTTP_CLIENT_DNS_FAILED,
    HTTP_CLIENT_CONNECTION_FAILED,
    HTTP_CLIENT_SEND_REQUEST_FAILED,
    HTTP_CLIENT_RESPONSE_FAILED,
} http_client_result_t;

typedef struct {
     HTTPCli_Struct http_client;
} jumper_http_client_context_t;

http_client_result_t http_client_send_event(jumper_http_client_context_t * context, char * data, uint32_t data_length);

#endif
