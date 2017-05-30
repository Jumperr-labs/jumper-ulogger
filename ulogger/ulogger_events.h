#ifndef ULOGGER_EVENTS_H
#define ULOGGER_EVENTS_H

#include "compilers.h"

#define DEVICE_BOOT_EVENT       0
#define RADIO_STATE_EVENT       1
#define ADVERTISING_STATE_EVENT 2
#define ULOGGER_BATTERY_EVENT   3
#define BLE_STATE_EVENT         4
#define WLAN_EVENT              5
#define CC3XXX_PING_REPORT      6

typedef struct {
    uint8_t is_on;
} __PACKED radio_state_event_data_t;

typedef struct {
    uint8_t is_on;
} __PACKED advertising_state_event_t;

typedef struct {
    uint8_t level;
} __PACKED battery_state_event_data_t;

typedef struct {
    uint8_t connected;
} __PACKED ble_state_event_t;

typedef struct {
    uint8_t is_connected;
    char bssid[33]; //should be configured according to SSID_LEN_MAX
} wlan_event_t;

#define PLATFORM_EVENTS_START   10000

#define USER_EVENTS_START       1000000000

#endif
