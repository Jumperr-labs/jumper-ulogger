/**
 * @file
 * @defgroup nrf52_platform nRF52 Platform
 * @{
 */

#ifndef ULOGGER_NRF52
#define ULOGGER_NRF52

#include "ulogger.h"
#include "trace_nrf52.h
#include "ble.h"

#define ULOGGER_RTC 2

uLogger ulogger;

/**
 * @brief Initializes the supplied uLogger structure
 * @param ulogger A uLogger handle
 */
void ulogger_init_nrf52(uLogger* logger);

void ulogger_handle_ble_event(ble_evt_t * p_ble_evt);

#endif
/**
 * @}
 */
