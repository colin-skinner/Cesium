## To-do List
- [x] Better way to signal end of packet than just '\n\ (0x04) because packet fails every time the character is included
    - Added COBS
- [ ] Packetization on ground (Python)
- [ ] Packetization on ESP32 (C++)
  - [ ] Finish Jump Clock command to fully timestamp packets 
  - [ ] Finish unit tests for packet routing (1/14 done)
- [ ] Command routing on ESP32
- [ ] RabbitMQ routing of messages