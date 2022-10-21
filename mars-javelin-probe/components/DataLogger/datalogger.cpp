#include "datalogger.h"

void DataLogger::vLogLoop_Task(void* data_logger)
{
  DataLogger *data_log = (DataLogger*)data_logger;
  for(;;)
    {
      while (uxQueueMessagesWaiting(data_log->_data_out) != 0){
        data_log->handleQueueData();
      }
      // Delay so watchdog does not freak out
      vTaskDelay(1);
    }

  vTaskDelete(NULL);
}

// Returns the number of bytes received
void DataLogger::handleQueueData(){

  SDData *sd_data;
  if( xQueueReceive(_data_out, &(sd_data), 5/portTICK_PERIOD_MS) != pdTRUE){
    printf("Could not receive from queue\n");
    return; 
  }

  std::string sd_data_msg = sd_data->getMessage();
  int msg_len = sd_data_msg.length();
  // Can change number of setors to write (*20, or *40 or something) to increase write efficiency
  // Does take longer for buffer to fill though
  int size_left = (SECTOR_SIZE * 1)- (_dataOutBuf.length());
  if (size_left - msg_len<= 0)
  {
    // Append to sector size
    _dataOutBuf.append(sd_data_msg, 0, size_left);

    // Fill sd_data_msg with remaining bytes
    sd_data_msg = sd_data_msg.substr(size_left+1, msg_len - size_left);

    // TODO:: Don't clear bufer if appendFile fails
    appendFile(_path1, _dataOutBuf);
#ifdef SD2_ATTACHED
    appendFile(_path2, _dataOutBuf);
#endif

    // reset data out buf
    _dataOutBuf = "";
  }
  if (sd_data_msg.length() != 0){
    _dataOutBuf.append(sd_data_msg);
  }
}

void DataLogger::appendFile(std::string path, std::string message){
#ifdef SPEED_LOG
  uint32_t start = millis();
#endif
  printf("Appending to file: %s\n", path.c_str());
  modifyFile(path, message, "a");
#ifdef SPEED_LOG
  printf("Took %lu ms to append %u B\n", millis() - start, message.length());
#endif
}

void DataLogger::writeFile(std::string path, std::string message){
#ifdef SPEED_LOG
  uint32_t start = millis();
#endif
  printf("Writing to file: %s\n", path.c_str());
  modifyFile(path, message, "w");
#ifdef SPEED_LOG
  printf("Took %lu ms to write %u B\n", millis() - start, message.length());
#endif
}

void DataLogger::modifyFile(std::string path, std::string message, std::string open_mode){

  FILE *file = fopen(path.c_str(), open_mode.c_str());
  if (!file){
    printf("Failed to open file for writing\n");
    return;
  }

  if(fprintf(file, "%s", message.c_str()) >= 0){
    printf("Message written\n");
  }
  else{
    printf("Append failed\n");
  }
  fclose(file);
}


void DataLogger::setup()
{
  // Init mount points
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
  };
  const char mount_point1[] = MOUNT1;
  const char mount_point2[] = MOUNT2;

  // Host config. Use SPI3_HOST as LoRa is on SPI2_HOST
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI3_HOST;

  // Init spi bus pins
  spi_bus_config_t spi_conf = {
    .mosi_io_num = SPI2_MOSI,
    .miso_io_num = SPI2_MISO,
    .sclk_io_num = SPI2_SCK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4092,
  };

  // Initialize the SPI bus
  esp_err_t ret = spi_bus_initialize((spi_host_device_t)host.slot, &spi_conf, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK){
    printf("Failed to initialize bus.\n");
    return; 
  }

  // Configure SD1 device and mount its filesystem
  sdspi_device_config_t onboard_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  onboard_sd_conf.host_id = (spi_host_device_t)host.slot;
  onboard_sd_conf.gpio_cs = (gpio_num_t)SD1_CS;

  printf("Mounting Filesystem 1\n");
  ret = esp_vfs_fat_sdspi_mount(mount_point1, &host, &onboard_sd_conf, &mount_config, &_card1);

  if (ret != ESP_OK){
    if (ret == ESP_FAIL){
      printf("Failed to mount filesystem.");
    }
  else{
      printf("Failed to initialize the card. Make sure the SD card lines have pull up resistors in place.");
    }
    return;
  }
  printf("Filesystem 1 Mounted\n");

  sdmmc_card_print_info(stdout, _card1);

  // Clear the log file
  _path1 = MOUNT1;
  _path1 += "/";
  _path1 += LOGFILE;
  writeFile(_path1, "");

#ifdef SD2_ATTACHED
  // Configure SD2 device and mount its filesystem
  sdspi_device_config_t external_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  external_sd_conf.host_id = (spi_host_device_t)host.slot;
  external_sd_conf.gpio_cs = (gpio_num_t)SD2_CS;

  printf("Mounting Filesystem 2\n");
  ret = esp_vfs_fat_sdspi_mount(mount_point2, &host, &external_sd_conf, &mount_config, &_card2);

  if (ret != ESP_OK){
    if (ret == ESP_FAIL){
      printf("Failed to mount filesystem.");
    }
  else{
      printf("Failed to initialize the card. Make sure the SD card lines have pull up resistors in place.");
    }
    return;
  }
  printf("Filesystem 2 Mounted\n");

  sdmmc_card_print_info(stdout, _card2);

  // Clear the log file
  _path2 = MOUNT2;
  _path2 += "/";
  _path2 += LOGFILE;
  writeFile(_path2, "");

#endif /* SD2_ATTACHED */
}
