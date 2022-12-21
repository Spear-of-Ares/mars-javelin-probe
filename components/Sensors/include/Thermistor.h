/*********************************************************************************
 *     File Name           :     /components/Thermistor/include/Thermistor.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-24 21:04]
 *     Last Modified       :     [2022-10-27 00:13]
 *     Description         :     Class definition for gathering data from both thermistors
 **********************************************************************************/

#ifndef __THERMISTOR_H__
#define __THERMISTOR_H__

#include <math.h>
#include "driver/adc.h"

#define INTERNAL_THERMISTOR_GPIO CONFIG_THERM1_GPIO
#define EXTERNAL_THERMISTOR_GPIO CONFIG_THERM2_GPIO

#define INTERNAL_THERMISTOR_R1 CONFIG_THERM1_R1 // Resistance between Vs and thermistor
#define EXTERNAL_THERMISTOR_R1 CONFIG_THERM2_R1

// Cannot be defined in Kconfig as they are float values
#define INTERNAL_THERMISTOR_VS 3.3 // Voltage reference for thermistors
#define EXTERNAL_THERMISTOR_VS 3.3 // Voltage reference for thermistors

// Float values don't work in Kconfig
#define INTERNAL_THERMISTOR_TO 298.15
#define EXTERNAL_THERMISTOR_TO 298.15

#define INTERNAL_THERMISTOR_RO CONFIG_THERM1_RO
#define EXTERNAL_THERMISTOR_RO CONFIG_THERM2_RO

#define INTERNAL_THERMISTOR_B CONFIG_THERM1_B
#define EXTERNAL_THERMISTOR_B CONFIG_THERM2_B

// in math.h, log == ln
#define CALCULATE_RT(R1, VS, Vo) (R1) * ((Vo) / ((VS) - (Vo)))
#define KELVIN_TO_CELSIUS(T_k) ((T_k)-273.15)
#define CELSIUS_TO_F(T_c) (((T_c)*9 / 5) + 32)

namespace Thermistors
{
  float calcTemp(float To, int Beta, float Ro, float RT);

  void readThermistors(float &internal_thermistor_c, float &external_thermistor_c);
}
#endif // !__THERMISTOR_H__
