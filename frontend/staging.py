import json
import os
from threading import Thread
import webview

from api import API
from processing import Processing

class Staging():

    def __init__(self, window) -> None:
        self.window = window

        self.current_array = None
        self.current_window = None
        self.current_username = None
        self.selected_filepath = None
        self.user_id = -1

        self.api = API()
        self.processing = Processing()

    def save_image(self, image_url: str):
        image_name = image_url.split('/')[-1]
        file_types = ('PNG Image Files (*.png)',)
        filepath = self.window.create_file_dialog(webview.SAVE_DIALOG, save_filename=image_name, file_types=file_types)
        filepath = ''.join(filepath)
        with open(filepath, 'wb') as handle:
            content = self.api.download_image(image_url)
            if content == -1:
                self.window.evaluate_js(f'failed_download("{image_name}")')
            else:
                handle.write(content)
                self.window.evaluate_js(f'success_download("{image_name}")')

    def open_file_dialog(self):
        file_types = ('CSV and Text files (*.csv;*.txt)',)

        file = self.window.create_file_dialog(webview.OPEN_DIALOG, allow_multiple=False, file_types=file_types)
        file = file[0] if file else None
        self.selected_filepath = file

        if not file:
            return

        try:
            filename = file.split(os.path.sep)[-1]
            self.window.evaluate_js(f'on_file_selected("{filename}")')

            file_contents = self.processing.read_file(self.selected_filepath)
            file_contents = file_contents.split('\n')

            lines = []
            for line in file_contents:
                line = line.strip()
                if len(line) > 0:
                    lines.append(line)

            self.current_array = self.processing.lines_to_array(lines)
        except Exception as e:
            print('Error reading file:', e)
            self.window.evaluate_js('file_read_error()')

    def process_file(self):
        try:
            self.current_array = self.processing.signal_gain(self.current_array)
            self.window.evaluate_js('process_done()')
        except Exception as e:
            print('Error processing file:', e)
            self.window.evaluate_js('process_error()')

    def send_image(self, quality_index, algo):
        try:
            if self.current_array is None:
                return

            # Create body of the message as JSON
            body = {
                'user': self.user_id,
                'algo': algo,
                'quality': int(quality_index),
            }

            # convert array to string with \n separators
            image_str = '\n'.join(map(str, self.current_array))

            image_id = self.api.send_image(body, image_str)

            if image_id == -1:
                return

            print('Image sent with ID', image_id)
        except Exception as e:
            print('Error sending image:', e)

    def refresh_images(self):
        self.get_images()

        self.window.evaluate_js('refresh_images_done()')

    def login(self, username) -> bool:
        result = self.api.get_user(username)

        if result == -1:
            result = self.api.create_user(username)
            if result == -1:
                print('Error creating user')
                return False

        self.current_username = username
        self.user_id = result
        self.refresh_images()

        callbacks = {
            'on_start_processing': self.reload_image,
            'on_finish_processing': self.reload_image,
            'on_failed_processing': self.reload_image,
            'on_enqueued': self.on_enqueued,
        }
        self.api.open_websocket(self.user_id, callbacks)

        print('Logged in as', username, 'with ID', self.user_id)
        return True

    def reload_image(self, data: dict):
        jsonStr = json.dumps(data)
        self.window.evaluate_js(f'reload_image(\'{jsonStr}\')')

    def on_enqueued(self, data: dict):
        jsonStr = [data]
        jsonStr = json.dumps(jsonStr)
        self.window.evaluate_js(f'create_images(\'{jsonStr}\')')

    def get_images(self):
        try:
            self.window.evaluate_js(f'clear_images()')

            images = self.api.get_images(self.user_id, self.current_username)

            if len(images) == 0:
                return

            jsonStr = json.dumps(images)
            self.window.evaluate_js(f'create_images(\'{jsonStr}\')')
        except Exception as e:
            print('Error getting images:', e)