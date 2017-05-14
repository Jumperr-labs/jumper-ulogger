# Jumper uLogger
Jumper uLogger is a lightweight and extensible logging and analytics tool designed with embedded development in mind, with a tiny footprint.

## Documentation
http://docs.jumper.io/ulogger/index.html

## How to use uLogger
uLogger is similar to other logging frameworks from languages such as python:
1. Declare a uLogger struct, such as `uLogger logger`
2. Initialize it with any number of handlers - "print to uart" handlers, "send to network" handlers, the sky is the limit! (Many handlers will come pre-configured for specific platforms).
3. You can now start logging! 

## Samples and platforms
Currently we support nRF52 logging with a GATT service out of the box. Samples and further documentation available [here](https://github.com/Jumperr-labs/jumper-ulogger/tree/master/platforms/nrf52).
