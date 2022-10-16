#include "datalogger.h"

void vBeginLogLoop_Task(void *pvParameters)
{
  const TickType_t xDelay = 200 / portTICK_PERIOD_MS;
  uint32_t notificationValue;
  for(;;)
    {
      notificationValue = ulTaskNotifyTake(pdTRUE, (TickType_t) portMAX_DELAY);
      if(notificationValue > 0){
        printf("Data logging: %d\n", notificationValue);
      }
    }

  vTaskDelete(NULL);
}
