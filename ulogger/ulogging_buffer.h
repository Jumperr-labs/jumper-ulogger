#include "ubuffer.h"

#ifndef JUMPER_ULOGGER_ULOGGING_BUFFER_H
#define JUMPER_ULOGGER_ULOGGING_BUFFER_H

#ifndef NUM_OF_LOGGING_BUFFERS
#define NUM_OF_LOGGING_BUFFERS 1
#endif

uLoggingBuffer ulogging_buffers[NUM_OF_LOGGING_BUFFERS];

ulogging_buffer_init(ulogging_buffer, start, buffer_capacity);

void push_event_to_buffer(EventType event_type, timestamp time, ...);



#endif //JUMPER_ULOGGER_ULOGGING_BUFFER_H