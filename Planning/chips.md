
# Microcontrollers
* ESP32-WROOM-32E
    * Has Ethernet MAC
    
# Ducers
* ADS8688

# Load Cells
* ADS1256

# Load Switching
* Simple circuit with transistors
    * NMOS pulling down PMOS to activate switch
* Parts
    * NMOS (https://www.st.com/resource/en/datasheet/cd00005134.pdf)
        * V_b = 60V
        * Drain current = 0.2A
    * PMOS (https://www.st.com/resource/en/datasheet/std46p4llf6.pdf)
        * V_b = 40V
        * Drain current = 32.5A
    

# Current sensing
* INA233 and PCA9544A
    * 16 total (4 I2C with 4 MUX)
    * Bus voltage
    * 15A max
* INA4180 and ADS8688
    * 8 total (8 single)
    * Multiple ADS8688
    * 40A max

# Feedback sensing
* ADS7957 (8-bit 16 channel)