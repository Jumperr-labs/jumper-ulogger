#include <string.h>
#include "ubuffer.h"

#define START_OF_EMPTY_BYTES(buffer) (buffer->capacity - buffer->num_empty_bytes_at_end)

uBuffer_Error_Code ubuffer_init(uBuffer *ubuffer, char *start, size_t buffer_capacity) {
    ubuffer->start = start;
	ubuffer->head = 0;
	ubuffer->size = 0;
	ubuffer->capacity = buffer_capacity;
    ubuffer->num_empty_bytes_at_end = 0;
    return UBUFFER_SUCCESS;
}

uBuffer_Error_Code ubuffer_push(uBuffer *ubuffer, void **item, size_t item_size) {
    size_t item_location;

    if (ubuffer->size + item_size > ubuffer->capacity) {
        return UBUFFER_FULL;
    }

    item_location = ubuffer->head + ubuffer->size;
    if (item_location + item_size > ubuffer->capacity) {
        size_t num_empty_bytes_at_end, new_buffer_size;
        num_empty_bytes_at_end = ubuffer->capacity - (ubuffer->head + ubuffer->size);
        new_buffer_size = ubuffer->size + num_empty_bytes_at_end;
        if (new_buffer_size + item_size > ubuffer->capacity) {
            return UBUFFER_FULL;
        }
        ubuffer->num_empty_bytes_at_end = num_empty_bytes_at_end;
        ubuffer->size = new_buffer_size;
        item_location = 0;
    };

    ubuffer -> size += item_size;
    *item = (void*) ubuffer->start + item_location;

    return UBUFFER_SUCCESS;
}

uBuffer_Error_Code ubuffer_pop(uBuffer *ubuffer, void **item, size_t item_size) {
    if (ubuffer->size < item_size) {
        return UBUFFER_EMPTY;
    }

    *item = (void*) ubuffer->start + ubuffer->head;

    ubuffer->head += item_size;
    ubuffer->size -= item_size;

    if (START_OF_EMPTY_BYTES(ubuffer) <= ubuffer->head) {
        ubuffer->head = 0;
        ubuffer->size -= ubuffer->num_empty_bytes_at_end;
        ubuffer->num_empty_bytes_at_end = 0;
    }

    return UBUFFER_SUCCESS;
}
