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
#include "esp_vfs_fat.h"
#include "ComBus.h"


#define MOUNT1    CONFIG_SD1_MOUNT
#define MOUNT2    CONFIG_SD2_MOUNT

#define LOGFILE   "javelin.log"

#if CONFIG_SD2_ATTACHED == true
#define SD2_ATTACHED
#endif

#define SPEED_LOG

#define SECTOR_SIZE 512
#define NUM_SECTORS 8
#define WRITE_BLK_SIZE SECTOR_SIZE * NUM_SECTORS


class SDData{
public:
  SDData(){
    file_name = NULL;
    message = NULL;
  }
  ~SDData(){
    if(file_name != NULL){
      delete file_name;
    }
    if(message != NULL){
      delete message;
    }
  }
  std::string *file_name;
  std::string *message;
};

class DataLogger {
public:
  DataLogger(QueueHandle_t data_out){
    _data_out = data_out;
  }
  ~DataLogger(){
    delete _dataOutBuf;
  }
  void setup();
  static void vLogLoop_Task(void* data_logger);
private:

  esp_err_t appendFile(std::string path, std::string message);
  esp_err_t writeFile(std::string path, std::string message);
  esp_err_t modifyFile(std::string path, std::string message, std::string open_mode);

  void mountSDFileSystem(sdspi_device_config_t sd_conf, const char* mount_point, uint8_t card_num);
  void initSPIBus();

  std::string getPath1(){ return _path1; }
  std::string getPath2(){ return _path2; }

  std::string _path1;
  std::string _path2;

  bool _sd1_connected;
  bool _sd2_connected;


  // Configurations to allow for reaquisition of SD cards
  esp_vfs_fat_sdmmc_mount_config_t _mount_config;
  sdmmc_host_t                     _host;
  sdspi_device_config_t            _onboard_sd_conf;
  sdspi_device_config_t            _external_sd_conf;

  void handleQueueData();
  std::string *_dataOutBuf;
  QueueHandle_t _data_out;
  sdmmc_card_t *_card1;
  sdmmc_card_t *_card2;
  
};


#endif /* __DATA_LOGGER_H__ */
