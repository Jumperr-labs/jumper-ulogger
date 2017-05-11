#include "logging_config.h"

#if ULOGGER_PLATFORM == PLATFORM_NRF52

#include "trace_nrf52.h"
#include "ulogger.h"

extern uLogger NRF_BLE_LOGGER;
void ulogger_trace_nrf_ble_event(ble_evt_t *p_ble_evt) {
    ble_state_event_t state;
    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            state.connected = 1;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            state.connected = 0;
            break;
        default:
            return; //not tracking other events now
    }
    ulogger_log(&NRF_BLE_LOGGER, ULOGGER_INFO, ADVERTISING_STATE_EVENT, &state, sizeof(state));
}

void ulogger_trace_nrf_ble_adv_event(ble_adv_evt_t ble_adv_evt) {
    advertising_state_event_t state;
    switch (ble_adv_evt) {
        case BLE_ADV_EVT_DIRECTED:            /**< Direct advertising mode has started. */
        case BLE_ADV_EVT_DIRECTED_SLOW:       /**< Directed advertising (low duty cycle) has started. */
        case BLE_ADV_EVT_FAST:                /**< Fast advertising mode has started. */
        case BLE_ADV_EVT_SLOW:                /**< Slow advertising mode has started. */
        case BLE_ADV_EVT_FAST_WHITELIST:     /**< Fast advertising mode using the whitelist has started. */
        case BLE_ADV_EVT_SLOW_WHITELIST:
            state.is_on = 1;
            break;
        case BLE_ADV_EVT_IDLE:
            state.is_on = 0;
            break;

        case BLE_ADV_EVT_WHITELIST_REQUEST:
        case BLE_ADV_EVT_PEER_ADDR_REQUEST:
            return; // ignore for now
    }

    ulogger_log(&NRF_BLE_LOGGER, ULOGGER_INFO, ADVERTISING_STATE_EVENT, &state, sizeof(state));
}

#endif
