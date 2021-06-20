import json
import boto3
import re
from datetime import datetime
from boto3.dynamodb.conditions import Key

def lambda_handler(event, context):
    
    # Decode the JSON message
    head = event.get('headers')
    boat_id = head.get('boat-id')
    dim_str = head.get('boat-dim')
    owner = head.get('owner')
    
    # Check for errors in the message
    if boat_id is None or dim_str is None or owner is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 1, 'info_mess': "Missing custom headers"})
        }
        
    # Check boat license plate
    if re.match("^[A-Z]{2}[0-9]{2,6}$", boat_id) is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 2, 'info_mess': "Wrong boat license plate"})
        }
    
    # Get boat dimension
    dim = int(dim_str)
    if not dim in [1,2,3]:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 3, 'info_mess': "Wrong boat dimension"})
        }
        
    # Check owner name    
    if re.match("^([a-zA-Z]{2,}\s[a-zA-Z]{1,}'?-?[a-zA-Z]{1,}\s?([a-zA-Z]{1,})?)", owner) is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 4, 'info_mess': "Invalid owner name"})
        }
    
    # Store the reservation
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table = dynamodb.Table('reservations')
    
    response = table.scan(
        FilterExpression=Key('boat_id').eq(boat_id)
        )
    
    if len(response['Items']) != 0:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps({'ret_code': 5, 'info_mess': "The boat already has a reservation"})
        }
    
    # Store new item
    table.put_item(
       Item={
            'boat_id': boat_id,
            'dim': dim,
            'owner': owner.upper(),
            'boat_status': "WAIT"
        }
    )
    
    # Return all the informations to the web page
    return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': json.dumps({'ret_code': 0, 'info_mess': "Reservation done"})
    }
