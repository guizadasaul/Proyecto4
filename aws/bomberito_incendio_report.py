import json
import boto3

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('Bomberito_Incendios_data')

def lambda_handler(event, context):
    try:
        estado_bomba = 'apagada'
        if event.get('bomba') == 1:
            estado_bomba = 'prendida'
        data_item = {
            'gas_ppm': event.get('gas_ppm'),
            'thing_name': event.get('thing_name'),
            'timestamp': event.get('timestamp'),
            'hubo_incendio': event.get('evento_incendio'),
            'ubicacion' : event.get('ubicacion'),
            'estado_bomba': estado_bomba,
        }
        response = table.put_item(Item=data_item)
        return {
            'statusCode': 200
        }

    except Exception as e:
        print(f"Error: {e}")
        