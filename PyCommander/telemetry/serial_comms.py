import serial
import serial.tools.list_ports as port_list
from telemetry.base_packet import BasePacket, PacketError
from time import sleep,time
import binascii

class SerialComms:

    @staticmethod
    def list_ports(print_output=False):
        ports = list(port_list.comports())
        
        if print_output:
            for i,p in enumerate(ports):
                print(f"{i}: {p.name}")

        return ports

    def connect(self, port_name: str, baud_rate = 115200, timeout = 0.3):
        self.port = serial.Serial(port=port_name,
                    baudrate=baud_rate, timeout=timeout)  # open serial port
        
    def restart_port(self):
        # Resets ESP32
        def reset():
            DEFAULT_PORT.port.dtr = False
            sleep(.1)
            DEFAULT_PORT.port.dtr = True
            sleep(.1)
    
        reset()

        timeout_start = time()
        while (DEFAULT_PORT.port.in_waiting > 0):
            received = DEFAULT_PORT.readline()
            # print(received)

            if time() - timeout_start > 1:
                reset()

        

    def emit_packet(self, packet: BasePacket) -> bool:

        # Open port if not already
        if self.port.is_open == False:
            self.port.open()

        self.port.write(packet.packet_bytes)
        self.port.write(b"\x00") # Delimeter

    def readline(self, delete_zero_byte = True):

        # Open port if not already
        if self.port.is_open == False:
            self.port.open()

        if self.port.in_waiting == 0:
            return bytearray()

        serial_bytes = bytearray(self.port.read_until(b"\x00"))

        if delete_zero_byte:
            serial_bytes = serial_bytes[:-1]
        
        return serial_bytes
        
DEFAULT_PORT: SerialComms = SerialComms() 

    

        
        
# class RadioComms:

