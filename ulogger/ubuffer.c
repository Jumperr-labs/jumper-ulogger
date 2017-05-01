#include <string.h>
#include "ubuffer.h"

#define START_OF_EMPTY_BYTES(buffer) (buffer->capacity - buffer->num_empty_bytes_at_end)

uBufferErrorCode ubuffer_init(void *ubuffer, char *start, size_t buffer_capacity) {
    uBuffer *ubuffer_handle = ubuffer;
    ubuffer_handle->start = start;
    ubuffer_handle->head = 0;
    ubuffer_handle->size = 0;
    ubuffer_handle->capacity = buffer_capacity;
    ubuffer_handle->num_empty_bytes_at_end = 0;
    return UBUFFER_SUCCESS;
}

uBufferErrorCode ubuffer_allocate_next(void *ubuffer, void **item, size_t item_size) {
    uBuffer *ubuffer_handle = ubuffer;
    size_t item_location;

    if (ubuffer_handle->size + item_size > ubuffer_handle->capacity) {
        return UBUFFER_FULL;
    }

    item_location = ubuffer_handle->head + ubuffer_handle->size;
    if (item_location + item_size > ubuffer_handle->capacity) {
        if (ubuffer_handle->size > 0) {
            size_t num_empty_bytes_at_end, new_buffer_size;
            num_empty_bytes_at_end = ubuffer_handle->capacity - (ubuffer_handle->head + ubuffer_handle->size);
            new_buffer_size = ubuffer_handle->size + num_empty_bytes_at_end;
            if (new_buffer_size + item_size > ubuffer_handle->capacity) {
                return UBUFFER_FULL;
            }
            ubuffer_handle->num_empty_bytes_at_end = num_empty_bytes_at_end;
            ubuffer_handle->size = new_buffer_size;
        } else {
            ubuffer_handle->num_empty_bytes_at_end = 0;
            ubuffer_handle->size = 0;
            ubuffer_handle->head = 0;
        }
        item_location = 0;
    };

    ubuffer_handle->size += item_size;
    *item = (void*) (ubuffer_handle->start + item_location);
    return UBUFFER_SUCCESS;
}

uBufferErrorCode ubuffer_free_first(void *ubuffer, void **item, size_t item_size) {
    uBuffer *ubuffer_handle = ubuffer;
    uBufferErrorCode err_code;

    err_code = ubuffer_peek_first(ubuffer, item, item_size);
    if (err_code) return err_code;

    ubuffer_handle->head += item_size;
    ubuffer_handle->size -= item_size;
    if (START_OF_EMPTY_BYTES(ubuffer_handle) <= ubuffer_handle->head) {
        ubuffer_handle->head = 0;
        ubuffer_handle->size -= ubuffer_handle->num_empty_bytes_at_end;
        ubuffer_handle->num_empty_bytes_at_end = 0;
    }

    return UBUFFER_SUCCESS;
}

uBufferErrorCode ubuffer_peek_first(void *ubuffer, void **item, size_t item_size) {
    uBuffer *ubuffer_handle = ubuffer;
    if (item_size != 0 && (ubuffer_handle->size < item_size ||
            START_OF_EMPTY_BYTES(ubuffer_handle) <= ubuffer_handle->head + item_size - 1)) {
        return UBUFFER_EMPTY;
    }

    *item = (void*) (ubuffer_handle->start + ubuffer_handle->head);

    return UBUFFER_SUCCESS;
}
