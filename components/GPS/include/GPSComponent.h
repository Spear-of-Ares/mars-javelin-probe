/*********************************************************************************
*     File Name           :     /components/GPS/include/GPSComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:38]
*     Last Modified       :     [2022-11-06 02:11]
*     Description         :     Component for handling both GPS modules and their data
**********************************************************************************/

#ifndef __GPS_COMPONENT_H__
#define __GPS_COMPONENT_H__

#include <cstdio>
#include <string>
#include <sstream>
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "TinyGPS.h"
#include "Wire.h"
#include "driver/gpio.h"

#include "datalogger.h"

#define ADDRESS 0x42
#define GPS_COMP_ID "GPS_COMP  "

#ifdef false
#define DUAL_GPS
#endif

class GPSComponent{
public:
  GPSComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium, TaskHandle_t xCmdCenter);
  static void vMainLoop_Task(void *arg);
private:
  void setup();
  void get_data();
  std::string getGPS_MSG(int gps);
  void sendData(std::string msg);
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  QueueHandle_t _dataOutLoRa;
  SFE_UBLOX_GNSS _GNSS_1;
  SFE_UBLOX_GNSS _GNSS_2;
  TickType_t _lastUpdateIridium;
  TickType_t _lastUpdateLoRa;
  TaskHandle_t _cmd_center;
};

#endif /* __GPS_COMPONENT_H__ */

