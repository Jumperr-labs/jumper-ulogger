#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "unity_fixture.h"
#include <ubuffer.h>

#define BUFFER_CAPACITY 15
#define MAX_INT_TYPE_IN_BUFFER (BUFFER_CAPACITY / sizeof(int))
TEST_GROUP(TestUbuffer);

static char ubuffer[SIZEOF_UBUFFER];
static uBuffer *ubuffer_handle;
static char *buffer_memory;

void fill_buffer() {
    int *item;
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(UBUFFER_SUCCESS, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL((int *) buffer_memory + i, item);
        TEST_ASSERT_EQUAL((i + 1) * sizeof(int), ubuffer_handle->size);
        *item = i;
    };
    TEST_ASSERT_EQUAL(MAX_INT_TYPE_IN_BUFFER * sizeof(int), ubuffer_handle->size);
}

void empty_a_full_buffer(int first_value_in_buffer) {
    int *item;

    //    Pop everything out
    for (int i = 0; i < MAX_INT_TYPE_IN_BUFFER; i++) {
        TEST_ASSERT_EQUAL(UBUFFER_SUCCESS, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
        TEST_ASSERT_EQUAL(*item, i + first_value_in_buffer);
    }

    //    Try to pop an item when the buffer is empty
    TEST_ASSERT_EQUAL(UBUFFER_EMPTY, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(0, ubuffer_handle->size);
}

TEST_SETUP(TestUbuffer) {
    buffer_memory = (char*) malloc(BUFFER_CAPACITY);
    ubuffer_init(&ubuffer, buffer_memory, BUFFER_CAPACITY);
    ubuffer_handle = (uBuffer*) ubuffer;
}

TEST_TEAR_DOWN(TestUbuffer) {
    free(buffer_memory);
}

TEST(TestUbuffer, Test_Init) {
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, ubuffer_handle->capacity);
    TEST_ASSERT_EQUAL(buffer_memory, ubuffer_handle->start);
    TEST_ASSERT_EQUAL(0, ubuffer_handle->head);
    TEST_ASSERT_EQUAL(0, ubuffer_handle->size);
    TEST_ASSERT_EQUAL(0, ubuffer_handle->num_empty_bytes_at_end);
}

TEST(TestUbuffer, Test_Push) {
    int *item;
    ubuffer_push(&ubuffer, (void **) &item, sizeof(int));
    *item = 10;
    TEST_ASSERT_EQUAL(ubuffer_handle->start, item);
    TEST_ASSERT_EQUAL(sizeof(int), ubuffer_handle->size);
}

TEST(TestUbuffer, Test_Pop) {
    int *item_pushed, *item_poped;
    ubuffer_push(&ubuffer, (void **) &item_pushed, sizeof(int));
    ubuffer_pop(&ubuffer, (void **) &item_poped, sizeof(int));
    TEST_ASSERT_EQUAL(item_pushed, item_poped);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, ubuffer_handle->capacity);
    TEST_ASSERT_EQUAL(buffer_memory, ubuffer_handle->start);
    TEST_ASSERT_EQUAL(sizeof(int), ubuffer_handle->head);
    TEST_ASSERT_EQUAL(0, ubuffer_handle->size);
}

TEST(TestUbuffer, Test_Full) {
    int *item;
    uBuffer ubuffer_clone;
    char *data_clone[BUFFER_CAPACITY];

    fill_buffer();

    memcpy((char*) &ubuffer_clone, (char*) &ubuffer, sizeof(ubuffer));
    memcpy((char*) data_clone, buffer_memory, BUFFER_CAPACITY);

    TEST_ASSERT_EQUAL(UBUFFER_FULL, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL_MEMORY(&ubuffer_clone, &ubuffer, sizeof(ubuffer));
    TEST_ASSERT_EQUAL_MEMORY(data_clone, buffer_memory, BUFFER_CAPACITY);
}

TEST(TestUbuffer, Test_Empty) {
    int *item;
    uBuffer ubuffer_clone;
    char *data_clone[BUFFER_CAPACITY];

    memcpy((char*) &ubuffer_clone, (char*) &ubuffer, sizeof(ubuffer));
    memcpy((char*) data_clone, buffer_memory, BUFFER_CAPACITY);

    TEST_ASSERT_EQUAL(UBUFFER_EMPTY, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL_MEMORY(&ubuffer_clone, &ubuffer, sizeof(ubuffer));
    TEST_ASSERT_EQUAL_MEMORY(data_clone, buffer_memory, BUFFER_CAPACITY);
}

TEST(TestUbuffer, Test_Circular_Tail) {
    int *item;

//    Fill the buffer to the maximum amount of integers possible
    fill_buffer();

//    Take one item out
    TEST_ASSERT_EQUAL(UBUFFER_SUCCESS, ubuffer_pop(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(0, *item);

//    Add one item (first cyclic function)
    TEST_ASSERT_EQUAL(UBUFFER_SUCCESS, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY % sizeof(int), ubuffer_handle->num_empty_bytes_at_end);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, ubuffer_handle->size);
    *item = MAX_INT_TYPE_IN_BUFFER;
    TEST_ASSERT_EQUAL(buffer_memory, item);

//    Try to add one more item when buffer is full
    TEST_ASSERT_EQUAL(UBUFFER_FULL, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));

    empty_a_full_buffer(1);
}

TEST(TestUbuffer, Test_Circular_Head) {
    int *item;

//  Bring the head to the end (minus num_empty_bytes_at_end) of the buffer
    fill_buffer();
    empty_a_full_buffer(0);

    TEST_ASSERT_EQUAL(UBUFFER_SUCCESS, ubuffer_push(&ubuffer, (void **) &item, sizeof(int)));
    TEST_ASSERT_EQUAL(buffer_memory, item);
    TEST_ASSERT_EQUAL(sizeof(int), ubuffer_handle->size);
}
