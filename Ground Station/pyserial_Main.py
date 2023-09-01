import time
import serial.tools.list_ports
import requests
import serial

# List available serial ports
def list_available_ports():
    ports = serial.tools.list_ports.comports()
    available_ports = [port.device for port in ports]
    return available_ports

# Select a serial port
def select_serial_port():
    available_ports = list_available_ports()
    print("Available serial ports:")
    for i, port in enumerate(available_ports):
        print(f"{i+1}: {port}")
    while True:
        try:
            choice = int(input("Enter the number of the port you want to use: "))
            if 1 <= choice <= len(available_ports):
                return available_ports[choice - 1]
            else:
                print("Invalid choice. Please enter a valid number.")
        except ValueError:
            print("Invalid input. Please enter a valid number.")

# Main program
selected_port = select_serial_port()
ser = serial.Serial(selected_port, 9600)

server_url = "http://localhost:3001"  # Change this to the actual server URL

try:
    while True:
        simulated_data = str(time.time())  # Simulated data as a timestamp
        ser.write(simulated_data.encode())
        
        # Send data to the server
        payload = {"data": simulated_data}
        response = requests.post(server_url, data=payload)
        if response.status_code == 200:
            print("Data sent to server:", response.text)
        else:
            print("Failed to send data to the server (Status Code:", response.status_code, ")")
        
        time.sleep(1)  # Adjust the delay as needed
except KeyboardInterrupt:
    ser.close()
    print("Serial connection closed.")
