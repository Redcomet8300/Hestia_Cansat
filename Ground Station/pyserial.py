import serial
import http.server
import socketserver
import threading
import websocket

# Global variable to store received serial data
received_data = ""

# Serial port configuration
serial_port = '/dev/ttyUSB0'
baud_rate = 9600
timeout = 1

# Serial port thread function
def serial_thread():
    global received_data

    ser = serial.Serial(serial_port, baud_rate, timeout=timeout)

    while True:
        data = ser.readline().decode('utf-8').strip()
        if data:
            received_data = data

# HTTP server to serve the HTML page
class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.path = 'index.html'
        return http.server.SimpleHTTPRequestHandler.do_GET(self)

# WebSocket server thread function
def websocket_thread():
    server = websocket.WebSocketServer(
        host='localhost',
        port=8765,
        loglevel=websocket.LOGLEVEL_INFO
    )
    
    for client in server.serve_forever():
        while True:
            client.send(received_data)

# Start the serial thread
serial_thread = threading.Thread(target=serial_thread)
serial_thread.daemon = True
serial_thread.start()

# Start the WebSocket server thread
websocket_thread = threading.Thread(target=websocket_thread)
websocket_thread.daemon = True
websocket_thread.start()

# Start the HTTP server
httpd = socketserver.TCPServer(('localhost', 8000), MyHTTPRequestHandler)
print("Server running at http://localhost:8000/")
httpd.serve_forever()
