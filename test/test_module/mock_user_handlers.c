#include "unity.h"
#include "mock_user_handlers.h"

static uint32_t timestamp_counter, log_handler_counter;

void init_user_handlers() {
    handler_data[0] = NULL;
    log_handlers[0] = (handler_func) &log_handler;
    timestamp_counter = (uint32_t) -1;
    log_handler_counter = (uint32_t) -1;
}

void get_timestamp(timestamp *data) {
    *data = (timestamp) ++timestamp_counter;
}

HandlerReturnType log_handler(LogLevel level, EventType event_type, timestamp time, ...) {
    log_handler_counter = (uint32_t) time;
    return HANDLER_SUCCESS;
}

void assert_counters(uint32_t expected_value) {
    TEST_ASSERT_EQUAL(expected_value, timestamp_counter);
    TEST_ASSERT_EQUAL(expected_value, log_handler_counter);
}

