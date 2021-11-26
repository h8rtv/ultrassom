import requests
import json
import os

class API():

    def __init__(self) -> None:
        self.BASE_URL = os.getenv('BASE_URL') or 'http://localhost:5000'

    # Send a POST request to /images
    def send_image(self, body: dict) -> int:
        response = requests.post(f'{self.BASE_URL}/images', json=body).json()

        return response['image_id']

    # Send a GET request to /users/:name/images
    def get_images(self, username: str) -> dict:
        response = requests.get(f'{self.BASE_URL}/users/{username}/images').json()

        return response

    def mock_get_images(self, username: str) -> dict:
        return {
            'images': [
                {
                    'name': username,
                    'algo': 'CGNE',
                    'start_date': '2020-04-01T00:00:00',
                    'end_date': '2020-04-01T00:00:00',
                    'img': "iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==",
                    'size': {
                        'height': 60,
                        'width': 60
                    },
                    'iterations': 0,
                    'reconstruction_time': 0
                },
                {
                    'name': username,
                    'algo': 'CGNE',
                    'start_date': '2020-04-02T00:00:00',
                    'end_date': '2020-04-02T00:00:00',
                    'img': "iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==",
                    'size': {
                        'height': 60,
                        'width': 60
                    },
                    'iterations': 0,
                    'reconstruction_time': 0
                },
            ]
        }
