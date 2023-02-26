/*********************************************************************************
 *     File Name           :    /DataLogger/include/datalogger.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-06 20:25]
 *     Last Modified       :     [2022-10-27 01:49]
 *     Description         :
 **********************************************************************************/

#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>
#include "esp_vfs_fat.h"
#include "ComBus.h"
#include "dataline.h"
#include "umsg_toDataLine.h"
extern "C"
{
#include "umsg_CommandCenter.h"
#include "umsg_GPS.h"
#include "umsg_Iridium.h"
#include "umsg_LoRa.h"
#include "umsg_Sensors.h"
#include "umsg_Stats.h"
#include "umsg_StatusMsgs.h"
}

#define MOUNT1 CONFIG_SD1_MOUNT
#define MOUNT2 CONFIG_SD2_MOUNT

#if CONFIG_SD2_ATTACHED == true
#define SD2_ATTACHED
#endif

#define SPEED_LOG

#define LOGFILE "javelin2.log"

#define SECTOR_SIZE 512
#define NUM_SECTORS 8
#define WRITE_BLK_SIZE SECTOR_SIZE *NUM_SECTORS

class DataLogger
{
public:
  void setup();
  static void vLogLoop_Task(void *data_logger);

private:
  esp_err_t appendFile(std::string path, std::string message);
  esp_err_t writeFile(std::string path, std::string message);
  esp_err_t modifyFile(std::string path, std::string message, std::string open_mode);

  void mountSDFileSystem(sdspi_device_config_t sd_conf, const char *mount_point, uint8_t card_num);
  void initSPIBus();

  std::vector<DataLine> _datalines;
  void initSubs();
  void readSubs();
  bool receiveMsgs();

  std::string getPath1() { return _path1; }
  std::string getPath2() { return _path2; }

  std::string _path1;
  std::string _path2;

  bool _sd1_connected;
  bool _sd2_connected;

  // Configurations to allow for reaquisition of SD cards
  esp_vfs_fat_sdmmc_mount_config_t _mount_config;
  sdmmc_host_t _host;
  sdspi_device_config_t _onboard_sd_conf;
  sdspi_device_config_t _external_sd_conf;

  void handleQueueData();
  void log_to_sd();

  sdmmc_card_t *_card1;
  sdmmc_card_t *_card2;

  // Sub Handles

  umsg_sub_handle_t _commandCenter_command_sub;
  umsg_CommandCenter_command_t _commandCenter_command_data;

  umsg_sub_handle_t _gps_data_sub;
  umsg_sub_handle_t _gps_configuration_sub;
  umsg_sub_handle_t _gps_state_sub;
  umsg_GPS_configuration_t _gps_configuration_data;
  umsg_GPS_data_t _gps_data;
  umsg_GPS_state_t _gps_state_data;

  umsg_sub_handle_t _iridium_received_msg_sub;
  umsg_sub_handle_t _iridium_sent_msg_sub;
  umsg_sub_handle_t _iridium_state_sub;
  umsg_Iridium_received_msg_t _iridium_received_data;
  umsg_Iridium_sent_msg_t _iridium_sent_data;
  umsg_Iridium_state_t _iridium_state_data;

  umsg_sub_handle_t _LoRa_received_msg_sub;
  umsg_sub_handle_t _LoRa_sent_msg_sub;
  umsg_sub_handle_t _LoRa_state_msg_sub;
  umsg_LoRa_received_msg_t _LoRa_received_data;
  umsg_LoRa_sent_msg_t _LoRa_sent_data;
  umsg_LoRa_state_msg_t _LoRa_state_data;

  umsg_sub_handle_t _imu_configuration_sub;
  umsg_sub_handle_t _imu_data_sub;
  umsg_sub_handle_t _imu_state_sub;
  umsg_Sensors_imu_configuration_t _imu_configuration_data;
  umsg_Sensors_imu_data_t _imu_data;
  umsg_Sensors_imu_state_t _imu_state_data;

  umsg_sub_handle_t _baro_configuration_sub;
  umsg_sub_handle_t _baro_data_sub;
  umsg_sub_handle_t _baro_state_sub;
  umsg_Sensors_baro_configuration_t _baro_configuration_data;
  umsg_Sensors_baro_data_t _baro_data;
  umsg_Sensors_baro_state_t _baro_state_data;

  umsg_sub_handle_t _therm_0_config_sub;
  umsg_sub_handle_t _therm_0_data_sub;
  umsg_sub_handle_t _therm_0_state_sub;
  umsg_sub_handle_t _therm_1_config_sub;
  umsg_sub_handle_t _therm_1_data_sub;
  umsg_sub_handle_t _therm_1_state_sub;
  umsg_Sensors_thermistor_configuration_t _therm_0_config_data;
  umsg_Sensors_thermistor_configuration_t _therm_1_config_data;
  umsg_Sensors_thermistor_state_t _therm_0_state_data;
  umsg_Sensors_thermistor_state_t _therm_1_state_data;
  umsg_Sensors_thermistor_data_t _therm_0_data;
  umsg_Sensors_thermistor_data_t _therm_1_data;

  umsg_sub_handle_t _stats_system_run_time_sub;
  umsg_sub_handle_t _stats_task_run_time_sub;
  umsg_Stats_system_run_time_stats_t _system_run_time_data;
  umsg_Stats_task_run_time_stats_t _task_run_time_data;
};

#endif /* __DATA_LOGGER_H__ */
