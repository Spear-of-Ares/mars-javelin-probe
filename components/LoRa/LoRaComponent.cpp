/*********************************************************************************
*     File Name           :     /components/lora/lora_commlogic.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-18 20:25]
*     Last Modified       :     [2022-11-06 02:38]
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
    lora_component.checkQueue();
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

void LoRaComponent::checkQueue(){
  while (uxQueueMessagesWaiting(_dataOutLoRa) != 0)
  {
    std::string *msg;
    if (xQueueReceive(_dataOutLoRa, &(msg), 5 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("LoRa could not receive from queue\n");
      return;
    }
    vTX(&msg);
    delete msg;
  }
}

void LoRaComponent::vTX(std::string msg)
{
  log_i("Sending LoRa packet\n");
  LoRa.beginPacket();
  LoRa.print(msg.c_str());
  LoRa.endPacket();
  log_i("Packet sent\n");
}

void LoRaComponent::vRX()
{
  int packetSize = LoRa.parsePacket();
  std::string empty="";
  if (packetSize)
  {
    std::string received("");
    SDData *sdOut = new SDData();
    sdOut->file_name = new std::string("LoRa Component");

    
    while(LoRa.available())
      {
        received += (char)LoRa.read();
      }

    std::ostringstream response;
    response << "[" << xTaskGetTickCount() << "] Message received";
    vTX(response.str());

    if(received[0] == 0x01){
      xTaskNotify(_cmd_center, received[1], eSetBits);
    }

    std::ostringstream sd_msg;
    sd_msg << xTaskGetTickCount() << " || " LoRa_TASK_ID << " || Received: "<< received << "| RSSI: " << LoRa.packetRssi();
    sd_msg << " | SNR: " << LoRa.packetSnr() << " | Freq Err: " << LoRa.packetFrequencyError() << "\n";
    sdOut->message =  new std::string(sd_msg.str());
    if(xQueueSend(_dataOutSD, &(sdOut), 10/portTICK_PERIOD_MS) != pdTRUE){
      printf("Failed to post stats data\n");
    }
  }
}
