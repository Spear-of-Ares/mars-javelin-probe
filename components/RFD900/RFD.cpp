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
  rfd_comp.init();
  for (;;)
  {

    rfd_comp.readSubs();
    for (int i = 0; i < 100; i++)
    {
      rfd_comp.vRX();
      vTaskDelay(45 / portTICK_PERIOD_MS);
    }
  }
}

void RFDComponent::init()
{
  umsg_RFD_state_t state_data;
  state_data.state = RFD_OK;
  state_data.initializing = 1;
  state_data.measure_tick = xTaskGetTickCount();
  umsg_RFD_state_publish(&state_data);

  initSubs();
  memset(&_rfd_data, 0, sizeof(_rfd_data));

  state_data.initializing = 0;
  state_data.initialized = 1;
  state_data.measure_tick = xTaskGetTickCount();
  umsg_RFD_state_publish(&state_data);
}

void RFDComponent::vRX()
{
  if (i2cuart.available(SC16IS752_CHANNEL_A) > 0)
  {
    umsg_RFD_received_msg_t recv_msg;
    recv_msg.receive_tick = xTaskGetTickCount();

    String received = i2cuart.readStringUntil(SC16IS752_CHANNEL_A, '\0');
    received += '\0'; // We want messages to end in '\0' as per command center requirements
    printf("RFD received: \n");
    for (int i = 0; i < received.length(); i++)
    {
      recv_msg.msg_data[i] = received[i];
      printf("%02x ", received[i]);
    }

    printf("\n");

    DataLine response;
    response.name = "RESPONSE";
    response.recorded_tick = recv_msg.receive_tick;

    // RFD sometimes receives long messages of the same byte over and over again
    // If it happens to be a long string of 0x01, then a premature cutdown is possible
    // Having a two byte prefix for commands with different bytes prevents this
    if (received[0] == 0x01 && received[1] == 0x55)
    {
      response.data.push_back("Command");
      recv_msg.recv_msg_type = RFD_MSG_COMMAND;
    }
    else if (received[0] == 0x00)
    {
      response.data.push_back("Message");
      recv_msg.recv_msg_type = RFD_MSG_OTHER;
    }
    else
    {
      return;
    }

    response.data.push_back("Received RFD");
    umsg_RFD_received_msg_publish(&recv_msg);
    std::string response_string = response.toString();

    uint8_t *response_bytes = new uint8_t[response_string.length()];

    for (int i = 0; i < response_string.length(); i++)
    {
      response_bytes[i] = response_string[i];
    }
    sendBytes((uint8_t *)response_bytes, response.toString().length(), RFD_MSG_RESPONSE);
    delete response_bytes;
  }
}

void RFDComponent::readSubs()
{
  int timeout = 1 / portTICK_PERIOD_MS;

  bool has_gps = false;
  if (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
  {
    has_gps = true;
    _rfd_data.latitude = _gps_data.lat_long[0];
    _rfd_data.longitude = _gps_data.lat_long[1];
    _rfd_data.altitude_gps = _gps_data.altitude;
    _rfd_data.pdop = _gps_data.p_dilution_precision;
    _rfd_data.hour = _gps_data.time_ymd_hms[3];
    _rfd_data.minute = _gps_data.time_ymd_hms[4];
    _rfd_data.second = _gps_data.time_ymd_hms[5];
  }

  // Don't bother sending data if there is no gps data.
  if (!has_gps)
    return;

  while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, timeout) == pdPASS)
  {
    _rfd_data.pressure_hpa = _baro_data.pressure_pa;
    _rfd_data.altitude_baro = _baro_data.alt_above_sea_m;
    _rfd_data.baro_temp = _baro_data.temperature_c;
  }

  size_t data_bytes_size;
  uint8_t *data_bytes = RFD::toByteArray<RFD::RFDRemoteData>(_rfd_data, data_bytes_size);
  size_t sent_size = sendBytes(data_bytes, data_bytes_size, RFD_MSG_GPS_DATA);

  delete data_bytes;
  printf("Sent Size RFD: %d\n", sent_size);
}

// I would like to add some header and footer to the packet we are sending
size_t RFDComponent::sendBytes(uint8_t *data, size_t data_size, umsg_RFD_msg_type_t msg_type)
{
  uint8_t packet_size = data_size + 4;
  uint8_t *packet = new uint8_t[packet_size];
  packet[0] = (uint8_t)'S';                             // 'S' for start
  packet[1] = msg_type == RFD_MSG_GPS_DATA ? 'D' : 'M'; // 'D' for data; 'M' for message
  packet[2] = packet_size;
  for (int i = 0; i < data_size; i++)
  {
    packet[i + 3] = data[i];
  }
  packet[data_size + 3] = (uint8_t)'E'; // 'E' for end

  i2cuart.writeBytes(SC16IS752_CHANNEL_A, packet, packet_size);
  delete packet;
  return packet_size;
}
