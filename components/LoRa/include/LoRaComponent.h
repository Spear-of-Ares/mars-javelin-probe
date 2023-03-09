/*********************************************************************************
 *     File Name           :     /components/lora/include/lora_commlogic.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-18 20:25]
 *     Last Modified       :     [2022-11-06 02:37]
 *     Description         :      Communication logic for LoRa modules
 **********************************************************************************/

#ifndef __LORA_COMPONENT_H__
#define __LORA_COMPONENT_H__
#include <sstream>
#include <vector>
#include <string>

#include "LoRa.h"
#include "datalogger.h"
#include "dataline.h"
#include "umsg_toDataline.h"
#include "SPI.h"

extern "C"
{
#include "umsg_LoRa.h"
#include "umsg_Sensors.h"
#include "umsg_GPS.h"
#include "umsg_CommandCenter.h"
}

#define LoRa_TASK_ID "LoRa_TASK "

namespace LoRaData{
  typedef struct{
    uint64_t latitude;     /*! < Latitude. 10^-7*/
    uint64_t longitude;    /*! < Longitude. 10^-7*/
    uint32_t altitude_gps; /*! < Altitude in mm */
    uint16_t pdop;         /*! < Accuracy of position. PDOP 10^-2*/
    int32_t year;          /*! < Year from GPS*/
    int32_t month;         /*! < Month from GPS */
    int32_t day;           /*! < Day from GPS*/
    int32_t hour;          /*! < Hour in UTC from GPS*/
    int32_t minute;        /*! < Minute in UTC from GPS*/
    int32_t second;        /*! < Seconds in UTC from GPS*/
    float pressure_hpa;    /*! < Pressure in hpa / millibar*/
    float altitude_baro;   /*! < Altitude calculated by the barometer in meters*/
    float baro_temp;       /*! < Temperature from the barometer*/
    double attitude_x;     /*! < Attitude on x axis from imu in degrees*/
    double attitude_y;     /*! < Attitude on y axis from imu in degrees*/
    double attitude_z;     /*! < Attitude on z axis from imu in degrees*/
    double linear_accel_x; /*! < Linear acceleration on x axis from imu in m/s^2*/
    double linear_accel_y; /*! < Linear acceleration on y axis from imu in m/s^2*/
    double linear_accel_z; /*! < Linear acceleration on z axis from imu in m/s^2*/
    double velocity_x;     /*! < Velocity calculated from imu in m/s on z axis*/
    double velocity_y;     /*! < Velocity calculated from imu in m/s on y axis*/
    double velocity_z;     /*! < Velocity calculated from imu in m/s on z axis*/
    double position_x;     /*! < Position calculated from imu in m on x axis*/
    double position_y;     /*! < Position calculated from imu in m on y axis*/
    double position_z;     /*! < Position calculated from imu in m on z axis*/
    double vertical_speed; /*! < Vertical speed in m/s calculated from velocity*/
    double horizontal_speed; /*! < Horizonal speed in m/s calculated from velocity*/
    double angle_of_attack; /*! < Angle of attack calcualted from velocity in rad*/
    uint32_t imu_temp;       /*! < Temperature of the imu*/
    float internal_temp;   /*! Internal thermistor temp*/
    float external_temp;   /*! External thermistor temp*/
  }LoRaRemoteData;

  template<class T>
  uint8_t* toByteArray(T data_struct, size_t &size);
}

class LoRaComponent
{
public:
  static void vMainLoop_Task(void *arg);

private:
  std::vector<std::pair<DataLine, umsg_LoRa_msg_type_t>> _datalines;
  void initSubs();
  void readSubs();
  static void vRX();
  static void vTX(const uint8_t *msg, size_t size, umsg_LoRa_msg_type_t msg_type);
  static void vTX(std::string msg, umsg_LoRa_msg_type_t msg_type);
  bool setup();

  umsg_sub_handle_t _gps_data_sub;
  umsg_GPS_data_t _gps_data;

  umsg_sub_handle_t _imu_data_sub;
  umsg_Sensors_imu_data_t _imu_data;

  umsg_sub_handle_t _baro_data_sub;
  umsg_Sensors_baro_data_t _baro_data;

  umsg_sub_handle_t _therm_0_data_sub;
  umsg_sub_handle_t _therm_1_data_sub;
  umsg_Sensors_thermistor_data_t _therm_0_data;
  umsg_Sensors_thermistor_data_t _therm_1_data;
};

#endif /* __LORA_COMPONENT_H__ */
