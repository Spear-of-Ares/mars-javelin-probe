/*********************************************************************************
*     File Name           :     /components/lora/include/lora_commlogic.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-11-06 02:37]
*     Description         :      Communication logic for LoRa modules
**********************************************************************************/

#ifndef __LORA_COMPONENT_H__
#define __LORA_COMPONENT_H__
#include <sstream>

#include "LoRa.h"
#include "datalogger.h"
#include "SPI.h"

#define LoRa_TASK_ID    "LoRa_TASK "

class LoRaComponent{
public:
  LoRaComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, TaskHandle_t cmd_center){
    _dataOutSD = dataOutSD;
    _dataOutLoRa = dataOutLoRa;
    _cmd_center = cmd_center;
  }
  static void vMainLoop_Task(void *arg);
private:
  void vRX();
  void vTX(std::string msg);
  bool setup();
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  TaskHandle_t _cmd_center;
};

#endif /* __LORA_COMPONENT_H__ */
