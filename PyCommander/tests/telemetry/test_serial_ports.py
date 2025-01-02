import os, sys
sys.path.append(".") # Adds PyCommender 
print(os.getcwd())

from telemetry.serial_comms import SerialComms

def test_list_ports():
    ports = SerialComms.list_ports()

    assert len(ports) > 0
    for p in ports:
        print(p.name)
        assert "cu" in p.name # Hmm I think this only works on Mac

# Exception if port is not started


# def test_serial_connection():
#     PORT_NAME = "/dev/cu.usbserial-0001"
#     BAUD_RATE = 115200
#     serial_port = SerialComms()
#     serial_port.connect(PORT_NAME, BAUD_RATE)