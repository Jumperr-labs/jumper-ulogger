#include "http_client.h"

static struct HTTPCli_SecureParams sparams = {
    .method = SL_SO_SEC_METHOD_SSLv3_TLSV1_2,
    .mask = SL_SEC_MASK_TLS_DHE_RSA_WITH_AES_256_CBC_SHA,
    .cafile = "/cert/jmpr_ca_cert.cer",
    .privkey = {0}, 
    .cert = {0}, 
    .dhkey = {0}
};

static http_client_result_t connect_to_http_server(jumper_http_client_context_t * context);
static http_client_result_t http_send_post(jumper_http_client_context_t * context, uint8_t * request_uri, uint8_t * request_data, uint32_t data_length);
static http_client_result_t read_response(jumper_http_client_context_t * context);
static int flush_response(jumper_http_client_context_t * context);

http_client_result_t http_client_send_event(jumper_http_client_context_t * context, char * data, uint32_t data_length) {
    http_client_result_t result;
    HTTPCli_setSecureParams(&sparams); 
    result = connect_to_http_server(context);
    if (result != HTTP_CLIENT_OK) {
        return result;
    }
    result = http_send_post(context, JUMPER_HTTP_EVENTS_URI, data, data_length);
    if (result != HTTP_CLIENT_OK) {
        return result;
    }

    return HTTP_CLIENT_OK;
}

void http_client_init(jumper_http_client_context_t * context) {
    HTTPCli_construct(&context->http_client);
    context->destination_ip = 0;
}

static http_client_result_t connect_to_http_server(jumper_http_client_context_t * context) {
    int32_t ret_val = 0;
    struct sockaddr_in addr;
    /* Resolve HOST NAME/IP */
    if (context->destination_ip == 0) {
        ret_val = sl_NetAppDnsGetHostByName(JUMPER_HTTP_HOST_NAME,
                                              strlen(JUMPER_HTTP_HOST_NAME),
                                              &context->destination_ip,SL_AF_INET);
        if(ret_val < 0)
        {
            Report("DNS Failed with status %d\n", ret_val);
            return HTTP_CLIENT_DNS_FAILED;
        }
    }


    /* Set up the input parameters for HTTP Connection */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(JUMPER_HTTP_HOST_PORT);
    addr.sin_addr.s_addr = sl_Htonl(context->destination_ip);

    /* Testing HTTPCli open call: handle, address params only */

    ret_val = HTTPCli_connect(&context->http_client, (struct sockaddr *)&addr, HTTPCli_TYPE_TLS, NULL);
    if (ret_val < 0)
    {
        Report("Connection to server failed. error(%d)\n\r", ret_val);
        return HTTP_CLIENT_CONNECTION_FAILED;
    }    
    else
    {
        Report("Connection to server created successfully\r\n");
    }

    return HTTP_CLIENT_OK;
}


static http_client_result_t http_send_post(jumper_http_client_context_t * context, uint8_t * request_uri, uint8_t * request_data, uint32_t data_length) {
    bool moreFlags = 1;
    bool lastFlag = 1;
    char tmpBuf[4];
    long lRetVal = 0;
    HTTPCli_Field fields[] = {
                                {HTTPCli_FIELD_NAME_HOST, JUMPER_HTTP_HOST_NAME},
                                {HTTPCli_FIELD_NAME_ACCEPT, "*/*"},
                                {HTTPCli_FIELD_NAME_CONTENT_TYPE, "application/json"},
                                {HTTPCli_FIELD_NAME_AUTHORIZATION, JUMPER_WRITE_KEY},
                                {NULL, NULL}
                            };

    HTTPCli_setRequestFields(&context->http_client, fields);

    moreFlags = 1;
    lRetVal = HTTPCli_sendRequest(&context->http_client, HTTPCli_METHOD_POST, request_uri, moreFlags);
    if(lRetVal < 0)
    {
        Report("Failed to send HTTP POST request header %d.\n\r", lRetVal);
        return HTTP_CLIENT_SEND_REQUEST_FAILED;
    }

    sprintf((char *)tmpBuf, "%d", data_length);

    lastFlag = 1;
    lRetVal = HTTPCli_sendField(&context->http_client, HTTPCli_FIELD_NAME_CONTENT_LENGTH, (const char *)tmpBuf, lastFlag);
    if(lRetVal < 0)
    {
        Report("Failed to send HTTP POST request header %d.\n\r", lRetVal);
        return HTTP_CLIENT_SEND_REQUEST_FAILED;
    }

    lRetVal = HTTPCli_sendRequestBody(&context->http_client, request_data, data_length);
    if(lRetVal < 0)
    {
        Report("Failed to send HTTP POST request body %d.\n\r", lRetVal);
        return HTTP_CLIENT_SEND_REQUEST_FAILED;
    }


    lRetVal = read_response(context);

    HTTPCli_disconnect(&context->http_client);

    return HTTP_CLIENT_OK;
}


static http_client_result_t read_response(jumper_http_client_context_t * context) {
	long lRetVal = 0;
	int bytesRead = 0;
	bool moreFlags = 1;

	/* Read HTTP POST request status code */
	lRetVal = HTTPCli_getResponseStatus(&context->http_client);

    flush_response(context);
	
    if (200 <= lRetVal <= 299) {
        return HTTP_CLIENT_OK;
    } else {
        return HTTP_CLIENT_RESPONSE_FAILED;
    }
}

static int flush_response(jumper_http_client_context_t * context) {
    const char *ids[2] = {
                            HTTPCli_FIELD_NAME_CONNECTION, /* App will get connection header value. all others will skip by lib */
                            NULL
                         };
    char buf[64];
    int id;
    int len = 1;
    bool moreFlag = 0;
    char ** prevRespFilelds = NULL;


    /* Store previosly store array if any */
    prevRespFilelds = HTTPCli_setResponseFields(&context->http_client, ids);

    /* Read response headers */
    while ((id = HTTPCli_getResponseField(&context->http_client, buf, sizeof(buf), &moreFlag))
            != HTTPCli_FIELD_ID_END)
    {

        if(id == 0)
        {
            if(!strncmp(buf, "close", sizeof("close")))
            {
                Report("Connection terminated by server\n\r");
            }
        }

    }

    /* Restore previosuly store array if any */
    HTTPCli_setResponseFields(&context->http_client, (const char **)prevRespFilelds);

    while(1)
    {
        HTTPCli_readResponseBody(&context->http_client, buf, sizeof(buf) - 1, &moreFlag);
        if (len < 0) {
            return len;
        }

        if ((len - 2) >= 0 && buf[len - 2] == '\r' && buf [len - 1] == '\n'){
            break;
        }

        if(!moreFlag)
        {
            /* There no more data. break the loop. */
            break;
        }
    }
    return 0;
}
