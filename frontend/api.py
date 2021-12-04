import requests
import os

class API():

    def __init__(self) -> None:
        self.BASE_URL = os.getenv('BASE_URL') or 'http://localhost:8000'

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

        return response

    # Send a POST request to /users to create a new user
    def create_user(self, name: str) -> int:
        response = requests.post(f'{self.BASE_URL}/users', json={'name': name})

        if response.status_code != 201:
            return -1

        return response.json()['id']