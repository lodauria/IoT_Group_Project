### Hardware components

Lighthouse hardware high level diagram

![Lighthouse hardware diagram](resources/images/lighthouse hardware component.png)

- **Lighthouse**
  - **Dimmer** to set the light level.
  - **2 Servo motors** to orient the beam.
  - **Visibility sensor** to measure the visibility of the air.
  - **Power consumption meter** helpful for evaluation.
  - **STM32 Board**
  - **LoRa receiver** to receive data from the boats when they are near.
  - **Internet interface** to connect the lighthouse to the Internet connection provided by an ISP.
  - **Light sensor** to detect if it's day or night.

Boat hardware high level diagram

![Boat hardware diagram](resources/images/boat hardware component.png)

- **Boats**
  - **GPS** to get the position to share it with the lighthouse.
  - **Visibility sensor** to measure the visibility of the air.
  - **Light sensor** to detect if it's day or night, if it's day the boat doesn't send data
  - **STM32 Board**
  - **Internet interface** (LoRa/4G/Satellite)

### Software components

![Lighthouse hardware diagram](resources/images/software architecture.png)

- **Boats** Sends GPS position and visibility data over MQTT-SN to a cloud broker

- **Lighthouse** sends visibility data via an MQTT broker and receive the position of the boats to light, not in GPS coordinates but form the lighthouse point of view (Pan, Tilt degrees and light intensity).

  It also send power consumption to the cloud via MQTT that is used for monitoring.

- **Cloud** Receives the GPS position and visibility data from the boats. It process data and send to the lighthouse a list of all positions to light, each of them with a computed intensity, the list is ordered in such a way to have a smooth movement of the light and that all boats are lighted in the minimum amount of time.

- **Web interface** the system provide also a web interface for monitoring and eventually control the lighthouse manually.

### Network infrastructure

![Network infrastructure](resources/images/network infrastructure.png)

- **Lighthouse** is connected to Internet via wired connection and provide a LoRa connection for the boats that send their position with this modulation.
- **Boats** are free to use their connection or the LoRa connection provided by the lighthouse, for example if a ship has a satellite or 4G connection it can use that.