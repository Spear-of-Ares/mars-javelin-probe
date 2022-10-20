#include "datalogger.h"

void testSD(uint8_t CS, SPIClass *vspi)
{
  printf("Ticks Before begin: %d\n", xTaskGetTickCount());
  if(!SD.begin(CS, *vspi)){
    printf("Card Mount Failed!\n");
    printf("SS: %d\n", vspi->pinSS());
    return; }
  printf("Ticks After begin: %d\n", xTaskGetTickCount());

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    printf("No SD card attached\n");
    return;
  }

  printf("SD Card Type:");
  switch(cardType){
    case CARD_MMC:
    printf("MMC\n");
      break;
    case CARD_SD:
    printf("SDSC\n");
      break;
    case CARD_SDHC:
    printf("SDHC\n");
      break;
    default:
    printf("UNKOWN\n");
      break;
  }

  uint64_t cardSize_1 = SD.cardSize() / (1024*1024);
  printf("SD Card Size: %lluMB\n", cardSize_1);

  if(CS == SPI_2_SD_2_CS){
    testFileIO(SD, "/test2.txt");
  }
  else{
    testFileIO(SD, "/test.txt");
  } 
}
void vBeginLogLoop_Task(void *pvParameters)
{
  SPIClass *vspi = new SPIClass(VSPI);

  for(;;)
    {
      printf("Testing 1\n");
      testSD(SPI_2_SD_1_CS, vspi);
      printf("Testing 2\n");
      testSD(SPI_2_SD_2_CS, vspi);
      printf("Round Finished\n\n");
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }

  vTaskDelete(NULL);
}

void BMI085_Message(char* message){
  // // Just use String class here. We are in C++ so don't make it hard on yourself
  // char ts = [32];
  // char ac = [32];
  // char gy = [64];
  //
  // snprintf(ts, "TS %llu\n", toff_micro);
  // snprintf(ac, "AC %d %d %d %d\n", ax, ay, az, temp);
  // snprintf(gy, "GY %d %d %d %d %d %d\n\n", gx, gy, gz, pitch, roll, yaw);
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file){
    printf("Failed to open file for appending\n");
    return;
  }
  
  if(file.print(message)){
    printf("Message appended\n");
  }
  else{
    printf("Append failed\n");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  }else{
    printf("Failed to open file for reading\n");
  }

  file = fs.open(path, FILE_WRITE);
  if(!file){
    printf("Failed to open file for writing\n");
    return;
  }

  size_t i = 0;
  start = millis();
  while (millis() - start < 40)
    {
      file.write(buf, 512);
      i++;
    }
  end = millis() - start;
  printf("%u bytes written for %u ms\n", i * 512, end);
  file.close();
}

void setup()
{
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
  };
  sdmmc_card_t *card;
  const char mount_point1[] = "/sdcard1";
  const char mount_point2[] = "/sdcard2";
  printf("Initializing SD card\n");

  printf("Using SPI peripheral\n");
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = SPI2_HOST;



  // ESP_ERROR_CHECK(sdspi_host_init());
  spi_bus_config_t spi_conf = {
    .mosi_io_num = SPI_2_MOSI,
    .miso_io_num = SPI_2_MISO,
    .sclk_io_num = SPI_2_SCK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4092,
  };

  esp_err_t ret = spi_bus_initialize((spi_host_device_t)host.slot, &spi_conf, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK){
    printf("Failed to initialize bus.\n");
    return; 
  }
  // ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &spi_conf, SPI_DMA_DISABLED));
  // //ESP_ERROR_CHECK(gpio_install_isr_service());
  //
  // // Initializing The onboard SD card
  sdspi_device_config_t onboard_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  onboard_sd_conf.host_id = (spi_host_device_t)host.slot;
  onboard_sd_conf.gpio_cs = (gpio_num_t)SPI_2_SD_1_CS;
  printf("Mounting Filesystem 1\n");
  ret = esp_vfs_fat_sdspi_mount(mount_point1, &host, &onboard_sd_conf, &mount_config, &card);

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

  sdmmc_card_print_info(stdout, card);

  sdspi_device_config_t external_sd_conf = SDSPI_DEVICE_CONFIG_DEFAULT();
  external_sd_conf.host_id = (spi_host_device_t)host.slot;
  external_sd_conf.gpio_cs = (gpio_num_t)SPI_2_SD_2_CS;

  printf("Mounting Filesystem 2\n");
  ret = esp_vfs_fat_sdspi_mount(mount_point2, &host, &external_sd_conf, &mount_config, &card);

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

  sdmmc_card_print_info(stdout, card);


  const char *file_hello = "/sdcard1/hello.txt";
  printf("Writing file %s", file_hello);
  FILE *f = fopen(file_hello, "w");
  if(f == NULL){
    printf("Failed to open file for writing\n");
    return;
  }
  fprintf(f, "Hello %s!\n", card->cid.name);
  fclose(f);
  printf("File Written\n");

  const char *file_hello_2 = "/sdcard2/hello.txt";
  printf("Writing file %s", file_hello_2);
  f = fopen(file_hello_2, "w");
  if(f == NULL){
    printf("Failed to open file for writing\n");
    return;
  }
  fprintf(f, "Hello %s!\n", card->cid.name);
  fclose(f);
  printf("File Written\n");
}
