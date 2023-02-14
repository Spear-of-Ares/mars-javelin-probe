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
#include "ukf_imu_engl.h"

#define IMU_TASK_ID "IMU TASK  "
#define IMU_ATTACHED

#ifdef false
#define IMU_LOG_IRIDIUM
#endif

#define IMU_LOG_LoRa

#define DEG2RAD(x) ((x) * (3.14159f/180))
#define RAD2DEG(x) ((x) * (180/3.14159f))

#define SAMPLE_RATE_HZ 10

class IMUComponent{
public:
  IMUComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium);
  static void vMainLoop_Task(void *arg);
private:
  void setup();
  void logIMU();


  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  QueueHandle_t _dataOutIridium;
  MPU6050 _device;

  TickType_t _lastUpdateLoRa;
  TickType_t _lastUpdateIridium;

  float fusion_boundary;
};

#endif
