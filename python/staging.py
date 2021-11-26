import webview
import numpy as np

from api import API

class Staging():

    def __init__(self, window) -> None:
        self.window = window

        self.current_array = None
        self.current_window = None
        self.current_username = 'user'
        self.selected_filepath = None

        self.api = API()

    def open_file_dialog(self):
        global selected_filepath

        file_types = ('All files (*.*)',)

        file = self.window.create_file_dialog(webview.OPEN_DIALOG, allow_multiple=False, file_types=file_types)
        file = file[0] if file else None
        self.selected_filepath = file

        if file is not None:
            print('File selected:', file)
            self.window.evaluate_js('file_selected(true)')
        else:
            print('No file selected')
            self.window.evaluate_js('file_selected(false)')

    def process_file(self):
        try:
            file_contents = self.read_file(self.selected_filepath)
            file_contents = file_contents.split('\n')

            lines = []
            for line in file_contents:
                line = line.strip()
                if len(line) > 0:
                    lines.append(line)

            # Convert the lines to a numpy array
            self.current_array = np.array(lines, dtype=np.float32)

            # Process the array
            self.signal_gain(self.current_array)

            # Notify webview that we're done
            print('Done processing file')
            self.window.evaluate_js('process_done()')
        except Exception as e:
            print('Error processing file:', e)


    def send_image(self):
        try:
            if self.current_array is None:
                print('WARNING: No array to send')
                return

            # Create body of the message as JSON
            body = {
                'data': self.current_array.tolist(),
                'name': self.current_username,
                'algo': 'CGNE',
                'size': {
                    'width': 60,
                    'height': 60
                }
            }

            image_id = self.api.send_image(body)
            print('New Image ID:', image_id)

        except Exception as e:
            print('Error sending image:', e)

    def signal_gain(self, g):
        N = 64
        S = 794
        for c in range(N):
            for l in range(S):
                y = 100 + 1 / 20 * (l + 1) * np.sqrt(l + 1)
                index = l + S * c
                g[index] = g[index] * y

    def read_file(self, file_name):
        with open(file_name, 'rb') as f:
            return f.read().decode('utf-8')