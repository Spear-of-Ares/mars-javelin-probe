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

class LoRaComponent
{
public:
  static void vMainLoop_Task(void *arg);

private:
  std::vector<std::pair<DataLine, umsg_LoRa_msg_type_t>> _datalines;
  void initSubs();
  void readSubs();
  static void vRX();
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
