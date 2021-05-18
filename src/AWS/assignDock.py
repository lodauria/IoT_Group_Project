import json
import boto3
from datetime import datetime
from boto3.dynamodb.conditions import Key, Attr

def lambda_handler(event, context):
    
    # Decode the JSON message (gives error and stop if wrong format)
    boat_id = event["boat_id"]
    
    # Check for reservation
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table_res = dynamodb.Table('reservations')
    
    response = table_res.scan(
        FilterExpression=Key('boat_id').eq(boat_id)
        )
    
    if len(response['Items']) == 0:
        return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': json.dumps("No reservations")}
    
    # Get boat dimension
    boat_dim = response['Items'][0]['dim']
    
    # Assign docking spot
    table_dock = dynamodb.Table('docking_spots')
    
    response = table_dock.scan(
        FilterExpression = Attr('boat').eq("NULL") & Attr('dim').eq(boat_dim)
        )
        
    if len(response['Items']) == 0:
        return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': json.dumps("No free spots")}
        
    dock_num = response['Items'][0]['num']
        
    # Update reservation status
    response = table_res.update_item(
        Key={
            'boat_id': boat_id
        },
        UpdateExpression="set boat_status=:s",
        ExpressionAttributeValues={
            ':s': "IN"
        },
        ReturnValues="UPDATED_NEW"
    )
    
    # Update dockings table
    response = table_dock.update_item(
        Key={
            'num': dock_num
        },
        UpdateExpression="set boat=:b, last_update=:t",
        ExpressionAttributeValues={
            ':b': boat_id,
            ':t': int(datetime.utcnow().timestamp())
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
        payload=json.dumps({"boat_id":boat_id, "dock_num":str(dock_num), "event":"0"})
    )