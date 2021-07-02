import json
import boto3
from math import sqrt

def lambda_handler(event, context):
    
    # Marina parameters defintion
    lat_des = 45.69
    lon_des = 13.74
    trig = 0.5
    
    # Decode the JSON message
    head = event.get('headers')
    boat_id = head.get('boat-id')
    lat = head.get('latitude')
    lon = head.get('longitude')
    
    # Check for errors in the message
    if boat_id is None or lat is None or lon is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 1, 'info_mess': "Missing custom headers"})
        }
    
    if sqrt((lat - lat_des)**2 + (lon - lon_des)**2) < trig:
        
        # Send boat detection message
    
        # Create an MQTT client
        client = boto3.client('iot-data', region_name='us-east-1')
        
        # Publish the message for the assignDock function
        response = client.publish (
            topic='camera/boat',
            qos=1,
            payload=json.dumps({"boat_id":boat_id})
        )
        
        # Return the result to the web page
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 0, 'info_mess': "Smart signage activated"})
        }
        
    else:
    
        # Return all the informations to the web page
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 10, 'info_mess': "Too far, try again when closer"})
        }