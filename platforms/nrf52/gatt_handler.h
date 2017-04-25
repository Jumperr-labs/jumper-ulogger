#ifndef JUMPER_ULOGGER_GATT_HANDLER_H
#define JUMPER_ULOGGER_GATT_HANDLER_H

#include "ble.h"
#include "ulogger.h"
#include <stdbool.h>

typedef struct {
    uint16_t connection_handle;
    uint16_t service_handle;
    bool is_waiting_for_tx_complete;
    uint8_t uuid_type;
    ble_gatts_char_handles_t send_char_handles;
} uLoggerGattHandler;

uint32_t gatt_handler_init(uLoggerGattHandler * handler, uint8_t * buffer, uint32_t buffer_length);

uint32_t gatt_handler_logging_timer_start(uLoggerGattHandler * handler);

HandlerReturnType gatt_handler_handle_log(LogLevel level, EventType event_type, timestamp time, void* handler_data, ...);

void gatt_handler_handle_ble_event(ble_evt_t *p_ble_evt, uLoggerGattHandler * handler);

#endif //JUMPER_ULOGGER_GATT_HANDLER_H
