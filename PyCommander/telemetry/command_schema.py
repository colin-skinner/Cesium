from enum import Enum

##############################
#           Topic ID         #
##############################
class Topic(Enum):
    SYSTEM_STATUS = 0
    POWER = 1
    RADIO = 2
    CLOCK = 3
    NETWORK = 4
    FILESYSTEM = 5
    SCHEDULER = 6
    GPS = 7
    IMU = 8
    BAROMETER = 9
    SHOCK_ACCEL = 10
    
    GNC_NAVIGATION = 20
    GNC_GUIDANCE = 21
    GNC_CONTROL = 22
    ACTUATORS = 23
    
    TEST_ROCKET = 30

    BAD_TOPIC = -1

##############################
#         Command ID         #
##############################

class SystemStatusCMD(Enum):
    REQUEST_ACK = 0
    ACK = 1
    NACK = 2
    RESET = 3
    RESET_STATS = 4
    MCU_STATS = 5
    SYSTEM_UPDATE = 6
    SUM = 7

class PowerCMD(Enum):
    BATTERY_STATS = 0
    GSE_STATS = 1
    BUS_STATS = 2
    POWER_STATES = 3
    BOARD_SWITCHES = 4


class RadioCMD(Enum):
    SET_POWER = 0
    MODE_SWITCH = 1
    DUMP_REGS = 2

class ClockCMD(Enum):
    STATUS = 0
    DAY_SYNC = 1
    JUMP_CLOCK_TELEM = 2
    JUMP_CLOCK_GPS = 3

class NetworkCMD(Enum):
    STATUS = 0
    WIFI_CONNECT = 1
    ARP = 2
    PING = 3

class FilesystemCMD(Enum):
    LIST_DIR = 0
    WRITE_FILE = 1 # Includes directories
    READ_FILE = 2
    DELETE_FILE = 3 # Includes directories
    FILESYSTEM_STATS = 4

class SchedulerCMD(Enum):
    NotImplemented

class GpsCMD(Enum):
    BASIC_TELEM = 0
    POSITION_ACCURACY = 1
    SPEED_HEADING = 2
    TIME_TELEM = 3
    READ_REG = 4
    WRITE_REG = 5

class ImuCMD(Enum):
    TELEM = 0
    FIFO_STATUS = 1
    FIFO_READ = 2 # Reads fifo to a file
    READ_REG = 3
    WRITE_REG = 4

class BarometerCMD(Enum):
    TELEM = 0
    READ_REG = 1
    WRITE_REG = 2

class ShockAccelCMD(Enum):
    TELEM = 0
    FIFO_STATUS = 1
    FIFO_READ = 2 # Reads fifo to a file
    READ_REG = 3
    WRITE_REG = 4

class GncNavigationCMD(Enum):
    NotImplemented

class GncGuidanceCMD(Enum):
    NotImplemented

class GncControlCMD(Enum):
    NotImplemented

class ActuatorCMD(Enum):
    STATUS = 0
    COMMAND = 1







