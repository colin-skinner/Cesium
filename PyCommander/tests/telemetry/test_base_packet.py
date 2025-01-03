
import os, sys
sys.path.append(".") # Adds PyCommender 
print(os.getcwd())
from telemetry.packets.base_packet import Packet, PacketError
import pytest


@pytest.fixture
def packet():
    return Packet()
##############################
#           Topic ID         #
##############################

def test_topic_id(packet: Packet):
    packet.configure(1, 0, bytearray())

    assert packet.topic == 1
    assert packet.command == 0
    assert len(packet.data) == 0

    packet.configure(Packet.MAX_TOPIC_ID, 0, bytearray())

    assert packet.topic == Packet.MAX_TOPIC_ID
    assert packet.command == 0
    assert len(packet.data) == 0

def test_topic_id_edge_cases(packet: Packet):
    with pytest.raises(PacketError) as e_info:
        packet.configure(-1, 0, bytes())
    
    with pytest.raises(PacketError) as e_info:
        packet.configure(Packet.MAX_TOPIC_ID + 1, 0, bytes())

##############################
#         Command ID         #
##############################

def test_command_id(packet: Packet):
    packet.configure(1, 1, bytearray())

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == 0

    packet.configure(1, Packet.MAX_COMMAND_ID, bytearray())

    assert packet.topic == 1
    assert packet.command == Packet.MAX_COMMAND_ID
    assert len(packet.data) == 0

def test_command_id_edge_cases(packet: Packet):
    with pytest.raises(PacketError) as e_info:
        packet.configure(0, -1, bytes())
    
    with pytest.raises(PacketError) as e_info:
        packet.configure(1, Packet.MAX_COMMAND_ID + 1, bytes())



##############################
#             Data           #
##############################

def test_data(packet: Packet):
    packet.configure(1, 1, bytearray([i for i in range(2)]))

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == 2
    assert packet.data == bytearray(b'\x00\x01')

    
    packet.configure(1, 1, bytearray(Packet.MAX_DATA_LENGTH * [0]))

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == Packet.MAX_DATA_LENGTH

    
def test_data_length_edge_cases(packet: Packet):
    with pytest.raises(PacketError) as e_info:
        packet.configure(0, 0, bytearray( (Packet.MAX_DATA_LENGTH+1) * [0]))
        
##############################
#        Millistamp          #
##############################

def test_millistamp(packet: Packet):
    packet.configure(1, 1, bytearray())
    packet.stamp()

    milliseconds_in_day = 24 * 3600 * 1000
    assert packet.millistamp >= 0.0
    assert packet.millistamp < milliseconds_in_day

##############################
#       Encode Header        #
##############################

def test_encode_set_stamp(packet: Packet):
    # ["000 0001 0", -> millistamp
    #  "010 0011 0", -> millistamp
    #  "100 0101 0", -> millistamp
    #  "110 00000", -> millistamp, topic
    #  "1 0001 000", -> topic, command, data_length
    #  "00000100"] 4 -> data_length 
    packet.configure(1, 1, bytearray([1,2,3,4]))

    packet.millistamp = 0x123456 # Artificial timestamp
    packet.encode_header(stamp=False)


    assert list(packet.header_bytes) == [0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04]

def test_header_random_stamp(packet: Packet):
    # ["XXXXXXXX", -> millistamp
    #  "XXXXXXXX", -> millistamp
    #  "XXXXXXXX", -> millistamp
    #  "XXX 00000", -> millistamp, topic
    #  "1 0001 000", -> topic, command, data_length
    #  "00000100"] 4 -> data_length 
    packet.configure(1, 1, bytearray([1,2,3,4]))

    packet.encode_header()


    assert (packet.header_bytes[3] & 0b00011111) == 0
    assert packet.header_bytes[4] == 0x88
    assert packet.header_bytes[5] == 0x04

def test_must_stamp(packet: Packet):

    packet.configure(1, 1, bytearray([1,2,3,4]))

    with pytest.raises(PacketError) as e_info:
        packet.encode_header(stamp=False)



##############################
#        Decode Header       #
##############################

def test_invalid_header_size():
    with pytest.raises(PacketError) as e_info:
        Packet.decode_header(bytearray([1,2,3,4]))

def test_decode_set_stamp(packet: Packet):
    packet.configure(12, 8, bytearray([1,2,3,4]))

    MILLISTAMP = 0x123456
    packet.millistamp = MILLISTAMP # Artificial timestamp
    header = packet.encode_header(stamp=False)

    millistamp, topic_id, command_id, data_length = Packet.decode_header(header)
    assert millistamp == MILLISTAMP
    assert topic_id == 12
    assert command_id == 8
    assert data_length == 4

def test_header_random_stamp(packet: Packet):
    packet.configure(3, 4, bytearray([1,2,3]))

    header = packet.encode_header()

    millistamp, topic_id, command_id, data_length = Packet.decode_header(header)

    assert millistamp == packet.millistamp
    assert topic_id == 3
    assert command_id == 4
    assert data_length == 3

##############################
#             CRC            #
##############################

def test_crc(packet: Packet):
    # Verified using https://crccalc.com/
    rslt = Packet.calc_crc(bytearray([1,2,3,4,5,6]))
    assert rslt == 0xD90C

    rslt = Packet.calc_crc(bytearray("This is a test", "ascii"))
    assert rslt == 0x982B

##############################
#         Packetizing        #
##############################

def test_must_encode_header(packet: Packet):

    packet.configure(1, 1, bytearray([1,2,3,4]))

    with pytest.raises(PacketError) as e_info:
        packet.packetize(encode_header=False)

def test_with_crc(packet: Packet):

    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,3,4])
    expected_crc = 0x9C54
    expected_crc_bytes = bytearray([ (expected_crc >> 8) & 0xFF, expected_crc & 0xFF])

    packet.configure(1, 1, DATA)
    MILLISTAMP = 0x123456
    packet.millistamp = MILLISTAMP # Artificial timestamp
    packet.packetize(stamp=False)

    assert packet.crc == expected_crc
    assert packet.packet_bytes == expected_header + DATA + expected_crc_bytes


##############################
#        Depacketizing       #
##############################

def test_crc_error():

    expected_header =bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,3,4])
    expected_crc = 0x9C54
    expected_crc_bytes = bytearray([ ((expected_crc >> 8) & 0xFF) + 1, expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    with pytest.raises(PacketError) as e_info:
        Packet.depacketize(packet_bytes)

    assert str(e_info.value) == f"Packet CRC {hex(expected_crc + 0x0100)} does not match decoded CRC {hex(expected_crc)}"

def test_data_length_error():

    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    _, _, _, data_length = Packet.decode_header(expected_header)
    DATA = bytearray([1,2,3,4,5])
    expected_crc = 0x4690 # New CRC when data has 5 elements
    expected_crc_bytes = bytearray([ ((expected_crc >> 8) & 0xFF), expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    with pytest.raises(PacketError) as e_info:
        Packet.depacketize(packet_bytes)

    assert str(e_info.value) == f"Packet data length ({data_length} bytes) does not match actual data length ({len(DATA)} bytes)"

def test_millistamp_error():

    # Changed MSB of first byte to 0xF 
    expected_header =bytearray([0xF2, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,3,4])
    millistamp, _, _, _ = Packet.decode_header(expected_header)
    expected_crc = 0xA7E1 # New CRC when millistamp changes
    expected_crc_bytes = bytearray([ ((expected_crc >> 8) & 0xFF), expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    with pytest.raises(PacketError) as e_info:
        Packet.depacketize(packet_bytes)

    assert str(e_info.value) == f"Packet millistamp {millistamp} does not fit within the length of a day (86400000 ms)"

def test_depacketize():

    expected_header =bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    millistamp, topic_id, command_id, data_length = Packet.decode_header(expected_header)
    DATA = bytearray([1,2,3,4])
    expected_crc = 0x9C54
    expected_crc_bytes = bytearray([ (expected_crc >> 8) & 0xFF, expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    packet = Packet.depacketize(packet_bytes)

    assert packet.millistamp == millistamp
    assert packet.topic == topic_id
    assert packet.command == command_id
    assert packet.data_length == data_length
    assert packet.data == DATA
    assert packet.packet_bytes == packet_bytes
    assert packet.header_bytes == expected_header


    


