#include "datalogger.h"

/**
* @brief Handles the unqueueing of data from other tasks and writing that data to SD card
*
* @param data_logger Data logger class, giving access to other functions and fields
*/
void DataLogger::vLogLoop_Task(void* data_logger)
{
  DataLogger *data_log = (DataLogger*)data_logger;
  data_log->_dataOutBuf = new std::string();
  for(;;)
    {
      if(data_log->_data_out == NULL){
        printf("This is null rn dude\n");
        vTaskDelay(1);
      }
      
      while (uxQueueMessagesWaiting(data_log->_data_out) != 0){
        data_log->handleQueueData();
      }
      // Delay so watchdog does not freak out
      vTaskDelay(50);
    }

  vTaskDelete(NULL);
}


/**
* @brief Gets SDData from the dataOut queue and writes to both SD1 and SD2 if data if larger than
*        the internal buffer size.
*/
void DataLogger::handleQueueData(){

  SDData *sd_data;
  if( xQueueReceive(_data_out, &(sd_data), 5/portTICK_PERIOD_MS) != pdTRUE){
    printf("Could not receive from queue\n");
    return; 
  }
  //printf("%d || Data Logger Start || %s || %d\n", xTaskGetTickCount(), sd_data->file_name->c_str(), xPortGetFreeHeapSize());

  std::string sd_data_msg = *(sd_data->message);
  int msg_len = sd_data_msg.length();

  // Can change number of setors to write (*20, or *40 or something) to increase write efficiency
  // Does take longer for buffer to fill though
  int size_left = (SECTOR_SIZE * 1)- (_dataOutBuf->length());
  if (size_left - msg_len < 0)
  {
    // Append to sector size
    _dataOutBuf->append(sd_data_msg, 0, size_left);

    // Fill sd_data_msg with remaining bytes
    sd_data_msg = sd_data_msg.substr(size_left, msg_len-size_left);

    // TODO:: Don't clear bufer if appendFile fails
    // if not connected, attemp to remount
    if (!_sd1_connected){
      printf("Mount\n");
      mountSDFileSystem(_onboard_sd_conf, MOUNT1, 1);
    }
    else{
      esp_err_t ret = appendFile(_path1, *_dataOutBuf);
      if (ret == ESP_FAIL){
        _sd1_connected = false;
      }
    }

#ifdef SD2_ATTACHED
    if ( !_sd2_connected ){
      mountSDFileSystem(_external_sd_conf, MOUNT2, 2);
    }
  else{
      appendFile(_path2, *_dataOutBuf);
      if (ret == ESP_FAIL){
        _sd2_connected = false;
      }
    }
#endif
    printf("     Written to SD\n");
    printf("++++++++++++++++++++++++++\n");
    printf("%s\n\n", _dataOutBuf->c_str());

    // reset data out buf
    _dataOutBuf->clear();
  }
  if (sd_data_msg.length() != 0){
    _dataOutBuf->append(sd_data_msg);
  }
  delete sd_data;
}                                                   


/**
* @brief Appends message to file at path
*
* @param path       Path of file to append to
* @param message    Message to append
*/
esp_err_t DataLogger::appendFile(std::string path, std::string message){
#ifdef SPEED_LOG
  uint32_t start = millis();
#endif
  esp_err_t ret = modifyFile(path, message, "a");
#ifdef SPEED_LOG
  printf("Took %lu ms to append %u B\n", millis() - start, message.length());
#endif
  return ret;
}


/**
* @brief Writes message to file at path
*
* @param path       Path of file to write
* @param message    Message to write
*/
esp_err_t DataLogger::writeFile(std::string path, std::string message){
#ifdef SPEED_LOG
  uint32_t start = millis();
#endif
  esp_err_t ret =modifyFile(path, message, "w");
#ifdef SPEED_LOG
  printf("Took %lu ms to write %u B\n", millis() - start, message.length());
#endif
  return ret;
}


/**
* @brief Modifys a file by either writing or appending
*
* @param path       Path of file to modify
* @param message    Message to add to file
* @param open_mode  Mode to open file in. Accepted values are "w" and "a"
*/
esp_err_t DataLogger::modifyFile(std::string path, std::string message, std::string open_mode){

  FILE *file = fopen(path.c_str(), open_mode.c_str());
  if (!file){
    printf("Failed to open file for writing\n");
    return ESP_FAIL;
  }

  if(fprintf(file, "%s", message.c_str()) < 0){
    printf("Append failed\n");
  }
  fclose(file);

  return ESP_OK;
}

/**
* @brief Setup of the various things needed for SD card writing
*        
*        Initializes an SPI bus using SPI3_HOST and custom SPI ports defined in the
*        Kconfig file.
*
*        Mounts both SD1 and SD2 at MOUNT1 and MOUNT2 locations. 
*/
void DataLogger::setup()
{
  // Init mount points
  _mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
  };
  const char mount_point1[] = MOUNT1;
  const char mount_point2[] = MOUNT2;

  _host = SDHost; // defined in ComBus.h
  
  // Configure SD1 device and mount its filesystem
  _onboard_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  _onboard_sd_conf.host_id = (spi_host_device_t)_host.slot;
  _onboard_sd_conf.gpio_cs = (gpio_num_t)SD1_CS_GPIO;


  printf("Mounting Filesystem 1\n");
  mountSDFileSystem(_onboard_sd_conf, mount_point1, 1);
  printf("Filesystem 1 Mounted\n");


  // Clear the log file
  _path1 = MOUNT1;
  _path1 += "/";
  _path1 += LOGFILE;
  appendFile(_path1, "\n\n SD 1 Mounted\n");

#ifdef SD2_ATTACHED
  // Configure SD2 device and mount its filesystem
  _external_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  _external_sd_conf.host_id = (spi_host_device_t)host.slot;
  _external_sd_conf.gpio_cs = (gpio_num_t)SD2_CS_GPIO;

  printf("Mounting Filesystem 2\n");
  mountSDFileSystem(_external_sd_conf, mount_point2, 2)
  printf("Filesystem 2 Mounted\n");

  // Clear the log file
  _path2 = MOUNT2;
  _path2 += "/";
  _path2 += LOGFILE;
  appendFile(_path1, "\n\n SD 2 Mounted\n");

#endif /* SD2_ATTACHED */
}


/**
  * @brief Mounts the file system of an SD card
  *
  * @param mount_point place to mount the SD card at
  */
void DataLogger::mountSDFileSystem(sdspi_device_config_t sd_conf, const char* mount_point, uint8_t card_num){
  sdmmc_card_t *card;
  bool *connected;
   
  switch(card_num){
    case 1:
      card = _card1;
      connected = &_sd1_connected;
      break;
    case 2:
      card = _card2;
      connected = &_sd2_connected;
      break;
    default:
      printf("Not a valid card number. card in range [0, 1]");
      return;
  }

  esp_err_t ret = esp_vfs_fat_sdspi_mount(mount_point, &_host, &sd_conf, &_mount_config, &card);

  *connected = true;
  if (ret != ESP_OK){
    if (ret == ESP_FAIL){
      printf("Failed to mount filesystem.");
    }
  else{
      printf("Failed to initialize the card. Make sure the SD card lines have pull up resistors in place.");
      *connected = false;
    }
    return;
  }

  printf("Mounted Card:\n");
  sdmmc_card_print_info(stdout, card);
  printf("\n");
}

