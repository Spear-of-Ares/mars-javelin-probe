/*********************************************************************************
*     File Name           :    /DataLogger/include/datalogger.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-06 20:25]
*     Last Modified       :     [2022-10-20 23:29]
*     Description         :      
**********************************************************************************/

#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include <string>
#include <cstring>
#include <stdio.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SPI2_MISO CONFIG_SPI2_MISO_GPIO
#define SPI2_MOSI CONFIG_SPI2_MOSI_GPIO
#define SPI2_SCK  CONFIG_SPI2_SCK_GPIO
#define SD1_CS    CONFIG_SD1_CS_GPIO
#define SD2_CS    CONFIG_SD2_CS_GPIO

#define MOUNT1    CONFIG_SD1_MOUNT
#define MOUNT2    CONFIG_SD2_MOUNT

#define LOGFILE   "javelin.log"

#if CONFIG_SD2_ATTACHED == true
#define SD2_ATTACHED
#endif

#define SPEED_LOG

#define SECTOR_SIZE 512

class SDData{
public:
  SDData(std::string file_name, std::string message){
    _file_name = file_name;
    _message = message;
  }
  ~SDData(){}
  std::string getFileName(){ return _file_name; }
  std::string getMessage() { return _message; }
private:
  std::string _file_name;
  std::string _message;
};

class DataLogger {
public:
  DataLogger(QueueHandle_t data_out){
    _data_out = data_out;
  }
  void setup();
  static void vLogLoop_Task(void* data_logger);
private:

  void appendFile(std::string path, std::string message);
  void writeFile(std::string path, std::string message);
  void modifyFile(std::string path, std::string message, std::string open_mode);

  std::string getPath1(){ return _path1; }
  std::string getPath2(){ return _path2; }

  std::string _path1;
  std::string _path2;

  void handleQueueData();
  std::string _dataOutBuf;
  QueueHandle_t _data_out;
  sdmmc_card_t *_card1;
  sdmmc_card_t *_card2;
  
};


void appendFile(fs::FS &fs, const char * path, const char * messgae);
void testFileIO(fs::FS &fs, const char * path);


#endif /* __DATA_LOGGER_H__ */
