# Jumper uLogger - Nordic nRF52 Sample Project
This sample project is the best way to get you started with Jumper Insights for the Nordic nRF52.

## Prerequisites
- A Linux gateway
- nRF52 Development board

## Getting Started

### First time setup
- Create an account on https://www.jumper.io/insights  
- Install Jumper's BLE Logger on your gateway: https://github.com/Jumperr-labs/jumper-ble-logger
- Compile this project and flash it to you nRF52 development board. All you need is the nRF5 SDK.

### See it in Action
- Create a new project on https://www.jumper.io/insights and add the config file to your system.
- Run `hciconfig` to see the currently connected HCI devices. In this tutorial we assume you only have `hci0` at the moment.
- Start the BLE Logger on the gateway:
    ```shell
    sudo service jumper-agent start
    sudo jumper-ble-logger -v
    ```
- Run `hciconfig` again to see the newly created HCI device `hci1`
- Start gatttool. We will use gatttool to act as a gateway program running on your Linux device. `gatttool -b <mac_address> -t random -i hci1 -I`
- Run the following commands inside gatttool:
    ```
    connect
    characteristics
    ```
- At this stage the logging data will pass through to you dashboard on https://www.jumper.io/insights .
You will notice that your gateway process (in this case gatttool) will not be notified on logging events and will work the same as it would if the BLE Logger wasn't running.


### Want More?
Check out the _"nrf52-ble-template"_ project and _"<this_repo>/platforms/nrf52"_ to implement this on your own project.

We are happy to help! Feel free to contact us about any issue and give us your feedback at [info@jumper.io](mailto:info@jumper.io)
