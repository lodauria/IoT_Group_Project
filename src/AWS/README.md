## Making a reservation

To make a reservation you can use [this web page](https://marinaserver.s3.amazonaws.com/dashboard.html).
In this web page there is a API to call the lambda function `getReservation` that will save the information in the DynamoDB.

## Boat entering in the marina

When the camera detects a boat, sends to topic `camera/boat` a message in the form

        {"boat_id": "TS49875"}

There is a rule on topic `camera/boat` which calls the lambda function `assignDock`. This function finds and assigns a docking spots based on the reservation that has to be already presents. Then communicates its decision to all the devices (displays and dock devices) with a message on the topic `dock/assign` in the form

        {"boat_id": "TS49875", "dock_num": 21, "event":"0"}

## Boat docking in the marina

When a dock device detect the boat arriving, it sends a message to topic `dock/boat` in the form

        {"dock_num": 21, "event": "0"}

This will trigger the lambda function `removeReservation` that will change the reservation status in the DynamoDB so store the fact that the boat arrived. Then alerts all the devices (displays and dock devices) that the boat is docked by sending a message with tipic `dock/assign` in the form

        {"boat_id": "TS49875", "dock_num":21, "event": 1}

## Boat leaving in the marina

When a dock device detect a boat leaves, it sends a message to topic `dock/boat` in the form

        {"dock_num": 21, "event": "1"}

This will trigger the lambda function `removeReservation` that will delete the reservation in the DynamoDB.
