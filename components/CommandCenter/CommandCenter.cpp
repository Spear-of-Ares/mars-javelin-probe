/*********************************************************************************
 *     File Name           :     /components/CommandCenter/CommandCenter.cpp
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-26 00:34]
 *     Last Modified       :     [2022-10-28 00:50]
 *     Description         :     Component to control messages from LoRa and Iridium SBD
 **********************************************************************************/

#include "CommandCenter.h"

void CommandComponent::vMainLoop_Task(void *arg)
{
  CommandComponent cmd_component = *((CommandComponent *)arg);
  const TickType_t xMaxBlockTime = 500 / portTICK_PERIOD_MS;
  cmd_component.initSubs();

  for (;;)
  {
    cmd_component.command_check();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void CommandComponent::initSubs()
{
  // Every 30 seconds at 1hz
  _gps_data_sub = umsg_GPS_data_subscribe(30, 1);

  // Every 3 seconds at 10hz
  _baro_data_sub = umsg_Sensors_baro_data_subscribe(30, 10);

  _iridium_data_sub = umsg_Iridium_received_msg_subscribe(1, 5);

  _lora_data_sub = umsg_LoRa_received_msg_subscribe(1, 10);
}

void CommandComponent::handleCommands(uint8_t cmd_data[512])
{
  int i;
  printf("Command: ");
  for (i = 0; i < 512 && cmd_data[i] != 0x00; i++)
  {
    printf("0x%02x ", cmd_data[i]);
  }
  printf("\n");
  if (cmd_data[1] == 0x01)
  {
    cutdown();
    return;
  }

  // All messages end in 0x00
}

void CommandComponent::command_check()
{
  int timeout = 1 / portTICK_PERIOD_MS; // 1 ms timeout
  if (umsg_GPS_data_peek(&_gps_data))
  {
    while (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
    {
      if (altitude_cutdown_check())
      {
        cutdown();
      }
    }
  }

  if (umsg_Iridium_received_msg_peek(&_iridium_data))
  {
    while (umsg_Iridium_received_msg_receive(_iridium_data_sub, &_iridium_data, timeout) == pdPASS)
    {
      if (_iridium_data.recv_msg_type == IRIDIUM_MSG_COMMAND)
      {
        handleCommands(_iridium_data.msg_data);
      }
    }
  }

  if (umsg_LoRa_received_msg_peek(&_lora_data))
  {
    while (umsg_LoRa_received_msg_receive(_lora_data_sub, &_lora_data, timeout) == pdPASS)
    {
      if (_lora_data.recv_msg_type == LORA_MSG_COMMAND)
      {
        handleCommands(_lora_data.msg_data);
      }
    }
  }
}

bool CommandComponent::altitude_cutdown_check()
{
  if (_gps_data.altitude == 0)
  {
    // Average alt over 10 samples to reduce likelyhood of random sample causing cutdown.

    float alts[10];
    int i = 0;
    if (umsg_Sensors_baro_data_peek(&_baro_data))
    {
      while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, 1 / portTICK_PERIOD_MS) == pdPASS)
      {
        alts[i] = _baro_data.alt_above_sea_m;
        i++;
      }
    }

    float sum = 0;
    for (int i = 0; i < sizeof(alts); i++)
    {
      sum += alts[i];
    }
    float avg = sum / sizeof(alts);

    if (avg > MAX_ALTITUDE)
    {
      printf("Baro cutdown: %f\n", avg);
      return true;
    }
  }

  // Do not attempt cutdown if GPS accuracy is bad (ie above PDOP of 7.0)
  if ((float)(_gps_data.p_dilution_precision / 100.0) > 7.0)
  {
    return false;
  }

  // Convert gps_data mm to m and make sure position is precise enough
  if (_gps_data.altitude / 1000 > MAX_ALTITUDE)
  {
    _high_alt_readings += 1;
    if (_high_alt_readings > HIGH_READINGS)
    {
      printf("Altitude cutdown: %d\n", _gps_data.altitude / 1000);
      return true;
    }
  }

  // Reset if we get a valid position that is not above max altitude
  _high_alt_readings = 0;
  return false;
}

void CommandComponent::cutdown()
{
  umsg_CommandCenter_command_t _cmd_data;

  _cmd_data.cutdown = 1;

  gpio_set_level(CUT_DWN_GPIO, 1);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  gpio_set_level(CUT_DWN_GPIO, 0);

  umsg_CommandCenter_command_publish(&_cmd_data);
}

void CommandComponent::selftest()
{
  umsg_CommandCenter_command_t _cmd_data;

  _cmd_data.self_test = 1;

  umsg_CommandCenter_command_publish(&_cmd_data);
}
