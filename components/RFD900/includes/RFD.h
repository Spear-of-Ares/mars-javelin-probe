/*********************************************************************************
 *     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-21 00:33]
 *     Last Modified       :     [2022-10-26 02:09]
 *     Description         :     Component to handle the task for the IridiumSBD
 **********************************************************************************/

#ifndef __IRIDIUMSBD_COMPONENT_H__
#define __IRIDIUMSBD_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include <string>
#include <time.h>

#include "ComBus.h"

extern "C" {
  #include "umsg_GPS.h"
  #include "umsg_Sensors.h"
}

namespace RFD{
  typedef struct
  {
    uint64_t latitude;     /*! < Latitude. 10^-7*/
    uint64_t longitude;    /*! < Longitude. 10^-7*/
    uint32_t altitude_gps; /*! < Altitude in mm */
    uint16_t pdop;         /*! < Accuracy of position. PDOP 10^-2*/
    int32_t hour;          /*! < Hour in UTC from GPS*/
    int32_t minute;        /*! < Minute in UTC from GPS*/
    int32_t second;        /*! < Seconds in UTC from GPS*/
    float pressure_hpa;    /*! < Pressure in hpa / millibar*/
    float altitude_baro;   /*! < Altitude calculated by the barometer in meters*/
    float baro_temp;       /*! < Temperature from the barometer*/
  } RFDRemoteData;

  template <class T>
  uint8_t* toByteArray(T data_struct, size_t &size);
}

class RFDComponent
{
public:
  RFDComponent()
  {

  }
  static void vMainLoop_Task(void *arg);

private:

  void initSubs();
  void vRX();
  void readSubs();

  umsg_sub_handle_t _gps_data_sub;
  umsg_GPS_data_t _gps_data;

  umsg_sub_handle_t _baro_data_sub;
  umsg_Sensors_baro_data_t _baro_data;
};

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
