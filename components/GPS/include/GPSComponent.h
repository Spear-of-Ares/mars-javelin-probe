/*********************************************************************************
*     File Name           :     /components/GPS/include/GPSComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:38]
*     Last Modified       :     [2022-10-21 00:45]
*     Description         :     Component for handling both GPS modules and their data
**********************************************************************************/

#ifndef __GPS_COMPONENT_H__
#define __GPS_COMPONENT_H__

#include <cstdio>
#include <string>
#include <sstream>
#include "TinyGPS.h"
#include "Wire.h"

#include "datalogger.h"

class GPSComponent{
public:
  GPSComponent(QueueHandle_t dataOutSD){_dataOutSD = dataOutSD; }
  //GPSComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutIridium);
  static void vMainLoop_Task(void *arg);
private:
  void get_data();
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  TinyGPS   _tiny_gps;
};

#endif /* __GPS_COMPONENT_H__ */

