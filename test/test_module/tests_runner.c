#include "unity_fixture.h"

TEST_GROUP_RUNNER(TestLogger) {
    RUN_TEST_CASE(TestLogger, Test_Sanity);
}

TEST_GROUP_RUNNER(TestBuffer) {
    RUN_TEST_CASE(TestBuffer, Test_Init);
    RUN_TEST_CASE(TestBuffer, Test_Push);
    RUN_TEST_CASE(TestBuffer, Test_Pop);
    RUN_TEST_CASE(TestBuffer, Test_Alligned_Circle);
}

static void RunAllTests(void)
{
    RUN_TEST_GROUP(TestLogger);
    RUN_TEST_GROUP(TestBuffer);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
