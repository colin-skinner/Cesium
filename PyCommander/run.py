import serial
import serial.tools.list_ports as port_list
from time import sleep

from asyncio import get_event_loop
from serial_asyncio import open_serial_connection


ports = list(port_list.comports())
for p in ports:
    print (p)

# port_name = [port for port in ports if "usbserial" in port]
ser = serial.Serial(port='/dev/cu.usbserial-0001',
                    baudrate=115200)  # open serial port

print(ser.get_settings())

count=1

array_tmp = []

serialString = ""  # Used to hold data coming over UART
print("Starting")
while True:

    # send_string = bytearray(input(), "ascii")

    
    ser.write(b"H")
    ser.write(b"i")
    ser.write(b"\n")
    sleep(.5)


    serialString = ser.readline()

        # Print the contents of the serial data
    print(serialString.decode("Ascii"))

    # sleep()
ser.close()
# print(ser.name)         # check which port was really used
# ser.write(b'hello')     # write a string
# ser.close()             


