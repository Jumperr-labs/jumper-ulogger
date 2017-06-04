#include "logging_config.h"

#if ULOGGER_PLATFORM == PLATFORM_NRF52

#include <stdint.h>
#include "nrf_drv_rtc.h"

#define NRF_LOG_MODULE_NAME "ULOGGER"
#include "nrf_log.h"
#include "ulogger_nrf52.h"
#include "gatt_handler.h"
#include "trace_nrf52.h"
#include "network_log_handler.h"
#include "app_timer.h"


APP_TIMER_DEF(track_seconds_timer);

/**
* Compatability with old SDK
**/
#ifndef NRF_LOG_INFO
#define NRF_LOG_INFO NRF_LOG_PRINTF
#endif

static timestamp msecs = 0;
static const nrf_drv_rtc_t rtc_log = NRF_DRV_RTC_INSTANCE(ULOGGER_RTC);

HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

static uLoggerGattHandler handler;
network_log_config config;

static handler_func log_handlers[] = {&log_handler, &gatt_handler_handle_log};
static void * handler_data[] = {NULL, &config};

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

static void seconds_timeout(void * p_context) {
    msecs++;
}


/** @brief Function initialization and configuration of RTC driver instance.
 */
static int timer_config(void)
{
       uint32_t err_code;

    err_code = app_timer_create(&track_seconds_timer, APP_TIMER_MODE_REPEATED, seconds_timeout);
    if (err_code != NRF_SUCCESS) {
        NRF_LOG_INFO("Failed to create timer\n");
        return err_code;
    }

    ret_code_t ret_code; 
    ret_code = app_timer_start(track_seconds_timer, APP_TIMER_TICKS(1000, 0)  , (void *)NULL);
    if (ret_code) {
        NRF_LOG_INFO("Failed to create timer\n");
        return ret_code;
    }

    return NRF_SUCCESS;
}

void get_timestamp(timestamp* time)
{
    *time = msecs;
}

void ulogger_init_nrf52(uLogger* logger) {
    uint32_t err_code;
    timer_config();

    config.context = (void*) &handler;
    config.callback = &netowork_logger_periodic_callback;
    err_code = gatt_handler_init(&config, gatt_buffer_data, GATT_BUFFER_SIZE);
    NRF_LOG_INFO("Got res %d\n", err_code);
    ulogger_init(logger, log_handlers, handler_data, (size_t) 2);
}


void ulogger_handle_ble_event(ble_evt_t * p_ble_evt) {

    gatt_handler_handle_ble_event(p_ble_evt, &handler);
    ulogger_trace_nrf_ble_event(p_ble_evt);
}

#endif
