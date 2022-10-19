/*********************************************************************************
*     File Name           :     /home/jon/esp/projects/mars-javelin-probe/mars-javelin-probe/components/lora/include/lora_commlogic.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-10-18 22:00]
*     Description         :      Communication logic for LoRa modules
**********************************************************************************/

#ifndef __LORA_COMM_LOGIC_H__
#define __LORA_COMM_LOGIC_H__

#include "LoRa.h"
#include "SPI.h"

#define SPI_1_MISO 19
#define SPI_1_MOSI 27
#define SPI_1_SCLK 5
#define SPI_1_DI0  26
#define SPI_1_RST  23
#define SPI_1_CS   18


void vTX_Task(void *p);
void vRX_Task(void *p);
#endif /* __LORA_COMMLOGIC_H__ */
