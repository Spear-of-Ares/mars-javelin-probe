/*********************************************************************************
 *     File Name           :     mars-javelin-probe/main/mars-javelin-probe.c
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-03 22:40]
 *     Last Modified       :     [2022-11-06 03:29]
 *     Description         :     Coordinates and controls generation of new tasks
 *                               Using the ESP Arduino library for access to a wider number of
 *                               libraries for components, such as the IridiumSBD library.
 *
 *                               Using Arduino as a component from the following article:
 *                               https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/esp-idf_component.html **********************************************************************************/

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

// #define LoRaTRANSMITTER

// Alternate main task for an outside LoRa transmitter to test receive and transmit of probe
#ifdef LoRaTRANSMITTER
#include "SPI.h"
#include "LoRa.h"
void vRX()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    std::string received("");
    SDData *sdOut = new SDData();
    sdOut->file_name = new std::string("comms");

    while (LoRa.available())
    {
      received += (char)LoRa.read();
    }
    printf("Received: %s", received.c_str());
    printf(" || RSSI: %d | SNR: %f\n", LoRa.packetRssi(), LoRa.packetSnr());
  }
}

void vTX()
{
  static int count = 0;
  LoRa.beginPacket();
  LoRa.write(0x01);
  LoRa.write(0x01);
  LoRa.endPacket();
  printf("Packet %d sent...\n", count++);
}
extern "C" void app_main(void)
{
  initArduino();

  SPIClass *hspi = new SPIClass(HSPI);
  hspi->begin(CONFIG_HSPI_SCK_GPIO, CONFIG_HSPI_MISO_GPIO, CONFIG_HSPI_MOSI_GPIO, CONFIG_LORA_CS_GPIO);
  LoRa.setPins(CONFIG_LORA_CS_GPIO, CONFIG_HSPI_RST_GPIO, CONFIG_HSPI_DI0_GPIO);
  LoRa.setSPI(*hspi);
  LoRa.setSPIFrequency(1E6);
  if (!LoRa.begin(915E6))
  {
    printf("Starting LoRa failed\n");
    return;
  }
  Serial.begin(115200);

  std::string cut_down = "0x0101";
  // Check if something was sent on serial
  std::string msg = "";
  int time = 0;
  for (;;)
  {
    vRX();
    while (Serial.available())
    {
      char new_char = char(Serial.read());
      if (new_char == 'c')
      {
        msg = "";
      }
      else
      {
        msg += new_char;
      }
    }

    // if something was sent, then send the TX packet
    if (msg == "fire")
    {
      vTX();
      msg = "";
    }
    if (time % 300 == 0 && msg != "")
    {
      printf("%s\n", msg.c_str());
      time = 0;
    }
    else
    {
      time += 1;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
#else

extern "C" void app_main(void)
{
  gpio_set_level(GPIO_NUM_4, 0);

  // Initialize artuidno library
  initArduino();

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  initComBus();
  i2cScan();

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  /**************************************
   *
   *  Initialization
   *
   ***************************************/
  BaseType_t xReturned;

  QueueHandle_t dataOutSD = xQueueCreate(50, sizeof(SDData *));

  QueueHandle_t dataOutLoRa = xQueueCreate(50, sizeof(std::string *));
  QueueHandle_t dataOutIridium = xQueueCreate(50, sizeof(std::string *));

  /**************************************
   *
   *  Creating the DataLogger process
   *
   ***************************************/
  // Init before other tasks that need dataOutSD Queue
  DataLogger data_log = DataLogger(dataOutSD);
  data_log.setup();

  TaskHandle_t xDataLogHandle = NULL;
  xReturned = xTaskCreate(
      DataLogger::vLogLoop_Task, // Function for task
      "Log Loop Task",           // Name of task
      1024 * 5,                  // Stack size of task
      (void *)(&data_log),       // task parameters
      9,                         // Task priority
      &xDataLogHandle            // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Data log loop task\n");
  }

  /**************************************
   *
   *  Creating the Command Center process
   *
   ***************************************/
  // Init before other tasks that need dataOutSD Queue
  CommandComponent cmd_center = CommandComponent(dataOutSD, dataOutLoRa, dataOutIridium);

  TaskHandle_t xCmdCenter = NULL;
  xReturned = xTaskCreate(
      CommandComponent::vMainLoop_Task, // Function for task
      "Command Center Task",            // Name of task
      1024 * 2,                         // Stack size of task
      (void *)(&cmd_center),            // task parameters
      10,                               // Task priority
      &xCmdCenter                       // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Data log loop task\n");
  }

  /**************************************
   *
   *  Creating the GPS process
   *
   ***************************************/
  GPSComponent gps_component = GPSComponent(dataOutSD, dataOutLoRa, dataOutIridium, xCmdCenter);

  TaskHandle_t xGPSComponentHandle = NULL;
  xReturned = xTaskCreatePinnedToCore(
      GPSComponent::vMainLoop_Task, // Function for task
      "GPS Component Task",         // Name of task
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
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  /**************************************
   *
   *  Creating the LoRa process
   *
   ***************************************/
  LoRaComponent lora_component = LoRaComponent(dataOutSD, dataOutLoRa, xCmdCenter);

  TaskHandle_t xLoraHandle = NULL;
  xReturned = xTaskCreate(
      LoRaComponent::vMainLoop_Task,
      "LoRa Component Task",
      1024 * 3,
      (void *)(&lora_component),
      10,
      &xLoraHandle);
  if (xReturned != pdPASS)
  {
    printf("Could not start the Lora RX task\n");
  }

  // /**************************************
  //  *
  //  *  Creating the IMU process
  //  *
  //  ***************************************/
  Sensors sensors_component = Sensors();

  TaskHandle_t xSensorsComponentHandle = NULL;
  xReturned = xTaskCreate(
      Sensors::vMainLoop_Task,      // Function for task
      "Sensors",                    // Name of task
      1024 * 6,                     // Stack size of task
      (void *)(&sensors_component), // task parameters
      8,                            // Task priority
      &xSensorsComponentHandle      // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Sensors task\n");
  }

  /**************************************
   *
   *  Creating the IridiumSBD process
   *
   ***************************************/
  IridiumComponent iridium_component = IridiumComponent(dataOutSD, dataOutIridium, xCmdCenter);

  TaskHandle_t xIridiumComponentHandle = NULL;
  xReturned = xTaskCreate(
      IridiumComponent::vMainLoop_Task, // Function for task
      "Iridium Component Task",         // Name of task
      1024 * 2,                         // Stack size of task
      (void *)(&iridium_component),     // task parameters
      15,                               // Task priority
      &xIridiumComponentHandle          // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the IridiumSBD Component task\n");
  }

  /**************************************
   *
   *  Creating the Stats process
   *
   ***************************************/
  RunTimeStats stats_component = RunTimeStats(dataOutSD);

  TaskHandle_t xStatsComponent = NULL;
  xReturned = xTaskCreate(
      RunTimeStats::stats_task,   // Function for task
      "stats",                    // Name of task
      1024 * 4,                   // Stack size of task
      (void *)(&stats_component), // task parameters
      5,                          // Task priority
      &xStatsComponent            // Handle to resulting task
  );
  if (xReturned != pdPASS)
  {
    printf("Could not start the Run Time stats Component task\n");
  }
  // Main task must stay alive as it has references to classes that will be deleted otherwise
  for (;;)
  {
    vTaskDelay(10000);
  }
}
#endif
