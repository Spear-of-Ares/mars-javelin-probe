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
  i2cuart.write(SC16IS752_CHANNEL_A, 0x55);
  vTaskDelay(10 / portTICK_PERIOD_MS);

  if (i2cuart.available(SC16IS752_CHANNEL_A) == 0)
  {
    printf("Not connected correctly. No value\n");
    return;
  }
  if (i2cuart.read(SC16IS752_CHANNEL_A) != 0x55)
  {
    printf("Not connected correctly. Wrong value\n");
    return;
  }
  printf("Working!!\n");
}
