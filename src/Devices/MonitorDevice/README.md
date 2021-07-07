# Monitor device

More details about the monitor devices are reported below.

## Hardware wiring

![Hardware monitor connection](./../../../resources/images/Monitor%20connection.png)
The monitor chosen for the prototype is an OLED screen with a resolution of 128x64 driven by an SSD1306. It uses the I2C bus for communication with the main board (the STM32 Nucleo).

## How to build

To build a version compatible with ETHOS run this command:
```
make ETHOS=1
```
This version automatically setup the TAP interface on the PC and uses `monitor_id=3` by default.

Without the parameter `ETHOS=1` we build the version with LoRaWAN connection.
## Network interface

For the tests with real hardware the STM32 Nucleo board is connected to internet through the PC using ETHOS.

For the network tests with IoT-Lab the board will be replaced with a [ST-LRWAN1](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) that has a LoRaWAN connection.

## MQTT messages
To show the signage the devices are subscribed to the topic `dock/assign` and they accept messages with this format:
```
{
"boat_id":"TS57845",
"dock_num": "13",
"event":"0"
}
```
`boat_id` is the license plate of the boat,
`dock_num` is the dock assigned to the boat,
`event = "0"` means that the boat is entering in the marina.