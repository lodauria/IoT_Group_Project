### HOW TO BUILD

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