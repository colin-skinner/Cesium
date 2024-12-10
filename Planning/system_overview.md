<!-- Networking -->

# Onboard
* Telemetry
    * Parsing
        *  Decoder
            * COBS -> Depacketizer -> Network
        * Encoder
            * Network -> Packetizer -> COBS
    * Timings
        * More important data will be shared more often in dedicated packets
* Vehicle Networking
    * Separate ports?
        * Telemetry -> 42001
        * Raw data -> 42002
        * Housekeeping -> 42004
        * Acks -> 42008
        * Test -> 42256
    * Single port - __42000__
* Network/Radio board (router)
    * Echos data from telem port on GSE side to port on vehicle network
    * Acts as gateway
    * Activates GPIOs for power over the bus


# Ground  
* Telemetry Parsing (42000)
    * Decoder
        * COBS -> Depacketizer -> Network
    * Encoder
        * Network -> Packetizer -> COBS
* Put data into SQL server (localhost port 1433)
* Telem Server (port 42001)
    * Search telemetry based on regex
    * Caching?
* FizzVizz (port 42002)
    * Using FastAPI/NiceGUI
    * Sends commands
