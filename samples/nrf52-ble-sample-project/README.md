# Jumper µlogger - Nordic nRF52 Sample Project
This sample project is the best way to get you started with Jumper Insights for the Nordic nRF52.

## Prerequisites
- A Linux gateway
- nRF52 based BLE peripheral
- [nRF5 SDK v13.0.0](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK)

## Getting Started
### First time setup
- Create an account on [TBD]()
- Install Jumper's BLE Logger on your gateway:
`sudo pip install https://github.com/Jumperr-labs/jumper-ble-logger/archive/v0.0.1.tar.gz`
- Set the following variable `export NRF52_SDK=PATH/TO/NRF5_SDK_13.0.0`
- Compile this project: `make`
- Flash the binary to the nRF52 board: `make flash`

### See it in Action
- Create a new project on [TBD]() and add the generated config file to your system according to the given instruction.
- Run `hciconfig` to see the currently connected HCI devices. In this tutorial we assume you only have `hci0` at the moment.
- Start the BLE Logger on the gateway with the `events_config.json` file of this sample project:
    ```shell
    sudo service jumper-agent start
    sudo jumper-ble-logger -v --hci 0 --config-file events_config.json
    ```
- Run `hciconfig` again to see the newly created HCI device `hci1`
- Start gatttool. We will use gatttool to act as a gateway program running on your Linux device. `gatttool -b <mac_address> -t random -i hci1 -I`
- Inside gatttool, connect to the BLE peripheral `connect` and discover characteristics by running `characteristics`.
- At this stage the logging data will pass through to you dashboard on [TBD]() .
You will notice that your gateway process (in this case gatttool) will not be notified on logging events and will work the same as it would if the BLE Logger wasn't running.


### Want More?
Check out the _"nrf52-ble-template"_ project and _"<this_repo>/platforms/nrf52"_ to implement this on your own project.

We are happy to help! Feel free to contact us about any issue and give us your feedback at [info@jumper.io](mailto:info@jumper.io)
