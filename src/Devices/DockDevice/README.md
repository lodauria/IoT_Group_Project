# Dock device

More details about the dock devices are reported below.

## Hardware wiring

![Hardware monitor connection](../../../resources/images/dock_device_connection.png)

In the prototype we used a small LED to simulate the light on the docking spot. The boat presence is detect using our [special cleat](../../../resources/cleat%203D%20model) equipped with a button. All the components are connected to the main board (the STM32 Nucleo).

## How to setup

A single board can also be used for multiple docking spots. Each dock is equipped with a LED and a button inside the cleat.
To define the number of docks connected to the board and the connection pins the following lines of code inside [main.c](./main.c) has to be changed.

```c
static const dockSetting_t settings[] = {
        //{DockId, SwitchPin, LedPin}
        {1, GPIO_PIN(PORT_B, 3), GPIO_PIN(PORT_B, 5)},  //D3 - D4
        {2, GPIO_PIN(PORT_B, 4), GPIO_PIN(PORT_B, 10)}, //D5 - D6
        {3, GPIO_PIN(PORT_A, 8), GPIO_PIN(PORT_A, 9)}   //D7 - D8
};
```

## How to build

To build a version compatible with ETHOS run this command:
```
make ETHOS=1
```
This version automatically setup the TAP interface on the PC.

Without the parameter `ETHOS=1` we build the version with LoRaWAN connection.

If we use the option `FAKE=1`, the boat detection sensor output will be replaced with dummy random data.
You can also use these options together, e.g.:

```
make ETHOS=1 FAKE=1
```

## Network interface

For the tests with real hardware the STM32 Nucleo board is connected to internet through the PC using ETHOS.

For the network tests with IoT-Lab the board will be replaced with a [ST-LRWAN1](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) that has a LoRaWAN connection.

## Network messages
On the ETHOS version the received and sent messages are MQTT messages, while in the LoRaWAN version, they are LoRaWAN messages, and they are received and sent to AWS IoT Core through the MQTT broker of The Things Network

The messages if a boat is detected or not are published<sup>*1</sup> on the topic `dock/boat` with this format:

```
{
    "dock_id": 13,
    "event": "0"
}
```
`dock_id` identifies the sender,
`event` has value 1 if the boat leaves the docking spot, 0 otherwise.

To turn on the LED when the boat has to reach the docking spot, the board subscribes<sup>*2</sup> to the topic `dock/assign` and accepts messages with this format:
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

<sup>*1</sup> In case of ETHOS the device publish directly the messages with MQTT, while when we use LoRaWAN the messages are sent to The Things Network and its MQTT broker publish the messages to the AWS IoT Core.

<sup>*2</sup> In case of ETHOS the subscription is done at device level, while when we use LoRaWAN the subscription is done on The Things Network MQTT broker.