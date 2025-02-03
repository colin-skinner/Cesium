from telemetry.topic_packets import *
from telemetry.serial_comms import SerialComms, DEFAULT_PORT
from telemetry.packets.filesystem_task import FilesystemTask
from telemetry.packets.clock_task import ClockTask
from time import sleep, time



if __name__ == "__main__":


    ports = DEFAULT_PORT.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    DEFAULT_PORT.connect(port_name)

    packet_list: list[BasePacket] = list()


    filesystem_packet = FilesystemPacket()
    filesystem_packet.configure(FilesystemCMD.LIST_DIR, bytearray("/test", "ascii"))
    filesystem_packet.packetize()


    clock_packet = ClockPacket()

    DEFAULT_PORT.restart_port()

    # Jumping clock
    ClockTask.send_jump_clock()
    sleep(0.2)

    i = 0

    # comm.port.rtscts
    while True:
        print("Emitting packets...")

        DEFAULT_PORT.emit_packet(filesystem_packet)

        sleep(.01)

        while (DEFAULT_PORT.port.in_waiting > 0):
            received = DEFAULT_PORT.readline()
            # received += b"\x01"
            # print(received.hex())
            # print(received)
            # print(list(received))
            received_packet = BasePacket.depacketize(received)
            # print(received_packet)

            if (FilesystemCMD)(received_packet.command) == FilesystemCMD.LIST_DIR:

                names = FilesystemTask.read_list_directory(received_packet.data)

                print(names)
                # print(len(names))


        # sleep(.005)

        # print()
        sleep(1)
        # i += 1