/*********************************************************************************
 *     File Name           :     /components/IridiumSBD/IridiumSBDComponent.cpp
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-21 00:37]
 *     Last Modified       :     [AUTO_UPDATE_BEFORE_SAVE]
 *     Description         :     Component for handling IridiumSBD things
 **********************************************************************************/

#include "RFD.h"

namespace RFD
{

  template <class T>
  uint8_t *toByteArray(T data_struct, size_t &size)
  {
    void *temp_pointer = &data_struct;
    size = sizeof(T);
    uint8_t *buffer = new uint8_t[size];

    memcpy(buffer, temp_pointer, size);

    return buffer;
  }

}

void RFDComponent::initSubs()
{

  _gps_data_sub = umsg_GPS_data_subscribe(5, 1);

  _baro_data_sub = umsg_Sensors_baro_data_subscribe(50, 1);
}

void RFDComponent::vMainLoop_Task(void *arg)
{
  RFDComponent rfd_comp = *((RFDComponent *)arg);
  for (;;)
  {
    rfd_comp.vRX();
    rfd_comp.readSubs();
    vTaskDelay(4500 / portTICK_PERIOD_MS);
  }
}

void RFDComponent::vRX()
{
}

void RFDComponent::readSubs()
{
  int timeout = 1 / portTICK_PERIOD_MS;
  RFD::RFDRemoteData data;

  if (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
  {
    data.latitude = _gps_data.lat_long[0];
    data.longitude = _gps_data.lat_long[1];
    data.altitude_gps = _gps_data.altitude;
    data.pdop = _gps_data.p_dilution_precision;
    data.hour = _gps_data.time_ymd_hms[3];
    data.minute = _gps_data.time_ymd_hms[4];
    data.second = _gps_data.time_ymd_hms[5];
  }

  while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, timeout) == pdPASS)
  {
    data.pressure_hpa = _baro_data.pressure_pa;
    data.altitude_baro = _baro_data.alt_above_sea_m;
    data.baro_temp = _baro_data.temperature_c;
  }

  size_t data_bytes_size;
  uint8_t *data_bytes = RFD::toByteArray<RFD::RFDRemoteData>(data, data_bytes_size);
  size_t sent_size = sendBytes(data_bytes, data_bytes_size);
  printf("Sent Size RFD: %d\n", sent_size);
}

// I would like to add some header and footer to the packet we are sending
size_t RFDComponent::sendBytes(uint8_t *data, size_t data_size)
{
  uint8_t packet_size = data_size + 3;
  uint8_t *packet = new uint8_t[packet_size];
  packet[0] = (uint8_t)'S'; // 'S' for start
  packet[1] = packet_size;
  for (int i = 0; i < data_size; i++)
  {
    packet[i + 2] = data[i];
  }
  packet[data_size + 2] = (uint8_t)'E'; // 'E' for end
  i2cuart.writeBytes(SC16IS752_CHANNEL_A, packet, packet_size);
  return packet_size;
}
