# AWS infrastructure

In this folder are defined all the components implemented on the AWS Cloud.

## Making a reservation

To make a reservation a user can connect to this [web page](https://marinaserver.s3.amazonaws.com/dashboard.html) hosted on an S3 bucket. In this web page there is a HTTP API which calls the lambda function `getReservation` that saves the reservation in the DynamoDB.

## Boat entering in the marina

When the camera detects a boat, sends to topic `camera/boat` a message in the form

        {"boat_id": "TS49875"}

There is a rule on topic `camera/boat` which calls the lambda function `assignDock`. This function finds and assigns a docking spot based on the reservations and the docking spots status stored in the DynamoDB. Then communicates its decision to all the devices (displays and dock devices) with a message on the topic `dock/assign` in the form.

        {"boat_id": "TS49875", "dock_num": 14, "event":"0"}

All the devices will react to guide the user to his docking.

## Boat docking in the marina

When a dock device detects that the boat has stopped in its docking, it sends a message to topic `dock/boat` in the form

        {"dock_num": 14, "event": "0"}

This will trigger the lambda function `removeReservation` that will change the reservation status in the DynamoDB so store the fact that the boat docked. Then alerts all the devices (displays and dock devices) that the boat is docked by sending a message with topic `dock/assign` in the form

        {"boat_id": "TS49875", "dock_num":14, "event": 1}

All the devices will react deleting the signage for the boat that docked.

## Boat leaving in the marina

When a dock device detect a boat leaving its docking, it sends a message to topic `dock/boat` in the form

        {"dock_num": 14, "event": "1"}

This will trigger the lambda function `removeReservation` that deletes the reservation from the DynamoDB and updates the docking spot status to free.
