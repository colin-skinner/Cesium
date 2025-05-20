# Parts/Modules
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


# Tools
* supervisord
* Examples of multi service containers
    * https://docs.docker.com/engine/containers/multi-service_container/