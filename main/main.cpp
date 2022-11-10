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
#include "Thermistor.h"
#include "CommandCenter.h"
#include "IMUComponent.h"
#include "BME280.h"
#include "GPSComponent.h"
#include "IridiumComponent.h"

//#define LoRaTRANSMITTER

// Alternate main task for an outside LoRa transmitter to test receive and transmit of probe
#ifdef LoRaTRANSMITTER
#include "SPI.h"
#include "LoRa.h"
extern "C" void app_main(void){
  initArduino();

  SPIClass *hspi = new SPIClass(HSPI);
  hspi->begin(CONFIG_HSPI_SCK_GPIO, CONFIG_HSPI_MISO_GPIO, CONFIG_HSPI_MOSI_GPIO, CONFIG_LORA_CS_GPIO);
  LoRa.setPins(CONFIG_LORA_CS_GPIO, CONFIG_HSPI_RST_GPIO, CONFIG_HSPI_DI0_GPIO);
  LoRa.setSPI(*hspi);
  LoRa.setSPIFrequency(1E6);
  if(!LoRa.begin(915E6)){
    printf("Starting LoRa failed\n");
    return;
  }

  std::string cut_down = "0x0101";
  int count = 0;
  for(;;){
    vTaskDelay(10000/portTICK_PERIOD_MS);
    LoRa.beginPacket();
    LoRa.write(0x01);
    LoRa.write(0x01);
    LoRa.endPacket();
    printf("Packet %d sent. \n", count++);
  }
}
#else

extern "C" void app_main(void)
{
    // Initialize artuidno library
    initArduino();


    vTaskDelay(1000/portTICK_PERIOD_MS);

    initComBus();
    i2cScan();

    vTaskDelay(1000/portTICK_PERIOD_MS);
    /**************************************
     *
     *  Initialization
     *
     ***************************************/
    BaseType_t xReturned;

    QueueHandle_t dataOutSD = xQueueCreate(50, sizeof(SDData*));

    // TODO:: Change from SDData to LoRaData class
    QueueHandle_t dataOutLoRa = xQueueCreate(50, sizeof(std::string*));
    QueueHandle_t dataOutIridium = xQueueCreate(50, sizeof(std::string*));

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
      1024 * 5,                 // Stack size of task
      (void*)(&data_log),        // task parameters
      9,                        // Task priority
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
      "Command Center Task",           // Name of task
      1024 * 2,                 // Stack size of task
      (void*)(&cmd_center),        // task parameters
      10,                        // Task priority
      &xCmdCenter            // Handle to resulting task
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
    GPSComponent gps_component = GPSComponent(dataOutSD, dataOutLoRa, dataOutIridium);

    TaskHandle_t xGPSComponentHandle = NULL;
    xReturned = xTaskCreatePinnedToCore(
      GPSComponent::vMainLoop_Task,    // Function for task
      "GPS Component Task",   // Name of task
      1024 * 4,                        // Stack size of task
      (void*)(&gps_component),         // task parameters
      8,                              // Task priority
      &xGPSComponentHandle,             // Handle to resulting task
      0
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the GPS Component task\n");
    }

    // Allow GPS to configure i2c address
    printf("Waiting for GPS setup\n");
    vTaskDelay(10000/portTICK_PERIOD_MS);

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
      1024*3,
      (void*)(&lora_component),
      10,
      &xLoraHandle
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Lora RX task\n");
    }


    /**************************************
     *
     *  Creating the IMU process
     *
     ***************************************/
    IMUComponent imu_component = IMUComponent(dataOutSD, dataOutLoRa, dataOutIridium);

    TaskHandle_t xIMUComponentHandle= NULL;
    xReturned = xTaskCreatePinnedToCore(
      IMUComponent::vMainLoop_Task, // Function for task
      "IMU Component Task",         // Name of task
      1024 * 3,                     // Stack size of task
      (void*)(&imu_component),      // task parameters
      8,                           // Task priority
      &xIMUComponentHandle,          // Handle to resulting task
      1
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the IMUComponent task\n");
    }
    
    /**************************************
     *
     *  Creating the BME280 process
     *
     ***************************************/
    BME280Component bme_component = BME280Component(dataOutSD, dataOutLoRa, dataOutIridium);

    TaskHandle_t xBMEComponentHandle = NULL;
    xReturned = xTaskCreatePinnedToCore(
      BME280Component::vMainLoop_Task,   // Function for task
      "BME280 Component Task",           // Name of task
      1024 * 3,                          // Stack size of task
      (void*)(&bme_component),           // task parameters
      8,                                // Task priority
      &xBMEComponentHandle,             // Handle to resulting task
      0
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the BME280 Component task\n");
    }


    /**************************************
     *
     *  Creating the IridiumSBD process
     *
     ***************************************/
    IridiumComponent iridium_component = IridiumComponent(dataOutSD, dataOutIridium, xCmdCenter);

    TaskHandle_t xIridiumComponentHandle = NULL;
    xReturned = xTaskCreate(
      IridiumComponent::vMainLoop_Task,    // Function for task
      "Iridium Component Task",   // Name of task
      1024 * 2,                      // Stack size of task
      (void*)(&iridium_component),   // task parameters
      15,                            // Task priority
      &xIridiumComponentHandle       // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the IridiumSBD Component task\n");
    }

    /**************************************
     *
     *  Creating the Thermistor process
     *
     ***************************************/
    ThermistorComponent thermistor_component = ThermistorComponent(dataOutSD, dataOutLoRa, dataOutIridium);

    TaskHandle_t xThermistorComponentHandle = NULL;
    xReturned = xTaskCreate(
      ThermistorComponent::vMainLoop_Task,    // Function for task
      "Therm Comp Task",   // Name of task
      1024 * 3,                      // Stack size of task
      (void*)(&thermistor_component),// task parameters
      6,                            // Task priority
      &xThermistorComponentHandle    // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Thermistor Component task\n");
    }

    /**************************************
     *
     *  Creating the Stats process
     *
     ***************************************/
    RunTimeStats stats_component = RunTimeStats(dataOutSD);

    TaskHandle_t xStatsComponent = NULL;
    xReturned = xTaskCreate(
      RunTimeStats::stats_task,         // Function for task
      "stats",                          // Name of task
      1024 * 4,                        // Stack size of task
      (void*)(&stats_component),         // task parameters
      5,                              // Task priority
      &xStatsComponent             // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Run Time stats Component task\n");
    }

    // Main task must stay alive as it has references to classes that will be deleted otherwise
    for(;;){
      vTaskDelay(1000);
    }
}
#endif
