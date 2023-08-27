import os
import serial

# Get the parent folder's path
parent_folder = os.path.dirname(os.path.abspath(__file__))

# Path to the file in the parent folder
config_file_path = os.path.join(parent_folder, 'config.json')

# Open the configuration file
with open(config_file_path, 'r') as config_file:
    config_data = config_file.read()

# Configure the serial connection
serial_port = 'COM3'  # Change this to the appropriate port
baud_rate = 9600  # Change this to match the baud rate set in your Arduino code

# Open the serial port
ser = serial.Serial(serial_port, baud_rate)

# Read data from the Arduino
while True:
    data = ser.readline().decode('utf-8').strip()
    print(data)

# Close the serial port when done
ser.close()
