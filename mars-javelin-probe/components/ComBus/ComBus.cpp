#include "ComBus.h"

/**************************
 *
 * SPI Component methods
 *
 *************************/

sdmmc_host_t SDHost;
int i2c_master_port;

// Need to init SPI in different ways. One uses espidf SPI and one uses Arduino SPI
void initSPI(){
  initLoRaSPI();
  initSDSPI();
}
// 5, 19, 27, 18
// 18, 23, 26
void initLoRaSPI(){
  SPIClass *hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCK_GPIO, HSPI_MISO_GPIO, HSPI_MOSI_GPIO, LORA_CS_GPIO);
  LoRa.setPins(LORA_CS_GPIO, HSPI_RST_GPIO, HSPI_DI0_GPIO);
  LoRa.setSPI(*hspi);
}

void initSDSPI(){
  // Host config. Use SPI3_HOST as LoRa is on SPI2_HOST
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.slot = VSPI_HOST;
  SDHost = host;

  // Init spi bus pins
  spi_bus_config_t spi_conf = {
    .mosi_io_num = VSPI_MOSI_GPIO,
    .miso_io_num = VSPI_MISO_GPIO,
    .sclk_io_num = VSPI_SCK_GPIO,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4092,
  };

  // Initialize the SPI bus
  esp_err_t ret = spi_bus_initialize((spi_host_device_t)SDHost.slot, &spi_conf, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK){
    printf("Failed to initialize bus.\n");
    return; 
  }
}

/**************************
 *
 * I2C Component methods
 *
 *************************/
void initI2C(){
  i2c_master_port = I2C_MASTER_PORT;

  i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_SDA_GPIO,
    .scl_io_num = I2C_SCL_GPIO,
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
    .master {.clk_speed = I2C_MASTER_FREQ_HZ}
  };

  //conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
  ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0));
}


/**************************
 *
 * ComBus methods
 *
 *************************/
void initComBus(){
  initSPI();
  initI2C();
}

void i2cScan(){
  printf("i2c scan: \n");
  for (uint8_t i = 1; i < 127; i++)
    {
      int ret;
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1);
      i2c_master_stop(cmd);
      ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS);
      i2c_cmd_link_delete(cmd);

      if (ret == ESP_OK)
      {
        printf("Found device at: 0x%2x\n", i);
      }
    }

}
