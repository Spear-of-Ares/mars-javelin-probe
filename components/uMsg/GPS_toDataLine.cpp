#include "GPS_toDataLine.h"

std::string GPS_state_toString(umsg_GPS_gps_state_t state)
{
    switch (state)
    {
    case GPS_OK:
        return "OK";
    case GPS_SLEEPING:
        return "SLEEPING";
    case GPS_OFF:
        return "OFF";
    case GPS_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "?";
    }
}

DataLine GPS_data_toDataLine(umsg_GPS_data_t &data)
{
    DataLine dataline;
    dataline.name = "GPS";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.lat_long[0]));
    dataline.data.push_back(std::to_string(data.lat_long[1]));
    dataline.data.push_back(std::to_string((float)(data.altitude / 1000.0))); // Convert mm to m
    dataline.data.push_back(std::to_string(data.ground_speed));
    dataline.data.push_back(std::to_string(data.p_dilution_precision));
    dataline.data.push_back(std::to_string(data.time_ymd_hms[0]) + "-" + std::to_string(data.time_ymd_hms[1]) + "-" + std::to_string(data.time_ymd_hms[2]));
    dataline.data.push_back(std::to_string(data.time_ymd_hms[3]) + ":" + std::to_string(data.time_ymd_hms[4]) + ":" + std::to_string(data.time_ymd_hms[5]) + ":" + std::to_string(data.time_ymd_hms[6]));
    dataline.data.push_back(std::to_string(data.valid_time));
    dataline.data.push_back(std::to_string(data.locked_sats));

    return dataline;
}
DataLine GPS_configuration_toDataLine(umsg_GPS_configuration_t &data)
{
    DataLine dataline;
    dataline.name = "GPS_CONF";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.sample_freq_hz));

    return dataline;
}
DataLine GPS_state_toDataLine(umsg_GPS_state_t &data)
{
    DataLine dataline;
    dataline.name = "GPS_STATE";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(GPS_state_toString(data.state));
    dataline.data.push_back(std::to_string(data.initializing) + std::to_string(data.not_enough_sats) + std::to_string(data.receiving_data) + std::to_string(data.reconnecting) + std::to_string(data.not_present));

    return dataline;
}