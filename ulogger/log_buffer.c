#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "log_buffer.h"

void log_buffer_init(LogBuffer* buffer, char* start, size_t buffer_capacity) {
    assert(buffer_capacity % sizeof(int) == 0);
    buffer->start = start;
	buffer->end = start + buffer_capacity - 1;
	buffer->head = start;
	buffer->tail = start;
    buffer->size = 0;
	buffer->capacity = buffer_capacity;
}

void log_buffer_push_event(LogBuffer* buffer, EventType event_type, timestamp time, ...) {
    LoggingEvent *event;
    assert(sizeof(EventType) % sizeof(int) == 0);
    if (buffer->size + sizeof(LoggingEvent) > buffer->capacity) {
//        TODO: ERROR
    }

    if (buffer->tail + sizeof(LoggingEvent) - 1 > buffer->end) {
        memset(buffer->tail, NO_EVENT, 1 + (int*) (buffer->end - buffer->tail));
        buffer->size += buffer->end - buffer->tail;
        buffer->tail = buffer->start;
        if (buffer->size + sizeof(LoggingEvent) > buffer->capacity) {
//        TODO: ERROR
        }
    }

    event = (LoggingEvent*) buffer->tail;

    event->event_type = event_type;
    event->time = time;

    buffer->tail += sizeof(LoggingEvent);
    buffer->size += sizeof(LoggingEvent);

    if (buffer->tail > buffer->end) {
        buffer->tail = buffer->start;
    }
}

void log_buffer_push_head(LogBuffer *buffer, size_t size_to_move) {
    if (buffer->size < size_to_move) {
//        TODO: ERROR
    }

    if (buffer->head + size_to_move > buffer->end) {
        buffer->head = buffer->start + size_to_move - (1 + buffer->end - buffer->head);
    } else {
        buffer->head += size_to_move;
    }

    buffer->size -= size_to_move;
}
