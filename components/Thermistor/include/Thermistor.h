/*********************************************************************************
*     File Name           :     /components/Thermistor/include/Thermistor.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-24 21:04]
*     Last Modified       :     [2022-10-27 00:13]
*     Description         :     Class definition for gathering data from both thermistors 
**********************************************************************************/


#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__
#include <string>

#include <stdio.h>
#include <math.h>
#include <sstream>

#include "driver/adc.h"
#include "datalogger.h"

#define THERM1_GPIO     CONFIG_THERM1_GPIO
#define THERM2_GPIO     CONFIG_THERM2_GPIO

#define THERM1_R1       CONFIG_THERM1_R1   // Resistance between Vs and thermistor
#define THERM2_R1       CONFIG_THERM2_R1

// Cannot be defined in Kconfig as they are float values
#define THERM1_VS       3.3                // Voltage reference for thermistors
#define THERM2_VS       3.3                // Voltage reference for thermistors

// #define THERM1_VS       CONFIG_THERM1_VS   // Voltage reference for thermistors
// #define THERM2_VS       CONFIG_THERM12VS   // Voltage reference for thermistors

// Float values don't work in Kconfig
#define THERM1_TO       298.15
#define THERM2_TO       298.15

#define THERM1_RO       CONFIG_THERM1_RO
#define THERM2_RO       CONFIG_THERM2_RO

#define THERM1_B        CONFIG_THERM1_B
#define THERM2_B        CONFIG_THERM2_B

#define THERM_SAMPLE_HZ CONFIG_THERM_SAMPLE_HZ

#define THERM_TASK_ID   "THERM_COMP"

// in math.h, log == ln
#define KELVIN_TO_CELSIUS(T_k) ((T_k) - 273.15)
#define CELSIUS_TO_F(T_c)      (((T_c) * 9 / 5) + 32)

#define THERM2_ATTACHED

#ifdef false
#define THERM_LOG_IRIDIUM
#endif
#define THERM_LOG_LoRa

class ThermistorComponent{
public:
  ThermistorComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium);
  static void vMainLoop_Task(void* arg);

private:
  void logThermistors();
  void readThermistors();
  float calcRT(float R1, float VS, float Vo);
  float calcTemp(float To, int Beta, float Ro, float RT);

  
  float _therm1_vo;
  float _therm2_vo;
  QueueHandle_t _dataOutSD;
  QueueHandle_t _dataOutLoRa;
  QueueHandle_t _dataOutIridium;

  TickType_t _lastUpdateLoRa;
  TickType_t _lastUpdateIridium;
};

#endif // !__THERMISTOR_H__
