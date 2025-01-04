from telemetry.packets.topic_packets import *
from telemetry.serial_comms import SerialComms

from time import sleep



if __name__ == "__main__":

    comm = SerialComms()

    ports = comm.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    comm.connect(port_name)

    packet_list: list[Packet] = list()

    # packet = SystemStatusPacket()
    # packet.configure(SystemStatusCMD.ACK, bytearray("THIS IS A PAYLOAD", "ascii"))
    # packet_list.append(packet)

    # packet = PowerPacket()
    # packet.configure(PowerCMD.BATTERY_STATS, bytearray("THIS IS A PAYLOAD ABOUT BATTERIES", "ascii"))
    # packet_list.append(packet)

    # packet = ActuatorPacket()
    # packet.configure(ActuatorCMD.STATUS, bytearray("THIS IS A PAYLOAD ABOUT ACTUATORS", "ascii"))
    # packet_list.append(packet)

    # packet = SystemStatusPacket()
    # packet.configure(SystemStatusCMD.SYSTEM_UPDATE, bytearray("UPDATING......", "ascii"))
    # packet_list.append(packet)

    # packet = SystemStatusPacket()
    # packet.configure(SystemStatusCMD.REQUEST_ACK, bytearray("Requesting ACK", "ascii"))
    # packet_list.append(packet)

    packet = SystemStatusPacket()
    packet.configure(4, bytearray(b"\x01\x07"))
    packet_list.append(packet)

    comm.port.rtscts
    while True:
        print("Emitting packet...",end="")

        for packet in packet_list:
            packet.packetize()
            comm.emit_packet(packet)
            print(packet.packet_bytes)

        # packet_list[0].packetize()
        # comm.emit_packet(packet_list[0])

        print("success.")

        sleep(.1)

        while (comm.port.in_waiting > 0):
            received = comm.readline()
            # print(list(received))
            received_packet = Packet.depacketize(received)
            print(received_packet)

        sleep(.5)