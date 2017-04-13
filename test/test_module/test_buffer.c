#include <stdint.h>
#include <stdio.h>
#include <ulogger.h>
#include <log_buffer.h>
#include "unity.h"
#include "unity_fixture.h"
#include "log_buffer.h"

#define MAX_EVENTS 3
#define BUFFER_CAPACITY MAX_EVENTS*sizeof(LoggingEvent)
#define DEFAULT_EVENT {.time = 5, .event_type = STOP_RADIO}
#define PUSH_DEFAULT_EVENT log_buffer_push_event(&buffer, event.event_type, event.time);

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
}

TEST(TestBuffer, Test_Push) {
    LoggingEvent event = DEFAULT_EVENT;
    PUSH_DEFAULT_EVENT;
    TEST_ASSERT_EQUAL_MEMORY(&event, buffer.head, sizeof(LoggingEvent));
    TEST_ASSERT_EQUAL(sizeof(LoggingEvent), buffer.size);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(LoggingEvent), buffer.tail);
}

TEST(TestBuffer, Test_Pop) {
    LoggingEvent event = DEFAULT_EVENT;
    PUSH_DEFAULT_EVENT;
    log_buffer_push_head(&buffer, sizeof(LoggingEvent));
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, buffer.capacity);
    TEST_ASSERT_EQUAL(buffer_memory, buffer.start);
    TEST_ASSERT_EQUAL(buffer_memory + BUFFER_CAPACITY - 1, buffer.end);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(LoggingEvent), buffer.head);
    TEST_ASSERT_EQUAL(buffer_memory + sizeof(LoggingEvent), buffer.tail);
    TEST_ASSERT_EQUAL(0, buffer.size);
}

TEST(TestBuffer, Test_Alligned_Circle) {
    LoggingEvent event = DEFAULT_EVENT;

    for (int i=0; i < MAX_EVENTS; i++) PUSH_DEFAULT_EVENT;
    TEST_ASSERT_EQUAL(buffer.start, buffer.tail);

    log_buffer_push_head(&buffer, sizeof(LoggingEvent));
    TEST_ASSERT_EQUAL(buffer.start + sizeof(LoggingEvent), buffer.head);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY - sizeof(LoggingEvent), buffer.size);

    PUSH_DEFAULT_EVENT;
    TEST_ASSERT_EQUAL(buffer.head, buffer.tail);
    TEST_ASSERT_EQUAL(BUFFER_CAPACITY, buffer.size);

    for (int i=0; i < MAX_EVENTS; i++) log_buffer_push_head(&buffer, sizeof(LoggingEvent));
    TEST_ASSERT_EQUAL(0, buffer.size);
    TEST_ASSERT_EQUAL(buffer.tail, buffer.head);
}

