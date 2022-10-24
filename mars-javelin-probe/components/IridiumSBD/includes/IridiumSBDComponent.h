/*********************************************************************************
*     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:33]
*     Last Modified       :     [2022-10-21 00:45]
*     Description         :     Component to handle the task for the IridiumSBD 
**********************************************************************************/

#ifndef __IRIDIUMSBD_COMPONENT_H__
#define __IRIDIUMSBD_COMPONENT_H__

#include <stdio>
#include "IridiumSBD.h"

class IridiumSBDComponent{
public:
  IridiumSBDComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutIridium);
  static void vMainLoop_Task();
private:
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  IridiumSBD _device;
}

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
