#include "logging_config.h"

#if ULOGGER_PLATFORM == PLATFORM_NRF52

#include "sdk_common.h"
#include "gatt_handler.h"
#include "ble_srv_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ulogger.h"
#include "ubuffer.h"
#include "app_timer.h"
#include "logging_config.h"

#define NRF_LOG_MODULE_NAME "GATT_LOG"
#include "nrf_log.h"
#include "network_log_handler.h"

APP_TIMER_DEF(send_log_timer);

static const ble_uuid128_t jumper_log_uuid = {
        {
                //8ff40000-0a29-4a73-ab8d-b16ce0f1a2df
                0xdf, 0xa2, 0xf1, 0xe0, 0x6c, 0xb1, 0x8d, 0xab, 0x73, 0x4a, 0x29, 0x0a, 0x00, 0x00, 0xf4, 0x8f
        }
};

#define LOGGER_UUID_SERVICE		0x5677
#define LOGGER_UUID_CHAR		0x5678


static int send_to_gatt(void * network_context, uint8_t * data, uint32_t length);
static bool can_send_gatt_message(void * network_context);
static int gatt_handler_logging_timer_start(uint32_t time_in_ms, periodic_callback_function func);

#define LOG_SEND_PERIOD_MS 5000

uLoggerGattHandler handler;

network_log_config config = {
    .log_send_period = LOG_SEND_PERIOD_MS,
    .context = (void *) &handler,
    .send = send_to_gatt,
    .can_send = can_send_gatt_message,
    .timer_init = gatt_handler_logging_timer_start,
};

static uint32_t add_logging_chatactaristic(uLoggerGattHandler *handler)
{
    ble_gatts_char_md_t charactaristic_metadata;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t attr_char_value;
    ble_uuid_t ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&charactaristic_metadata, 0, sizeof(charactaristic_metadata));

    charactaristic_metadata.char_props.read = 1;
    charactaristic_metadata.char_props.notify = 1;
    charactaristic_metadata.p_char_user_desc = NULL;
    charactaristic_metadata.p_char_pf = NULL;
    charactaristic_metadata.p_user_desc_md = NULL;
    charactaristic_metadata.p_cccd_md = &cccd_md;
    charactaristic_metadata.p_sccd_md = NULL;

    ble_uuid.type = handler->uuid_type;
    ble_uuid.uuid = LOGGER_UUID_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 20;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 20;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(handler->service_handle, &charactaristic_metadata, &attr_char_value, &handler->send_char_handles);
}

static uint32_t add_logging_service(uLoggerGattHandler * handler) {
    ble_uuid_t ble_uuid;
    uint32_t err_code;

    err_code = sd_ble_uuid_vs_add(&jumper_log_uuid, &handler->uuid_type);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed at uuid\n");
        return err_code;
    }

    ble_uuid.type = handler->uuid_type;
    ble_uuid.uuid = LOGGER_UUID_SERVICE;

    return sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &handler->service_handle);

}

static bool can_send_gatt_message(void * network_context) {
    uLoggerGattHandler * handler = (uLoggerGattHandler *) network_context;
    return (handler->connection_handle != BLE_CONN_HANDLE_INVALID);
}

uint32_t gatt_handler_init(uint8_t * buffer, uint32_t buffer_length) {
    uint32_t err_code;
    uLoggerGattHandler * handler = (uLoggerGattHandler *) config.context; 
    memset(handler, 0, sizeof(uLoggerGattHandler));

    handler->connection_handle = BLE_CONN_HANDLE_INVALID;

    err_code = add_logging_service(handler);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed at service\n");
        return err_code;
    }

    err_code = add_logging_chatactaristic(handler);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed at char\n");
        return err_code;
    }

    err_code = network_logger_init(&config, buffer, buffer_length);
    if (err_code) {
        NRF_LOG_INFO("Failed at network logger\n");
        return err_code;
    }

    return NRF_SUCCESS;
}

void gatt_handler_handle_ble_event(ble_evt_t *p_ble_evt, uLoggerGattHandler * handler) {
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            handler->connection_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            handler->connection_handle = BLE_CONN_HANDLE_INVALID;
            break;
        default:
            // No implementation needed.
            break;
    }
}

static int send_to_gatt(void * network_context, uint8_t * data, uint32_t length) {
    uLoggerGattHandler * handler = (uLoggerGattHandler *) network_context;
    //TODO assert length < uint16
    uint32_t err_code;
    uint16_t len = length;
    ble_gatts_hvx_params_t hvx_params = {
                .handle = handler->send_char_handles.value_handle,
                .type   = BLE_GATT_HVX_NOTIFICATION,
                .p_len  = &len,
                .p_data = data
        };
    err_code = sd_ble_gatts_hvx(handler->connection_handle, &hvx_params);

    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to send log\n");
        return 1;
    }
    return 0;
}

HandlerReturnType gatt_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length) {
 //HandlerReturnType network_handler_log(network_log_config * config, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);
    return network_handler_log(&config, level, event_type, time, data, data_length);
}

static int gatt_handler_logging_timer_start(uint32_t time_in_ms, periodic_callback_function func) {
    uint32_t err_code;
    
    err_code = app_timer_create(&send_log_timer, APP_TIMER_MODE_REPEATED, func);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to create timer\n");
        return err_code;
    }

    ret_code_t ret_code; 
    ret_code = app_timer_start(send_log_timer, APP_TIMER_TICKS(time_in_ms)  , (void *)&config);
    if (ret_code) {
        NRF_LOG_INFO("Failed to create timer\n");
        return ret_code;
    }

    return NRF_SUCCESS;
}

#endif
