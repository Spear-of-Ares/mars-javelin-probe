/*********************************************************************************
*     File Name           :     /mars-javelin-probe/components/BME280/include/BME280.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-20 23:54]
*     Last Modified       :     [2022-10-27 20:15]
*     Description         :     Component handler for the BME280, a pressure sensor
**********************************************************************************/
#ifndef __BME280_COMPONENT_H__
#define __BME280_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include "datalogger.h"
#include "ComBus.h"
#include "Adafruit_BME280.h"

#define BME_TASK_ID "BME TASK  "
#define BME_SAMPLE_RATE_HZ  10
#if true
#define BME_ATTACHED
#endif

#ifdef false
#define BME_LOG_IRIDIUM
#endif

#define BME_LOG_LoRa
class BME280Component{
public:
  BME280Component(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium){
    _dataOutSD = dataOutSD;
    _dataOutLoRa = dataOutLoRa;
    _dataOutIridium = dataOutIridium;

    _lastUpdateIridium = 0x0000;
    _lastUpdateLoRa = 0x0000;
  }
  static void vMainLoop_Task(void *bme_280_component);
private:
  void setup();
  void logBME();
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  QueueHandle_t _dataOutIridium;

  TickType_t _lastUpdateLoRa;
  TickType_t _lastUpdateIridium;

  Adafruit_BME280 device;

  float _startup_pressure;
  float _startup_alt;
  float _startup_sealevel_pressure;

};
#endif /* __BME280_COMPONENT_H__ */
