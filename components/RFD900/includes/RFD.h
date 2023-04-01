/*********************************************************************************
 *     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-21 00:33]
 *     Last Modified       :     [2022-10-26 02:09]
 *     Description         :     Component to handle the task for the IridiumSBD
 **********************************************************************************/

#ifndef __RFD_COMPONENT_H__
#define __RFD_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include <string>
#include <time.h>

#include "ComBus.h"
#include "datalogger.h"

extern "C"
{
#include "umsg_GPS.h"
#include "umsg_RFD.h"
#include "umsg_Sensors.h"
}

namespace RFD
{
  typedef struct __attribute__((__packed__))
  {
    int64_t latitude;     /*! < Latitude. 10^-7*/
    int64_t longitude;    /*! < Longitude. 10^-7*/
    int32_t altitude_gps; /*! < Altitude in mm */
    uint16_t pdop;        /*! < Accuracy of position. PDOP 10^-2*/
    uint16_t year;        /*! < Year from GPS*/
    uint8_t month;        /*! < Month from GPS */
    uint8_t day;          /*! < Day from GPS*/
    uint8_t hour;         /*! < Hour in UTC from GPS*/
    uint8_t minute;       /*! < Minute in UTC from GPS*/
    uint8_t second;       /*! < Seconds in UTC from GPS*/
    float pressure_hpa;   /*! < Pressure in hpa / millibar*/
    float altitude_baro;  /*! < Altitude calculated by the barometer in meters*/
    float baro_temp;      /*! < Temperature from the barometer*/
  } RFDRemoteData;

  template <class T>
  uint8_t *toByteArray(T data_struct, size_t &size);
}

class RFDComponent
{
public:
  static void vMainLoop_Task(void *arg);

private:
  void init();
  void initSubs();
  void vRX();
  void readSubs();
  size_t sendBytes(uint8_t *data, size_t data_size, umsg_RFD_msg_type_t msg_type);

  RFD::RFDRemoteData _rfd_data;

  umsg_sub_handle_t _gps_data_sub;
  umsg_GPS_data_t _gps_data;

  umsg_sub_handle_t _baro_data_sub;
  umsg_Sensors_baro_data_t _baro_data;
};

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
