from ..topic_packets import ClockPacket
from ..base_packet import BasePacket, get_day_millis
from ..command_schema import Topic, ClockCMD

from ..serial_comms import SerialComms, DEFAULT_PORT

from .packet_task import PacketTask

class ClockTask(PacketTask):

    @staticmethod
    def route_packet(packet: ClockPacket) -> ClockCMD:

        if (packet.topic != Topic.SYSTEM_STATUS.value):
            print(f"Routing - BasePacket Topic {packet.topic} not {Topic.SYSTEM_STATUS.value}")
            
        command = ClockCMD(packet.command)

        print(command)

        match command:
            case ClockCMD.STATUS:
                print("REQUEST ACK")
                pass

            case ClockCMD.DAY_SYNC:
                print("ACK")
                pass

            case ClockCMD.JUMP_CLOCK_TELEM:
                print("JUMP_CLOCK_TELEM")
                pass

            case ClockCMD.JUMP_CLOCK_GPS:
                print("JUMP_CLOCK_GPS")
                pass

            case _:
                print("Did not route packet")
                return 
            
        return command
    
    def send_jump_clock(offset_ms: int = 0):
        
        millis, date_time = get_day_millis()
        message: list[int] = []

        # # If adding millistamp
        # message.append( (millis >> 24) & 0xFF)
        # message.append( (millis >> 16) & 0xFF)
        # message.append( (millis >> 8) & 0xFF)
        # message.append( millis & 0xFF )

        message.append(date_time.day) # 1-31
        message.append(date_time.month) # 1-12
        message.append( (date_time.year >> 8) & 0xFF) # 1-9999
        message.append(date_time.year & 0xFF) # 1-9999

        packet = ClockPacket()
        packet.configure(ClockCMD.JUMP_CLOCK_TELEM, bytearray(message))
        packet.packetize()

        DEFAULT_PORT.emit_packet(packet)

        

