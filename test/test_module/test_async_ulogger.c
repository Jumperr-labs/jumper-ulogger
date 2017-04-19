#include "unity.h"
#include "unity_fixture.h"
#include "mock_user_handlers.h"
#include "ulogger_async.h"

#define LOGGER_CAPACITY_BYTES 64
#define LOGGER_CAPACITY_EVENTS LOGGER_CAPACITY/sizeof(LoggingEvent)

TEST_GROUP(TestAsyncLogger);

static char* logger_memory;

TEST_SETUP(TestAsyncLogger) {
    logger_memory = (char*) malloc(LOGGER_CAPACITY_BYTES);
    init_user_handlers();
}

TEST_TEAR_DOWN(TestAsyncLogger) {
}

TEST(TestAsyncLogger, Test_Sanity) {
    uLoggerAsync async_logger;

    ulogger_async_init(&async_logger, logger_memory, LOGGER_CAPACITY_BYTES, log_handlers, handler_data, (size_t) NUM_HANDLERS);

    ulogger_async_log_event(&async_logger, START_RADIO);
    ulogger_async_flush(&async_logger);
    test_counters(0);
    #define NUM_EVENTS_TO_ADD 3
    for (int i=0; i<NUM_EVENTS_TO_ADD; i++) {
        ulogger_async_log_event(&async_logger, START_RADIO);
        ulogger_async_log_event(&async_logger, START_RADIO);
        ulogger_async_log_event(&async_logger, START_RADIO);
    }
    test_counters(0);
    ulogger_async_flush(&async_logger);
    test_counters(NUM_EVENTS_TO_ADD);
}
