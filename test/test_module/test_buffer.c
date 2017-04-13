#include <stdio.h>
#include "unity.h"
#include "unity_fixture.h"
#include "log_buffer.h"

#define BUFFER_CAPACITY 10
#define MAX_INT_TYPE_IN_BUFFER BUFFER_CAPACITY / sizeof(int)
TEST_GROUP(TestBuffer);

static LogBuffer buffer;
static char* buffer_memory;

TEST_SETUP(TestBuffer) {
    buffer_memory = (char*) malloc(BUFFER_CAPACITY);
    log_buffer_init(&buffer, buffer_memory, BUFFER_CAPACITY);
}

TEST_TEAR_DOWN(TestBuffer) {
    free(buffer_memory);
}

TEST(TestBuffer, Test_Init) {
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, buffer.capacity);
    TEST_ASSERT_EQUAL(buffer_memory, buffer.start);
    TEST_ASSERT_EQUAL(buffer_memory + BUFFER_CAPACITY - 1, buffer.end);
    TEST_ASSERT_EQUAL(buffer_memory, buffer.head);
    TEST_ASSERT_EQUAL(buffer_memory, buffer.tail);
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(buffer.num_empty_bytes_at_end, 0);
}

TEST(TestBuffer, Test_Push) {
    int *item;
    log_buffer_push(&buffer, (void**) &item, sizeof(int));
    *item = 10;
    TEST_ASSERT_EQUAL(buffer.head, item);
    TEST_ASSERT_EQUAL(sizeof(int), buffer.size);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(int), buffer.tail);
}

TEST(TestBuffer, Test_Pop) {
    int *item_pushed, *item_poped;
    log_buffer_push(&buffer, (void**) &item_pushed, sizeof(int));
    log_buffer_pop(&buffer, (void**) &item_poped, sizeof(int));
    TEST_ASSERT_EQUAL(item_pushed, item_poped);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, buffer.capacity);
    TEST_ASSERT_EQUAL(buffer_memory, buffer.start);
    TEST_ASSERT_EQUAL(buffer_memory + BUFFER_CAPACITY - 1, buffer.end);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(int), buffer.head);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(int), buffer.tail);
    TEST_ASSERT_EQUAL(0, buffer.size);
}

TEST(TestBuffer, Test_Circular) {
    int *item;

//    Fill the buffer to the maximum amount of integers possible
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(0, log_buffer_push(&buffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL((int *) buffer_memory + i, item);
        TEST_ASSERT_EQUAL((i + 1) * sizeof(int), buffer.size);
        *item = i;
    };
    TEST_ASSERT_EQUAL(MAX_INT_TYPE_IN_BUFFER * sizeof(int), buffer.size);

//    Take one item out
    TEST_ASSERT_EQUAL(0, log_buffer_pop(&buffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(0, *item);

//    Add one item (first cyclic function)
    TEST_ASSERT_EQUAL(0, log_buffer_push(&buffer, (void **) &item, sizeof(int)));
    *item = MAX_INT_TYPE_IN_BUFFER;
    TEST_ASSERT_EQUAL(buffer_memory, item);

//    Pop everything out
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(0, log_buffer_pop(&buffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL(*item, i + 1);
    }
}
