#include "ImuTask.h"
#include "SystemStatusTask.h"

#include "../comms/serial_comms.h"

using namespace std;

namespace Cesium {

std::vector<Sensor::AccelerometerBase*> ImuTask::accels{};
std::vector<Sensor::GyroscopeBase*> ImuTask::gyros{};
std::vector<Sensor::MagnetometerBase*> ImuTask::mags{};


ImuCMD ImuTask::route_packet(BasePacket& packet) {
    ImuCMD command = (ImuCMD)packet.get_command();
    switch(command) {
    case ImuCMD::TELEM:
        DEBUGLN("TELEM");
        ImuTask::create_telem_packet(packet.get_data());
        break;

    case ImuCMD::FIFO_STATUS: // TODO
        DEBUGLN("FIFO_STATUS");
        SystemStatusTask::send_not_implemented("IMU::FIFO_STATUS");
        break;

    case ImuCMD::FIFO_READ: // TODO
        DEBUGLN("FIFO_READ");
        SystemStatusTask::send_not_implemented("IMU::FIFO_READ");
        break;

    case ImuCMD::READ_REG: // TODO
        DEBUGLN("READ_REG");
        SystemStatusTask::send_not_implemented("IMU::READ_REG");
        break;

    case ImuCMD::WRITE_REG: // TODO
        DEBUGLN("WRITE_REG");
        SystemStatusTask::send_not_implemented("IMU::WRITE_REG");
        break;


    default:
        DEBUGLN("Did not finish routing packet");
        SystemStatusTask::send_nack("BAD COMMAND");
        return ImuCMD(-1);
    }

    return command;
}

bool ImuTask::create_telem_packet(vector<uint8_t> data)
{
    // Packet size bounds

    size_t data_size = data.size();

    // Verifies data length
    if (data_size != 4) {
        String err = "Data size '" + String(data_size) + "' when 4 expected";
        DEBUGLN(err);
        send_telem_response(err.c_str());
        return false;
    }

    uint8_t accel_id = data[0];
    uint8_t gyro_id = data[1];
    uint8_t mag_id = data[2];
    Frame frame = (Frame)data[3];
    
    RETURN_FALSE_IF_FALSE(check_ranges(data));
    // TODO: Frame bounds checking

    Vector3<float> accel_data;
    Vector3<float> gyro_data;
    Vector3<float> mag_data;

    // Get data here, but for now, make fake vector
    // TODO: Implement real data
    if (frame == Frame::Sensor) {
        accels[accel_id]->read();
        accel_data = accels[accel_id]->get_accel_mps();
        gyro_data = gyros[gyro_id]->get_w_rps();
        mag_data = mags[mag_id]->get_B_uT();
    }
    // else if (frame == Frame::Sensor) {
    //     accel_data = accels[accel_id]->get_accel_mps();
    //     gyro_data = gyros[gyro_id]->get_w_rps();
    //     mag_data = mags[mag_id]->get_B_uT();
    // }

    // float temp_data = 0; // = temp_somewhere
    vector<uint8_t> response;
    response.resize(40);

    constexpr size_t vec_size = sizeof(Vector3<float>);

    // Serial.print("Norm: " + String(norm(accel_data)));
    // Serial.print("\t\tAccel: ");
    // for (auto i : accel_data) {
    //     Serial.print(i[0]);
    //     Serial.print("\t");
    // }
    // Serial.println();

    // ENDIANNESS PROBLEM
    // 12 = 3 * sizeof(float)
    vec_to_buffer(accel_data, response, 0);
    vec_to_buffer(gyro_data, response, 1 * vec_size);
    vec_to_buffer(accel_data, response, 2 * vec_size);
    
    uint8_t temp_temp[] = {0, 0, 0, 0};
    memcpy(&response[3*vec_size], temp_temp, 4);

    // vector<uint8_t> response = {
    //     data[0], data[1], data[2], data[3], // accelx
    //     1, 1, 1, 1, // accely
    //     2, 2, 2, 2, // accelz
    //     3, 3, 3, 3, // gyrox
    //     4, 4, 4, 4, // gyroy
    //     5, 5, 5, 5, // gyroz
    //     6, 6, 6, 6, // magx
    //     7, 7, 7, 7, // magy
    //     8, 8, 8, 8, // magz
    //     9, 9, 9, 9  // temp
    // };
    

    BasePacket packet;
    packet.configure((int)Topic::IMU, (int)ImuCMD::TELEM, response);
    packet.packetize();

    SerialComms::emit_packet(packet);


    return true;
}

bool ImuTask::send_telem_response(const char* message)
{
    vector<uint8_t> data(message, message + strlen(message));

    BasePacket packet;
    packet.configure((int)Topic::IMU, (int)ImuCMD::TELEM, data);
    packet.packetize();

    SerialComms::emit_packet(packet);

    return true;
}


bool ImuTask::check_ranges(vector<uint8_t>& data) {

    uint8_t accel_id = data[0];
    uint8_t gyro_id = data[1];
    uint8_t mag_id = data[2];

    // Range checking for Accel, gyro, and mag
    if (accel_id < 0 || accel_id > ImuTask::accels.size()) {
        String err = "accel ID '" + String(accel_id) + "' with maximum of " + String(ImuTask::accels.size()-1);
        DEBUGLN(err);
        send_telem_response(err.c_str());
        return false;
    }
    if (gyro_id < 0 || gyro_id > ImuTask::gyros.size()) {
        String err = "gyro ID '" + String(gyro_id) + "' with maximum of " + String(ImuTask::gyros.size()-1);
        DEBUGLN(err);
        send_telem_response(err.c_str());
        return false;
    }
    if (mag_id < 0 || mag_id > ImuTask::mags.size()) {
        String err = "mag ID '" + String(mag_id) + "' with maximum of " + String(ImuTask::mags.size()-1);
        DEBUGLN(err);
        send_telem_response(err.c_str());
        return false;
    }

    return true;
}



}