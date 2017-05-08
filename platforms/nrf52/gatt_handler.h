/**
 * @file
 * @brief Handler implementation for sending data over GATT to the HCI proxy.
 * The GATT handler handles everything related to initialising its own service, charactaristic, and managing the supplied buffer.
 */
#ifndef JUMPER_ULOGGER_GATT_HANDLER_H
#define JUMPER_ULOGGER_GATT_HANDLER_H
#include "logging_config.h"

#if ULOGGER_PLATFORM == PLATFORM_NRF52

#include "network_log_handler.h"
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

/**
 * @brief Initialize the GATT handler with the supplied buffer.
 * @param buffer A buffer for storing the data before sending it in large chunks to the server. Increasing the buffer size means you can wait
 * more between bursts.
 * @param buffer_length The size of the given buffer.
 * @return 0 on success, error code otherwise.
 */
uint32_t gatt_handler_init(uint8_t * buffer, uint32_t buffer_length);

/**
 *
 * @param handler_data
 * @param level
 * @param event_type
 * @param time
 * @param data
 * @param data_length
 * @return
 */
HandlerReturnType gatt_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

/**
 * A callback function that needs to be called with an event from the soft device.
 * @param p_ble_evt
 * @param handler
 */
void gatt_handler_handle_ble_event(ble_evt_t *p_ble_evt, uLoggerGattHandler * handler);

#endif

#endif //JUMPER_ULOGGER_GATT_HANDLER_H
