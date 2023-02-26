/*********************************************************************************
 *     File Name           :     /components/lora/lora_commlogic.cpp
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-18 20:25]
 *     Last Modified       :     [2022-11-06 02:38]
 *     Description         :     Communication logic for LoRa modules
 **********************************************************************************/
#include "LoRaComponent.h"

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
    lora_component.vRX();
    lora_component.readSubs();
    vTaskDelay(500);
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
  return true;
}

void LoRaComponent::readSubs()
{
  int timeout = 1 / portTICK_PERIOD_MS;

  // Peek first, then receive so we don't have to wait for timeout if there is no data.
  while (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
  {
    _datalines.push_back(std::make_pair(GPS_data_toDataLine(_gps_data), LORA_MSG_GPS_DATA));
  }

  while (umsg_Sensors_imu_data_receive(_imu_data_sub, &_imu_data, timeout) == pdPASS)
  {
    _datalines.push_back(std::make_pair(imu_data_toDataLine(_imu_data), LORA_MSG_SENSOR_DATA));
  }

  while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, timeout) == pdPASS)
  {
    _datalines.push_back(std::make_pair(baro_data_toDataLine(_baro_data), LORA_MSG_SENSOR_DATA));
  }

  std::string therm_0_name = "INT";
  std::string therm_1_name = "EXT";
  while (umsg_Sensors_thermistor_data_receive(_therm_0_data_sub, &_therm_0_data, timeout) == pdPASS)
  {
    _datalines.push_back(std::make_pair(therm_data_toDataLine(_therm_0_data, therm_0_name), LORA_MSG_SENSOR_DATA));
  }
  while (umsg_Sensors_thermistor_data_receive(_therm_1_data_sub, &_therm_1_data, timeout) == pdPASS)
  {
    _datalines.push_back(std::make_pair(therm_data_toDataLine(_therm_1_data, therm_1_name), LORA_MSG_SENSOR_DATA));
  }

  // Sort all entries by the time at which they were recorded
  std::sort(_datalines.begin(), _datalines.end(), [](const std::pair<DataLine, umsg_LoRa_msg_type_t> &data1, const std::pair<DataLine, umsg_LoRa_msg_type_t> &data2)
            { return data1.first.recorded_tick < data2.first.recorded_tick; });

  for (int i = 0; i < _datalines.size(); i++)
  {
    vTX(_datalines[i].first.toString(), _datalines[i].second);
    // Delay to allow LoRa to actually send packets.
    vTaskDelay(50 / portTICK_PERIOD_MS);
    // Many failures because we aren't receiving packets. Attempt to check for reception here
    // increases the chance that one goes through. Having another LoRa board would alleiviate this.
    vRX();
  }

  _datalines.clear();
}

void LoRaComponent::vTX(std::string msg, umsg_LoRa_msg_type_t msg_type)
{
  umsg_LoRa_sent_msg_t sent_msg;
  sent_msg.sent_msg_type = msg_type;

  LoRa.beginPacket();
  LoRa.print(msg.c_str());
  LoRa.endPacket();
  sent_msg.send_tick = xTaskGetTickCount();

  umsg_LoRa_sent_msg_publish(&sent_msg);
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
