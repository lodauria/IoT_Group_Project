# Second delivery update

## Comments received after the first delivery

- The concept idea doesn't solve a real world problem and so it wouldn't be a valuable product.
- The data collected by the sensors can be used in a more clever way.

## Changes done after the first delivery

Based on the feedback received after the first delivery we decided to start from scratch on a new concept idea: a smart marina.

## Technology update

### Work done up to now

We've developed:
- Firmware for the screen controller
- Firmware for boat detection device
- Cloud software to control devices via MQTT
- Cloud software to assign reservation
- Software for license plate recognition

### Future work
- Improve web interface
- Security, all data stored in the database will be encrypted to ensure that the marina clients cannot be tracked
- The system have to work also with the boats without a license plate (<10 m).
  We will work on one of these proposals: 
    - QR Code ticket to scan at the entrance
    - Contactless check-in using bluetooth low energy
- The system have to response also if there isn't a reservation for entering boat
- Change network connection to a more reliable one
- Oil spill detection [?]
## Evaluation update

### Work done up to now
-the plate recogntion is working for a simple boat without aditional design near the plate.

-We tested the power consumption and the delay in the license plate recognition.

-the plate recognition is working slowly.

### Future work
- making a more accurate plate recognition with taking in account cases where the boat is more designed and more writen word are near the plate. 
- making a neural network algorithem to make the result of the plate recognition more precise.
- make the response faster than the first one.
