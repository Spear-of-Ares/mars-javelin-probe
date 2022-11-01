/*********************************************************************************
*     File Name           :     /components/GPS/include/GPSComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:38]
*     Last Modified       :     [2022-10-21 00:45]
*     Description         :     Component for handling both GPS modules and their data
**********************************************************************************/

#ifndef __GPS_COMPONENT_H__
#define __GPS_COMPONENT_H__

#include <stdio>
#include "TinyGPS.h"

class GPSComponent{
public:
  GPSComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutIridium);
  static void vMainLoop_Task();
private:
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  TinyGPS   _tiny_gps;
};

#endif /* __GPS_COMPONENT_H__ */

