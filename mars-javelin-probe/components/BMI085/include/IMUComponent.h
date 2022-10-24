/*********************************************************************************
*     File Name           :     /components/BMI085/include/IMUComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:42]
*     Last Modified       :     [2022-10-23 19:18]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#ifndef __IMU_COMPONENT_H__
#define __IMU_COMPONENT_H__

#include <stdio>
#include "datalogger.h"
#include "BMI085.h"

#define I2C_MASTER CONFIG_I2C_MASTER_PORT

class IMUComponent{
public:
  IMUComponent(QueueHandle_t dataOutSD);
  static void vMainLoop_Task(void *arg);
private:
  QueueHandle_t _dataOutSD;
  BMI085 _device;
};

#endif
