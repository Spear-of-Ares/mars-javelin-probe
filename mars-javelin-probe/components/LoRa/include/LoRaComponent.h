/*********************************************************************************
*     File Name           :     /components/lora/include/lora_commlogic.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-10-21 00:51]
*     Description         :      Communication logic for LoRa modules
**********************************************************************************/

#ifndef __LORA_COMPONENT_H__
#define __LORA_COMPONENT_H__

#include "LoRa.h"
#include "SPI.h"

#define SPI1_MISO CONFIG_MISO_GPIO
#define SPI1_MOSI CONFIG_MOSI_GPIO
#define SPI1_SCK  CONFIG_SCK_GPIO
#define SPI1_DI0  CONFIG_DI0_GPIO
#define SPI1_RST  CONFIG_RST_GPIO
#define SPI1_CS   CONFIG_CS_GPIO

class LoRaComponent{
public:
  LoRaComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa);
  static void vMainLoop_Task();
private:
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  LoRaClass _device = LoRa;
};

void vTX_Task(void *p);
void vRX_Task(void *p);
#endif /* __LORA_COMPONENT_H__ */
