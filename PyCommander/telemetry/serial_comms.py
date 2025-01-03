import serial
import serial.tools.list_ports as port_list
from telemetry.packets.base_packet import Packet, PacketError
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

    def emit_packet(self, packet: Packet) -> bool:

        # Open port if not already
        if self.port.is_open == False:
            self.port.open()

        self.port.write(packet.packet_bytes)
        self.port.write(b"\n")

    def readline(self):

        # Open port if not already
        if self.port.is_open == False:
            self.port.open()

        if self.port.in_waiting == 0:
            return False

        serial_bytes = bytearray(self.port.readline())

        return serial_bytes[:-2]
        
    

    

        
        
# class RadioComms:

