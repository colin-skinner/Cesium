from ..topic_packets import FilesystemPacket
from ..base_packet import BasePacket
from ..command_schema import Topic, FilesystemCMD

from ..serial_comms import SerialComms, DEFAULT_PORT

from .packet_task import PacketTask

import struct

class FilesystemTask(PacketTask):

    @staticmethod
    def read_list_directory(data: bytearray) -> list[str]:

        filenames: list[str] = []

        # TODO: bounds checking
        
        index = 0
        while index < len(data):
            
            filename_len = data[index]
            index += 1

            # TODO: fix
            # if index + filename_len > len(data):
            #     return False

            filenames.append(struct.unpack(f"{filename_len}s", data[index:index+filename_len]))
            index += filename_len


        return filenames


