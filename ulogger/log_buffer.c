#include <string.h>
#include "log_buffer.h"

#define START_OF_EMPTY_BYTES(buffer) buffer->end - buffer->num_empty_bytes_at_end + 1

int log_buffer_init(LogBuffer* buffer, char* start, size_t buffer_capacity) {
    buffer->start = start;
	buffer->end = start + buffer_capacity - 1;
	buffer->head = start;
	buffer->tail = start;
    buffer->size = 0;
	buffer->capacity = buffer_capacity;
    buffer->num_empty_bytes_at_end = 0;
    return 0;
}

int log_buffer_push(LogBuffer* buffer, void** item, size_t item_size) {
    if (buffer->size + item_size > buffer->capacity) {
        return 1;
    }

    if (buffer->tail + item_size - 1 > buffer->end) {
        size_t num_empty_bytes_at_end, new_buffer_size;
        num_empty_bytes_at_end = buffer->end - buffer->tail + 1;
        new_buffer_size = buffer->size + buffer->num_empty_bytes_at_end;
        if (new_buffer_size + item_size > buffer->capacity) {
            return 2;
        }

        buffer->num_empty_bytes_at_end = num_empty_bytes_at_end;
        buffer->size = new_buffer_size;
        buffer->tail = buffer->start;
    }

    *item = (void*) buffer->tail;

    buffer->tail += item_size;
    buffer->size += item_size;

    if (buffer->tail > buffer->end) {
        buffer->tail = buffer->start;
    }

    return 0;
}

int log_buffer_pop(LogBuffer *buffer, void** item, size_t item_size) {
    if (buffer->size < item_size) {
        return 1;
    }

    *item = (void*) buffer->head;

    buffer->head += item_size;

    if (START_OF_EMPTY_BYTES(buffer) <= buffer->head) {
        buffer->head = buffer->start;
        buffer->num_empty_bytes_at_end = 0;
    }

    buffer->size -= item_size;

    return 0;
}
