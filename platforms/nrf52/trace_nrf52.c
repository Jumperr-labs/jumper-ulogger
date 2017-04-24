#include "trace_nrf52.h"
#include "ulogger.h"

extern uLogger NRF_BLE_LOGGER;
void ulogger_trace_nrf_ble_event(ble_evt_t *p_ble_evt) {
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:

            break;
        case BLE_GAP_EVT_DISCONNECTED:

            break;
    }
}

void ulogger_trace_nrf_ble_adv_event(ble_adv_evt_t ble_adv_evt) {
    switch (ble_adv_evt) {
        case BLE_ADV_EVT_DIRECTED:            /**< Direct advertising mode has started. */
        case BLE_ADV_EVT_DIRECTED_SLOW:       /**< Directed advertising (low duty cycle) has started. */
        case BLE_ADV_EVT_FAST:                /**< Fast advertising mode has started. */
        case BLE_ADV_EVT_SLOW:                /**< Slow advertising mode has started. */
        case BLE_ADV_EVT_FAST_WHITELIST:     /**< Fast advertising mode using the whitelist has started. */
        case BLE_ADV_EVT_SLOW_WHITELIST:
            ulogger_log(&NRF_BLE_LOGGER, ULOGGER_INFO, START_ADVERTISING);
            break;
        case BLE_ADV_EVT_IDLE:
            ulogger_log(&NRF_BLE_LOGGER, ULOGGER_INFO, STOP_ADVERTISING);
            break;
        case BLE_ADV_EVT_WHITELIST_REQUEST:
        case BLE_ADV_EVT_PEER_ADDR_REQUEST:
            break; // ignore for now
    }
}