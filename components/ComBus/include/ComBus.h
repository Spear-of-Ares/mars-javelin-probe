/*********************************************************************************
*     File Name           :     /components/ComBus/include/ComBus.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-23 19:22]
*     Last Modified       :     [2022-10-28 01:04]
*     Description         :     Definitions and initializtion for the communication busses (1 I2C and 2 SPI)
**********************************************************************************/

#ifndef __COM_BUS_H__
#define __COM_BUS_H__

#include <stdio.h>
#include <Wire.h>

#include "driver/i2c.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_vfs_fat.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "SPI.h"

#include "LoRa.h"

#define I2C_MASTER_PORT     CONFIG_I2C_MASTER_PORT
#define I2C_MASTER_FREQ_HZ  CONFIG_I2C_MASTER_FREQ_HZ
#define I2C_SDA_GPIO        CONFIG_I2C_SDA_GPIO
#define I2C_SCL_GPIO        CONFIG_I2C_SCL_GPIO

#define HSPI_MOSI_GPIO      CONFIG_HSPI_MOSI_GPIO
#define HSPI_MISO_GPIO      CONFIG_HSPI_MISO_GPIO
#define HSPI_SCK_GPIO       CONFIG_HSPI_SCK_GPIO
#define LORA_CS_GPIO        CONFIG_LORA_CS_GPIO
#define HSPI_RST_GPIO       CONFIG_HSPI_RST_GPIO
#define HSPI_DI0_GPIO       CONFIG_HSPI_DI0_GPIO

#define VSPI_MOSI_GPIO      CONFIG_VSPI_MOSI_GPIO
#define VSPI_MISO_GPIO      CONFIG_VSPI_MISO_GPIO
#define VSPI_SCK_GPIO       CONFIG_VSPI_SCK_GPIO
#define SD1_CS_GPIO         CONFIG_SD1_CS_GPIO
#define SD2_CS_GPIO         CONFIG_SD2_CS_GPIO


static void initLoRaSPI();
static void initSDSPI();

extern sdmmc_host_t SDHost;
extern int i2c_master_port;

static void initI2C();

void initComBus();
void i2cScan();

#endif /* __COM_BUS_H__ */
