/*********************************************************************************
 *     File Name           :     /components/CommandCenter/include/CommandCenter.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-26 00:29]
 *     Last Modified       :     [2022-10-28 01:34]
 *     Description         :     Component to control messages from LoRa and Iridium SBD
 **********************************************************************************/

#ifndef __COMMAND_CENTER_H__
#define __COMMAND_CENTER_H__
#include <stdio.h>
#include <sstream>
#include "CommandDefs.h"
#include "datalogger.h"

extern "C"
{
#include "umsg_GPS.h"
#include "umsg_Sensors.h"
#include "umsg_CommandCenter.h"
#include "umsg_LoRa.h"
#include "umsg_Iridium.h"
}
#include "umsg_toDataLine.h"

#include "driver/gpio.h"

#define MAX_ALTITUDE 30000
#define HIGH_READINGS 5

#define CUT_DWN_GPIO GPIO_NUM_4
#define CUT_DWN_DUR CONFIG_CUT_DWN_DUR

#define CMDCNTR_TASK_ID "CMD CENTER"

class CommandComponent
{
public:
  CommandComponent()
  {
    gpio_set_direction(CUT_DWN_GPIO, GPIO_MODE_OUTPUT);
  }
  static void vMainLoop_Task(void *arg);

private:
  void initSubs();
  void command_check();
  void handleCommands(uint8_t cmd_data[512]);
  bool altitude_cutdown_check();
  void cutdown();
  void selftest();

  uint8_t _high_alt_readings;

  umsg_sub_handle_t _gps_data_sub;
  umsg_GPS_data_t _gps_data;

  umsg_sub_handle_t _baro_data_sub;
  umsg_Sensors_baro_data_t _baro_data;

  umsg_sub_handle_t _iridium_data_sub;
  umsg_Iridium_received_msg_t _iridium_data;

  umsg_sub_handle_t _lora_data_sub;
  umsg_LoRa_received_msg_t _lora_data;
};

#endif
