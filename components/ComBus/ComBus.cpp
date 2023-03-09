/*!
 * @file ComBus.cpp
 * @author Jon Kopf (kopf0033@vandals.uidaho.edu)
 * @brief Implementation of ComBus.h
 * @version 1.0
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ComBus.h"

sdmmc_host_t SDHost;
int i2c_master_port;
SC16IS752 i2cuart;

/*!
 * Both SPI busses are intialized. One is for LoRa and the other for the two storage SD cards.
 *
 */
void initSPI()
{
  initSDSPI();
  initLoRaSPI();
}

/*!
 * Initializes the lora SPI bus using HSPI port. Also sets the pins and SPI bus in the global LoRa object.
 *
 */
void initLoRaSPI()
{
  SPIClass *hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCK_GPIO, HSPI_MISO_GPIO, HSPI_MOSI_GPIO, LORA_CS_GPIO);
  LoRa.setPins(LORA_CS_GPIO, HSPI_RST_GPIO, HSPI_DI0_GPIO);
  LoRa.setSPI(*hspi);
}

/*!
 * Initializes the SPI bus for the 2 SD cards. This uses sdmmc and esp32 libraries.
 * Sets maximum frequency and pins.
 */
void initSDSPI()
{
  // Host config. Use SPI3_HOST as LoRa is on SPI2_HOST
  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  host.max_freq_khz = 20 * 1000; // Too fast of freq results in failures on breadboard
  host.slot = SPI3_HOST;
  SDHost = host;

  // Init spi bus pins
  spi_bus_config_t spi_conf = {
      .mosi_io_num = VSPI_MOSI_GPIO,
      .miso_io_num = VSPI_MISO_GPIO,
      .sclk_io_num = VSPI_SCK_GPIO,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 16 * 1024,
  };

  // Initialize the SPI bus
  esp_err_t ret = spi_bus_initialize((spi_host_device_t)SDHost.slot, &spi_conf, SDSPI_DEFAULT_DMA);
  if (ret != ESP_OK)
  {
    printf("Failed to initialize bus: %s\n", esp_err_to_name(ret));
    return;
  }
}

/*!
 * Initializes the I2C bus using arduino libraries at 400khz frequency (max)
 *
 */
void initI2C()
{
  Wire.begin(I2C_SDA_GPIO, I2C_SCL_GPIO, 400000);
  Wire.setTimeOut(100);

  // Initialize the i2c to uart bridge
  i2cuart = SC16IS752(SC16IS750_PROTOCOL_I2C, SC16IS750_ADDRESS_BB);
  // Begin A channel. Baud rate of RFD is 38400
  i2cuart.beginA(38400);
  if (i2cuart.ping() != 1)
  {
    printf("Device not found!\n");
  }
  else
  {
    printf("Device found\n");
  }
}

/*!
 * Initializes both SPI and I2C
 *
 */
void initComBus()
{
  initSPI();
  initI2C();
}

/*!
 * Scans the I2C bus for all available addresses. Only works if the I2C bus on the master port has been initialized.
 *
 */
void i2cScan()
{
  printf("i2c scan (num_0): \n");
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

void i2cScanArduino()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);

    error = Wire.endTransmission();

    if (error == 0)
    {
      printf("I2C device found at address 0x");
      if (address < 16)
        printf("0");
      printf("%2x", address);
      printf("  !\n");

      nDevices++;
    }
    else if (error == 4)
    {
      printf("Unknown error at address 0x");
      if (address < 16)
        printf("0");
      printf("%2x", address);
    }
  }
  if (nDevices == 0)
    printf("No I2C devices found\n");
  else
    printf("done\n");
}
