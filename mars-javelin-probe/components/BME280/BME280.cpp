/*********************************************************************************
*     File Name           :     /mars-javelin-probe/components/BME280/BME280.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-20 23:56]
*     Last Modified       :     [2022-10-27 01:40]
*     Description         :     Component handler for the BME280, a pressure sensor      
**********************************************************************************/

#include "BME280.h"

void BME280Component::vMainLoop_Task(void *bme_280_component){
  BME280Component bme_comp = *((BME280Component*)bme_280_component);
  bme_comp.setup();
  const TickType_t xDelay = (1000/BME_SAMPLE_RATE_HZ) / portTICK_PERIOD_MS;

  for(;;){
    bme_comp.logBME();
    vTaskDelay(xDelay);
  }
}

void BME280Component::setup(){
  device = Adafruit_BME280();
}

void BME280Component::logBME(){
#ifdef BME_ATTACHED
  float temp = device.readTemperature();
  float pressure = device.readPressure();
  float humidity = device.readHumidity();
  float altitude = device.readAltitude();
#else
  float temp = 0;
  float pressure = 0;
  float humidity = 0;
  float altitude = 0;
#endif

  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << BME_TASK_ID << " || ";
  data << "Temp: " << temp << " C | Pressure: " << pressure << " ? | humidity: " << humidity << " ? | altitude: " << altitude << " ?\n";
  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post IMU data\n");
  }
}
