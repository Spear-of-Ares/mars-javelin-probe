/*********************************************************************************
*     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:33]
*     Last Modified       :     [2022-10-26 02:09]
*     Description         :     Component to handle the task for the IridiumSBD 
**********************************************************************************/

#ifndef __IRIDIUMSBD_COMPONENT_H__
#define __IRIDIUMSBD_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include <string>
#include <time.h>
#include <HardwareSerial.h>
#include "IridiumSBD.h"
#include "datalogger.h"

#define IRID_TASK_ID "IRID_TASK "

#ifdef false
#define IRIDIUM_ATTACHED
#endif

extern HardwareSerial SerialPort;
extern IridiumSBD Iridium;

class IridiumComponent{
public:
  IridiumComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutIridium, TaskHandle_t cmd_center)
  {
    _dataOutSD = dataOutSD;
    _dataOutIridium = dataOutIridium;
    _cmd_center = cmd_center;
#ifdef IRIDIUM_ATTACHED
    SerialPort.begin(19200, SERIAL_8N1, GPIO_NUM_1, GPIO_NUM_3);
    Iridium.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);
    if (Iridium.begin() != ISBD_SUCCESS)
    {
      printf("Failed to init iridium\n");
    }
#endif
  }
  static void vMainLoop_Task(void *arg);

private:
  void vRX();
  void checkQueue();
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutIridium;
  TaskHandle_t _cmd_center;
};

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
