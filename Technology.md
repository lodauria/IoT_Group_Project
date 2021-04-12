## Technology

## ![Marina](resources/images/harbour.png)

The main macro components are:

- **Boat name identifier** at the marina entrance there is camera used to identify the boat name to check if there is a dock reservation.
- **Screens**, inside the marina there are some screens, useful to give indications at the sailor to find his dock spot.
- **Park sensor**, for each dock there is a sensor to detect if a boat is presents and a LED that blink when a sailor is searching his dock.

### Dock device

For each dock there is a LED (that blink when someone was finding his dock position) and a sonar to detects if the boat is present.
A single stm32 board is used for multiple dock, and it use a LoRa interface to communicate with the marine server.

<img src="resources/images/dock_device.png" alt="Dock device" style="zoom: 50%;" />

### Signature screen

A LED screen is driven by an STM32 board, that it's connected via LoRa to the marina server.

### Entrance sensor

<img src="resources/images/entrance_device.png" alt="Entrance sensor" style="zoom:50%;" />
For each side there are:

- **Camera** 
- **Raspberry PI** to read the name of the boat.
- **LoRa Interface** that sends computed data to the marina server.

Image processing is executed in the raspberry pi and the read name is sended.

**Marina server** in which are local computation are executed.

It's composed by:

- **LoRa interface** that communicate with all the devices installed in the marina.
- **STM32 Board** the local elaboration node.
- **Internet interface** to connect the local system to the cloud.

### Software components and Network infrastructure

The major software components are:

- In entrance device there is a **computer vision software** to estimate the boat size and name.
- A **web interface**  used to see the current status of the marina and to book a dock.

![Software component](resources/images/network_infrastructure.png)

For the communication inside the marina LoRa is used.
The marina server send updating data over MQTT to the cloud system.

Software work flow examples:

- A boat enter in the marina and the entrance device send name and size of the boat to the marina server.
- The marina server check on the cloud system if there are a reserved dock for the entering boat, otherwise it'll assign a free dock suitable for the boat size.
- The marina server send data to the monitors and dock device that start to show useful information at the sailor.
