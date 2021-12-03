import requests
import json
import os

class API():

    def __init__(self) -> None:
        self.BASE_URL = os.getenv('BASE_URL') or 'http://localhost:8080'

    # Send a POST request to /images
    def send_image(self, body: dict, image: str) -> int:
        ## Send the creation request
        response_create = requests.post(f'{self.BASE_URL}/images', json=body)

        if response_create.status_code != 201:
            print('Error while creating image:', response_create.content)
            return -1
        image_id = response_create.json()['id']

        ## Upload image content as plain text
        response_signal = requests.post(f'{self.BASE_URL}/images/{image_id}/signal', data=image, headers={'Content-Type': 'text/plain'})
        if response_signal.status_code != 202:
            print('Error while sending image:', response_signal.content)
            return -1

        return image_id

    # Send a GET request to /users/:name/images
    def get_images(self, user_id: int, user_name: str) -> dict:
        response = requests.get(f'{self.BASE_URL}/users/{user_id}/images').json()

        for i in range(len(response)):
            if response[i]["data"] != None:
                response[i]['image_url'] = f'{self.BASE_URL}/images/{response[i]["data"]}'
            response[i]['username'] = user_name

        print(response)

        return response

    # Send a POST request to /users to create a new user
    def create_user(self, name: str) -> int:
        response = requests.post(f'{self.BASE_URL}/users', json={'name': name})

        if response.status_code != 201:
            return -1

        return response.json()['id']

    def mock_get_images(self, user_id: int) -> dict:
        image_mock = "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"

        return {
            'images': [
                {
                    'name': user_id,
                    'algo': 'CGNE',
                    'start_date': '2020-04-01T00:00:00',
                    'end_date': '2020-04-01T00:00:00',
                    'img': image_mock,
                    'size': {
                        'height': 60,
                        'width': 60
                    },
                    'iterations': 0,
                    'reconstruction_time': 0
                },
                {
                    'name': user_id,
                    'algo': 'CGNR',
                    'start_date': '2020-04-02T00:00:00',
                    'end_date': '2020-04-02T00:00:00',
                    'img': image_mock,
                    'size': {
                        'height': 60,
                        'width': 60
                    },
                    'iterations': 0,
                    'reconstruction_time': 0
                },
            ]
        }
