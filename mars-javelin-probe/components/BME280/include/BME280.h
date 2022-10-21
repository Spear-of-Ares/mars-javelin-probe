/*********************************************************************************
*     File Name           :     /mars-javelin-probe/components/BME280/include/BME280.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-20 23:54]
*     Last Modified       :     [2022-10-21 00:30]
*     Description         :     Component handler for the BME280, a pressure sensor
**********************************************************************************/
#ifndef __BME280_COMPONENT_H__
#define __BME280_COMPONENT_H__

#include <stdio.h>
#include "Adafruit_BME280.h"
class BME280Component{
public:
  BME280Component(QueueHandle_t dataOutSD){
    _dataOutSD = dataOutSD;
  }
  static void vMainLoop(void *bme_280_component);
private:
  void writeToSD();
  QueueHandle_t _dataOutSD;
  Adafruit_BME280 device;

};
#endif /* __BME280_COMPONENT_H__ */
