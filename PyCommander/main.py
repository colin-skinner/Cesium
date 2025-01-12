from telemetry.topic_packets import *
from telemetry.serial_comms import SerialComms, DEFAULT_PORT
# import telemetry.packets.telem_00_system_packets as SystemStatus
from telemetry.packets.clock_task import ClockTask
from time import sleep, time



if __name__ == "__main__":


    ports = DEFAULT_PORT.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    DEFAULT_PORT.connect(port_name)

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
    def reset():
        DEFAULT_PORT.port.dtr = False
        sleep(.1)
        DEFAULT_PORT.port.dtr = True
        sleep(.1)
    
    reset()

    timeout_start = time()
    while (DEFAULT_PORT.port.in_waiting > 0):
        received = DEFAULT_PORT.readline()
        print(received)

        if time() - timeout_start > 1:
            reset()

    # Jumping clock
    ClockTask.send_jump_clock()
    sleep(0.2)

    i = 0

    # comm.port.rtscts
    while True:
        print("Emitting packets...")

        # # SUM
        i += 1
        i %= 256
        packet.configure(SystemStatusCMD.SUM, bytearray([i, 0]))
        packet.packetize()
        DEFAULT_PORT.emit_packet(packet)
        print(packet)

        # print("success.")

        sleep(.01)

        while (DEFAULT_PORT.port.in_waiting > 0):
            received = DEFAULT_PORT.readline()
            # received += b"\x01"
            # print(received.hex())
            # print(received)
            # print(list(received))
            received_packet = BasePacket.depacketize(received)
            print(received_packet)


        # sleep(.005)

        # print()
        # sleep(.01)
        # i += 1