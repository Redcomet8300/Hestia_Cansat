from flask import Flask, render_template, request, jsonify, Response
import serial
import serial.tools.list_ports

app = Flask(__name__)
ser = None

def data_gen():
    while True:
        if ser and ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            yield f'data: {line}\n\n'

@app.route('/')
def index():
    available_ports = [port.device for port in serial.tools.list_ports.comports()]
    return render_template('webpage.html', available_ports=available_ports)

@app.route('/data')
def serial_data():
    return Response(data_gen(), content_type='text/event-stream')

@app.route('/connect', methods=['POST'])
def connect():
    global ser
    port = request.form.get('port', 'COM3')
    baud_rate = request.form.get('baudRate', '9600')

    if not ser:
        ser = serial.Serial(port, int(baud_rate))
        print("Serial connection established.")
    else:
        print("Serial connection already exists.")

    return 'Connected to {} at {} baud rate.'.format(port, baud_rate)

@app.route('/disconnect', methods=['POST'])
def disconnect():
    global ser
    if ser:
        ser.close()
        ser = None
        print("Disconnected from the Arduino.")
    else:
        print("No active serial connection to disconnect.")

if __name__ == '__main__':
    available_ports = [port.device for port in serial.tools.list_ports.comports()]
    print("Available Ports:", available_ports)
    app.run(debug=True)
