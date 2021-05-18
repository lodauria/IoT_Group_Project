## HARDWARE WIRING

![Hardware monitor connection](./../../../resources/images/Monitor%20connection.png)
The monitor chosen for the prototype is an OLED screen with a resolution of 128x64 driven by an SSD1306.
It uses the I2C bus for communication with the main board, that is an STM nucleo-f401re.

## HOW TO BUILD

To build a version compatible with ETHOS run this command
```
make ETHOS=1
```
this version by default has monitor_id 3 and automatically setup TAP on your pc

Without ETHOS=1 parameter will build a version with 6LoWPAN connection
```
make ETHOS_BAUDRATE=500000 DEFAULT_CHANNEL=17
```
You have to use the same channel of the [border router](https://github.com/RIOT-OS/RIOT/tree/master/examples/gnrc_border_router)

Once the application is launched, you have to connect to the MQTTS broker, entering
this command in the serial interface (only in the 6LoWPAN version)

```
connect <ip_address_of_mqtts_broker> <node_id>
```

## NETWORK INTERFACE

For the tests with real hardware the nucleo board is connected to the internet connection of the pc using ETHOS.

For the network tests with IoT-Lab the board will be replaced with a IoT-Lab m3 that has a 6LoWPAN connection.

## MQTT TOPICS
The monitors to show the signage are subscribed to the topic `assignment` and they accept messages with this format
```
{
"dock_id": 13,
"boat_id":"TS57845"
}
```
`dock_id` The dock assigned to the boat

`boat_id` The plate of the boat