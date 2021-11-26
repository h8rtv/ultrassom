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
    def get_images(self, username: str) -> int:
        response = requests.get(f'{self.BASE_URL}/users/{username}/images').json()

        return response



