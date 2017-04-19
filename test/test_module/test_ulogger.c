#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ulogger.h"

TEST_GROUP(TestLogger);

static uint32_t timestamp_counter, log_handler_counter;

TEST_SETUP(TestLogger) {
    timestamp_counter = (uint32_t) -1;
    log_handler_counter = (uint32_t) -1;
}

TEST_TEAR_DOWN(TestLogger) {
}

void get_timestamp(timestamp *data) {
    *data = (timestamp) ++timestamp_counter;
}

void log_handler(EventType event_type, timestamp time, ...) {
    log_handler_counter = (uint32_t) time;
}

void test_counters(uint32_t expected_value) {
    TEST_ASSERT_EQUAL(expected_value, timestamp_counter);
    TEST_ASSERT_EQUAL(expected_value, log_handler_counter);
}

TEST(TestLogger, Test_Sanity) {
    uLogger logger;
    void* handler_data[] = {NULL};
    handler_func log_handlers[1] = {(handler_func) &log_handler};

    ulogger_init(&logger, log_handlers, handler_data, (size_t) 1);

    ulogger_log(&logger, START_RADIO);
    test_counters(0);
    ulogger_log(&logger, START_RADIO);
    test_counters(1);
}
