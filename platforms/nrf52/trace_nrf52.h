#ifndef JUMPER_ULOGGER_TRACE_NRF52_H_H
#define JUMPER_ULOGGER_TRACE_NRF52_H_H
//
//#include "ulogger.h"
//
//extern uLogger ulogger;
//
//static inline uint32_t traced_sd_ble_gap_adv_start(ble_gap_adv_params_t const *p_adv_params, uint8_t conn_cfg_tag) {
//    ulogger_log(&ulogger, START_ADVERTISING);
//    return sd_ble_gap_adv_start(p_adv_params, conn_cfg_tag);
//}
//#define sd_ble_gap_adv_start traced_sd_ble_gap_adv_start
//
//static inline uint32_t traced_sd_ble_gap_adv_stop(void) {
//    ulogger_log(&ulogger, STOP_ADVERTISING);
//    return sd_ble_gap_adv_stop();
//}
//#define sd_ble_gap_adv_stop traced_sd_ble_gap_adv_stop

#include "ble_advertising.h"
#include "ble.h"

#define NRF_BLE_LOGGER ulogger

void ulogger_trace_nrf_ble_event(ble_evt_t *p_ble_evt);
void ulogger_trace_nrf_ble_adv_event(ble_adv_evt_t ble_adv_evt);

#endif //JUMPER_ULOGGER_TRACE_NRF52_H_H
