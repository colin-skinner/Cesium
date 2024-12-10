# Formatting
* Data length (2 bytes for 65'536 length, must be at least 6)
* Command Code (2 bytes for 65'536 commands/telemetries)
* Data size(2 bytes)
* Data (0-65'530 bytes)
* CRC (2 bytes)

# To process
* Verify data length
* 