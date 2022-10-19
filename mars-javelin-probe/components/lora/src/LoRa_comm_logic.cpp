/*********************************************************************************
*     File Name           :     mars-javelin-probe/components/lora/lora_commlogic.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-10-19 01:39]
*     Description         :     Communication logic for LoRa modules 
**********************************************************************************/
#include "LoRa_comm_logic.h"

void vTX_Task(void *p)
{
  SPIClass SPI_1(3);
  SPI_1.begin(SPI_1_SCLK, SPI_1_MISO, SPI_1_MOSI, SPI_1_CS);
  LoRa.setPins(SPI_1_CS, SPI_1_RST, SPI_1_DI0);
  LoRa.setSPI(SPI_1);
  LoRa.setSPIFrequency(1E6);
  if( !LoRa.begin(915E6))
  {
    printf("Starting LoRa failed!\n");
    vTaskDelete(NULL);
  }

  int counter = 0;
  for(;;){
    vTaskDelay(5000/portTICK_PERIOD_MS);
    LoRa.beginPacket();
    LoRa.print("Hello ");
    LoRa.print(counter);
    LoRa.endPacket();
    counter++;
    printf("Packet sent...\n");
  }
}

void vRX_Task(void *p)
{
  SPIClass *hspi = new SPIClass(HSPI);
  hspi->begin(SPI_1_SCLK, SPI_1_MISO, SPI_1_MOSI, SPI_1_CS);
  LoRa.setPins(SPI_1_CS, SPI_1_RST, SPI_1_DI0);
  LoRa.setSPI(*hspi);
  if (!LoRa.begin(915E6))
  {
    printf("Starting LoRa failed!\n");
    vTaskDelete(NULL);
  }

  for(;;){
    // Try and parse the packet
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
      printf("Received packet '");
      
      while(LoRa.available())
        {
          printf("%c", (char)LoRa.read());
        }
      printf("' with RSSI %d\n", LoRa.packetRssi());
    }
    vTaskDelay(1);
  }
}
