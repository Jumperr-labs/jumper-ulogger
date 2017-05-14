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

## Porting
Check out our [porting guide](https://github.com/Jumperr-labs/jumper-ulogger/blob/master/porting.md)
