/*********************************************************************************
*     File Name           :     /components/CommandCenter/include/CommandCenter.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-26 00:29]
*     Last Modified       :     [2022-10-26 01:56]
*     Description         :     Component to control messages from LoRa and Iridium SBD 
**********************************************************************************/

#ifndef __COMMAND_CENTER_H__
#define __COMMAND_CENTER_H__
#include <stdio.h>
#include <sstream>
#include "CommandDefs.h"
#include "datalogger.h"

#include "driver/gpio.h"

#define CUT_DWN_GPIO    GPIO_NUM_25
#define CUT_DWN_DUR     CONFIG_CUT_DWN_DUR

class CommandComponent{
public:
  CommandComponent(QueueHandle_t dataOutSD){
    _dataOutSD = dataOutSD;
    gpio_set_direction(CUT_DWN_GPIO, GPIO_MODE_OUTPUT);
  }
  static void vMainLoop_Task(void* arg);

private:
  void handleCommands();
  void cutdown();
  void selftest();

  QueueHandle_t _dataOutSD;
  uint32_t _ulNotifiedValue;
};

#endif