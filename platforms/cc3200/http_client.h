#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

// HTTP Client lib
#include <http/client/httpcli.h>
#include <http/client/common.h>

#define JUMPER_HTTP_HOST_PORT 443
#define JUMPER_HTTP_API_VERSION "1.0"
#define JUMPER_HTTP_EVENTS_URI "/" ## JUMPER_HTTP_API_VERSION ## "/projects/" ## JUMPER_PROJECT_ID ## "/events"
#define JUMPER_HTTP_HOST_NAME "eventsapi.jumper.io"

typedef enum {
    HTTP_CLIENT_OK = 0,
    HTTP_CLIENT_DNS_FAILED,
    HTTP_CLIENT_CONNECTION_FAILED,
    HTTP_CLIENT_SEND_REQUEST_FAILED,
    HTTP_CLIENT_RESPONSE_FAILED,
} http_client_result_t;

#endif