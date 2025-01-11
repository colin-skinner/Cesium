import os, sys
sys.path.append(".") # Adds PyCommander 
print(os.getcwd())
from telemetry.base_packet import BasePacket, PacketError
from telemetry.topic_packets import *
from telemetry.command_schema import *

import telemetry.packets as packets


 
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

def test_clock_pkt():
    packet = ClockPacket()
    packet.configure(ClockCMD.STATUS)

    assert packet.topic == Topic.CLOCK.value
    assert packet.command == ClockCMD.STATUS.value

##############################
#     System Status Pkt      #
##############################





