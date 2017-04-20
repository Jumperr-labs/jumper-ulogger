#include "unity_fixture.h"

TEST_GROUP_RUNNER(TestLogger) {
    RUN_TEST_CASE(TestLogger, Test_Sanity);
}

TEST_GROUP_RUNNER(TestUbuffer) {
    RUN_TEST_CASE(TestUbuffer, Test_Init);
    RUN_TEST_CASE(TestUbuffer, Test_Allocate);
    RUN_TEST_CASE(TestUbuffer, Test_Free);
    RUN_TEST_CASE(TestUbuffer, Test_Full);
    RUN_TEST_CASE(TestUbuffer, Test_Empty);
    RUN_TEST_CASE(TestUbuffer, Test_Circular_Tail);
    RUN_TEST_CASE(TestUbuffer, Test_Circular_Head);
    RUN_TEST_CASE(TestUbuffer, Test_Free_Size_0);
    RUN_TEST_CASE(TestUbuffer, Test_Free_Size_0_When_Empty);
    RUN_TEST_CASE(TestUbuffer, Test_Free_Item_Too_Large);
    RUN_TEST_CASE(TestUbuffer, Test_Free_Item_Too_Large_With_Empty_Bytes_At_End);
    RUN_TEST_CASE(TestUbuffer, Test_Peek);
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
