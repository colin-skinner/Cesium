#include "ina233.h"
#include "../globals.h"

namespace Cesium {
    
Ina233::Ina233(uint8_t ina_addr) : addr(ina_addr) {}

bool Ina233::setup()
{
    device.begin();

    CAL = device.setCalibration(R_shunt_IC1,I_max_IC1,&Current_LSB,&Power_LSB,&m_c,&R_c,&m_p,&R_p,&Set_ERROR);
    
    // Changes polarity of ALERT pin
    device.wireWriteByte(MFR_DEVICE_CONFIG, 0x06);

    return true;
}

bool Ina233::read()
{
    bus_voltage_V = device.getBusVoltage_V();
    DEBUG("Bus Voltage:   "); DEBUG(bus_voltage_V); DEBUGLN(" V, ");

    return true;
}

}