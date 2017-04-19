#include "unity_fixture.h"

TEST_GROUP_RUNNER(TestLogger) {
    RUN_TEST_CASE(TestLogger, Test_Sanity);
}

TEST_GROUP_RUNNER(TestUbuffer) {
    RUN_TEST_CASE(TestUbuffer, Test_Init);
    RUN_TEST_CASE(TestUbuffer, Test_Push);
    RUN_TEST_CASE(TestUbuffer, Test_Pop);
    RUN_TEST_CASE(TestUbuffer, Test_Circular);
}

static void RunAllTests(void)
{
    RUN_TEST_GROUP(TestLogger);
    RUN_TEST_GROUP(TestUbuffer);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
