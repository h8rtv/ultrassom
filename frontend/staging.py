import json
import webview

from api import API
from processing import Processing

class Staging():

    def __init__(self, window) -> None:
        self.window = window

        self.current_array = None
        self.current_window = None
        self.current_username = 'user'
        self.selected_filepath = None
        self.user_id = -1

        self.api = API()
        self.processing = Processing()

    def open_file_dialog(self):
        file_types = ('All files (*.*)',)

        file = self.window.create_file_dialog(webview.OPEN_DIALOG, allow_multiple=False, file_types=file_types)
        file = file[0] if file else None
        self.selected_filepath = file

    def process_file(self):
        try:
            file_contents = self.processing.read_file(self.selected_filepath)
            file_contents = file_contents.split('\n')

            lines = []
            for line in file_contents:
                line = line.strip()
                if len(line) > 0:
                    lines.append(line)

            self.current_array = self.processing.lines_to_array(lines)
            self.current_array = self.processing.signal_gain(self.current_array)

            # Notify webview that we're done
            self.window.evaluate_js('process_done()')
        except Exception as e:
            print('Error processing file:', e)

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
            image_str = '\n'.join(map(str, self.current_array.flatten()))

            image_id = self.api.send_image(body, image_str)

            if image_id == -1:
                return

            print('Image sent with ID', image_id)

        except Exception as e:
            print('Error sending image:', e)

    def refresh_images(self):
        self.get_images()

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

        print('Logged in as', username, 'with ID', self.user_id)
        return True

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