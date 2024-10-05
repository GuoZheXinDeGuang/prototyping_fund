import serial
import time

arduino = serial.Serial(port='/dev/cu.usbmodem1301',   baudrate=115200, timeout=.1)


def write_read(x):
    arduino.write(bytes(x + '\n', 'utf-8'))  # Send input followed by a newline character
    time.sleep(0.05)  # Small delay
    data = arduino.readline().decode('utf-8').strip()  # Read the response and strip newline/space characters
    return data


while True:
    num = input("Enter a number: ")
    value = write_read(num)
    # print(f"Arduino response: {value}")
