#include "datalogger.h"

void DataLogger::initSubs()
{
  _commandCenter_command_sub = umsg_CommandCenter_command_subscribe(1, 1);

  _gps_data_sub = umsg_GPS_data_subscribe(1, 2);
  _gps_configuration_sub = umsg_GPS_configuration_subscribe(1, 2);
  _gps_state_sub = umsg_GPS_state_subscribe(1, 4);

  _iridium_received_msg_sub = umsg_Iridium_received_msg_subscribe(1, 2);
  _iridium_sent_msg_sub = umsg_Iridium_sent_msg_subscribe(1, 2);
  _iridium_state_sub = umsg_Iridium_state_subscribe(1, 2);

  _LoRa_received_msg_sub = umsg_LoRa_received_msg_subscribe(1, 4);
  _LoRa_sent_msg_sub = umsg_LoRa_sent_msg_subscribe(1, 10);
  _LoRa_state_msg_sub = umsg_LoRa_state_msg_subscribe(1, 4);

  _imu_configuration_sub = umsg_Sensors_imu_configuration_subscribe(1, 5);
  _imu_data_sub = umsg_Sensors_imu_data_subscribe(1, 30);
  _imu_state_sub = umsg_Sensors_imu_state_subscribe(1, 5);

  _baro_configuration_sub = umsg_Sensors_baro_configuration_subscribe(1, 3);
  _baro_data_sub = umsg_Sensors_baro_data_subscribe(1, 30);
  _baro_state_sub = umsg_Sensors_baro_state_subscribe(1, 3);

  _therm_0_config_sub = umsg_Sensors_thermistor_configuration_subscribe_ch(1, 3, 0);
  _therm_1_config_sub = umsg_Sensors_thermistor_configuration_subscribe_ch(1, 3, 1);
  _therm_0_data_sub = umsg_Sensors_thermistor_data_subscribe_ch(1, 30, 0);
  _therm_1_data_sub = umsg_Sensors_thermistor_data_subscribe_ch(1, 30, 1);
  _therm_0_state_sub = umsg_Sensors_thermistor_state_subscribe_ch(1, 3, 0);
  _therm_1_state_sub = umsg_Sensors_thermistor_state_subscribe_ch(1, 3, 1);

  _stats_system_run_time_sub = umsg_Stats_system_run_time_stats_subscribe(1, 10);
  _stats_task_run_time_sub = umsg_Stats_task_run_time_stats_subscribe(1, 30);

  // umsg_StatusMsgs_msg_subscribe(1, 1);
}

void DataLogger::readSubs()
{
  int timeout = 1 / portTICK_PERIOD_MS;

  //=================
  // Commands
  //=================
  if (umsg_CommandCenter_command_peek(&_commandCenter_command_data))
  {
    while (umsg_CommandCenter_command_receive(_commandCenter_command_sub, &_commandCenter_command_data, timeout) == pdPASS)
    {
      _datalines.push_back(Command_toDataLine(_commandCenter_command_data));
    }
  }

  //=================
  // GPS
  //=================
  if (umsg_GPS_data_peek(&_gps_data))
  {
    while (umsg_GPS_data_receive(_gps_data_sub, &_gps_data, timeout) == pdPASS)
    {
      _datalines.push_back(GPS_data_toDataLine(_gps_data));
    }
  }

  if (umsg_GPS_configuration_peek(&_gps_configuration_data))
  {
    while (umsg_GPS_configuration_receive(_gps_configuration_sub, &_gps_configuration_data, timeout) == pdPASS)
    {
      _datalines.push_back(GPS_configuration_toDataLine(_gps_configuration_data));
    }
  }

  if (umsg_GPS_state_peek(&_gps_state_data))
  {
    while (umsg_GPS_state_receive(_gps_state_sub, &_gps_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(GPS_state_toDataLine(_gps_state_data));
    }
  }

  //=================
  // Iridium
  //=================
  umsg_Iridium_received_msg_receive(_iridium_received_msg_sub, &_iridium_received_data, timeout);

  //=================
  // LoRa
  //=================

  if (umsg_LoRa_received_msg_peek(&_LoRa_received_data))
  {
    while (umsg_LoRa_received_msg_receive(_LoRa_received_msg_sub, &_LoRa_received_data, timeout) == pdPASS)
    {
      _datalines.push_back(LoRa_received_toDataLine(_LoRa_received_data));
    }
  }

  if (umsg_LoRa_sent_msg_peek(&_LoRa_sent_data))
  {
    while (umsg_LoRa_sent_msg_receive(_LoRa_sent_msg_sub, &_LoRa_sent_data, timeout) == pdPASS)
    {
      _datalines.push_back(LoRa_sent_toDataLine(_LoRa_sent_data));
    }
  }

  if (umsg_LoRa_state_msg_peek(&_LoRa_state_data))
  {
    while (umsg_LoRa_state_msg_receive(_LoRa_state_msg_sub, &_LoRa_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(LoRa_state_toDataLine(_LoRa_state_data));
    }
  }

  //=================
  // IMU
  //=================
  while (umsg_Sensors_imu_data_receive(_imu_data_sub, &_imu_data, timeout) == pdPASS)
  {
    _datalines.push_back(imu_data_toDataLine(_imu_data));
  }

  if (umsg_Sensors_imu_configuration_peek(&_imu_configuration_data))
  {
    while (umsg_Sensors_imu_configuration_receive(_imu_configuration_sub, &_imu_configuration_data, timeout) == pdPASS)
    {
      _datalines.push_back(imu_configuration_toDataLine(_imu_configuration_data));
    }
  }

  if (umsg_Sensors_imu_state_peek(&_imu_state_data))
  {
    while (umsg_Sensors_imu_state_receive(_imu_state_sub, &_imu_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(imu_state_toDataLine(_imu_state_data));
    }
  }

  //=================
  // BARO
  //=================
  while (umsg_Sensors_baro_data_receive(_baro_data_sub, &_baro_data, timeout) == pdPASS)
  {
    _datalines.push_back(baro_data_toDataLine(_baro_data));
  }

  if (umsg_Sensors_baro_configuration_peek(&_baro_configuration_data))
  {
    while (umsg_Sensors_baro_configuration_receive(_baro_configuration_sub, &_baro_configuration_data, timeout) == pdPASS)
    {
      _datalines.push_back(baro_configuration_toDataLine(_baro_configuration_data));
    }
  }

  // Messages need to be removed from subscription queue
  if (umsg_Sensors_baro_state_peek(&_baro_state_data))
  {
    while (umsg_Sensors_baro_state_receive(_baro_state_sub, &_baro_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(baro_state_toDataLine(_baro_state_data));
    }
  }

  //=================
  // THERM
  //=================
  std::string therm_0_name = "INT";
  std::string therm_1_name = "EXT";
  while (umsg_Sensors_thermistor_data_receive(_therm_0_data_sub, &_therm_0_data, timeout) == pdPASS)
  {
    _datalines.push_back(therm_data_toDataLine(_therm_0_data, therm_0_name));
  }
  while (umsg_Sensors_thermistor_data_receive(_therm_1_data_sub, &_therm_1_data, timeout) == pdPASS)
  {
    _datalines.push_back(therm_data_toDataLine(_therm_1_data, therm_1_name));
  }

  // Don't want to wait for timeout everytime, so peek then we can have possible timeout wait
  if (umsg_Sensors_thermistor_configuration_peek(&_therm_0_config_data))
  {
    if (umsg_Sensors_thermistor_configuration_receive(_therm_0_config_sub, &_therm_0_config_data, timeout) == pdPASS)
    {
      _datalines.push_back(therm_configuration_toDataLine(_therm_0_config_data, therm_0_name));
    }
    if (umsg_Sensors_thermistor_configuration_receive(_therm_1_config_sub, &_therm_1_config_data, timeout) == pdPASS)
    {
      _datalines.push_back(therm_configuration_toDataLine(_therm_1_config_data, therm_1_name));
    }
  }

  if (umsg_Sensors_thermistor_state_peek(&_therm_0_state_data))
  {
    if (umsg_Sensors_thermistor_state_receive(_therm_0_state_sub, &_therm_0_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(therm_state_toDataLine(_therm_0_state_data, therm_0_name));
    }

    if (umsg_Sensors_thermistor_state_receive(_therm_1_state_sub, &_therm_1_state_data, timeout) == pdPASS)
    {
      _datalines.push_back(therm_state_toDataLine(_therm_1_state_data, therm_1_name));
    }
  }

  //=================
  // Stats
  //=================
  if (umsg_Stats_task_run_time_stats_peek(&_task_run_time_data))
  {
    while (umsg_Stats_task_run_time_stats_receive(_stats_task_run_time_sub, &_task_run_time_data, timeout) == pdPASS)
    {
      _datalines.push_back(Stats_task_toDataLine(_task_run_time_data));
    }
  }
  if (umsg_Stats_system_run_time_stats_peek(&_system_run_time_data))
  {
    while (umsg_Stats_system_run_time_stats_receive(_stats_system_run_time_sub, &_system_run_time_data, timeout) == pdPASS)
    {
      _datalines.push_back(Stats_system_toDataLine(_system_run_time_data));
    }
  }
}

/**
 * @brief Handles the unqueueing of data from other tasks and writing that data to SD card
 *
 * @param data_logger Data logger class, giving access to other functions and fields
 */
void DataLogger::vLogLoop_Task(void *data_logger)
{
  DataLogger *data_log = (DataLogger *)data_logger;
  data_log->initSubs();
  for (;;)
  {
    data_log->handleQueueData();
    // Delay so watchdog does not freak out
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }

  vTaskDelete(NULL);
}

/**
 * @brief Gets SDData from the dataOut queue and writes to both SD1 and SD2 if data if larger than
 *        the internal buffer size.
 */
void DataLogger::handleQueueData()
{

  // std::string sd_data_msg = *(sd_data->message);

  readSubs();

  // Sort all entries by the time at which they were recorded
  std::sort(_datalines.begin(), _datalines.end(), [](const DataLine &data1, const DataLine &data2)
            { return data1.recorded_tick < data2.recorded_tick; });

  if (_datalines.size() > 50)
  {
    printf("Time of sd log: %d\n", xTaskGetTickCount());
    log_to_sd();
  }
}

void DataLogger::log_to_sd()
{
  std::string sd_data_msg = "";
  for (DataLine data : _datalines)
  {
    sd_data_msg += data.toString() + "\n";
  }
  // clear all datalines
  _datalines.clear();

  // TODO:: Don't clear bufer if appendFile fails
  // if not connected, attemp to remount
  if (!_sd1_connected)
  {
    mountSDFileSystem(_onboard_sd_conf, MOUNT1, 1);
  }
  else
  {
    esp_err_t ret = appendFile(_path1, sd_data_msg);
    if (ret == ESP_FAIL)
    {
      _sd1_connected = false;
    }
  }
#ifdef SD2_ATTACHED
  if (!_sd2_connected)
  {
    mountSDFileSystem(_external_sd_conf, MOUNT2, 2);
  }
  else
  {
    esp_err_t ret = appendFile(_path2, sd_data_msg);
    if (ret == ESP_FAIL)
    {
      _sd2_connected = false;
    }
  }
#endif
}

/**
 * @brief Appends message to file at path
 *
 * @param path       Path of file to append to
 * @param message    Message to append
 */
esp_err_t DataLogger::appendFile(std::string path, std::string message)
{
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
esp_err_t DataLogger::writeFile(std::string path, std::string message)
{
#ifdef SPEED_LOG
  uint32_t start = millis();
#endif
  esp_err_t ret = modifyFile(path, message, "w");
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
esp_err_t DataLogger::modifyFile(std::string path, std::string message, std::string open_mode)
{

  FILE *file = fopen(path.c_str(), open_mode.c_str());
  if (!file)
  {
    printf("Failed to open file for writing\n");
    return ESP_FAIL;
  }

  if (fprintf(file, "%s", message.c_str()) < 0)
  {
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
      .allocation_unit_size = 16 * 1024};
  const char mount_point1[] = MOUNT1;
  const char mount_point2[] = MOUNT2;

  _host = SDHost; // defined in ComBus.h

  // Configure SD1 device and mount its filesystem
  _onboard_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  _onboard_sd_conf.host_id = (spi_host_device_t)_host.slot;
  _onboard_sd_conf.gpio_cs = (gpio_num_t)SD1_CS_GPIO;

  printf("Mounting Filesystem 1\n");
  mountSDFileSystem(_onboard_sd_conf, mount_point1, 1);

  // Clear the log file
  _path1 = MOUNT1;
  _path1 += "/";
  _path1 += LOGFILE;
  appendFile(_path1, "\n\n SD 1 Mounted\n");

#ifdef SD2_ATTACHED
  // Configure SD2 device and mount its filesystem
  _external_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  _external_sd_conf.host_id = (spi_host_device_t)_host.slot;
  _external_sd_conf.gpio_cs = (gpio_num_t)SD2_CS_GPIO;

  printf("Mounting Filesystem 2\n");
  mountSDFileSystem(_external_sd_conf, mount_point2, 2);
  sdspi_dev_handle_t external_device;

  // Clear the log file
  _path2 = MOUNT2;
  _path2 += "/";
  _path2 += LOGFILE;
  appendFile(_path2, "\n\n SD 2 Mounted\n");

#endif /* SD2_ATTACHED */
}

/**
 * @brief Mounts the file system of an SD card
 *
 * @param mount_point place to mount the SD card at
 */
void DataLogger::mountSDFileSystem(sdspi_device_config_t sd_conf, const char *mount_point, uint8_t card_num)
{
  sdmmc_card_t *card;
  bool *connected;

  switch (card_num)
  {
  case 1:
    card = _card1;
    connected = &_sd1_connected;
    break;
  case 2:
    card = _card2;
    connected = &_sd2_connected;
    break;
  default:
    printf("Not a valid card number. card in range [0, 1]\n");
    return;
  }

  printf("(%d)Memory: %d\n", card_num, esp_get_free_heap_size());
  esp_err_t ret = esp_vfs_fat_sdspi_mount(mount_point, &_host, &sd_conf, &_mount_config, &card);

  *connected = true;
  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      printf("Failed to mount filesystem.\n");
    }
    else
    {
      printf("Failed to initialize the card: %s\n", esp_err_to_name(ret));
      *connected = false;
    }
    return;
  }

  printf("Mounted Card:\n");
  sdmmc_card_print_info(stdout, card);
  printf("\n");
}
