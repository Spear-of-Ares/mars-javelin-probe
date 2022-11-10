/*********************************************************************************
*     File Name           :     /components/IridiumSBD/IridiumSBDComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:37]
*     Last Modified       :     [AUTO_UPDATE_BEFORE_SAVE]
*     Description         :     Component for handling IridiumSBD things
**********************************************************************************/

#include "IridiumComponent.h"

HardwareSerial SerialPort(2);
IridiumSBD Iridium(SerialPort);

void IridiumComponent::vMainLoop_Task(void *arg){
    IridiumComponent irid_comp = *((IridiumComponent*)arg);
    for(;;){
        irid_comp.vRX();
        irid_comp.checkQueue();
        vTaskDelay(500);
    }
}

void IridiumComponent::vRX(){
  if (Iridium.getWaitingMessageCount() == 0){
    printf("No iridium msgs\n");
    return;
  }

  std::ostringstream sd_msg;
  uint8_t rx_buf[512];
  size_t rx_size = sizeof(rx_buf);
  struct tm t; 

  sd_msg << xTaskGetTickCount() << " || " << IRID_TASK_ID << " ||";

  int err = Iridium.sendReceiveSBDText(NULL, rx_buf, rx_size);
  
  if (err != ISBD_SUCCESS){
    printf("iridium.sendReceive failed: %d\n", err);
    sd_msg << " Failed to receive";
  }
  else{
    err = Iridium.getSystemTime(t);
    std::string received;
    printf("Inbound buffer size is %d\n", rx_size);
    for(int i = 0; i < rx_size; i++){
      received += (char)rx_buf[i];
      printf("%02x", rx_buf[i]);
      printf(" ");
    }
    printf("\n");
    printf("Messages remaining %d\n", Iridium.getWaitingMessageCount());

    if (err == ISBD_SUCCESS){
      sd_msg << " " << t.tm_year << "-" << t.tm_mon << "-" << t.tm_mday << " ";
      sd_msg << t.tm_hour << ":" << t.tm_min << ":" << t.tm_sec << " ||";
    }
    else{
      sd_msg << " Time not available ||";
    }
    sd_msg << " Received: " << received;

    if(rx_buf[0] == 0x01){
      xTaskNotify(_cmd_center, rx_buf[1], eSetBits);
    }


    Iridium.sendSBDBinary(rx_buf, rx_size);
  }

  SDData *sdOut = new SDData();
  sdOut->file_name = new std::string("comms");
  sdOut->message = new std::string(sd_msg.str());
  if (xQueueSend(_dataOutSD, &(sdOut), 10 / portTICK_PERIOD_MS) != pdTRUE)
  {
    printf("Failed to post stats data\n");
  }
}

void IridiumComponent::checkQueue(){
  while (uxQueueMessagesWaiting(_dataOutIridium) != 0)
  {
    std::string *msg;
    if (xQueueReceive(_dataOutIridium, &(msg), 5 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Iridium could not receive from queue\n");
      return;
    }
    Iridium.sendSBDText(msg->c_str());
    delete msg;
  }
}

