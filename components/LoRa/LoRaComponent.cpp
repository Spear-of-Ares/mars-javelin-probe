/*********************************************************************************
 *     File Name           :     /components/lora/lora_commlogic.cpp
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-18 20:25]
 *     Last Modified       :     [2022-11-06 02:38]
 *     Description         :     Communication logic for LoRa modules
 **********************************************************************************/
#include "LoRaComponent.h"

namespace LoRaData
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

void LoRaComponent::initSubs()
{
  _gps_data_sub = umsg_GPS_data_subscribe(1, 5);

  _imu_data_sub = umsg_Sensors_imu_data_subscribe(10, 10);

  _baro_data_sub = umsg_Sensors_baro_data_subscribe(10, 10);

  // Different channels still use the same message count #.
  // Different prescalar values to need to be used to get messages.
  // Stream: AB AB AB AB AB AB AB AB AB AB
  //         12 34 56 78 90 12 34 56 78 90
  // To get the n-th message from m channels, do N*M = last channel prescaler
  // and subtract 1 each time for previous channels
  _therm_0_data_sub = umsg_Sensors_thermistor_data_subscribe_ch(79, 3, 0);
  _therm_1_data_sub = umsg_Sensors_thermistor_data_subscribe_ch(80, 3, 1);
}

void LoRaComponent::vMainLoop_Task(void *arg)
{
  LoRaComponent lora_component = *((LoRaComponent *)arg);
  if (!lora_component.setup())
  {
    vTaskDelete(NULL);
  }

  for (;;)
  {
    // Sleeping happens within readSubs and TX, therefore don't sleep here.
    lora_component.vRX();
    lora_component.readSubs();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Assumes that initLoRaSPI() has been called from the ComBus component
bool LoRaComponent::setup()
{
  initSubs();
  LoRa.setSPIFrequency(1E6);

  if (!LoRa.begin(915E6))
  {
    printf("Starting LoRa failed!\n");
    return false;
  }

  LoRa.enableCrc();

  return true;
}

void LoRaComponent::readSubs()
{
  int timeout = 1 / portTICK_PERIOD_MS;
  LoRaData::LoRaRemoteData data;

  // Peek first, then receive so we don't have to wait for timeout if there is no data.
  while (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
  {
    data.latitude = _gps_data.lat_long[0];
    data.longitude = _gps_data.lat_long[1];
    data.altitude_gps = _gps_data.altitude;
    data.pdop = _gps_data.p_dilution_precision;
    data.hour = _gps_data.time_ymd_hms[3];
    data.minute = _gps_data.time_ymd_hms[4];
    data.second = _gps_data.time_ymd_hms[5];
  }

  while (umsg_Sensors_imu_data_receive(_imu_data_sub, &_imu_data, timeout) == pdPASS)
  {
    data.attitude_x = _imu_data.attitude[0];
    data.attitude_y = _imu_data.attitude[1];
    data.attitude_z = _imu_data.attitude[2];
    data.linear_accel_x = _imu_data.linear_accel[0];
    data.linear_accel_y = _imu_data.linear_accel[1];
    data.linear_accel_z = _imu_data.linear_accel[2];
    data.velocity_x = _imu_data.velocity[0];
    data.velocity_y = _imu_data.velocity[1];
    data.velocity_z = _imu_data.velocity[2];
    data.position_x = _imu_data.position[0];
    data.position_y = _imu_data.position[1];
    data.position_z = _imu_data.position[2];
    data.vertical_speed = _imu_data.vertical_speed;
    data.horizontal_speed = _imu_data.horizontal_speed;
    data.angle_of_attack = _imu_data.angle_of_attack;
    data.imu_temp = _imu_data.temperature_c;
  }

  while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, timeout) == pdPASS)
  {
    data.pressure_hpa = _baro_data.pressure_pa;
    data.altitude_baro = _baro_data.alt_above_sea_m;
    data.baro_temp = _baro_data.temperature_c;
  }

  while (umsg_Sensors_thermistor_data_receive(_therm_0_data_sub, &_therm_0_data, timeout) == pdPASS)
  {
    data.internal_temp = _therm_0_data.temperature_c;
  }

  while (umsg_Sensors_thermistor_data_receive(_therm_1_data_sub, &_therm_1_data, timeout) == pdPASS)
  {
    data.external_temp = _therm_1_data.temperature_c;
  }

  size_t data_bytes_size;
  uint8_t *data_bytes = LoRaData::toByteArray<LoRaData::LoRaRemoteData>(data, data_bytes_size);
  vTX((uint8_t *)data_bytes, data_bytes_size, LORA_MSG_SENSOR_DATA);
}

void LoRaComponent::vTX(uint8_t *msg, size_t size, umsg_LoRa_msg_type_t msg_type)
{

  uint8_t packet_size = size + 4;
  uint8_t *packet = new uint8_t[packet_size];
  packet[0] = (uint8_t)'S';
  packet[1] = (uint8_t)'T';
  packet[1] = packet_size;
  for (int i = 0; i < size; i++)
  {
    packet[i + 3] = msg[i];
  }
  packet[size + 3] = (uint8_t)'E';

  while (LoRa.beginPacket() == 0)
  {
    vRX();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }

  umsg_LoRa_sent_msg_t sent_msg;
  sent_msg.sent_msg_type = msg_type;
  LoRa.beginPacket();
  LoRa.write(packet, packet_size);
  LoRa.endPacket(true); // Async send
  sent_msg.send_tick = xTaskGetTickCount();

  umsg_LoRa_sent_msg_publish(&sent_msg);
}

void LoRaComponent::vTX(std::string msg, umsg_LoRa_msg_type_t msg_type)
{
  vTX((uint8_t *)msg.c_str(), msg.length(), msg_type);
}

void LoRaComponent::vRX()
{

  int packetSize = LoRa.parsePacket();
  std::string empty = "";
  if (packetSize)
  {
    umsg_LoRa_received_msg_t recv_msg;
    recv_msg.receive_tick = xTaskGetTickCount();
    recv_msg.RSSI = LoRa.packetRssi();
    recv_msg.signal_to_noise = LoRa.packetSnr();
    recv_msg.frequency_error = LoRa.packetFrequencyError();

    std::string received("");

    while (LoRa.available())
    {
      received += (char)LoRa.read();
    }

    // Copy received data into message to be passed to other tasks
    int i;
    for (i = 0; i < sizeof(recv_msg.msg_data) && i < received.length(); i++)
    {
      recv_msg.msg_data[i] = received[i];
    }

    // Make sure msg_data ends with 0x00
    if (i < sizeof(recv_msg.msg_data))
    {
      recv_msg.msg_data[i] = 0x00;
    }

    DataLine response;
    response.name = "RESPONSE";
    response.recorded_tick = recv_msg.receive_tick;

    if (received[0] == 0x01)
    {
      response.data.push_back("Command");
      recv_msg.recv_msg_type = LORA_MSG_COMMAND;
    }
    else
    {
      response.data.push_back("Message");
      recv_msg.recv_msg_type = LORA_MSG_OTHER;
    }

    response.data.push_back("Received");
    umsg_LoRa_received_msg_publish(&recv_msg);

    vTX(response.toString(), LORA_MSG_RESPONSE);
  }
}
