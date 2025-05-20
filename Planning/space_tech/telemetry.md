# Formatting
* Telemetry fields (8+ bytes)
    * Timestamp (27 bits)
        * 86,400,000 milliseconds in a day
            * 2^27 = 134,217,728
        * Sends "date update" to synchronize date every X seconds (5?)
    * Command Code (10 bits for 1024 commands/telemetries)
        * Stage 1: 6 bits for 64 topics, 4 bits for 16 commands 
        * Stage 2+: All enumerated in config 
    * Data length (11 for 2048 length)
    * Data (0-2048 bytes)
    * CRC (2 bytes)
* Each telemetry is at minimum 8 bytes

# Encoding Process
* Add
    * Command code (10 bitfs)
    * Data length (11 bits)
    * Data (0-2048 bytes)
    * Millistamp (27 bits)
* Calculate and add CRC
* COBS?

# Decoding Process
* COBS?
* Read in 6 bytes
    * uint32 timestamp
    * uint8_t topic, command
    * uint16_t length
* Read in data length
* Validate CRC

# Command Codes (loose list)
- System Status
    - Request ACK (RACK)
        - Receive ACK/NACK
    - ACK (response to RACK)
    - NACK (response to RACK)
    - Reset
    - Reset statistics
    - MCU stats (10Hz cycle count)
    - System update (with file on filesystem?)
- Power
    - Current/Voltage/Power from Battery (V, A, W, LSB)
    - Current/Voltage/Power from Ground (V, A, W, LSB)
    - Bus Current/Voltage/Power (V, A, W, LSB)
    - Get board power states (commanded, feedback, current A)
    - Switch board (on/off, board #)
- Radio
    - Set power (dBm, LSB)
    - Test/set/confirm mode switch (frequency, syncword, etc.)
    - Dump registers
- Clock
    - Clock status
    - Day sync (every 5 minutes)
    - Jump clock telem
    - Jump clock with next GPS
- Network
    - Network status
    - Connect with SSID and Password
    - ARP output
- File system
    - List directory
    - Write to file
    - Read from file (option to peek)
        - Each packet will have a counter byte
    - Delete file
    - Statistics (bytes used, bytes available, % usage)
- Scheduler (Writes .schdl files to filesystem)
- GPS
    - Basic Position Telemetry (lat, long, alt, SIV)
    - Position Accuracy (VDOP, HDOP)
    - Speed/heading (speed, )
    - Time Telemetry (sec, min, hour, day, month, year)
    - Read register(s)
    - Write register(s)
- IMU
    - Telemetry (accel, gyro, mag)
    - FIFO Status
    - FIFO Read (to file)
    - Read register(s)
    - Write register(s)
- Barometer
    - Telemetry (alt, temp)
    - Read register(s)
    - Write register(s)
- Shock accelerometer
    - Telemetry (accel)
    - FIFO Status
    - FIFO Read (to file)
    - Read register(s)
    - Write register(s)
- Navigation (outputs for each algorithm, Maybe duplicate GNC topics)
- Guidance (outputs for each algorithm, Maybe duplicate GNC topics)
- Control (outputs for each algorithm, Maybe duplicate GNC topics)
- Actuators (power switches)
    - Status (commanded states, actual states, current)
    - Command actuators
    - 


# Colors
- Green: Acks
- Blue: Telemetry
- Red: NACK