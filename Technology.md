## Technology

*[First delivery version](https://github.com/kernel-machine/IoTGroupProject/tree/first_assignment/Technology.md)*

*[Second delivery version](https://github.com/kernel-machine/IoTGroupProject/tree/second_assignment/Technology.md)*

## ![Marina](resources/images/harbour.png)

The main macro components are:

- **Boat name identifier** at the marina entrance there is camera used to identify the boat name to check if there is a dock reservation.
- **Screens**, inside the marina there are some screens, useful to give indications at the sailor to find his dock spot.
- **Park sensor**, for each dock there is a sensor to detect if a boat is presents and a LED that blink when a sailor is searching his dock.

### Dock device

For each dock there is a LED (that blink when someone was finding his dock position) and a sonar to detects if the boat is present.
A single stm32 board is used for multiple dock, and it use a MQTT interface to communicate with the marine server.

<img src="resources/images/dock_device.png" alt="Dock device" style="zoom: 50%;" />

![Dock device wiring](resources/images/dock_device_connection.png)

The main components of the prototype of the dock device are:

- The main board STM32 Nucleo F401RE
- Ultrasonic range meter HC-SR04
- LED

[Get more details here](src/Devices/DockDevice)

### Signature screen

![Monitor](resources/images/Monitor%20connection.png)

The prototype of the monitor device is composed by a board:

- Nucleo STM32 F401RE
- OLED display 0.96" 128x64 

The display is driven by a SSD1306 and is connected to the main board via I2C 

This device is connected via MQTT to the marina server using 6LoWPAN.

[Get more details here](src/Devices/MonitorDevice)

### Entrance sensor

<img src="resources/images/entrance_device.png" alt="Entrance sensor" style="zoom:50%;" />
For each side there are:

- **Camera** 
- **Raspberry PI** to read the name of the boat.
- **Network Interface** that sends computed data to the marina server over MQTT on 6LoWPAN.

Image processing is executed in the raspberry pi and the read name is sent.

**Marina server** in which are local computation are executed.

It's composed by:

- **Network interface** that communicate with all the devices installed in the marina over 6LoWPAN.
- **STM32 Board** the local elaboration node.
- **Internet interface** to connect the local system to the cloud.

### Software components and Network infrastructure

The major software components are:

- In entrance device there is a **computer vision software** to estimate the boat size and name.
- A **web interface**  used to see the current status of the marina and to book a dock.

![Software component](resources/images/network_infrastructure.png)

For the communication inside the marina 6LoWPAN is used.
The marina server send updating data over MQTT to the cloud system.

Software work flow examples:

- A boat enter in the marina and the entrance device send name and size of the boat to the marina server.
- The marina server check on the cloud system if there are a reserved dock for the entering boat, otherwise it'll assign a free dock suitable for the boat size.
- The marina server send data to the monitors and dock device that start to show useful information at the sailor.

### AWS infrastructure

On AWS cloud we have developed the cloud system with the following architecture

![AWS architecture](resources/images/architecture.png)

We have 3 lambda function. The first one is `getReservation` and is called only from the web page to make a reservation that will be saved in a table of the marina DynamoDB. The second is called `assignDock` and is used to assign a docking spot when the camera detects an incoming boat. The assigned dock is stored in the DynamoDB in another table used only for the docking spot status and the reservation status is updated to take into account that the boat arrived. At the end this lambda function sends an MQTT message to the docking devices and the displays to generate the correct signage to reach the docking.

When the boat reaches its docking spot a dock device message triggers the last lambda function called `removeReservation` which will communicate to all the docking devices that the docking procedure is completed.

To conclude when a docking device detects that a docking spot is free again because a boat leaved, it sends a message that triggers again the `removeReservation` function that in this case will delete the reservation and restore the docking spot status to free.

For more details about the messages exchanged by these components check the [dedicated folder](src/AWS).