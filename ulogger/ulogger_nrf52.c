#include <stdint.h>
#include "nrf_drv_rtc.h"
#define NRF_LOG_MODULE_NAME "ULOGGER"
#include "nrf_log.h"
#include "ulogger_nrf52.h"

static const nrf_drv_rtc_t rtc_log = NRF_DRV_RTC_INSTANCE(ULOGGER_RTC);
void log_handler(EventType event_type, timestamp time, ...);
static handler_func log_handlers[1] = {&log_handler};

void log_handler(EventType event_type, timestamp time, ...) {
    NRF_LOG_INFO("Event: %d\r\nTime: %d\r\n", event_type, time);
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

// void get_timestamp(timestamp* time)
// {
//     uint32_t counter;
//     float freq;

//     freq = 32768/rtc_log.p_reg->PRESCALER+1;
//     counter = nrf_drv_rtc_counter_get(&rtc_log); 
//     *time = (timestamp) counter/freq;
// }

void get_timestamp(timestamp* time)
{
    *time = (timestamp) nrf_drv_rtc_counter_get(&rtc_log) / 16;
}

void ulogger_init_nrf52(uLogger* logger) {
    rtc_config();
    ulogger_init(logger, log_handlers, (size_t) 1);
}