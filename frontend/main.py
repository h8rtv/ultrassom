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

from staging import Staging

def echo(arg1):
  print(arg1)

def start_window(window):
  window.evaluate_js('pywebview.api.echo(1)')

  staging = Staging(window=window)
  window.expose(staging.process_file)
  window.expose(staging.open_file_dialog)
  window.expose(staging.send_image)
  window.expose(staging.refresh_images)
  window.expose(staging.login)

if __name__ == '__main__':
  window = webview.create_window('Ultrassom', './html/index.html')
  window.expose(echo)

  webview.start(start_window, window, debug=True)