#ifndef JUMPER_ULOGGER_TRACE_NRF52_H_H
#define JUMPER_ULOGGER_TRACE_NRF52_H_H

#include "ble_advertising.h"
#include "ble.h"

#define NRF_BLE_LOGGER ulogger

void ulogger_trace_nrf_ble_event(ble_evt_t *p_ble_evt);
void ulogger_trace_nrf_ble_adv_event(ble_adv_evt_t ble_adv_evt);

#endif //JUMPER_ULOGGER_TRACE_NRF52_H_H
