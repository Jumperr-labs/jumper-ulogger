# Jumper µlogger - Nordic nRF52 Sample Project
This sample project is the best way to get you started with Jumper Insights for the Nordic nRF52.

## Prerequisites
- A Linux gateway
   - Make sure gatttool is installed. (In Ubuntu you can run - sudo apt-get install bluez)
- nRF52 based BLE peripheral
- Compling computer with:
  - [nRF5 SDK v13.0.0](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK)
  - [nrfjproj](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52832)
  - [jlinkARM](https://www.segger.com/downloads/jlink)

## Getting Started
### First time setup
#### Gateway setup
- Install Jumper's BLE Logger on your gateway:
`sudo -H pip install https://github.com/Jumperr-labs/jumper-ble-logger/archive/0.0.18.tar.gz`
- Create the following blank file on your gateway - `/etc/jumper_logging_agent/config.json`
#### BLE peripheral setup
- Set the following variable `export NRF52_SDK=PATH/TO/NRF5_SDK_13.0.0` on the compiling computer
- Compile this project: `make`
- Connect device to the computer
- Flash `softdevice` to the nRF52 board: `make flash_softdevice`
- Flash the project's binary to the nRF52 board: `make flash`
- Reset your device
#### Insights dashboard setup
- Create an account on https://app-events.jumper.io/
- Create a new project
- Under Manage Projects, find your new project and click it's settings button
- Copy the token and project ID accroding to the instructions 

### See it in Action
#### BLE peripheral
- Plug your nRF52 board
#### Gateway 
- Run `hciconfig` to see the currently connected HCI devices. In this tutorial we assume you only have `hci0` at the moment.
- Make sure Jumper's Logging Agent is running
`sudo service jumper-agent start`
- Start the BLE Logger: `sudo service jumper-ble start`
- Run `hciconfig` again to see the newly created HCI device `hci1`. We'll assume hci1 is the newly created HCI device.
- Make sure when running `hciconfig` that both hci0 and hci1 are UP and RUNNING. If not run - `sudo hciconfig hciX up`.ning
- Start gatttool. We will use gatttool to act as a gateway program running on your Linux device. `sudo gatttool -b <BLE_peripheral_mac_address> -t random -i hci1 -I`
   - Scan for the BLE peripheral MAC address - `sudo hcitool -i hci1 lesacn`. The device with JumperSampleApp name is our BLE peripheral.
- Inside gatttool, connect to the BLE peripheral `connect` and discover characteristics by running `characteristics`.
- At this stage the logging data will pass through to your Insights project dashboard on https://app-events.jumper.io/ .
- You will notice that your gateway process (in this case gatttool) will not be notified on logging events and will work the same as it would if the BLE Logger wasn't running.
#### Insights dashboard 
- Go to your project's dashboard on https://app-events.jumper.io/ .

### Want More?
Check out our support for [nRF52](https://github.com/Jumperr-labs/jumper-ulogger/tree/master/platforms/nrf52) and the [nrf52-ble-template project](https://github.com/Jumperr-labs/jumper-ulogger/tree/master/samples/nrf52-ble-template) to implement this on your own project.

We are happy to help! Feel free to contact us about any issue and give us your feedback at [info@jumper.io](mailto:info@jumper.io)
