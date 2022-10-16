/*********************************************************************************
*     File Name           :    /DataLogger/include/datalogger.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-06 20:25]
*     Last Modified       :     [2022-10-06 21:26]
*     Description         :      
**********************************************************************************/

#ifndef __DATA_LOGGER_H__
#define __DATA_LOGGER_H__

#include <stdio.h>
#include "driver/uart.h"

void vBeginLogLoop_Task(void* queue);

#endif /* __DATA_LOGGER_H__ */
