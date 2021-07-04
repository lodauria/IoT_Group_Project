import json
import boto3
from datetime import datetime
from boto3.dynamodb.conditions import Key, Attr

def lambda_handler(event, context):
    
    # Decode the JSON message (gives error and stop if wrong format)
    dock_num = int(event["dock_num"])
    action_type = event["event"]
    
    # Check attributes values
    if dock_num<1 or dock_num>30 or int(action_type)<0 or int(action_type)>1:
        return json.dumps({'ret_code': 1, 'info_mess': "Attributes error"})
    
    # Initialize tables
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table_dock = dynamodb.Table('docking_spots')
    table_res = dynamodb.Table('reservations')
    
    # Get boat id (if present)
    response = table_dock.scan(
        FilterExpression=Key('num').eq(dock_num) & Attr('boat').ne("NULL")
        )
    if len(response['Items']) == 0:
            return json.dumps({'ret_code': 2, 'info_mess': "No boat found"})
    boat_id = response['Items'][0]['boat']
    
    # If the boat reached the docking spot
    if action_type == "0":
        
        # Check previous reservation status
        response = table_res.scan(
            FilterExpression=Key('boat_id').eq(boat_id) & Attr('boat_status').eq("IN")
            )
        if len(response['Items']) == 0:
            return json.dumps({'ret_code': 3, 'info_mess': "Error in the reservation status"})
        
        # Update reservation status
        response = table_res.update_item(
            Key={
                'boat_id': boat_id
            },
            UpdateExpression="set boat_status=:s",
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
            topic='dock/assign',
            qos=1,
            payload=json.dumps({"boat_id": boat_id, "dock_num":str(dock_num), "event":"1"})
        )
    
    # If the boat left the docking
    if action_type == "1":
        
        # Check previous reservation status
        response = table_res.scan(
            FilterExpression=Key('boat_id').eq(boat_id) & Attr('boat_status').eq("STOP")
            )
        if len(response['Items']) == 0:
            return json.dumps({'ret_code': 4, 'info_mess': "Error in the reservation status"})
            
        response = table_res.delete_item(
            Key={'boat_id': boat_id}
            )
            
        # Update dockings status
        response = table_dock.update_item(
        Key={
            'num': dock_num
        },
        UpdateExpression="set boat=:b, last_update=:t",
        ExpressionAttributeValues={
            ':b': "NULL",
            ':t': int(datetime.utcnow().timestamp())
        },
        ReturnValues="UPDATED_NEW")