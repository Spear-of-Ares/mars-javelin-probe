#ifndef __GPS_TODATALINE_H__
#define __GPS_TODATALINE_H__

#include <string>
#include "dataline.h"

extern "C"
{
#include "umsg_GPS.h"
}

DataLine GPS_data_toDataLine(umsg_GPS_data_t &data);
DataLine GPS_configuration_toDataLine(umsg_GPS_configuration_t &data);
DataLine GPS_state_toDataLine(umsg_GPS_state_t &data);

#endif