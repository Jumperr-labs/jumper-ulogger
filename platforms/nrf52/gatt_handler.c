#include "sdk_common.h"
#include "gatt_handler.h"
#include "ble_srv_common.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ulogger.h"
#include "ubuffer.h"
#include "app_timer.h"

#define NRF_LOG_MODULE_NAME "GATT_LOG"
#include "nrf_log.h"

APP_TIMER_DEF(send_log_timer);

static const ble_uuid128_t jumper_log_uuid = {
        {
                //8ff40000-0a29-4a73-ab8d-b16ce0f1a2df

                0xdf, 0xa2, 0xf1, 0xe0, 0x6c, 0xb1, 0x8d, 0xab, 0x73, 0x4a, 0x29, 0x0a, 0x00, 0x00, 0xf4, 0x8f
        }
};

#define BUFFER_LENGTH 200
static uint8_t buffer_data[BUFFER_LENGTH];
static uBuffer buffer;

#define LOGGER_UUID_SERVICE		0x5677
#define LOGGER_UUID_CHAR		0x5678

typedef struct {
    EventType event_type;
    timestamp time;
} uLoggerEventHeader;


static void log_generating_function(void * p_context);

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

uint32_t gatt_handler_init(uLoggerGattHandler * handler)
{
    uint32_t err_code;
    ubuffer_init(&buffer, (char *)buffer_data, BUFFER_LENGTH);

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


    err_code = app_timer_create(&send_log_timer,
                                APP_TIMER_MODE_REPEATED,
                                log_generating_function);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to create timer\n");
        return err_code;
    }

    err_code = gatt_handler_logging_timer_start(handler);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to start timer\n");
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

static void log_generating_function(void * p_context) {
    uint32_t err_code;
    uLoggerGattHandler * handler = (uLoggerGattHandler *) p_context;
    uLoggerEventHeader * event;

    while (handler->connection_handle != BLE_CONN_HANDLE_INVALID &&
            ubuffer_peek_first(&buffer, (void**)&event, sizeof(uLoggerEventHeader)) == UBUFFER_SUCCESS) {
        uint16_t len = sizeof(uLoggerEventHeader);
        ble_gatts_hvx_params_t hvx_params = {
                .handle = handler->send_char_handles.value_handle,
                .type   = BLE_GATT_HVX_NOTIFICATION,
                .p_len  = &len,
                .p_data = (uint8_t*)event
        };

        err_code = sd_ble_gatts_hvx(handler->connection_handle, &hvx_params);

        if (err_code != NRF_SUCCESS) {
            NRF_LOG_INFO("Failed to send log\n");
        }
        ubuffer_free_first(&buffer, (void**)&event, sizeof(uLoggerEventHeader));
    }
}

HandlerReturnType gatt_handler_handle_log(LogLevel level, EventType event_type, timestamp time, void* handler_data, ...) {
    uLoggerEventHeader * stored_event;
    if (ubuffer_allocate_next(&buffer, (void **)&stored_event, sizeof(uLoggerEventHeader))) {
        return HANDLER_FAIL;
    }
    stored_event->time = time;
    stored_event->event_type = event_type;
    return HANDLER_SUCCESS;
}

uint32_t gatt_handler_logging_timer_start(uLoggerGattHandler * handler) {
    uint32_t err_code;

    err_code = app_timer_start(send_log_timer, APP_TIMER_TICKS(5000)  , (void *)handler);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to create timer\n");
        return err_code;
    }

    return NRF_SUCCESS;
}
