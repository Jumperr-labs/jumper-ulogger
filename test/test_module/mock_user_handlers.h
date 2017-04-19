#include <stdint.h>
#include "ulogger.h"

#ifndef JUMPER_ULOGGER_MOCK_USER_HANDLERS_H
#define JUMPER_ULOGGER_MOCK_USER_HANDLERS_H

#define NUM_HANDLERS 1

void init_user_handlers();

void get_timestamp(timestamp *data);

void log_handler(EventType event_type, timestamp time, ...);

void test_counters(uint32_t expected_value);

void* handler_data[NUM_HANDLERS];

handler_func log_handlers[NUM_HANDLERS];

#endif //JUMPER_ULOGGER_MOCK_USER_HANDLERS_H
