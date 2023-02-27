#include "Sensors_toDataLine.h"

std::string sensor_state_toString(umsg_Sensors_sensor_state_t state)
{
    switch (state)
    {
    case SENSOR_OK:
        return "OK";
    case SENSOR_SLEEPING:
        return "SLEEPING";
    case SENSOR_OFF:
        return "OFF";
    case SENSOR_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "?";
    }
}

DataLine imu_configuration_toDataLine(umsg_Sensors_imu_configuration_t &data)
{
    DataLine dataline;

    dataline.name = "IMU_CONF";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.sample_rate_hz));

    std::string str;
    switch (data.accelerometer_range)
    {
    case G_16:
        str = "16";
        break;
    case G_8:
        str = "8";
        break;
    case G_4:
        str = "4";
        break;
    case G_2:
        str = "2";
        break;
    default:
        str = "?";
        break;
    }

    dataline.data.push_back(str);

    switch (data.gyroscope_scale)
    {
    case DPS_2000:
        str = "2000";
        break;
    case DPS_1000:
        str = "1000";
        break;
    case DPS_500:
        str = "500";
        break;
    case DPS_250:
        str = "250";
        break;
    default:
        str = "?";
        break;
    }

    dataline.data.push_back(str);
    return dataline;
}
DataLine imu_data_toDataLine(umsg_Sensors_imu_data_t &data)
{
    DataLine dataline;
    dataline.name = "IMU";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.accelerometer[0]));
    dataline.data.push_back(std::to_string(data.accelerometer[1]));
    dataline.data.push_back(std::to_string(data.accelerometer[2]));
    dataline.data.push_back(std::to_string(data.gyroscope[0]));
    dataline.data.push_back(std::to_string(data.gyroscope[1]));
    dataline.data.push_back(std::to_string(data.gyroscope[2]));
    dataline.data.push_back(std::to_string(data.magnetometer[0]));
    dataline.data.push_back(std::to_string(data.magnetometer[1]));
    dataline.data.push_back(std::to_string(data.magnetometer[2]));
    dataline.data.push_back(std::to_string(data.attitude[0]));
    dataline.data.push_back(std::to_string(data.attitude[1]));
    dataline.data.push_back(std::to_string(data.attitude[2]));
    dataline.data.push_back(std::to_string(data.linear_accel[0]));
    dataline.data.push_back(std::to_string(data.linear_accel[1]));
    dataline.data.push_back(std::to_string(data.linear_accel[2]));
    dataline.data.push_back(std::to_string(data.gravity[0]));
    dataline.data.push_back(std::to_string(data.gravity[1]));
    dataline.data.push_back(std::to_string(data.gravity[2]));
    dataline.data.push_back(std::to_string(data.temperature_c));

    return dataline;
}
DataLine imu_state_toDataLine(umsg_Sensors_imu_state_t &data)
{
    DataLine dataline;
    dataline.name = "IMU_STATE";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(sensor_state_toString(data.state));
    dataline.data.push_back(std::to_string(data.initializing) + std::to_string(data.logging_data) + std::to_string(data.reconnecting) + std::to_string(data.not_present));

    return dataline;
}

DataLine baro_configuration_toDataLine(umsg_Sensors_baro_configuration_t &data)
{
    DataLine dataline;
    dataline.name = "BARO_CONF";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.start_pressure_hpa));
    dataline.data.push_back(std::to_string(data.start_alt_m));

    return dataline;
}
DataLine baro_data_toDataLine(umsg_Sensors_baro_data_t &data)
{
    DataLine dataline;
    dataline.name = "BARO";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.pressure_pa));
    dataline.data.push_back(std::to_string(data.relative_humidity));
    dataline.data.push_back(std::to_string(data.temperature_c));
    dataline.data.push_back(std::to_string(data.gained_alt_m));
    dataline.data.push_back(std::to_string(data.alt_above_sea_m));

    return dataline;
}
DataLine baro_state_toDataLine(umsg_Sensors_baro_state_t &data)
{
    DataLine dataline;
    dataline.name = "BARO_STATE";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(sensor_state_toString(data.state));
    dataline.data.push_back(std::to_string(data.initializing) + std::to_string(data.logging_data) + std::to_string(data.reconnecting) + std::to_string(data.not_present));

    return dataline;
}

DataLine therm_configuration_toDataLine(umsg_Sensors_thermistor_configuration_t &data, std::string name)
{
    DataLine dataline;
    dataline.name = "THERM_CONF_" + name;
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.R1_value));
    dataline.data.push_back(std::to_string(data.voltage_source_value));
    dataline.data.push_back(std::to_string(data.TO_value));
    dataline.data.push_back(std::to_string(data.RO_value));
    dataline.data.push_back(std::to_string(data.B_value));

    return dataline;
}
DataLine therm_data_toDataLine(umsg_Sensors_thermistor_data_t &data, std::string name)
{
    DataLine dataline;
    dataline.name = "THERM_" + name;
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.temperature_c));

    return dataline;
}
DataLine therm_state_toDataLine(umsg_Sensors_thermistor_state_t &data, std::string name)
{
    DataLine dataline;
    dataline.name = "THERM_STATE_" + name;
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.logging_data) + std::to_string(data.not_present));

    return dataline;
}