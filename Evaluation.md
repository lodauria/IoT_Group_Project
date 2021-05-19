# Performance evaluation:

Since our system is used to guide the boats inside a marina it has to be fast and reliable in indicating to each boat what to do.

## Devices performance

To evaluate our solution we first consider the devices performances as follows:

**Camera:** the performance of the camera will be evaluated by the capability of capturing clear images that allows to perform an accurate text recognition. We will consider the percentage of correct text detection on the number of given images.

**Oil spill sensor:** the performance of this sensor should be evaluated by detecting precisely the ratio of floating oil in the marina.

**Boat detection sensor** for this scope an ultrasonic sensor is used, for our propose the precision is not much important, we have to detect only if a boat is present or not.
The main issue for this kind of sensor is that we cannot detect the difference between an out of range measure and a faulty sensor.
When the device measurement is out of range the receiver components doesn't detects any ultrasonic sound and this phenomena could happen also if the receiver or the sender module is damaged.
Another problem of this family of sensors is that they doesn't work fine with sound absorbing material, but for detect boats, that are made by metal or wood, we haven't this problem. 

**Signage screens:** the evaluation of the screen considers the clarity and visibility of the indications to guide the tourist to the right spot.
If more boats enter in the marina, the screen have to show by cycling the signage for all boats, alternating the signage for different boats. If there are many boats, the delay between the information shown increases.

![Screen GIF](resources/images/screen.gif)

**Docking spot LED:** this led should help the tourist to find the place to dock, it should be visible and the state should be updated before that the boat arrives near the dock.

## Network technology:

The marina server should be able to acquire the sensors data via wireless connection. The server data will be exchanged with the cloud using MQTT protocol over a secure internet connection.

## Response time from an end-user point of view: 

This IoT system has to indicate to the user how to reach the assigned spot as soon as he reaches the marina entrance. The indications should be clear and easy to follow from the user.