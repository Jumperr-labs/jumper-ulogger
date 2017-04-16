#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

#ifndef UBUFFER
#define UBUFFER

typedef struct {
    char *start;
    char *end;
    char *head;
    char *tail;
    size_t size;
    size_t capacity;
    size_t num_empty_bytes_at_end;
} uBuffer;

typedef enum {
    UBUFFER_SUCCESS = 0,
    UBUFFER_FULL,
    UBUFFER_EMPTY,
} uBuffer_Error_Code;

uBuffer_Error_Code ubuffer_init(uBuffer *ubuffer, char *start, size_t buffer_capacity);

uBuffer_Error_Code ubuffer_push(uBuffer *ubuffer, void **item, size_t item_size);

uBuffer_Error_Code ubuffer_pop(uBuffer *ubuffer, void **item, size_t item_size);


#endif //UBUFFER