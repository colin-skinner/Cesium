import os, sys
sys.path.append(".") # Adds PyCommander 
print(os.getcwd())
from telemetry.packets.base_packet import Packet, PacketError
from telemetry.packets.topic_packets import *
from telemetry.packets.command_schema import *
import pytest

##############################
#     Grouped by Topic       #
##############################

def test_system_status_pkt():
    packet = SystemStatusPacket()
    packet.configure(SystemStatusCMD.ACK)

    assert packet.topic == Topic.SYSTEM_STATUS.value
    assert packet.command == SystemStatusCMD.ACK.value

def test_power_pkt():
    packet = PowerPacket()
    packet.configure(PowerCMD.BATTERY_STATS)

    assert packet.topic == Topic.POWER.value
    assert packet.command == PowerCMD.BATTERY_STATS.value

##############################
#     System Status Pkt      #
##############################



