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

// #define IRIDIUM_ATTACHED

extern HardwareSerial SerialPort;
extern IridiumSBD Iridium;

class IridiumComponent
{
public:
  IridiumComponent()
  {
#ifdef IRIDIUM_ATTACHED

    SerialPort.begin(19200, SERIAL_8N1, TX, RX);
    // Iridium.setPowerProfile(IridiumSBD::USB_POWER_PROFILE);
    if (Iridium.begin() != ISBD_SUCCESS)
    {
      printf("Failed to init iridium\n");
    }
    printf("Iridium init\n");
#endif
  }
  static void vMainLoop_Task(void *arg);

private:
  void vRX();
  void checkQueue();
};

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
