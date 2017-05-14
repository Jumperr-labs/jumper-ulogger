# Jumper uLogger
This project is part of the Jumper Insights framework. Visit https://www.jumper.io/insights/ for more information.

Jumper uLogger (pronounced micro-logger) is a lightweight and extensible logging and analytics tool designed with embedded development in mind, with a tiny footprint.

## Documentation
http://docs.jumper.io/ulogger/index.html

## Samples and platforms
Currently we support nRF52 logging with a GATT service out of the box. Samples and further documentation available [here](https://github.com/Jumperr-labs/jumper-ulogger/tree/master/platforms/nrf52).

## Using the uLogger
- Define your events in the _"ulogger_events.h"_ file (Check it out for examples)
- API for logging events:
  
  `uLoggerErrorCode ulogger_log (void *ulogger, LogLevel level, EventType 	event_type, void *log_data, size_t data_length)`
  
  example: `ulogger_log(logger, ULOGGER_INFO, DEVICE_STARTED_EVENT, 0, 0))`

## Porting Guide
If your platform is not supported. You can create a ulogger port for your platform.

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
5. Declare a uLogger struct, such as `uLogger logger`
6. Initialize it with any number of handlers - "print to uart" handlers, "send to network" handlers, the sky is the limit! 
  ```c 
  handler_func log_handlers[0];
  void* handler_data[NUM_HANDLERS];
  
  log_handlers[0] = (handler_func) &log_handler;
  handler_data[0] = NULL;
  ulogger_init(logger, log_handlers, handler_data, (size_t) 1);
  ```
7. You can now start logging!
