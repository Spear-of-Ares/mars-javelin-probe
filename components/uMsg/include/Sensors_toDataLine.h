#ifndef __SENSORS_TODATALINE_H__
#define __SENSORS_TODATALINE_H__

#include <string>
#include "dataline.h"

extern "C"
{
#include "umsg_Sensors.h"
}

DataLine imu_configuration_toDataLine(umsg_Sensors_imu_configuration_t &data);
DataLine imu_data_toDataLine(umsg_Sensors_imu_data_t &data);
DataLine imu_state_toDataLine(umsg_Sensors_imu_state_t &data);

DataLine baro_configuration_toDataLine(umsg_Sensors_baro_configuration_t &data);
DataLine baro_data_toDataLine(umsg_Sensors_baro_data_t &data);
DataLine baro_state_toDataLine(umsg_Sensors_baro_state_t &data);

DataLine therm_configuration_toDataLine(umsg_Sensors_thermistor_configuration_t &data, std::string name);
DataLine therm_data_toDataLine(umsg_Sensors_thermistor_data_t &data, std::string name);
DataLine therm_state_toDataLine(umsg_Sensors_thermistor_state_t &data, std::string name);

#endif