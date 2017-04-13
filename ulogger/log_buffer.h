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
} LogBuffer;

void log_buffer_init(LogBuffer* buffer, char* start, size_t buffer_capacity);

void log_buffer_push_event(LogBuffer* buffer, EventType event_type, timestamp time, ...);

void log_buffer_push_head(LogBuffer *buffer, size_t size_to_move);


#endif //LOG_BUFFER