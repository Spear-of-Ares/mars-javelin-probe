/**
 * @file main.cpp
 * @author Jon Kopf (kopf0033@vandals.uidaho.edu)
 * @brief  Main entrance to the code.
 * @version 2.0
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <string>
#include <cstdio>
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_freertos_hooks.h"

#include "Arduino.h"
#include "datalogger.h"
#include "LoRaComponent.h"
#include "Stats.h"
#include "ComBus.h"
#include "Sensors.h"
#include "CommandCenter.h"
#include "GPSComponent.h"
#include "IridiumComponent.h"

#include <umsg_StatusMsgs.h>
#include <umsg_GPS.h>

/*!
 * @brief Main entrance to program. Initializes all other tasks.
 *
 */
extern "C" void app_main(void)
{
  const TickType_t inter_task_delay = 1000 / portTICK_PERIOD_MS;
  gpio_set_level(GPIO_NUM_4, 0);

  // Initialize artuidno library
  initArduino();

  vTaskDelay(inter_task_delay);

  initComBus();
  i2cScan();

  vTaskDelay(inter_task_delay);
  /**************************************
   *
   *  Initialization
   *
   ***************************************/
  BaseType_t xReturned;
  /**************************************
   *
   *  Creating the DataLogger process
   *
   ***************************************/
  DataLogger data_log = DataLogger();
  data_log.setup();

  TaskHandle_t xDataLogHandle = NULL;
  xReturned = xTaskCreate(
      DataLogger::vLogLoop_Task, // Function for task
      "LOGG_TASK",               // Name of task
      1024 * 5,                  // Stack size of task
      (void *)(&data_log),       // task parameters
      9,                         // Task priority
      &xDataLogHandle            // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Data log loop task\n");
  }

  vTaskDelay(inter_task_delay);
  /**************************************
   *
   *  Creating the Command Center process
   *
   ***************************************/
  CommandComponent cmd_center = CommandComponent();

  TaskHandle_t xCmdCenter = NULL;
  xReturned = xTaskCreate(
      CommandComponent::vMainLoop_Task, // Function for task
      "CMDC_TASK",                      // Name of task
      1024 * 10,                        // Stack size of task
      (void *)(&cmd_center),            // task parameters
      10,                               // Task priority
      &xCmdCenter                       // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Data log loop task\n");
  }

  vTaskDelay(inter_task_delay);

  /**************************************
   *
   *  Creating the GPS process
   *
   ***************************************/
  GPSComponent gps_component = GPSComponent();

  TaskHandle_t xGPSComponentHandle = NULL;
  xReturned = xTaskCreatePinnedToCore(
      GPSComponent::vMainLoop_Task, // Function for task
      "GPS_TASK",                   // Name of task
      1024 * 4,                     // Stack size of task
      (void *)(&gps_component),     // task parameters
      8,                            // Task priority
      &xGPSComponentHandle,         // Handle to resulting task
      0);
  if (xReturned != pdPASS)
  {
    printf("Could not start the GPS Component task\n");
  }

  // Allow GPS to configure i2c address
  printf("Waiting for GPS setup\n");
  vTaskDelay(inter_task_delay);

  /**************************************
   *
   *  Creating the LoRa process
   *
   ***************************************/
  LoRaComponent lora_component = LoRaComponent();

  TaskHandle_t xLoraHandle = NULL;
  xReturned = xTaskCreate(
      LoRaComponent::vMainLoop_Task,
      "LoRa_TASK",
      1024 * 5,
      (void *)(&lora_component),
      10,
      &xLoraHandle);
  if (xReturned != pdPASS)
  {
    printf("Could not start the Lora RX task\n");
  }
  vTaskDelay(inter_task_delay);

  // /**************************************
  //  *
  //  *  Creating the IMU process
  //  *
  //  ***************************************/
  Sensors sensors_component = Sensors();

  TaskHandle_t xSensorsComponentHandle = NULL;
  xReturned = xTaskCreate(
      Sensors::vMainLoop_Task,      // Function for task
      "SENS_TASK",                  // Name of task
      1024 * 6,                     // Stack size of task
      (void *)(&sensors_component), // task parameters
      11,                           // Task priority
      &xSensorsComponentHandle      // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Sensors task\n");
  }
  vTaskDelay(inter_task_delay);

  /**************************************
   *
   *  Creating the IridiumSBD process
   *
   ***************************************/
  IridiumComponent iridium_component = IridiumComponent();

  TaskHandle_t xIridiumComponentHandle = NULL;
  xReturned = xTaskCreate(
      IridiumComponent::vMainLoop_Task, // Function for task
      "IRID_TASK",                      // Name of task
      1024 * 2,                         // Stack size of task
      (void *)(&iridium_component),     // task parameters
      15,                               // Task priority
      &xIridiumComponentHandle          // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the IridiumSBD Component task\n");
  }
  vTaskDelay(inter_task_delay * 10);

  /**************************************
   *
   *  Creating the Stats process
   *
   ***************************************/
  RunTimeStats stats_component = RunTimeStats();

  TaskHandle_t xStatsComponent = NULL;
  xReturned = xTaskCreate(
      RunTimeStats::stats_task,   // Function for task
      "STAT_TASK",                // Name of task
      1024 * 4,                   // Stack size of task
      (void *)(&stats_component), // task parameters
      5,                          // Task priority
      &xStatsComponent            // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Run Time stats Component task\n");
  }

  /// Main task must stay alive as it has references to classes that will be deleted otherwise
  for (;;)
  {
    vTaskDelay(10000);
  }
}
