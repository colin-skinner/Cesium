#pragma once

#include <Arduino.h>

enum class Topic {
    SYSTEM_STATUS,
    POWER = 1,
    RADIO = 2,
    CLOCK = 3,
    NETWORK = 4,
    FILESYSTEM = 5,
    SCHEDULER = 6,
    GPS = 7,
    IMU = 8,
    BAROMETER = 9,
    SHOCK_ACCEL = 10,
    GNC_NAVIGATION = 11,
    GNC_GUIDANCE = 12,
    GNC_CONTROL = 13,
    ACTUATORS = 14,

    TEST_ROCKETS = 30

};

// ##############################
// #         Command ID         #
// ##############################

enum class SystemStatusCMD {
    REQUEST_ACK = 0,
    ACK = 1,
    NACK = 2,
    RESET = 3,
    RESET_STATS = 4,
    MCU_STATS = 5,
    SYSTEM_UPDATE = 6,
    SUM = 7,
    NOT_IMPLEMENTED = 15
};

enum class PowerCMD {
    BATTERY_STATS = 0,
    GSE_STATS = 1,
    BUS_STATS = 2,
    POWER_STATES = 3,
    BOARD_SWITCHES = 4
};

enum class RadioCMD {
    SET_POWER = 0,
    MODE_SWITCH = 1,
    DUMP_REGS = 2
};

enum class ClockCMD {
    STATUS = 0,
    DAY_SYNC = 1,
    JUMP_CLOCK_TELEM = 2,
    JUMP_CLOCK_GPS = 3
};

enum class NetworkCMD {
    STATUS = 0,
    WIFI_CONNECT = 1,
    ARP = 2,
    PING = 3
};


enum class FilesystemCMD {
    LIST_DIR = 0,
    WRITE_FILE = 1, // Includes directories
    READ_FILE = 2,
    DELETE_FILE = 3, // Includes directories
    FILESYSTEM_STATS = 4,
    SUCCESS = 15

};

enum class SchedulerCMD {};
//     NotImplemented

enum class GpsCMD {
    BASIC_TELEM = 0,
    POSITION_ACCURACY = 1,
    SPEED_HEADING = 2,
    TIME_TELEM = 3,
    READ_REG = 4,
    WRITE_REG = 5
};

enum class ImuCMD {
    TELEM = 0,
    FIFO_STATUS = 1,
    FIFO_READ = 2, // Reads fifo to a file
    READ_REG = 3,
    WRITE_REG = 4
};

enum class BarometerCMD {
    TELEM = 0,
    READ_REG = 1,
    WRITE_REG = 2
};

enum class ShockAccelCMD {
    TELEM = 0,
    FIFO_STATUS = 1,
    FIFO_READ = 2, // Reads fifo to a file
    READ_REG = 3,
    WRITE_REG = 4
};

enum class GncNavigationCMD {};

enum class GncGuidanceCMD {
//     NotImplemented
};

enum class GncControlCMD {};
//     NotImplemented

enum class ActuatorCMD {
    STATUS = 0,
    COMMAND = 1
};

enum class TestRocketsCMD {
    PACKET_A = 0,
    PACKET_B = 1
};






