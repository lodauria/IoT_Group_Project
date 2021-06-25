## HARDWARE WIRING

![Hardware monitor connection](../../../resources/images/dock_device_connection.png)

In the prototype the light is replaced with a small led and to detect if a boat is present
is used an ultrasonic range meter.

## HOW TO SETUP

A single board can be used for multiple docks. Each dock has a led and a switch.
To define the number of dock connected to the board and the connection pins these lines of code inside [main.c](./main.c) should be changes.

```c
static const dockSetting_t settings[] = {
        //{DockId, SwitchPin, LedPin}
        {1, GPIO_PIN(PORT_B, 3), GPIO_PIN(PORT_B, 5)},  //D3 - D4
        {2, GPIO_PIN(PORT_B, 4), GPIO_PIN(PORT_B, 10)}, //D5 - D6
        {3, GPIO_PIN(PORT_A, 8), GPIO_PIN(PORT_A, 9)}   //D7 - D8
};
```

## HOW TO BUILD

To build a version compatible with ETHOS run this command
```
make ETHOS=1
```
this version by default has dock_id 13 and automatically setup TAP on your pc

Without ETHOS=1 parameter will build a version with LoRaWAN connection

Building with the option `FAKE=1` the boat detection sensor output will be replaced with dummy random data.
You can use these options together, ex.
```
make ETHOS=1 FAKE=1
```

## NETWORK INTERFACE

For the tests with real hardware the nucleo board is connected to the internet connection of the pc using ETHOS.

For the network tests with IoT-Lab the board will be replaced with a [ST-LRWAN1](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) that has a LoRaWAN connection.

## MQTT TOPICS
The messages if a boat is detected or not are published in the topic `dock/boat` with this format
```
{
  "dock_id": 13,
  "event": 0
}
```
`dock_id` identify the sender.

`event` if it is 1 the boat leaves the dock, 0 otherwise

The devices to turn on the LED when a boat is entering the marina, are subscribed to the topic `dock/assign` and 
they accept messages with this format
```
{
"dock_num": 13,
"boat_id":"TS57845",
"event":"0"
}
```
`dock_id` The dock assigned to the boat
`event = 0` means that a boat entering the marina
`boat_id` The plate of the boat