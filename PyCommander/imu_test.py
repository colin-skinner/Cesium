from telemetry.topic_packets import BasePacket, ImuPacket, SystemStatusPacket, ImuCMD, Topic
from telemetry.serial_comms import SerialComms, DEFAULT_PORT
# import telemetry.packets.telem_00_system_packets as SystemStatus
from telemetry.packets.clock_task import ClockTask
from time import sleep, time

import struct


if __name__ == "__main__":


    ports = DEFAULT_PORT.list_ports(print_output=True)
    port_num = int(input("Which port to connect to: "))

    port_name = f"/dev/{ports[port_num].name}"
    DEFAULT_PORT.connect(port_name)


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


    packet = ImuPacket()

    DEFAULT_PORT.restart_port()

    # Jumping clock
    ClockTask.send_jump_clock()
    sleep(0.2)

    i = 0

    sleep(1)

    # comm.port.rtscts
    while True:
        print("Emitting packets...")

        # # SUM
        i += 1
        i %= 256
        packet.configure(ImuCMD.TELEM, bytearray([0, 0, 0, 0]))
        packet.packetize()
        DEFAULT_PORT.emit_packet(packet)
        # print(packet)

        # print("success.")

        

        while (DEFAULT_PORT.port.in_waiting > 0):
            received = DEFAULT_PORT.readline()
            # received += b"\x01"
            # print(received.hex())
            # print(received)
            # print(list(received))
            received_packet = BasePacket.depacketize(received)

            # print(received_packet)

            if Topic(received_packet.topic) != Topic.IMU:
                continue

            data = received_packet.data

            # print(data)
            # print(len(data))

            accel_x = struct.unpack("f", data[0:4])[0]
            accel_y = struct.unpack("f", data[4:8])[0]
            accel_z = struct.unpack("f", data[8:12])[0]

            w_x = struct.unpack("f", data[12:16])[0]
            w_y = struct.unpack("f", data[16:20])[0]
            w_z = struct.unpack("f", data[20:24])[0]

            mag_x = struct.unpack("f", data[24:28])[0]
            mag_y = struct.unpack("f", data[28:32])[0]
            mag_z = struct.unpack("f", data[32:36])[0]
            
            temp = struct.unpack("f", data[36:])[0]


            
            
            print(f"Accel: {accel_x:.4f}, {accel_y:.4f}, {accel_z:.4f} ", end="")
            print(f"Gyro: {w_x:.4f}, {w_y:.4f}, {w_z:.4f} ", end="")
            print(f"Mag: {mag_x:.4f}, {mag_y:.4f}, {mag_z:.4f} ", end="")
            print(f"Temp: {temp:.4f}")
            


        sleep(.1)

        # print()
        # sleep(.01)
        # i += 1