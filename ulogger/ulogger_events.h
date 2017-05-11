#ifndef ULOGGER_EVENTS_H
#define ULOGGER_EVENTS_H

#define DEVICE_STARTED_EVENT    0
#define RADIO_STATE_EVENT       1
#define ADVERTISING_STATE_EVENT 2
#define ULOGGER_BATTERY_EVENT   3
#define BLE_STATE_EVENT         4

typedef struct {
    uint8_t is_on;
} radio_state_event_data_t;

typedef struct {
    uint8_t is_on;
} advertising_state_event_t;

typedef struct {
    uint8_t level;
} battery_state_event_data_t;

typedef struct {
    uint8_t connected;
} ble_state_event_t;

#define PLATFORM_EVENTS_START   10000

#define USER_EVENTS_START       1000000000

#endif