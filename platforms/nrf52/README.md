# nRF52 Readme
_INSERT INTRO HERE_
## Relevant code files
Add the following files and include paths to your code:
```
Repository Root
├── README.md
├── platforms
│   └── nrf52
│       ├── README.md
│       ├── gatt_handler.c
│       ├── gatt_handler.h
│       ├── trace_nrf52.c
│       ├── trace_nrf52.h
│       ├── ulogger_nrf52.c
│       └── ulogger_nrf52.h
└── ulogger
    ├── handlers
    │   ├── network_log_handler.c
    │   └── network_log_handler.h
    ├── ubuffer.c
    ├── ubuffer.h
    ├── ulogger.c
    └── ulogger.h
```
## Setup

### Changes to main.c
* Add the following includes to your set of includes:
```
#include "ulogger.h"
#include "ulogger_nrf52.h"
#include "trace_nrf52.h"
```
* Next up, on `ble_stack_init(void)`, increase the UUID count by 1 as we’re adding a GATT message. In Nordic’s example it was set to 0, so we’re increasing to 1.
```
ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = 1;
```
* We need to receive BLE events from the SoftDevice, to do so add the following line in the `ble_evt_dispatch`:
```
ulogger_handle_ble_event(p_ble_evt);
```

### Linking

#### ARMGCC
Moving on to the *.ld file, in our case `ble_app_template_gcc_nrf52.ld`:

From the `Memory.RAM` section, push the origin 0x10 bytes forward and reduce the length by 0x10 bytes. Based on the original example, this means changing this line:
```
RAM (rwx) :  ORIGIN = 0x20001fc0, LENGTH = 0xe040
```
To this:
```
RAM (rwx) :  ORIGIN = 0x20001fd0, LENGTH = 0xe030
```
#### IAR
In your *.icf file (here we used `ble_app_template_iar_nRF5x.icf`, change RAM start by 0x10 bytes, from:
```
define symbol __ICFEDIT_region_RAM_start__   = 0x20001fc0;
```
To this:
```
define symbol __ICFEDIT_region_RAM_start__   = 0x20001fd0;
```
## Initialization
Add a definition for the uLogger struct, we added following code right after the `assert_nrf_callback`:

```
uLogger ulogger;
```

Finally on the `main(void)` function, add the following line when initializing services
```
ulogger_init_nrf52(&ulogger);
```
Note - this has to happen after the SoftDevice is configured and BLE is initialized.
## Tracing
To trace advertising events add the following line to the beginning of the `on_adv_evt` function
```
ulogger_trace_nrf_ble_adv_event(ble_adv_evt);
```
## Configuration
_WIP_