/*********************************************************************************
*     File Name           :     mars-javelin-probe/main/mars-javelin-probe.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-03 22:40]
*     Last Modified       :     [2022-10-19 01:35]
*     Description         :     Coordinates and controls generation of new tasks 
*                               Using the ESP Arduino library for access to a wider number of
*                               libraries for components, such as the IridiumSBD library.
*
*                               Using Arduino as a component from the following article:
*                               https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/esp-idf_component.html
**********************************************************************************/



#include <cstdio>
#include "esp_log.h"
#include "esp_timer.h"
#include "Arduino.h"
#include "IridiumSBD.h"
#include "CopernicusII.h"
#include "datalogger.h"
#include "LoRa_comm_logic.h"

#define MICRO2MILLI(x)  ((x) / 1000UL)

const char* TAG = "MARS JAVELIN";



extern "C" void app_main(void)
{
    initArduino();
    BaseType_t xReturned;

    TaskHandle_t xLoraHandle = NULL;
    printf("Starting Lora RX Task\n");
    xReturned = xTaskCreate(
      vRX_Task,
      "LORA RX",
      1024*2,
      NULL,
      10,
      &xLoraHandle
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Lora RX task\n");
    }

    TaskHandle_t xDataLogHandle = NULL;
    printf("Starting Data Log Task\n");
    xReturned = xTaskCreate(
      vBeginLogLoop_Task,
      "Data Log",
      1024*4,
      NULL,
      11,
      &xDataLogHandle
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Data Log task\n");
    }
    // TaskHandle_t xGPSHandle = NULL;
    //
    // printf("Starting GPS loop\n");
    // xReturned = xTaskCreate(
    //                     vPrintReceived_Task,
    //                     "GPS",
    //                     1024*3,
    //                     NULL,
    //                     10,
    //                     &xGPSHandle
    // );
    // if (xReturned != pdPASS)
    // {
    //   printf("Could not start the GPS task");
    // }
}
