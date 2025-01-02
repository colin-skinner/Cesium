from .base_packet import Packet, PacketError
from .command_schema import * # SORRY FOR WILDCARD

##############################
#     Grouped by Topic       #
##############################


class SystemStatusPacket(Packet):

    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.SYSTEM_STATUS, command, data)

class PowerPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.POWER, command, data)

class RadioPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.RADIO, command, data)

class ClockPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.CLOCK, command, data)

class NetworkPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.NETWORK, command, data)

class FilesystemPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.FILESYSTEM, command, data)

class SchedulerPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.SCHEDULER, command, data)

class GpsPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.GPS, command, data)

class ImuPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.IMU, command, data)

class BarometerPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.BAROMETER, command, data)

class ShockAccelPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.SHOCK_ACCEL, command, data)

class GncNavigationPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.GNC_NAVIGATION, command, data)

class GncGuidancePacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.GNC_GUIDANCE, command, data)

class GncControlPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.GNC_CONTROL, command, data)

class ActuatorPacket(Packet):
    def configure(self, command: Topic, data = bytearray()):
        base: Packet = super()
        base.configure(Topic.GNC_NAVIGATION, command, data)