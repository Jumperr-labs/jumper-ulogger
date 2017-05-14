#ifndef KEEN_HANDLER_H
#define KEEN_HANDLER_H

#include <stdint.h>
#include <stdlib.h>
#include "ulogger.h"

HandlerReturnType keen_handler_handle_log(void * handler_data, LogLevel level, EventType event_type, timestamp time, void * data, size_t data_length);

#endif //KEEN_HANDLER_H