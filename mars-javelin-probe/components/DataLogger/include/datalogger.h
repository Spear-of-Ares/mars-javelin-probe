/*********************************************************************************
*     File Name           :    /DataLogger/include/datalogger.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-06 20:25]
*     Last Modified       :     [2022-10-19 01:35]
*     Description         :      
**********************************************************************************/

#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include <stdio.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define SPI_2_MISO 2
#define SPI_2_MOSI 15
#define SPI_2_SCK  14
#define SPI_2_CS   13



void vBeginLogLoop_Task(void* queue);

void appendFile(fs::FS &fs, const char * path, const char * messgae);
void testFileIO(fs::FS &fs, const char * path);
void setup();
#endif /* __DATA_LOGGER_H__ */
