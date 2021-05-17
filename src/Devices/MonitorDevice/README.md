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
make
```
Once the application is launched, you have to connect to the MQTTS broker, entering
this command in the serial interface (only in the 6LoWPAN version)

```
connect <ip_address_of_mqtts_broker> <node_id>
```

## NETWORK INTERFACE

For the tests with real hardware the nucleo board is connected to the internet connection of the pc using ETHOS.

For the network tests with IoT-Lab the board will be replaced with a IoT-Lab m3 that has a 6LoWPAN connection.
