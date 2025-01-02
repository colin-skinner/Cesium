from telemetry.packets.topic_packets import *
from telemetry.serial_comms import SerialComms

from time import sleep



if __name__ == "__main__":

    comm = SerialComms()

    ports = comm.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    comm.connect(port_name)

    packet = SystemStatusPacket()
    
    data = bytearray("THIS IS A PAYLOAD", "ascii")
    packet.configure(SystemStatusCMD.ACK, data)
    packet.packetize()

    while True:
        print("Emitting packet...",end="")
        comm.emit_packet(packet)
        print("success.")

        comm.readline()

        sleep(1)