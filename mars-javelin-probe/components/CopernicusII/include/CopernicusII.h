/*********************************************************************************
*     File Name           :     mars-javelin-probe/components/CopernicusII/CopernicusII.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-13 17:43]
*     Last Modified       :     [2022-10-13 22:15]
*     Description         :     Driver for the CopernicusII GPS receiver 
**********************************************************************************/


#ifndef __COPERNICUSII_H__
#define __COPERNICUSII_H__
#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "TinyGPS.h"

#define GPS_UART UART_NUM_1

#define TXD_PIN (GPIO_NUM_4)
#define RXD_PIN (GPIO_NUM_2)

/**
* @brief Initializes the UART connection to the CopernicusII GPS receiver
*/
void initUART();


/**
* @brief Prints the data received to stdout
*/
void vPrintReceived_Task(void* params);

#endif /* __COPERNICUSII_H__ */

