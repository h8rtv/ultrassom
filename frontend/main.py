try:
  import webview
except ImportError:
  print('PyWebView is not installed. Please install it with:')
  print('  pip install pywebview')
  exit(1)

try:
  import numpy as np
except ImportError:
  print('NumPy is not installed. Please install it with:')
  print('  pip install numpy')
  exit(1)

try:
  import requests
except ImportError:
  print('Requests is not installed. Please install it with:')
  print('  pip install requests')
  exit(1)

try:
  import websockets
except ImportError:
  print('Websocket is not installed. Please install it with:')
  print('  pip install websockets')
  exit(1)

from staging import Staging
import signal
import sys

def signal_handler_int(sig, frame):
    # Ctrl C was pressed
    sys.exit(0)

def echo(arg1):
  print(arg1)

def start_window(window):
  staging = Staging(window=window)
  window.expose(staging.process_file)
  window.expose(staging.open_file_dialog)
  window.expose(staging.send_image)
  window.expose(staging.refresh_images)
  window.expose(staging.login)

if __name__ == '__main__':
  signal.signal(signal.SIGINT, signal_handler_int)

  window = webview.create_window('Ultrassom', './html/index.html')
  window.expose(echo)

  webview.start(start_window, window, debug=True)