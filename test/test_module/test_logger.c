#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ulogger.h"

TEST_GROUP(TestLogger);

static uint32_t counter;

TEST_SETUP(TestLogger) {
    counter = (uint32_t) -1;
}

TEST_TEAR_DOWN(TestLogger) {
}

void get_timestamp(timestamp *data) {
    *data = (timestamp) ++counter;
}

void log_handler(EventType event_type, timestamp time, ...) {
//    printf("Event: %d\nTime: %d\n", event_type, time);
}

TEST(TestLogger, Test_Sanity) {
    uLogger logger;
    void* handler_data[] = {NULL};
    handler_func log_handlers[1] = {&log_handler};

    ulogger_init(&logger, log_handlers, handler_data, (size_t) 1);

    ulogger_log(&logger, START_RADIO);
    TEST_ASSERT_EQUAL(counter, 0);
    ulogger_log(&logger, START_RADIO);
    TEST_ASSERT_EQUAL(counter, 1);
}

//int main(void) {
//    UNITY_BEGIN();
//
//    RUN_TEST(test_sanity);
//
//    return UNITY_END();
//}
