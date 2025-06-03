import json
import boto3

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('DameTuCosita_data')

def lambda_handler(event, context):
    try:
        data_item = {
            'gas_ppm': event.get('gas_ppm'),
            'thing_name': event.get('thing_name'),
            'timestamp': event.get('timestamp'),
            'hubo_incendio': event.get('hubo_incendio'),
            'peligro': event.get('peligro'),
            'userId' : event.get('clientid')
        }
        response = table.put_item(Item=data_item)
        return {
            'statusCode': 200
        }

    except Exception as e:
        print(f"Error: {e}")