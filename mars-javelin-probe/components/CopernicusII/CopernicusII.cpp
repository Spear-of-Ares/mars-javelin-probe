/*********************************************************************************
*     File Name           :     mars-javelin-probe/components/CopernicusII/CopernicusII.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-13 17:46]
*     Last Modified       :     [2022-10-15 02:29]
*     Description         :     Driver for the CopernicusII GPS receiver 
**********************************************************************************/


#include "CopernicusII.h"



/**
* @brief Initializes the UART connection to the CopernicusII GPS receiver
*/
void initUART()
{
  uart_config_t gps_conf = {
    .baud_rate = 4800,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };

  const int uart_buffer_size = (1024 * 2);
  ESP_ERROR_CHECK(uart_driver_install(GPS_UART, uart_buffer_size, 
                                      uart_buffer_size, 10, NULL, 0));

  ESP_ERROR_CHECK(uart_param_config(GPS_UART, &gps_conf));
  ESP_ERROR_CHECK(uart_set_pin(GPS_UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

}

/**
* @brief Prints the data received to stdout
*/
void vPrintReceived_Task(void* params)
{
  initUART();
  TinyGPS gps;
  int length = 0;

  // Loop frequency 
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000;

  xLastWakeTime = xTaskGetTickCount();
  for(;;)
    {
      printf("Last Wake: %d\n", xLastWakeTime);
      ESP_ERROR_CHECK(uart_get_buffered_data_len(GPS_UART, (size_t*)&length));
      if (length > 0)
      {
        uint8_t data[256];
        length = uart_read_bytes(GPS_UART, data, length, 100/portTICK_PERIOD_MS);
        for (int i = 0; i < length; i++){
          bool good_encode = gps.encode(data[i]);
          printf("%c", data[i]);
          //printf("Good encode:%d ", good_encode);
          if (i+1 % 80 == 0)
          {
            printf("\n");
          }
        }
        printf("\n");

        float flat, flon;
        unsigned long age;
        gps.f_get_position(&flat, &flon, &age);
        printf("LAT=%.6f\n", flat);
        printf("LON=%.6f\n", flon);
        printf("SAT=%d\n", gps.satellites());
        printf("PREC=%ld\n", gps.hdop());
        printf("\n");
      }
      vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
  vTaskDelete(NULL); 
}
