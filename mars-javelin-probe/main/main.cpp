/*********************************************************************************
*     File Name           :     mars-javelin-probe/main/mars-javelin-probe.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-03 22:40]
*     Last Modified       :     [2022-10-26 02:17]
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
#include "IridiumSBD.h"
#include "datalogger.h"
#include "LoRaComponent.h"
#include "Stats.h"
#include "ComBus.h"
#include "Thermistor.h"
#include "CommandCenter.h"

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
    vTaskDelay(5000/portTICK_PERIOD_MS);
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
    initComBus();
    i2cScan();

    /**************************************
     *
     *  Initialization
     *
     ***************************************/
    BaseType_t xReturned;

    QueueHandle_t dataOutSD = xQueueCreate(50, sizeof(SDData*));

    // TODO:: Change from SDData to LoRaData class
    QueueHandle_t dataOutLoRa = xQueueCreate(50, sizeof(SDData*));

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
      1024 * 4,                 // Stack size of task
      (void*)(&data_log),        // task parameters
      10,                        // Task priority
      &xDataLogHandle            // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the Data log loop task\n");
    }

    // Delay to allow SPI bus to fully initialize
    vTaskDelay(1000/portTICK_PERIOD_MS);


    /**************************************
     *
     *  Creating the Command Center process
     *
     ***************************************/
    // Init before other tasks that need dataOutSD Queue
    CommandComponent cmd_center = CommandComponent(dataOutSD);

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


 // Don't include these as they are not implemented
#if false
    /**************************************
     *
     *  Creating the IMU process
     *
     ***************************************/
    IMUComponent imu_component = IMUComponent(dataOutSD);
    imu_component.setup();

    TaskHandle_t xIMUComponentHandle= NULL;
    xReturned = xTaskCreate(
      IMUComponent::vMainLoop_Task, // Function for task
      "IMU Component Task",         // Name of task
      1024 * 2,                     // Stack size of task
      (void*)(&imu_component),      // task parameters
      10,                           // Task priority
      &xIMUComponentHandle          // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the IMUComponent task\n");
    }
    

    /**************************************
     *
     *  Creating the IridiumSBD process
     *
     ***************************************/
    IridiumSBDComponent iridium_component = IridiumSBDComponent(dataOutSD);
    iridium_component.setup();

    TaskHandle_t xIridiumComponentHandle = NULL;
    xReturned = xTaskCreate(
      IridiumSBDComponent::vMainLoop_Task,    // Function for task
      "IridiumSBD Component Task",   // Name of task
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
     *  Creating the BME280 process
     *
     ***************************************/
    BME280Component bme_component = BME280Component(dataOutSD);
    bme_component.setup();

    TaskHandle_t xBMEComponentHandle = NULL;
    xReturned = xTaskCreate(
      BME280Component::vMainLoop_Task,   // Function for task
      "BME280 Component Task",           // Name of task
      1024 * 2,                          // Stack size of task
      (void*)(&bme_component),           // task parameters
      10,                                // Task priority
      &xxBMEComponentHandle             // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the BME280 Component task\n");
    }

    /**************************************
     *
     *  Creating the GPS process
     *
     ***************************************/
    GPSComponent gps_component = GPSComponent(dataOutSD);
    gps_component.setup();

    TaskHandle_t xGPSComponentHandle = NULL;
    xReturned = xTaskCreate(
      GPSComponent::vMainLoop_Task,    // Function for task
      "GPS Component Task",   // Name of task
      1024 * 2,                        // Stack size of task
      (void*)(&gps_component),         // task parameters
      10,                              // Task priority
      &xGPSComponentHandle             // Handle to resulting task
    );
    if (xReturned != pdPASS)
    {
      printf("Could not start the GPS Component task\n");
    }

#endif
    /**************************************
     *
     *  Creating the Thermistor process
     *
     ***************************************/
    ThermistorComponent thermistor_component = ThermistorComponent(dataOutSD);

    TaskHandle_t xThermistorComponentHandle = NULL;
    xReturned = xTaskCreate(
      ThermistorComponent::vMainLoop_Task,    // Function for task
      "Therm Comp Task",   // Name of task
      1024 * 2,                      // Stack size of task
      (void*)(&thermistor_component),// task parameters
      10,                            // Task priority
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
    xReturned = xTaskCreatePinnedToCore(
      RunTimeStats::stats_task,         // Function for task
      "stats",                          // Name of task
      1024 * 4,                        // Stack size of task
      (void*)(&stats_component),         // task parameters
      12,                              // Task priority
      &xStatsComponent,             // Handle to resulting task
      tskNO_AFFINITY
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
