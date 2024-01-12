import time
import serial

serialInst = serial.Serial()

portVar = input("Enter the COM port (e.g., COMX): ")
serialInst.baudrate = 9600
serialInst.port = portVar

try: 
    serialInst.open()  # Open the serial port
    print(f"Connected to {portVar}")

    while True:
        if serialInst.in_waiting:
            packet = serialInst.readline()
            print(packet.decode('latin-1', errors='replace').rstrip('\n'))

except Exception as e:
    print(f"Error: {e}")
finally:
    serialInst.close()  # Close the serial port when done
