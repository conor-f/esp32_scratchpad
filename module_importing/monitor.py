import serial
import argparse
import sys

def main(port, baudrate):
    with serial.Serial(port, baudrate) as ser:
        while True:
            line = ser.readline().decode('utf-8')
            sys.stdout.write(line)
            sys.stdout.flush()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Read data from a serial port and print it to stdout.'
    )
    parser.add_argument('port', help='Serial port (e.g., /dev/ttyUSB0)')
    parser.add_argument('baudrate', type=int, help='Baudrate (e.g., 115200)')
    args = parser.parse_args()

    main(args.port, args.baudrate)
