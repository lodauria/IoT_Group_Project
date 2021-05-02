# Performance evaluation:

Since our system is used to guide the boats inside a marina it has to be fast and reliable in indicating to each boat what to do.

## Devices performance

To evaluate our solution we first consider the devices performances as follows:

- **Camera:** the performance of the camera will be evaluated by the capability of capturing clear images that allows to perform an accurate text recognition. We will consider the percentage of correct text detection on the number of given images.
- **Oil spill sensor:** the performance of this sensor should be evaluated by detecting precisely the ratio of floating oil in the marina.
- **Boat detection sensor:** the ultrasonic sensor should always detect the presence of boat and be robust to the small boats movements. In addition is better if it's fast.
- **Signage screens:** the evaluation of the screen considers the clarity and visibility of the indications to guide the tourist to the right spot.
- **Docking spot LED:** this led should indicate if the place is busy or free updating its state as fast as possible.

## Network technology:

The marina server should be able to acquire the sensors data via wireless or wired connection. The server data will be exchanged with the cloud using MQTT protocol over a secure internet connection.

## Response time from an end-user point of view: 

This IoT system has to indicate to the user how to reach the assigned spot as soon as he reaches the marina entrance. The indications should be clear and easy to follow from the user.