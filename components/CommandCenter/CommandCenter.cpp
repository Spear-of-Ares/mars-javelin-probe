/*!
 * @file CommandCenter.cpp
 * @author Jon Kopf (kopf0033@vandals.uidaho.edu)
 * @brief Implmentation of CommandCenter.h
 * @version 0.1
 * @date 2023-03-09
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "CommandCenter.h"

/*!
 * @brief The main task loop. Everything happening within this task starts here
 *
 *
 * @param arg A reference to a CommandComponent object.
 */
void CommandComponent::vMainLoop_Task(void *arg)
{
  CommandComponent cmd_component = *((CommandComponent *)arg);
  const TickType_t xMaxBlockTime = 500 / portTICK_PERIOD_MS;
  cmd_component.initSubs();
  // Infinite loop, will quit when microcontroller dies
  for (;;)
  {

    if (!cmd_component.start_system)
    {
      cmd_component.start_system = cmd_component.check_start_system();
      cmd_component.send_start_system();
    }
    cmd_component.command_check();
    // 1 second delay. Don't want to wait too long after getting a
    // command to actually perform the command.

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

/*!
 * @brief Initializes all umsg_sub_handle_t that this class has
 *
 */
void CommandComponent::initSubs()
{
  //// Data Subs
  // Every 30 seconds at 1hz
  _gps_data_sub = umsg_GPS_data_subscribe(30, 1);

  // Every 3 seconds at 10hz
  _baro_data_sub = umsg_Sensors_baro_data_subscribe(30, 10);

  // Look at every message received from RDF and lora
  _rfd_data_sub = umsg_RFD_received_msg_subscribe(1, 5);

  _lora_data_sub = umsg_LoRa_received_msg_subscribe(1, 10);

  //// State Subs
  _gps_state_sub = umsg_GPS_state_subscribe(1, 5);
  _baro_state_sub = umsg_Sensors_baro_state_subscribe(1, 5);
  _imu_state_sub = umsg_Sensors_imu_state_subscribe(1, 5);
  _accel_state_sub = umsg_Sensors_accel_state_subscribe(1, 5);
  _therm_0_state_sub = umsg_Sensors_thermistor_state_subscribe_ch(1, 5, 0);
  _therm_1_state_sub = umsg_Sensors_thermistor_state_subscribe_ch(1, 5, 1);
  _rfd_state_sub = umsg_RFD_state_subscribe(1, 5);
  _lora_state_sub = umsg_LoRa_state_msg_subscribe(1, 5);
}

/*!
 * @brief Checks for any new commands
 *
 */
void CommandComponent::command_check()
{
  int timeout = 1 / portTICK_PERIOD_MS; // 1 ms timeout

  // Look for data from gps
  if (umsg_GPS_data_peek(&_gps_data))
  {
    // For every bit of gps data, check if cutdown should happen
    while (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
    {
      if (altitude_cutdown_check())
      {
        cutdown();
      }
    }
  }

  // Read messages from RFD and LoRa. If they are commands, do something
  if (umsg_RFD_received_msg_peek(&_rfd_data))
  {
    while (umsg_RFD_received_msg_receive(_rfd_data_sub, &_rfd_data, timeout) == pdPASS)
    {
      if (_rfd_data.recv_msg_type == RFD_MSG_COMMAND)
      {
        handleCommands(_rfd_data.msg_data);
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

#define LOG_INITIALIZATION

/*!
 * @brief Checks if all other components have been initialized and are ready to start running.
 *
 */
bool CommandComponent::check_start_system()
{
  bool begin = true;

  static bool gps_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_GPS_state_t>(_gps_state_sub, &_gps_state,
                                                       umsg_GPS_state_peek,
                                                       umsg_GPS_state_receive))

  {
    gps_initialized = (bool)_gps_state.initialized; // value of 1 is true, 0 is false
  }

  begin &= gps_initialized;

  static bool baro_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_Sensors_baro_state_t>(_baro_state_sub, &_baro_state,
                                                                umsg_Sensors_baro_state_peek,
                                                                umsg_Sensors_baro_state_receive))
  {
    baro_initialized = (bool)_baro_state.initialized;
  }
  begin &= baro_initialized;

  static bool imu_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_Sensors_imu_state_t>(_imu_state_sub, &_imu_state,
                                                               umsg_Sensors_imu_state_peek,
                                                               umsg_Sensors_imu_state_receive))
  {
    imu_initialized = (bool)_imu_state.initialized;
  }
  begin &= imu_initialized;

  static bool accel_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_Sensors_accel_state_t>(_accel_state_sub, &_accel_state,
                                                                 umsg_Sensors_accel_state_peek,
                                                                 umsg_Sensors_accel_state_receive))
  {
    accel_initialized = (bool)_accel_state.initialized;
  }
  begin &= accel_initialized;

  static bool therm_0_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_Sensors_thermistor_state_t>(_therm_0_state_sub, &_therm_0_state,
                                                                      umsg_Sensors_thermistor_state_peek,
                                                                      umsg_Sensors_thermistor_state_receive))
  {
    therm_0_initialized = (bool)_therm_0_state.initialized;
  }
  begin &= therm_0_initialized;

  static bool therm_1_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_Sensors_thermistor_state_t>(_therm_1_state_sub, &_therm_1_state,
                                                                      umsg_Sensors_thermistor_state_peek,
                                                                      umsg_Sensors_thermistor_state_receive))
  {
    therm_1_initialized = (bool)_therm_1_state.initialized;
  }
  begin &= therm_1_initialized;

  static bool rfd_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_RFD_state_t>(_rfd_state_sub, &_rfd_state,
                                                       umsg_RFD_state_peek,
                                                       umsg_RFD_state_receive))
  {
    rfd_initialized = (bool)_rfd_state.initialized;
  }
  begin &= rfd_initialized;

  static bool lora_initialized = false;
  while (umsg_helpers::umsg_get_data<umsg_LoRa_state_msg_t>(_lora_state_sub, &_lora_state,
                                                            umsg_LoRa_state_msg_peek,
                                                            umsg_LoRa_state_msg_receive))
  {
    lora_initialized = (bool)_lora_state.initialized;
  }
  begin &= lora_initialized;

#ifdef LOG_INITIALIZATION
  printf("Initializations:\n");
  printf("GPS: %d\t", gps_initialized);
  printf("BARO: %d\t", baro_initialized);
  printf("IMU: %d\t", imu_initialized);
  printf("ACCEL: %d\n", accel_initialized);
  printf("THERM_0: %d\t", therm_0_initialized);
  printf("THERM_1: %d\t", therm_1_initialized);
  printf("RFD: %d\t", rfd_initialized);
  printf("LORA: %d\n", lora_initialized);
#endif

  return begin;
}

/*!
 * @brief Sends a message with the start system bit set to the integer value of start_system variable
 *
 */
void CommandComponent::send_start_system()
{
  umsg_CommandCenter_command_t command;
  command.start_system = 1;
  command.measure_tick = xTaskGetTickCount();

  umsg_CommandCenter_command_publish(&command);
}

/*!
 * @brief If there is a new command, determine what it is and handle it
 *
 *
 * @param cmd_data byte data received from either LoRa or RFD.
 */
void CommandComponent::handleCommands(uint8_t cmd_data[512])
{
  int i;
  const int cutdown_index = 2;
  printf("Command: ");
  for (i = 0; i < 512 && cmd_data[i] != 0x00; i++)
  {
    printf("0x%02x ", cmd_data[i]);
  }
  printf("\n");
  if (cmd_data[cutdown_index] == 0x01)
  {
    cutdown();
    return;
  }

  // All messages end in 0x00
}

/*!
 * @brief Check if payload should cut down due to being too high.
 *
 *
 * @return True if cutown should happen, false otherwise
 */
bool CommandComponent::altitude_cutdown_check()
{

  // If there is no good gps altitude
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
