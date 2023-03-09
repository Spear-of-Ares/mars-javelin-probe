/*!
 * @file ComBus.h
 * @author Jon Kopf (kopf0033@vandals.uidaho.edu)
 * @brief Initialises the Communication bus. Starts I2C for sensors and SPI and for SD cards and LoRa
 * @version 1.0
 * @date 2023-02-23
 *
 * @copyright Copyright (c) 2023
 *
 */

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
#include "SC16IS752.h"

#include "SPI.h"

#include "LoRa.h"

#define I2C_MASTER_PORT CONFIG_I2C_MASTER_PORT
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQ_HZ
#define I2C_SDA_GPIO CONFIG_I2C_SDA_GPIO
#define I2C_SCL_GPIO CONFIG_I2C_SCL_GPIO

#define HSPI_MOSI_GPIO CONFIG_HSPI_MOSI_GPIO
#define HSPI_MISO_GPIO CONFIG_HSPI_MISO_GPIO
#define HSPI_SCK_GPIO CONFIG_HSPI_SCK_GPIO
#define LORA_CS_GPIO CONFIG_LORA_CS_GPIO
#define HSPI_RST_GPIO CONFIG_HSPI_RST_GPIO
#define HSPI_DI0_GPIO CONFIG_HSPI_DI0_GPIO

#define VSPI_MOSI_GPIO CONFIG_VSPI_MOSI_GPIO
#define VSPI_MISO_GPIO CONFIG_VSPI_MISO_GPIO
#define VSPI_SCK_GPIO CONFIG_VSPI_SCK_GPIO
#define SD1_CS_GPIO CONFIG_SD1_CS_GPIO
#define SD2_CS_GPIO CONFIG_SD2_CS_GPIO

/*!
 * @brief Initializes the SPI bus for LoRa. Uses HSPI.
 *
 */
void initLoRaSPI();

/*!
 * @brief Initializes the SPI bus for the 2 SD cards. Uses VSPI.
 *
 */
void initSDSPI();

extern sdmmc_host_t SDHost; /*!< SDSPI host. Holds configuration values like max frequency.*/
extern int i2c_master_port; /*!< The port that I2C will be running on*/
extern SC16IS752 i2cuart;   /*!< I2C to uart bridge*/

/*!
 * @brief Initializes the I2C
 *
 */
void initI2C();

/*!
 * @brief Initializes both I2C and SPI bus
 *
 */
void initComBus();

/*!
 * @brief Performs a scan on the I2C bus and prints out all addresses found.
 *
 */
void i2cScan();

/*!
 * @brief Performs a scan on the I2C bus using arduino and prints out all addresses found.
 *
 */
void i2cScanArduino();

#endif /* __COM_BUS_H__ */
