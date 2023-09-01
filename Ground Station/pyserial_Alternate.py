import serial.tools.list_ports
import serial

# List available serial ports
def list_available_ports():
    ports = serial.tools.list_ports.comports()
    portlist = [str(oneport) for oneport in ports]
    return portlist

# Select a serial port
def select_serial_port():
    portlist = list_available_ports()
    print("Available Ports:")
    for index, port in enumerate(portlist):
        print(f"{index}: {port}")

    while True:
        try:
            val = int(input("Select Port (enter the index): "))
            if val >= 0 and val < len(portlist):
                return portlist[val]
            else:
                print("Invalid selection. Please enter a valid index.")
        except ValueError:
            print("Invalid input. Please enter a valid index.")

# Main function
def main():
    # Select a serial port
    portVar = select_serial_port()
    print(f"Selected Port: {portVar}")

    # Create a serial instance
    serialInst = serial.Serial()
    serialInst.baudrate = 9600
    serialInst.port = portVar

    try:
        serialInst.open()
        print(f"Connected to {portVar}")

        while True:
            if serialInst.in_waiting:
                packet = serialInst.readline()
                print(packet.decode('utf-8').rstrip('\n'))

    except serial.SerialException as e:
        print(f"Error: {e}")
    finally:
        serialInst.close()

if __name__ == "__main__":
    main()
