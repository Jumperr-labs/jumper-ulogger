#include <string.h>
#include "ubuffer.h"
#include "logging_config.h"

#define START_OF_EMPTY_BYTES(buffer) (buffer->capacity - buffer->num_empty_bytes_at_end)

uBufferErrorCode ubuffer_init(uBuffer *ubuffer, char *start, size_t buffer_capacity) {
    ubuffer->start = start;
    ubuffer->head = 0;
    ubuffer->size = 0;
    ubuffer->capacity = buffer_capacity;
    ubuffer->num_empty_bytes_at_end = 0;
    return UBUFFER_SUCCESS;
}

uBufferErrorCode ubuffer_allocate_next(uBuffer *ubuffer, void **item, size_t item_size) {
    uBufferErrorCode err_code = UBUFFER_SUCCESS;
    size_t item_location;

    ULOGGER_ENTER_CRITICAL_SECTION();
    if (ubuffer->size + item_size > ubuffer->capacity) {
        err_code = UBUFFER_FULL;
    } else {
        item_location = (ubuffer->head + ubuffer->size) % ubuffer->capacity;

        if (item_location + item_size > ubuffer->capacity) {
            // Add empty bytes at the end of the buffer
            size_t new_num_empty_bytes_at_end, new_buffer_size;
            new_num_empty_bytes_at_end = ubuffer->capacity - (ubuffer->head + ubuffer->size);
            new_buffer_size = ubuffer->size + new_num_empty_bytes_at_end;
            if (new_buffer_size + item_size > ubuffer->capacity) {
                err_code = UBUFFER_FULL;
            } else {
                ubuffer->num_empty_bytes_at_end = new_num_empty_bytes_at_end;
                ubuffer->size = new_buffer_size;
                item_location = 0;
            }
        }
        if (err_code == UBUFFER_SUCCESS) {
            ubuffer->size += item_size;
            *item = (void*) (ubuffer->start + item_location);
        }
    }

    ULOGGER_EXIT_CRITICAL_SECTION();

    return err_code;
}

uBufferErrorCode ubuffer_free_first(uBuffer *ubuffer, void **item, size_t item_size) {
    uBufferErrorCode err_code = UBUFFER_SUCCESS;

    ULOGGER_ENTER_CRITICAL_SECTION();

    err_code = ubuffer_peek_first(ubuffer, item, item_size);
    if (err_code == UBUFFER_SUCCESS) {
        ubuffer->head += item_size;
        ubuffer->size -= item_size;

        if (START_OF_EMPTY_BYTES(ubuffer) <= ubuffer->head) {
            ubuffer->head = 0;
            ubuffer->size -= ubuffer->num_empty_bytes_at_end;
            ubuffer->num_empty_bytes_at_end = 0;
        }

        if (ubuffer->size == 0) {
            ubuffer->head = 0;
            ubuffer->num_empty_bytes_at_end = 0;
        }

    }

    ULOGGER_EXIT_CRITICAL_SECTION();
    return err_code;
}

uBufferErrorCode ubuffer_peek_first(uBuffer *ubuffer, void **item, size_t item_size) {
    if (item_size != 0 && (ubuffer->size < item_size ||
            START_OF_EMPTY_BYTES(ubuffer) <= ubuffer->head + item_size - 1)) {
        return UBUFFER_EMPTY;
    }

    *item = (void*) (ubuffer->start + ubuffer->head);

    return UBUFFER_SUCCESS;
}
