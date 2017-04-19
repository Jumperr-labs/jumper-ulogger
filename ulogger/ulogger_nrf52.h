#ifndef ULOGGER_NRF52
#define ULOGGER_NRF52

#include "ulogger.h"
#include "ble.h"

#define ULOGGER_RTC 2

void ulogger_init_nrf52(uLogger* logger);
void ulogger_handle_ble_event(ble_evt_t * p_ble_evt);

#endif