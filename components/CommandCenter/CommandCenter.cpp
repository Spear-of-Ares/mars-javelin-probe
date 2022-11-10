/*********************************************************************************
*     File Name           :     /components/CommandCenter/CommandCenter.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-26 00:34]
*     Last Modified       :     [2022-10-28 00:50]
*     Description         :     Component to control messages from LoRa and Iridium SBD 
**********************************************************************************/

#include "CommandCenter.h"

void CommandComponent::vMainLoop_Task(void* arg){
  CommandComponent cmd_component = *((CommandComponent*)arg);
  const TickType_t xMaxBlockTime = 500 / portTICK_PERIOD_MS;
  BaseType_t xResult;

  for(;;){
    xResult = xTaskNotifyWait(   
                              pdFALSE,      // Do not clear bits on entry.
                              ULONG_MAX,    // Clear all bits on exit
                              &(cmd_component._ulNotifiedValue), // Stores the notified value.
                              xMaxBlockTime
                              );
    if (xResult == pdPASS){
      cmd_component.handleCommands();
    }
    else{
      //printf("Time out on Command component\n");
    }
  }
}

void CommandComponent::handleCommands(){  
  if( (_ulNotifiedValue & CMD_CUT_DWN) != 0){
    cutdown();
  }
  if( (_ulNotifiedValue & CMD_SELF_TEST) != 0){
    selftest();
  }
}

void CommandComponent::cutdown(){
  SDData *sd_data = new SDData();
  sd_data->file_name = new std::string("status");

  std::ostringstream sd_msg;
  sd_msg << xTaskGetTickCount() << " || " << CMDCNTR_TASK_ID << " || Cutdown Command Received\n";
  sd_data->message = new std::string(sd_msg.str());

  gpio_set_level(CUT_DWN_GPIO, 1);
  vTaskDelay(CUT_DWN_DUR/portTICK_PERIOD_MS);
  gpio_set_level(CUT_DWN_GPIO, 0);


  if(xQueueSend(_dataOutSD, &(sd_data), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post thermistor data\n");
  }

  std::string *iriddata = new std::string("Cutting down");
  if (xQueueSend(_dataOutIridium, &(iriddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
  {
    printf("Failed to post GPS data to Iridium\n");
  }

  std::string *loradata = new std::string("Cutting down");
  if (xQueueSend(_dataOutLoRa, &(loradata), 10 / portTICK_PERIOD_MS) != pdTRUE)
  {
    printf("Failed to post GPS data to LoRa\n");
  }
}

void CommandComponent::selftest(){
  SDData *sd_data = new SDData();
  sd_data->file_name = new std::string("status");

  std::ostringstream sd_msg;
  sd_msg << xTaskGetTickCount() << " || " << CMDCNTR_TASK_ID << " ||  SelfTest Command Received\n";
  sd_data->message = new std::string(sd_msg.str());

  if(xQueueSend(_dataOutSD, &(sd_data), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post thermistor data\n");
  }
}

