/*********************************************************************************
*     File Name           :     /components/lora/include/lora_commlogic.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-10-23 22:21]
*     Description         :      Communication logic for LoRa modules
**********************************************************************************/

#ifndef __LORA_COMPONENT_H__
#define __LORA_COMPONENT_H__

#include "LoRa.h"
#include "SPI.h"

class LoRaComponent{
public:
  LoRaComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa);
  static void vMainLoop_Task(void *arg);
private:
  void vRX();
  void vTX();
  bool setup();
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  LoRaClass _device = LoRa;
};

#endif /* __LORA_COMPONENT_H__ */
