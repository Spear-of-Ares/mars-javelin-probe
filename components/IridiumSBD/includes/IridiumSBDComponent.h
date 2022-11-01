/*********************************************************************************
*     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:33]
*     Last Modified       :     [2022-10-26 02:09]
*     Description         :     Component to handle the task for the IridiumSBD 
**********************************************************************************/

#ifndef __IRIDIUMSBD_COMPONENT_H__
#define __IRIDIUMSBD_COMPONENT_H__

#include <stdio>
#include "IridiumSBD.h"

class IridiumSBDComponent{
public:
  IridiumSBDComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutIridium, TaskHandle_t cmd_center);
  static void vMainLoop_Task();
private:
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  IridiumSBD _device;
  TaskHandle_t _cmd_center;
}

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
