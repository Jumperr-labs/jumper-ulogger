#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

#ifndef LOG_BUFFER
#define LOG_BUFFER

typedef struct {
    char *start;
    char *end;
    char *head;
    char *tail;
    size_t size;
    size_t capacity;
    size_t num_empty_bytes_at_end;
} LogBuffer;

int log_buffer_init(LogBuffer* buffer, char* start, size_t buffer_capacity);

int log_buffer_push(LogBuffer* buffer, void** item, size_t item_size);

int log_buffer_pop(LogBuffer *buffer, void** item, size_t item_size);


#endif //LOG_BUFFER