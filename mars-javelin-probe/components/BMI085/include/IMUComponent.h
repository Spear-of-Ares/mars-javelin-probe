/*********************************************************************************
*     File Name           :     /components/BMI085/include/IMUComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:42]
*     Last Modified       :     [2022-10-21 00:45]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#ifndef __IMU_COMPONENT_H__
#define __IMU_COMPONENT_H__

#include <stdio>
#include "BMI085.h"

class IMUComponent{
public:
  IMUComponent(QueueHandle_t dataOutSD);
  static void vMainLoop_Task();
private:
  QueueHandle_t _dataOutSD;
  BMI085 _device;
};

#endif
