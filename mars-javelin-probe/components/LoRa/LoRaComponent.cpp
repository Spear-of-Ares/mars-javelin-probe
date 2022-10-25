/*********************************************************************************
*     File Name           :     /components/lora/lora_commlogic.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-10-25 00:38]
*     Description         :     Communication logic for LoRa modules 
**********************************************************************************/
#include "LoRaComponent.h"

void LoRaComponent::vMainLoop_Task(void *arg){
  LoRaComponent lora_component = *((LoRaComponent*)arg);
  if (!lora_component.setup()){
    vTaskDelete(NULL);
  }

  for(;;){
    lora_component.vRX();  
    vTaskDelay(10);
  }
}

// Assumes that initLoRaSPI() has been called from the ComBus component
bool LoRaComponent::setup(){
  LoRa.setSPIFrequency(1E6);

  if (!LoRa.begin(915E6))
  {
    printf("Starting LoRa failed!\n");
    return false;
  }
  return true;
}

void LoRaComponent::vTX()
{
  vTaskDelay(5000/portTICK_PERIOD_MS);
  LoRa.beginPacket();
  LoRa.print("Hello ");
  LoRa.endPacket();
  printf("Packet sent...\n");
}

void LoRaComponent::vRX()
{
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
}
