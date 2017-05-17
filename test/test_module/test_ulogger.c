#include "unity_fixture.h"
#include "mock_user_handlers.h"
#include "ulogger.h"

TEST_GROUP(TestLogger);

uLogger ulogger;

TEST_SETUP(TestLogger) {
    init_user_handlers();
}

TEST_TEAR_DOWN(TestLogger) {
}

TEST(TestLogger, Test_Sanity) {
    TEST_ASSERT_EQUAL(ULOGGER_SUCCESS, ulogger_init(&ulogger, log_handlers, handler_data, (size_t) NUM_HANDLERS));

    TEST_ASSERT_EQUAL(ULOGGER_SUCCESS, ulogger_log(&ulogger, ULOGGER_INFO, DEVICE_STARTED_EVENT, 0, 0));
    assert_counters(0);
    TEST_ASSERT_EQUAL(ULOGGER_SUCCESS, ulogger_log(&ulogger, ULOGGER_INFO, DEVICE_STARTED_EVENT, 0, 0));
    assert_counters(1);
}
