from flask import Flask, render_template, request, jsonify, Response
import serial

app = Flask(__name__)
ser = None

@app.route('/')
def index():
    return render_template('webpage.html')

def data_gen():
    while True:
        if ser:
            yield 'data: {}\n\n'.format(ser.readline().decode('utf-8'))

@app.route('/data')
def data():
    return Response(data_gen(), content_type='text/event-stream')

@app.route('/connect', methods=['POST'])
def connect():
    global ser
    port = request.form.get('port', 'COM3')
    baud_rate = request.form.get('baudRate', '9600')

    if not ser:
        ser = serial.Serial(port, int(baud_rate))
    return 'Connected to {} at {} baud rate.'.format(port, baud_rate)

@app.route('/disconnect', methods=['POST'])
def disconnect():
    global ser
    if ser:
        ser.close()
        ser = None
    return 'Disconnected from the Arduino.'

if __name__ == '__main__':
    app.run(debug=True)
