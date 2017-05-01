#include "logging_config.h"

#if ULOGGER_PLATFORM == PLATFORM_NRF52

#include <stdint.h>
#include "nrf_drv_rtc.h"

#define NRF_LOG_MODULE_NAME "ULOGGER"
#include "nrf_log.h"
#include "ulogger_nrf52.h"
#include "gatt_handler.h"
#include "trace_nrf52.h"

static const nrf_drv_rtc_t rtc_log = NRF_DRV_RTC_INSTANCE(ULOGGER_RTC);

HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

extern uLoggerGattHandler handler;
static handler_func log_handlers[] = {&log_handler, &gatt_handler_handle_log};
static void * handler_data[] = {NULL, &handler};

static uint8_t gatt_buffer_data[GATT_BUFFER_SIZE];

static char * log_level_strings[] = {
            "DEBUG",
            "INFO",
            "WARNING",
            "ERROR",
            "FATAL"
};

HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * data,size_t data_length) {
    NRF_LOG_INFO("Level %s\r\nEvent: %d\r\nTime: %d\r\n", (uint32_t)log_level_strings[level], event_type, time);
    return HANDLER_SUCCESS;
}

static void rtc_handler(nrf_drv_rtc_int_type_t int_type) 
{
}


/** @brief Function initialization and configuration of RTC driver instance.
 */
static void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    config.prescaler = 2048;
    err_code = nrf_drv_rtc_init(&rtc_log, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc_log, false);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc_log);
}

void get_timestamp(timestamp* time)
{
    *time = (timestamp) nrf_drv_rtc_counter_get(&rtc_log) / 16;
}

void ulogger_init_nrf52(uLogger* logger) {
    uint32_t err_code;
    rtc_config();
    err_code = gatt_handler_init(gatt_buffer_data, GATT_BUFFER_SIZE);
    NRF_LOG_INFO("Got res %d\n", err_code);
    ulogger_init(logger, log_handlers, handler_data, (size_t) 2);
}


void ulogger_handle_ble_event(ble_evt_t * p_ble_evt) {

    gatt_handler_handle_ble_event(p_ble_evt, &handler);
    ulogger_trace_nrf_ble_event(p_ble_evt);
}

#endif
