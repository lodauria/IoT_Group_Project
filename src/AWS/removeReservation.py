import json
import boto3
from datetime import datetime
from boto3.dynamodb.conditions import Key, Attr

def lambda_handler(event, context):
    
    # Decode the JSON message (gives error and stop if wrong format)
    dock_num = event["dock_num"]
    action_type = event["event"]
    
    # Check for reservation
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table_dock = dynamodb.Table('docking_spots')
    
    response = table_dock.scan(
        FilterExpression=Key('num').eq(dock_num)
        )
     
    # Get boat id
    boat_id = response['Items'][0]['boat']
    
    if action_type == "IN":
        
        # Update reservation status
        response = table_res.update_item(
            Key={
                'boat_id': boat_id
            },
            UpdateExpression="set status=:s",
            ExpressionAttributeValues={
                ':s': "STOP"
            },
            ReturnValues="UPDATED_NEW"
        )
        
        # Send instructions
    
        # Create an MQTT client
        client = boto3.client('iot-data', region_name='us-east-1')
        
        # Publish the message for the displays
        response = client.publish (
            topic='completed',
            qos=1,
            payload=json.dumps({"boat": boat_id, "bock_num":dock_num})
        )
    
    if action_type == "OUT":
        table_res = dynamodb.Table('reservations')
        response = table_res.delete_item(
            Key={'boat_id': boat_id}
            )