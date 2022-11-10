/*********************************************************************************
*     File Name           :     /components/IridiumSBD/IridiumSBDComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:37]
*     Last Modified       :     [AUTO_UPDATE_BEFORE_SAVE]
*     Description         :     Component for handling IridiumSBD things
**********************************************************************************/

#include "IridiumSBDComponent.h"

void IridiumSBDComponent::vMainLoop_Task(void *arg){
    IridiumSBDComponent irid_comp = *((IridiumSBD*)arg);
    for(;;){
        irid_comp.checkQueue();
        vTaskDelay(10);
    }
}

void IridiumSBDComponent::checkQueue(){
  while (uxQueueMessagesWaiting(_dataOutIridium) != 0)
  {
    std::string *msg;
    if (xQueueReceive(_dataOutIridium, &(msg), 5 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Iridium could not receive from queue\n");
      return;
    }
    //vTX(&msg);
    delete msg;
  }
}

