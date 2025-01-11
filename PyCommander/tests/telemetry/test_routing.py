from telemetry.packet_broker import Broker
from telemetry.packets import SystemStatusTask, ClockTask

from telemetry.base_packet import BasePacket

from telemetry.command_schema import *

# Tests all topics
def test_broker():
    for topic in list(Topic):

        if topic == Topic.BAD_TOPIC:
            continue
            
        packet = BasePacket()
        packet.configure(topic, 0, bytearray())
        packet.packetize()
        assert Broker.route_packet(packet) == topic

def test_elusive_bad_topic():
        packet = BasePacket()
        packet.configure(0, 0, bytearray())
        packet.topic = Topic.BAD_TOPIC

        assert Broker.route_packet(packet) == Topic.BAD_TOPIC
