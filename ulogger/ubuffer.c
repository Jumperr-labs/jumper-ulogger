#include <string.h>
#include "ubuffer.h"

#define START_OF_EMPTY_BYTES(buffer) buffer->end - buffer->num_empty_bytes_at_end + 1

uBuffer_Error_Code ubuffer_init(uBuffer *ubuffer, char *start, size_t buffer_capacity) {
    ubuffer->start = start;
	ubuffer->end = start + buffer_capacity - 1;
	ubuffer->head = start;
	ubuffer->tail = start;
    ubuffer->size = 0;
	ubuffer->capacity = buffer_capacity;
    ubuffer->num_empty_bytes_at_end = 0;
    return UBUFFER_SUCCESS;
}

uBuffer_Error_Code ubuffer_push(uBuffer *ubuffer, void **item, size_t item_size) {
    if (ubuffer->size + item_size > ubuffer->capacity) {
        return UBUFFER_FULL;
    }

    if (ubuffer->tail + item_size - 1 > ubuffer->end) {
        size_t num_empty_bytes_at_end, new_buffer_size;
        num_empty_bytes_at_end = ubuffer->end - ubuffer->tail + 1;
        new_buffer_size = ubuffer->size + ubuffer->num_empty_bytes_at_end;
        if (new_buffer_size + item_size > ubuffer->capacity) {
            return UBUFFER_FULL;
        }

        ubuffer->num_empty_bytes_at_end = num_empty_bytes_at_end;
        ubuffer->size = new_buffer_size;
        ubuffer->tail = ubuffer->start;
    }

    *item = (void*) ubuffer->tail;

    ubuffer->tail += item_size;
    ubuffer->size += item_size;

    if (ubuffer->tail > ubuffer->end) {
        ubuffer->tail = ubuffer->start;
    }

    return UBUFFER_SUCCESS;
}

uBuffer_Error_Code ubuffer_pop(uBuffer *ubuffer, void **item, size_t item_size) {
    if (ubuffer->size < item_size) {
        return UBUFFER_EMPTY;
    }

    *item = (void*) ubuffer->head;

    ubuffer->head += item_size;

    if (START_OF_EMPTY_BYTES(ubuffer) <= ubuffer->head) {
        ubuffer->head = ubuffer->start;
        ubuffer->num_empty_bytes_at_end = 0;
    }

    ubuffer->size -= item_size;

    return UBUFFER_SUCCESS;
}
