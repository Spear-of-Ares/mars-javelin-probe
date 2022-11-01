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
#if CONFIG_BME_ATTACHED == true
#define BME_ATTACHED
#endif
class BME280Component{
public:
  BME280Component(QueueHandle_t dataOutSD){
    _dataOutSD = dataOutSD;
  }
  static void vMainLoop_Task(void *bme_280_component);
private:
  void setup();
  void logBME();
  QueueHandle_t _dataOutSD;
  Adafruit_BME280 device;

};
#endif /* __BME280_COMPONENT_H__ */
