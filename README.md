# Jumper µLogger
This project is part of the Jumper Insights framework. Visit https://www.jumper.io/insights/ for more information.

Jumper µLogger is a lightweight and extensible logging and analytics tool designed with embedded development in mind, with a tiny footprint.

## Documentation
http://docs.jumper.io

## Samples and platforms
Currently we support nRF52 logging with a GATT service out of the box. Samples and further documentation available [here](https://github.com/Jumperr-labs/jumper-ulogger/tree/master/platforms/nrf52).

## µLogger Events
µLogger is made for logging in embedded software, RTOS or bare-metal firmware. µLogger should be used for logging structured data such as battery level, radio events, interrupts, etc. µLogger should not be used for logging arbitrary data such as customized strings.

Event types and data structures can be easily defined in the _"ulogger_events.h"_ file. Here is an example:
```c
#define ULOGGER_BATTERY_EVENT   3

typedef struct {
    uint8_t level;
} battery_state_event_data_t;
```

## Using the µLogger
- Define your events in the _"ulogger_events.h"_ file (Check it out for examples)
- API for logging events:
  
  `uLoggerErrorCode ulogger_log (void *ulogger, LogLevel level, EventType 	event_type, void *log_data, size_t data_length)`
  
  example:
  ```c
  battery_level.level = 91;
  ulogger_log(&ulogger, ULOGGER_INFO, ULOGGER_BATTERY_EVENT, &battery_level, 1);
  ```

## Porting
Check out our [porting guide](https://github.com/Jumperr-labs/jumper-ulogger/blob/master/porting.md)
