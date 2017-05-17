# Porting Guide
If your platform is not supported. You can create a µlogger port for your platform.

1. Add _"ulogger.h"_ and _"ulogger.c"_ to your project
2. `include "ulogger.h"`
3. Create log handlers:
  ```c
  HandlerReturnType log_handler(void *handler_data, LogLevel level, EventType event_type, timestamp time, void * log_data,      size_t data_length) {
      printf("Received event at %d\n", time);
      return HANDLER_SUCCESS;
  }
  ```
4. Create a timestamp function (varies by platform):
  ```c
  #include <sys/time.h>
  
  void get_timestamp(timestamp *data) {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      *data = (timestamp) tv.tv_sec;
  }
  ```
5. Declare a µlogger struct, such as `uLogger logger`
6. Initialize it with any number of handlers - "print to uart" handlers, "send to network" handlers, the sky is the limit! 
  ```c 
  handler_func log_handlers[0];
  void* handler_data[NUM_HANDLERS];
  
  log_handlers[0] = (handler_func) &log_handler;
  handler_data[0] = NULL;
  ulogger_init(logger, log_handlers, handler_data, (size_t) 1);
  ```
7. You can now start logging!