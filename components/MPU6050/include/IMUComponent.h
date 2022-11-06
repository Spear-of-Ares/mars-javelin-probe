/*********************************************************************************
*     File Name           :     /components/BMI085/include/IMUComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:42]
*     Last Modified       :     [2022-11-02 00:57]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#ifndef __IMU_COMPONENT_H__
#define __IMU_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include "datalogger.h"
#include "MPU6050.h"
#include "ComBus.h"

#define IMU_TASK_ID "IMU TASK  "
//#define IMU_ATTACHED

#define SAMPLE_RATE_HZ 10

class IMUComponent{
public:
  IMUComponent(QueueHandle_t dataOutSD);
  static void vMainLoop_Task(void *arg);
private:
  void setup();
  void logIMU();
  QueueHandle_t _dataOutSD;
  MPU6050 _device;

  float _pitch, _roll, _yaw;
};

#endif