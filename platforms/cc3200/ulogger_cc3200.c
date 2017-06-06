#include "logging_config.h"
#include "ulogger.h"
#include "events_api_handler.h"
#include "network_log_handler.h"

// Driverlib includes
#include "hw_types.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "hw_apps_rcm.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "timer.h"
#include "utils.h"
#include <stdio.h>
#include "osi.h"
#include "common.h"
#include "timer_if.h"

#define ULOGGER_STACK_SIZE 1024

static volatile unsigned long timer_base;
static volatile unsigned long timer_timestamp = 0;

static json_formatter_context json_context = {0};
static network_log_config events_api_handler_config = {0};
static uint8_t events_api_buffer[API_HANDLER_BUFFER_SIZE];
static uint8_t json_encoding_buffer[API_HANDLE_JSON_ENCODER_BUFFER_SIZE];

static char * log_level_strings[] = {
            "DEBUG",
            "INFO",
            "WARNING",
            "ERROR",
            "FATAL"
};

HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length);

static handler_func log_handlers[] = {&log_handler, &network_handler_log};
static void * handler_data[] = {NULL, &events_api_handler_config};

HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * log_data, size_t data_length) {
    UART_PRINT("Level: %s, Event: %d, Time: %d\r\n", (uint32_t)log_level_strings[level], event_type, time);
    return HANDLER_SUCCESS;
}

void LoggingTimerHandler(void)
{
    Timer_IF_InterruptClear(timer_base);
    timer_timestamp ++;
}

static void timer_init() {
    timer_base = TIMERA0_BASE;
    Timer_IF_Init(PRCM_TIMERA0, timer_base, TIMER_CFG_PERIODIC, TIMER_A, 0);
    Timer_IF_IntSetup(timer_base, TIMER_A, LoggingTimerHandler);
    Timer_IF_Start(timer_base, TIMER_A, 1000);
}

void get_timestamp(timestamp* time)
{
    *time = (timestamp) timer_timestamp;
}

void ulogger_init_cc3200(uLogger * logger) {
    timer_init();
    events_api_handler_init(&events_api_handler_config, &events_api_buffer, API_HANDLER_BUFFER_SIZE, &json_context, &json_encoding_buffer, API_HANDLE_JSON_ENCODER_BUFFER_SIZE);
    ulogger_init(logger, log_handlers, handler_data, (size_t) 2);
}