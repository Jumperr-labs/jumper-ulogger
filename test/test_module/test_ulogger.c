#include <stdint.h>
#include <stdio.h>
#include "unity_fixture.h"
#include "ulogger.h"
#include "mock_user_handlers.h"

TEST_GROUP(TestLogger);

TEST_SETUP(TestLogger) {
    init_user_handlers();
}

TEST_TEAR_DOWN(TestLogger) {
}

TEST(TestLogger, Test_Sanity) {
    uLogger logger;

    ulogger_init(&logger, log_handlers, handler_data, (size_t) NUM_HANDLERS);

    ulogger_log(&logger, START_RADIO);
    test_counters(0);
    ulogger_log(&logger, START_RADIO);
    test_counters(1);
}
