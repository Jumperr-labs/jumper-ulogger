#ifndef UBUFFER
#define UBUFFER

#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

typedef struct {
    char *start;
    size_t head;
    size_t size;
    size_t capacity;
    size_t num_empty_bytes_at_end;
} uBuffer;

#define SIZEOF_UBUFFER sizeof(uBuffer)

typedef enum {
    UBUFFER_SUCCESS = 0,
    UBUFFER_FULL,
    UBUFFER_EMPTY
} uBufferErrorCode;

uBufferErrorCode ubuffer_init(void *ubuffer, char *start, size_t buffer_capacity);

uBufferErrorCode ubuffer_allocate_next(void *ubuffer, void **item, size_t item_size);

uBufferErrorCode ubuffer_free_first(void *ubuffer, void **item, size_t item_size);

uBufferErrorCode ubuffer_peek_first(void *ubuffer, void **item, size_t item_size);

#endif //UBUFFER