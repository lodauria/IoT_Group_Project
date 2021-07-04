# Concept

*[First delivery version](https://github.com/kernel-machine/IoTGroupProject/tree/first_assignment/Concept.md)*

*[Second delivery version](https://github.com/kernel-machine/IoTGroupProject/tree/second_assignment/Concept.md)*

The concept behind this project is to simplify the docking operation of amateur sailors when entering in a marina they never visited. Based on what different sailors reported, a very common problem during holiday cruises is the docking procedure. In many cases the indications to reach the correct docking spot are given via radio or orally in a very unpractical way. For example an information like "pier G, spot 14" is not easy to be used if the sailor doesn't know the place. This impractical method creates also a safety problem for example in summer when the marina canals are crowded by the boats of a lot of tourists.

The solution that we propose simplifies the docking operation for the sailors and the docking spot assignment problem for the marina workers with the introduction of an IoT system to be added to the marina. When a boat enters in the marina, the system automatically identifies it either with a camera or through the GPS of the user's device and checks in the database the reservation associated to that boat. Based on the boat details present in the reservation (e.g. length and width) the cloud systems assigns a free docking spot to the boat and generate a path in the marina canals to reach it. Then the low level information about the direction to follow are passed to some LCD screens present in the marina intersections which will indicate to each boat which path to follow (e.g. "boat VE5984  turn left; boat LV9484 turn right"). Then in the correct docking spot there is a LED flashing indicating the assigned slot. When a boat reaches its destination a special cleat with a button inside communicates to the system that the indications for that spots can be deleted.

## Personas

To make things more clear here we describe two personas who can benefit from this system.

### Amateur sailor

John is an amateur sailor who like to spend holidays on his boat cruising with his family. They like to explore the beauty of the coasts stopping every night in a different marina. John is very happy when the marina he has to reach has a smart signage system because in this way he can easily and safely reach the assigned docking spot after a long sailing day.

### Marina worker

Scott is a worker of a touristic marina and it's job is to coordinate the reservations of the tourists that want to stop in the marina. Since it was introduced the new signage system his work has become more simple and organized. Every day Scott checks the reservations made online and adds the ones received by phone, he looks at the marina entrance and simply checks that each boat reaches the assigned spot. He is so happy that during summer he doesn't have to go crazy signaling to all the incoming boats what to do inside the marina.

## Storyboard

Here we explain with a simple storyboard how the interaction with the signage system can be.

- A tourist sailor is approaching a marina where he booked for docking for the night.
- He follows the indications for the entrance where the boat identification is done.
- He sees appearing on the LCD screen in front of him the license plate of his boat with the indication "turn right".
- He follows the indication and the following ones he sees on the screens.
- Finally he sees a free docking spot with a LED flashing and he safely docks there.

## Marina examples

In this project we will consider the following example of marina.

![MarinaExample](resources/images/MarinaMonitorPositionExample.png)

In this marina there 30 docking spots available placed in 5 different piers. Each pier can accommodate different type of boats:

- The docking spots from 1 to 10 has to be used by small boats (category 1)
- The docking spots from 11 to 15 and from 26 to 30 can accommodate also medium sized boats (category 2)
- The docking spots from 16 to 25 are the only one which can host also big boats (category 3)

The IoT system developed for this marina uses 30 docking devices, 4 LCD monitors and a camera.