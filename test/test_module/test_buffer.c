#include <stdio.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ubuffer.h"

#define BUFFER_CAPACITY 10
#define MAX_INT_TYPE_IN_BUFFER BUFFER_CAPACITY / sizeof(int)
TEST_GROUP(TestUbuffer);

static uBuffer ubuffer;
static char* buffer_memory;

TEST_SETUP(TestUbuffer) {
    buffer_memory = (char*) malloc(BUFFER_CAPACITY);
    ubuffer_init(&ubuffer, buffer_memory, BUFFER_CAPACITY);
}

TEST_TEAR_DOWN(TestUbuffer) {
    free(buffer_memory);
}

TEST(TestUbuffer, Test_Init) {
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, ubuffer.capacity);
    TEST_ASSERT_EQUAL(buffer_memory, ubuffer.start);
    TEST_ASSERT_EQUAL(0, ubuffer.head);
    TEST_ASSERT_EQUAL(0, ubuffer.size);
    TEST_ASSERT_EQUAL(0, ubuffer.num_empty_bytes_at_end);
}

TEST(TestUbuffer, Test_Push) {
    int *item;
    ubuffer_push(&ubuffer, (void **) &item, sizeof(int));
    *item = 10;
    TEST_ASSERT_EQUAL(ubuffer.start, item);
    TEST_ASSERT_EQUAL(sizeof(int), ubuffer.size);
}

TEST(TestUbuffer, Test_Pop) {
    int *item_pushed, *item_poped;
    ubuffer_push(&ubuffer, (void **) &item_pushed, sizeof(int));
    ubuffer_pop(&ubuffer, (void **) &item_poped, sizeof(int));
    TEST_ASSERT_EQUAL(item_pushed, item_poped);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, ubuffer.capacity);
    TEST_ASSERT_EQUAL(buffer_memory, ubuffer.start);
    TEST_ASSERT_EQUAL(sizeof(int), ubuffer.head);
    TEST_ASSERT_EQUAL(0, ubuffer.size);
}

TEST(TestUbuffer, Test_Circular) {
    int *item;

//    Fill the buffer to the maximum amount of integers possible
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(0, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL((int *) buffer_memory + i, item);
        TEST_ASSERT_EQUAL((i + 1) * sizeof(int), ubuffer.size);
        *item = i;
    };
    TEST_ASSERT_EQUAL(MAX_INT_TYPE_IN_BUFFER * sizeof(int), ubuffer.size);

//    Take one item out
    TEST_ASSERT_EQUAL(0, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(0, *item);

//    Add one item (first cyclic function)
    TEST_ASSERT_EQUAL(0, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
    *item = MAX_INT_TYPE_IN_BUFFER;
    TEST_ASSERT_EQUAL(buffer_memory, item);

//    Pop everything out
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(0, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL(*item, i + 1);
    }
}
