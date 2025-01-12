from ..topic_packets import SystemStatusPacket
from ..base_packet import BasePacket
from ..command_schema import Topic, SystemStatusCMD


from .packet_task import PacketTask



class SystemStatusTask(PacketTask):
    
    @staticmethod
    def route_packet(packet: SystemStatusPacket) -> SystemStatusCMD:


        if (packet.topic != Topic.SYSTEM_STATUS.value):
            print(f"Routing - BasePacket Topic {packet.topic} not {Topic.SYSTEM_STATUS.value}")
            
        command = SystemStatusCMD(packet.command)

        print(command)

        match command:
            case SystemStatusCMD.REQUEST_ACK:
                print("REQUEST ACK")
                pass

            case SystemStatusCMD.ACK:
                print("ACK")
                pass

            case SystemStatusCMD.NACK:
                print("REQUEST NACK")
                pass

            case SystemStatusCMD.ACK:
                print("REQUEST ACK")
                pass

            case SystemStatusCMD.RESET:
                print("RESET")
                pass

            case SystemStatusCMD.RESET_STATS:
                print("RESET_STATS")
                pass

            case SystemStatusCMD.MCU_STATS:
                print("MCU_STATS")
                pass

            case SystemStatusCMD.SUM:
                print("SUM")
                pass

            case _:
                print("Did not route packet")
                return 
            
        return command

    # @staticmethod
    # def send_ack(method = "serial"):
        
