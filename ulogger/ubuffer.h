#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

#ifndef UBUFFER
#define UBUFFER

typedef struct {
    char *start;
    size_t head;
    size_t size;
    size_t capacity;
    size_t num_empty_bytes_at_end;
} uBuffer;

typedef enum {
    UBUFFER_SUCCESS = 0,
    UBUFFER_FULL,
    UBUFFER_EMPTY,
} uBufferErrorCode;

uBufferErrorCode ubuffer_init(uBuffer *ubuffer, char *start, size_t buffer_capacity);

uBufferErrorCode ubuffer_push(uBuffer *ubuffer, void **item, size_t item_size);

uBufferErrorCode ubuffer_pop(uBuffer *ubuffer, void **item, size_t item_size);


#endif //UBUFFER