/*********************************************************************************
 *     File Name           :     /components/IridiumSBD/IridiumSBDComponent.cpp
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-21 00:37]
 *     Last Modified       :     [AUTO_UPDATE_BEFORE_SAVE]
 *     Description         :     Component for handling IridiumSBD things
 **********************************************************************************/

#include "RFD.h"

void RFDComponent::vMainLoop_Task(void *arg)
{
  RFDComponent rfd_comp = *((RFDComponent *)arg);
  for (;;)
  {
    rfd_comp.vRX();
    rfd_comp.checkQueue();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void RFDComponent::vRX()
{
}

void RFDComponent::checkQueue()
{
  uint8_t vals[5] = {0x48, 0x45, 0x4c, 0x4c, 0x4f};
  i2cuart.writeBytes(SC16IS752_CHANNEL_A, vals, 5);
  vTaskDelay(100 / portTICK_PERIOD_MS);
}
