import json
import boto3
from datetime import datetime

def lambda_handler(event, context):
    
    # Decode the JSON message
    head = event.get('headers')
    boat_id = head.get('boat-id')
    dim = head.get('boat-dim')
    owner = head.get('owner')

    # Check for errors in the message
    if boat_id is None or dim is None or owner is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps("Missing custom headers")
        }
        
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table = dynamodb.Table('reservations')
    
     # Store new item
    response = table.put_item(
       Item={
            'boat_id': boat_id,
            'dim': dim,
            'owner': owner,
            'status': "WAIT"
        }
    )
    print(response)
    
    message = "Reservation done!"
    
    # Return all the informations to the web page
    return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': message
    }
