#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "ulogger.h"

static uint32_t counter = (uint32_t) -1;

void get_timestamp(timestamp *data) {
    *data = (timestamp) ++counter;
}

void log_handler(EventType event_type, timestamp time, ...) {
    printf("Event: %d\nTime: %d\n", event_type, time);
}

void test_sanity(void) {
    Logger logger;
    handler_func log_handlers[1] = {&log_handler};

    logger_init(&logger, log_handlers, (size_t) 1);

    logger_log(&logger, START_RADIO);
    TEST_ASSERT_EQUAL(counter, 0);
    logger_log(&logger, START_RADIO);
    TEST_ASSERT_EQUAL(counter, 1);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sanity);

    return UNITY_END();
}
