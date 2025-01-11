from telemetry.topic_packets import *
from telemetry.serial_comms import SerialComms
# import telemetry.packets.telem_00_system_packets as SystemStatus
from time import sleep



if __name__ == "__main__":

    comm = SerialComms()

    ports = comm.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    comm.connect(port_name)

    packet_list: list[BasePacket] = list()

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

    i = 3
    packet = SystemStatusPacket()
    clock_packet = ClockPacket()

    # Resets ESP32
    comm.port.dtr = False
    sleep(.1)
    comm.port.dtr = True
    sleep(1)
    
    while (comm.port.in_waiting > 0):
            received = comm.readline()
            print(received)

    # comm.port.rtscts
    while True:
        print("Emitting packets...")

        # clock_packet.configure(ClockCMD.JUMP_CLOCK_TELEM, )

        packet.configure(SystemStatusCMD.SUM, bytearray([3, 8]))
        packet.packetize()
        comm.emit_packet(packet)
        # print(packet)

        # packet_list[0].packetize()
        # comm.emit_packet(packet_list[0])

        print("success.")

        sleep(.1)

        while (comm.port.in_waiting > 0):
            received = comm.readline()
            # print(received)
            # print(list(received))
            received_packet = BasePacket.depacketize(received)
            print(received_packet)


        # sleep(.1)

        print()
        # sleep(.01)
        # i += 1