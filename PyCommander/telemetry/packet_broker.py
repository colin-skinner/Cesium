from .command_schema import *
from .base_packet import BasePacket, PacketError
from .packets import *


class Broker:

    

    @staticmethod
    def route_packet(packet: BasePacket) -> Topic:

        topic = Topic(packet.topic)

        match topic:
            case Topic.SYSTEM_STATUS:
                print("Received System Status BasePacket - ", end="")
                SystemStatusTask.route_packet(packet)

            case Topic.POWER:
                print("Received Power BasePacket - ", end="")

            case Topic.RADIO:
                print("Received Radio BasePacket - ", end="")

            case Topic.CLOCK:
                print("Received Clock BasePacket - ", end="")
                ClockTask.route_packet(packet)

            case Topic.NETWORK:
                print("Received Network BasePacket - ", end="")

            case Topic.FILESYSTEM:
                print("Received Filesystem BasePacket - ", end="")

            case Topic.SCHEDULER:
                print("Received Scheduler BasePacket - ", end="")

            case Topic.GPS:
                print("Received GPS BasePacket - ", end="")

            case Topic.IMU:
                print("Received IMU BasePacket - ", end="")

            case Topic.BAROMETER:
                print("Received Barometer BasePacket - ", end="")

            case Topic.SHOCK_ACCEL:
                print("Received Shock Accel BasePacket - ", end="")

            case Topic.GNC_NAVIGATION:
                print("Received GNC Navigation BasePacket - ", end="")

            case Topic.GNC_GUIDANCE:
                print("Received GNC Guidance BasePacket - ", end="")

            case Topic.GNC_CONTROL:
                print("Received GNC Control BasePacket - ", end="")

            case Topic.ACTUATORS:
                print("Received Actuators BasePacket - ", end="")

            case Topic.BAD_TOPIC:
                print("Received Bad Topic BasePacket. How did you get here?!?")

            case _:
                print("Did not route packet")
                return Topic.BAD_TOPIC
            

        return topic



