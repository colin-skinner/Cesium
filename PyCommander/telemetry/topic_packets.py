from .base_packet import BasePacket, PacketError
from .command_schema import * # SORRY FOR WILDCARD

##############################
#     Grouped by Topic       #
##############################


class SystemStatusPacket(BasePacket):
    def configure(self, command: SystemStatusCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.SYSTEM_STATUS, command, data)

class PowerPacket(BasePacket):
    def configure(self, command: PowerCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.POWER, command, data)

class RadioPacket(BasePacket):
    def configure(self, command: RadioCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.RADIO, command, data)

class ClockPacket(BasePacket):
    def configure(self, command: ClockCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.CLOCK, command, data)

class NetworkPacket(BasePacket):
    def configure(self, command: NetworkCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.NETWORK, command, data)

class FilesystemPacket(BasePacket):
    def configure(self, command: FilesystemCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.FILESYSTEM, command, data)

class SchedulerPacket(BasePacket):
    def configure(self, command: SchedulerCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.SCHEDULER, command, data)

class GpsPacket(BasePacket):
    def configure(self, command: GpsCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.GPS, command, data)

class ImuPacket(BasePacket):
    def configure(self, command: ImuCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.IMU, command, data)

class BarometerPacket(BasePacket):
    def configure(self, command: BarometerCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.BAROMETER, command, data)

class ShockAccelPacket(BasePacket):
    def configure(self, command: ShockAccelCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.SHOCK_ACCEL, command, data)

class GncNavigationPacket(BasePacket):
    def configure(self, command: GncNavigationCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.GNC_NAVIGATION, command, data)

class GncGuidancePacket(BasePacket):
    def configure(self, command: GncGuidanceCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.GNC_GUIDANCE, command, data)

class GncControlPacket(BasePacket):
    def configure(self, command: GncControlCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.GNC_CONTROL, command, data)

class ActuatorPacket(BasePacket):
    def configure(self, command: ActuatorCMD, data = bytearray()):
        base: BasePacket = super()
        base.configure(Topic.ACTUATORS, command, data)