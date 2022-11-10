/*********************************************************************************
*     File Name           :     /components/CommandCenter/include/CommandCenter.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-26 00:29]
*     Last Modified       :     [2022-10-28 01:34]
*     Description         :     Component to control messages from LoRa and Iridium SBD 
**********************************************************************************/

#ifndef __COMMAND_CENTER_H__
#define __COMMAND_CENTER_H__
#include <stdio.h>
#include <sstream>
#include "CommandDefs.h"
#include "datalogger.h"

#include "driver/gpio.h"

#define CUT_DWN_GPIO    GPIO_NUM_4
#define CUT_DWN_DUR     CONFIG_CUT_DWN_DUR

#define CMDCNTR_TASK_ID "CMD CENTER"

class CommandComponent{
public:
  CommandComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium){
    _dataOutSD = dataOutSD;
    _dataOutLoRa = dataOutLoRa;
    _dataOutIridium = dataOutIridium;
    gpio_set_direction(CUT_DWN_GPIO, GPIO_MODE_OUTPUT);
  }
  static void vMainLoop_Task(void* arg);

private:
  void handleCommands();
  void cutdown();
  void selftest();

  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  QueueHandle_t _dataOutIridium;
  uint32_t _ulNotifiedValue;
};

#endif
