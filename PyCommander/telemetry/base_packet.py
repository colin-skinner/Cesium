
from datetime import datetime
from .crc import crc16xmodem
from enum import Enum
from .command_schema import *
from cobs import cobs

def check_within_bits(value: int, bits: int, title: str) -> None:
    # print(type(value))
    if value >= 2**bits:
        raise PacketError(f"{title} not within {bits} bits (less than {2**bits})")
    
    if value < 0:
        raise PacketError(f"{title} less than 0")
    
def get_day_millis() -> tuple[int, datetime]:
    dt = datetime.now()
    dt_midnight = datetime(dt.year, dt.month, dt.day)

    seconds_since_midnight = (dt - dt_midnight).total_seconds()
    millis_of_day = round(seconds_since_midnight * 1000)

    return millis_of_day, dt

class PacketError(OverflowError): ...

class BasePacket:

    TOPIC_BITS = 6
    COMMAND_BITS = 4
    DATA_LENGTH_BITS = 11
    MILLISTAMP_BITS = 27
    HEADER_LENGTH_BYTES = 6

    MAX_TOPIC_ID = 2**TOPIC_BITS - 1
    MAX_COMMAND_ID = 2**COMMAND_BITS - 1
    MAX_DATA_LENGTH = 2**DATA_LENGTH_BITS - 1
    MAX_MILLISTAMP_LENGTH = 2**MILLISTAMP_BITS - 1

    dt_midnight: datetime

    
    def __init__(self):
        
        self.topic = int(-1)
        self.command = int(-1)
        self.data = bytearray()
        self.header_bytes = None
        self.millistamp = None
        self.data_length = int(-1)
    
    def configure(self, topic: int, command: int, data: bytearray):

        
        # Correcting Enums
        if issubclass(type(topic), Enum):
            # print("Converting Topic into int")
            topic = int(topic.value)

        if issubclass(type(command), Enum):
            # print("Converting Command into int")
            command = int(command.value)

        check_within_bits(topic, BasePacket.TOPIC_BITS, "topic")

        check_within_bits(command, BasePacket.COMMAND_BITS, "command")

        self.data_length = len(data)
        check_within_bits(self.data_length, BasePacket.DATA_LENGTH_BITS, "data_length")
        
        self.topic = topic
        self.command = command
        self.data = data

    def __repr__(self):
        result = list()
        result.append(f"Operation ID: {self.topic}")
        result.append(f"Command ID: {self.command}")
        result.append(f"Data Length: {self.data_length}")
        result.append(f"Data: {self.data.hex(sep=',')}")
        result.append(f"ASCII Data: {self.data}")
        result.append(f"Millistamp: {self.millistamp}")
        return str(result)

    

    def get_topic_id(self):
        return self.topic
    
    def get_command_id(self):
        return self.command
    
    def get_data_len(self):
        return self.data_length
    
    def stamp(self) -> None:
        self.millistamp, BasePacket.dt_midnight = get_day_millis()
        check_within_bits(self.millistamp, BasePacket.MILLISTAMP_BITS, "millistamp")

    def encode_header(self, stamp = True) -> bytearray:
        # Timestamp (27 bits), Topic (6 bits), Command (4), Length (11), data_length, CRC (16 bits)
        # 6 Bytes long
        
        if stamp:
            self.stamp()

        # Millistamp must exist
        if self.millistamp is None:
            raise PacketError("Must add millistamp before encoding")
        
        packet_ints = []
        packet_ints.append((self.millistamp >> 19) & 0xFF)              # Millistamp (8)
        packet_ints.append((self.millistamp >> 11) & 0xFF)              # Millistamp (8) 
        packet_ints.append((self.millistamp >> 3) & 0xFF)               # Millistamp (8) 
        
        packet_ints.append((( (self.millistamp << 5) & 0b11100000)      # Millistamp (3) and topic (5)
                        + ( (self.topic >> 1) & 0b00011111)) & 0xFF)
        packet_ints.append((( (self.topic << 7) & 0b10000000)           # Topic (1), command (4), and data_length (3)
                        + ( (self.command << 3) & 0b01111000)
                        + ( (self.data_length >> 8) & 0b00000111)) & 0xFF)
        packet_ints.append((self.data_length) & 0xFF)              # data_length(8)

        # print(packet_ints)
        header_bytes = bytearray(packet_ints)

        self.header_bytes = header_bytes
        return header_bytes
    
    @staticmethod
    def decode_header(header_bytes: bytearray) -> tuple[int, int, int, int]:
        
        if len(header_bytes) != 6:
            raise(PacketError("Header must be 6 bytes"))
        
        millistamp = ( (int(header_bytes[0]) << 19)
                     + (int(header_bytes[1]) << 11)
                     + (int(header_bytes[2]) << 3)
                     + (int(header_bytes[3] & 0b11100000) >> 5) )
        
        topic = ( (int(header_bytes[3] & 0b00011111) << 1 )
                + (int(header_bytes[4] & 0b10000000) >> 7)   )
        
        command = ( (int(header_bytes[4] & 0b01111000) >> 3) )

        data_length = ( (int(header_bytes[4] & 0b00000111) << 8)
                      + int(header_bytes[5]) )

        return millistamp, topic, command, data_length
    
    @staticmethod 
    def calc_crc(data: bytearray):
        # Verified using https://crccalc.com/
        return crc16xmodem(data)

    def packetize(self, encode_header = True, stamp = True, cobs_encode = True):

        # Encodes header with stamp preference if specified
        if encode_header:
            self.encode_header(stamp)

        if self.header_bytes is None:
            raise PacketError("Must encode header before packetizing")
        
        pre_crc = self.header_bytes + self.data

        self.crc = BasePacket.calc_crc(pre_crc)

        self.packet_bytes = pre_crc + bytearray([ (self.crc >> 8) & 0xFF, self.crc & 0xFF])

        # COBS encode
        if cobs_encode:                                          # Delimeter
            self.packet_bytes = cobs.encode(self.packet_bytes)

    @staticmethod 
    def depacketize(data_bytes: bytearray, cobs_decode = True):

        if cobs_decode:
            data_bytes = cobs.decode(data_bytes)

        # Extracting CRC
        crc_bytes = data_bytes[-2:]
        crc = int(crc_bytes[0] << 8) + int(crc_bytes[1])

        # Extracting pre-CRC data
        pre_crc = data_bytes[:-2]

        # Checking CRC
        decoded_crc = BasePacket.calc_crc(pre_crc)
        if decoded_crc != crc:
            raise PacketError(f"BasePacket CRC {hex(crc)} does not match decoded CRC {hex(decoded_crc)}")
        
        # Extracting header
        header_bytes = pre_crc[:BasePacket.HEADER_LENGTH_BYTES]
        millistamp, topic_id, command_id, data_length = BasePacket.decode_header(header_bytes)

        # Verify data length
        data = data_bytes[BasePacket.HEADER_LENGTH_BYTES:-2]
        if len(data) != data_length:
            raise PacketError(f"BasePacket data length ({data_length} bytes) does not match actual data length ({len(data)} bytes)")
        
        # Verify millistamp fits within a day
        if millistamp >= 86400000:
            raise PacketError(f"BasePacket millistamp {millistamp} does not fit within the length of a day (86400000 ms)")
        
        # TODO: Verify topic and command are within the current command schema

        packet = BasePacket()
        packet.millistamp = millistamp
        packet.topic = topic_id
        packet.command = command_id
        packet.data_length = data_length
        packet.data = data
        packet.packet_bytes = data_bytes
        packet.header_bytes = header_bytes

        return packet




        
