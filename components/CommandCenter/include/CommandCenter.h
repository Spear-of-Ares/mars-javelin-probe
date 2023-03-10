/*!
 * @file CommandCenter.h
 * @author Jon Kopf (kopf0033@vandals.uidaho.edu)
 * @brief Gathers data from RDF, LoRa and sensors to determine if cutdown should happen and other important things.
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
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

#define MAX_ALTITUDE 30000 /*! < The maximum altitude that should be flown to in meters. Altitude readings above this will trigger cutdown*/
#define HIGH_READINGS 5    /*! < Number of readings in a row over max_altitude to cause cutdown. This is to avoid errors in baro cutting down*/

#define CUT_DWN_GPIO GPIO_NUM_4
#define CUT_DWN_DUR CONFIG_CUT_DWN_DUR

/*!
 * @brief Component responsible for commands. Holds umsg subscriptions as well as the main task for this
 *        comoponent.
 * 
 */
class CommandComponent
{
public:
  /*!
   * @brief Sets the cutdown gpio to output on component creation
   * 
   */
  CommandComponent()
  {
    gpio_set_direction(CUT_DWN_GPIO, GPIO_MODE_OUTPUT);
  }

  /*!
   * @brief The main task loop. Everything happening within this task starts here
   * 
   *
   * @param arg A reference to a CommandComponent object.
   */
  static void vMainLoop_Task(void *arg);

private:

  /*!
   * @brief Initializes all umsg_sub_handle_t that this class has
   * 
   */
  void initSubs();

  /*!
   * @brief Checks for any new commands
   * 
   */
  void command_check();

  /*!
   * @brief If there is a new command, determine what it is and handle it
   * 
   *
   * @param cmd_data byte data received from either LoRa or RFD.
   */
  void handleCommands(uint8_t cmd_data[512]);

  /*!
   * @brief Check if payload should cut down due to being too high.
   * 
   *
   * @return True if cutown should happen, false otherwise
   */
  bool altitude_cutdown_check();

  /*!
   * @brief Sets the cutdown pin to high, therefore starting cutdown
   * 
   */
  void cutdown();

  /*!
   * @brief Not implemented
   * 
   */
  void selftest();

  uint8_t _high_alt_readings; /*! < Number of High altitude readings*/


  // umsg Subscriptions and data handles.
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
