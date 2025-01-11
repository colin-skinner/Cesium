
import os, sys
sys.path.append(".") # Adds PyCommender 
print(os.getcwd())
from telemetry.base_packet import BasePacket, PacketError
import pytest
from cobs import cobs

@pytest.fixture
def packet():
    return BasePacket()
##############################
#           Topic ID         #
##############################

def test_topic_id(packet: BasePacket):
    packet.configure(1, 0, bytearray())

    assert packet.topic == 1
    assert packet.command == 0
    assert len(packet.data) == 0

    packet.configure(BasePacket.MAX_TOPIC_ID, 0, bytearray())

    assert packet.topic == BasePacket.MAX_TOPIC_ID
    assert packet.command == 0
    assert len(packet.data) == 0

def test_topic_id_edge_cases(packet: BasePacket):
    with pytest.raises(PacketError) as e_info:
        packet.configure(-1, 0, bytes())
    
    with pytest.raises(PacketError) as e_info:
        packet.configure(BasePacket.MAX_TOPIC_ID + 1, 0, bytes())

##############################
#         Command ID         #
##############################

def test_command_id(packet: BasePacket):
    packet.configure(1, 1, bytearray())

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == 0

    packet.configure(1, BasePacket.MAX_COMMAND_ID, bytearray())

    assert packet.topic == 1
    assert packet.command == BasePacket.MAX_COMMAND_ID
    assert len(packet.data) == 0

def test_command_id_edge_cases(packet: BasePacket):
    with pytest.raises(PacketError) as e_info:
        packet.configure(0, -1, bytes())
    
    with pytest.raises(PacketError) as e_info:
        packet.configure(1, BasePacket.MAX_COMMAND_ID + 1, bytes())



##############################
#             Data           #
##############################

def test_data(packet: BasePacket):
    packet.configure(1, 1, bytearray([i for i in range(2)]))

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == 2
    assert packet.data == bytearray(b'\x00\x01')

    
    packet.configure(1, 1, bytearray(BasePacket.MAX_DATA_LENGTH * [0]))

    assert packet.topic == 1
    assert packet.command == 1
    assert len(packet.data) == BasePacket.MAX_DATA_LENGTH

    
def test_data_length_edge_cases(packet: BasePacket):
    with pytest.raises(PacketError) as e_info:
        packet.configure(0, 0, bytearray( (BasePacket.MAX_DATA_LENGTH+1) * [0]))
        
##############################
#        Millistamp          #
##############################

def test_millistamp(packet: BasePacket):
    packet.configure(1, 1, bytearray())
    packet.stamp()

    milliseconds_in_day = 24 * 3600 * 1000
    assert packet.millistamp >= 0.0
    assert packet.millistamp < milliseconds_in_day

##############################
#       Encode Header        #
##############################

def test_encode_set_stamp(packet: BasePacket):
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

def test_header_random_stamp(packet: BasePacket):
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

def test_must_stamp(packet: BasePacket):

    packet.configure(1, 1, bytearray([1,2,3,4]))

    with pytest.raises(PacketError) as e_info:
        packet.encode_header(stamp=False)



##############################
#        Decode Header       #
##############################

def test_invalid_header_size():
    with pytest.raises(PacketError) as e_info:
        BasePacket.decode_header(bytearray([1,2,3,4]))

def test_decode_set_stamp(packet: BasePacket):
    packet.configure(12, 8, bytearray([1,2,3,4]))

    MILLISTAMP = 0x123456
    packet.millistamp = MILLISTAMP # Artificial timestamp
    header = packet.encode_header(stamp=False)

    millistamp, topic_id, command_id, data_length = BasePacket.decode_header(header)
    assert millistamp == MILLISTAMP
    assert topic_id == 12
    assert command_id == 8
    assert data_length == 4

def test_header_random_stamp(packet: BasePacket):
    packet.configure(3, 4, bytearray([1,2,3]))

    header = packet.encode_header()

    millistamp, topic_id, command_id, data_length = BasePacket.decode_header(header)

    assert millistamp == packet.millistamp
    assert topic_id == 3
    assert command_id == 4
    assert data_length == 3

##############################
#             CRC            #
##############################

def test_crc(packet: BasePacket):
    # Verified using https://crccalc.com/
    rslt = BasePacket.calc_crc(bytearray([1,2,3,4,5,6]))
    assert rslt == 0xD90C

    rslt = BasePacket.calc_crc(bytearray("This is a test", "ascii"))
    assert rslt == 0x982B

##############################
#         Packetizing        #
##############################

def test_must_encode_header(packet: BasePacket):

    packet.configure(1, 1, bytearray([1,2,3,4]))

    with pytest.raises(PacketError) as e_info:
        packet.packetize(encode_header=False, cobs_encode=False)

def test_with_crc(packet: BasePacket):

    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,3,4])
    expected_crc = 0x9C54
    expected_crc_bytes = bytearray([ (expected_crc >> 8) & 0xFF, expected_crc & 0xFF])

    packet.configure(1, 1, DATA)
    MILLISTAMP = 0x123456
    packet.millistamp = MILLISTAMP # Artificial timestamp
    packet.packetize(stamp=False, cobs_encode=False)

    assert packet.crc == expected_crc
    assert packet.packet_bytes == expected_header + DATA + expected_crc_bytes

def test_encode_with_cobs(packet: BasePacket):

    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,0,4])
    expected_crc = 0xC907 # Changed element 2 to 0

    expected_cobs_bytes = bytearray([0x09, 0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04, 0x01, 0x02, 0x04, 0x04, 0xC9, 0x07])
    

    packet.configure(1, 1, DATA)
    MILLISTAMP = 0x123456
    packet.millistamp = MILLISTAMP # Artificial timestamp
    packet.packetize(stamp=False, cobs_encode=True)

    assert packet.crc == expected_crc
    assert packet.packet_bytes == expected_cobs_bytes


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
        BasePacket.depacketize(packet_bytes, cobs_decode=False)

    assert str(e_info.value) == f"BasePacket CRC {hex(expected_crc + 0x0100)} does not match decoded CRC {hex(expected_crc)}"

def test_data_length_error():

    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    _, _, _, data_length = BasePacket.decode_header(expected_header)
    DATA = bytearray([1,2,3,4,5])
    expected_crc = 0x4690 # New CRC when data has 5 elements
    expected_crc_bytes = bytearray([ ((expected_crc >> 8) & 0xFF), expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    with pytest.raises(PacketError) as e_info:
        BasePacket.depacketize(packet_bytes, cobs_decode=False)

    assert str(e_info.value) == f"BasePacket data length ({data_length} bytes) does not match actual data length ({len(DATA)} bytes)"

def test_millistamp_error():

    # Changed MSB of first byte to 0xF 
    expected_header =bytearray([0xF2, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    DATA = bytearray([1,2,3,4])
    millistamp, _, _, _ = BasePacket.decode_header(expected_header)
    expected_crc = 0xA7E1 # New CRC when millistamp changes
    expected_crc_bytes = bytearray([ ((expected_crc >> 8) & 0xFF), expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    with pytest.raises(PacketError) as e_info:
        BasePacket.depacketize(packet_bytes, cobs_decode=False)

    assert str(e_info.value) == f"BasePacket millistamp {millistamp} does not fit within the length of a day (86400000 ms)"

def test_depacketize():

    expected_header =bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    millistamp, topic_id, command_id, data_length = BasePacket.decode_header(expected_header)
    DATA = bytearray([1,2,3,4])
    expected_crc = 0x9C54
    expected_crc_bytes = bytearray([ (expected_crc >> 8) & 0xFF, expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    packet = BasePacket.depacketize(packet_bytes, cobs_decode=False)

    assert packet.millistamp == millistamp
    assert packet.topic == topic_id
    assert packet.command == command_id
    assert packet.data_length == data_length
    assert packet.data == DATA
    assert packet.packet_bytes == packet_bytes
    assert packet.header_bytes == expected_header

def test_decode_with_cobs():

    ## Expected packet stuff
    expected_header = bytearray([0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04])
    millistamp, topic_id, command_id, data_length = BasePacket.decode_header(expected_header)
    DATA = bytearray([1,2,0,4])
    expected_crc = 0xC907 # Changed element 2 to 0
    expected_crc_bytes = bytearray([ (expected_crc >> 8) & 0xFF, expected_crc & 0xFF]) 

    packet_bytes = expected_header + DATA + expected_crc_bytes

    # COBS bytes
    cobs_bytes = bytearray([0x09, 0x02, 0x46, 0x8A, 0xC0, 0x88, 0x04, 0x01, 0x02, 0x04, 0x04, 0xC9, 0x07])
    
    packet = BasePacket.depacketize(cobs_bytes, cobs_decode=True)

    assert packet.millistamp == millistamp
    assert packet.topic == topic_id
    assert packet.command == command_id
    assert packet.data_length == data_length
    assert packet.data == DATA
    assert packet.packet_bytes == packet_bytes
    assert packet.header_bytes == expected_header


    


