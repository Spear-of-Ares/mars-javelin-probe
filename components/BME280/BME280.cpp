/*********************************************************************************
*     File Name           :     /mars-javelin-probe/components/BME280/BME280.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-20 23:56]
*     Last Modified       :     [2022-11-05 18:38]
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
#ifdef BME_ATTACHED
  while(device.begin((uint8_t)BME280_ADDRESS, &Wire) == false){
    printf("BME280 could not be connected\n");
  }
#endif
  _startup_pressure = device.readPressure(); // pressue in Pa
  _startup_pressure = _startup_pressure / 100.0; // Convert to hPa
  _startup_alt = device.readAltitude(1013.25);
  printf("BME280 setup complete\n");
}

void BME280Component::logBME(){
#ifdef BME_ATTACHED
  float temp = device.readTemperature();
  float pressure = device.readPressure();
  float humidity = device.readHumidity();
  // Standard pressure of sea level: 1013.25
  // Could be pressure at launch site to estimate hight above ground
  
  float altitude_from_start = device.readAltitude(_startup_pressure);
  float altitude_asl = device.readAltitude(1013.25);

  if( altitude_asl > 12000){
      //xTaskNotify(_cmd_center, 0x01, eSetBits);
  }
  #else

  float temp = 0;
  float pressure = 0;
  float humidity = 0;
  float altitude = 0;
  #endif

  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << BME_TASK_ID << " || ";
  data << "Temp: " << temp << " C | Pressure: " << pressure << " Pa | humidity: " << humidity << " % | altitude (asl): " << altitude_asl << " m | altitude (relative): " << altitude_from_start << " m";
  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post IMU data\n");
  }

  TickType_t curr_tick = xTaskGetTickCount();
#ifdef BME_LOG_IRIDIUM
  // Update iridium once every 60 seconds
  if (curr_tick - _lastUpdateIridium > 60000 / portTICK_PERIOD_MS)
  {
    std::string *iriddata = new std::string(data.str());
    _lastUpdateIridium = curr_tick;
    if (xQueueSend(_dataOutIridium, &(iriddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Failed to post GPS data to Iridium\n");
    }
  }
#endif

#ifdef BME_LOG_LoRa
  // Update LoRa every 15 seconds
  if (curr_tick - _lastUpdateLoRa > 15000 / portTICK_PERIOD_MS)
  {
    std::string *loradata = new std::string(data.str());
    _lastUpdateLoRa = curr_tick;
    if (xQueueSend(_dataOutLoRa, &(loradata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Failed to post GPS data to LoRa\n");
    }
  }
#endif
}
