/*********************************************************************************
*     File Name           :     mars-javelin-probe/components/rockblock/rockblock.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-03 22:44]
*     Last Modified       :     [2022-10-04 03:05]
*     Description         :      
**********************************************************************************/
#include <stdio.h>

#include "rockblock.h"


/**
* @brief Takes a response from RockBLOCK uart and breaks it up into an array of characters 
*
* @param uart_response          The bytes received from the RockBLOCK UART.
* @param uart_response_length   The length of uart_response. ie number of bytes.
* @param resp                   The Array that will contain the lines of the response.
*
* @return 
*/
static uint8_t rockblock_uart_response_to_lines(uint8_t *uart_response, uint32_t uart_response_length, uint8_t **resp)
{
  if (uart_response == NULL){
    resp = NULL;
    return 0;
  }

  // Iterate over response, adding a new array index on newlines
  int j = 0;
  int line_start = 0;
  // Guessing an average of 10 lines per response
  int resp_length = RESPONSE_SIZE / 10;
  resp = malloc(resp_length * sizeof(uint8_t*));
  if (resp == NULL)
  {
    // WARNING: Could not allocate space. 
    // TODO: Implement some form of response to this
  }

  for(int i = 0; i < uart_response_length; i++)
  {
    if (j >= resp_length)
    {
      resp_length = j+1;
      resp = realloc(resp, j+1 * sizeof(uint8_t));
      if (resp == NULL){
        // WARNING: Could not allocate space. 
        // TODO: Implement some form of response to this
      }
    }

    uint8_t *curr_val = &(uart_response[i]);
    if (*curr_val == (uint8_t)'\n')
    {
      // Change newline to '\0' for easier delimiting in the future
      *curr_val = '\0';
      resp[j] = uart_response + line_start;
      line_start = i + 1;
      j += 1;
    }
  }

  return j;
}

/**
  * @brief Send AT command and return response as an array of lines read.
  *
  * @param rb_modem         Adafruit RockBLOCK modem.
  * @param cmd              The command to send over the uart.
  * @param resp             The Array of lines that will be returned. Deliniates lines by each '\n' character.
  * @param return_response  Should the response be returned. Helpful to avoid allocations if not needed.
  *
  * @return Number of lines in resp. If return 5 then |resp| = [5][x] 
*/
static uint8_t rockblock_uart_xfer(rockblock_st *rb_modem, char *cmd, uint8_t **resp, rockblock_uart_response_et response_type)
{
  resp = NULL;

  if (rb_modem == NULL || cmd == NULL){
    return 0;
  }
  uart_port_t uart_num = rb_modem->uart_num;
  uint8_t resp_len = 0;

  // Discard all data in the UART RX buffer
  ESP_ERROR_CHECK(uart_flush(uart_num));

  // Send Command
  size_t msg_len = strlen(AT) + strlen(cmd) + strlen(AT_COMPLEMENT);
  char* msg = calloc(msg_len, sizeof(char));

  if (msg == NULL){
    // WARNING: Could not allocate space. 
    // TODO: Implement some form of response to this
  }

  strncat(msg, AT, strlen(AT)-1);
  strncat(msg, cmd, strlen(cmd)-1);
  strncat(msg, AT_COMPLEMENT, strlen(AT_COMPLEMENT)-1);

  if( uart_tx_chars(uart_num, msg, msg_len) && (response_type == RETURN_RESPONSE))
  {
    // If uart_tx_chars succeeds, get response
    uint32_t uart_response_length = sizeof(uint8_t) * RESPONSE_SIZE;
    uint8_t *uart_response = calloc(uart_response_length, sizeof(uint8_t));

    if ( uart_response == NULL)
    {
      // WARNING: Could not allocate space. 
      // TODO: Implement some form of response to this
    }

    int result = uart_read_bytes(uart_num, uart_response, uart_response_length, TICKS_TO_WAIT);

    if (result == -1){
      // We need to flush the uart RX buffer
      goto exit;
    }
     
    // Split into lines
    resp_len = rockblock_uart_response_to_lines(uart_response, uart_response_length, resp);
  }

exit:
  ESP_ERROR_CHECK(uart_flush(uart_num));
  return resp_len;
}


/**
  * @brief Copy out buffer to in buffer to simulate receiving a message.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
*/
static void rockblock_transfer_buffer(rockblock_st *rb_modem)
{
  rockblock_uart_xfer(rb_modem, "+SBDTC", NULL, NO_RESPONSE);
}

/**
 * @brief Install RockBlock driver and set default configurations.
 *
 * Usage: rockblock_st rb_modem = {};
 *        rockblock_driver_install(uart_num, BAUD_RATE, &rb_modem)
 * 
 * @param uart_num UART port number, the max port number is (UART_NUM_MAX -1).
 * @param baudrate UART baud rate.
 * @param rb_modem The Audafruit RockBLOCK modem that is install. This is the return pointer to the struct initilized.
 *
 */
void rockblock_driver_install(uart_port_t uart_num, uint32_t baudrate, rockblock_st* rb_modem)
{
  ESP_ERROR_CHECK(uart_set_baudrate(uart_num, baudrate));
  rb_modem->uart_num = uart_num; 
}


/**
* @brief Perform a software reset.
*
* @param rb_modem Adafruit RockBLOCK Modem
*
* @return 
*       - ESP_OK    Success
*/
esp_err_t rockblock_reset(rockblock_st *rb_modem)
{
  rockblock_uart_xfer(rb_modem, "&F0", NULL, NO_RESPONSE); // Factory defaults
  rockblock_uart_xfer(rb_modem, "&K0", NULL, NO_RESPONSE); // Flow control off
}

/* Don't think that this is neccessary. Will likely be removed*/
#ifndef True
/**
* @brief The binary data in the outbound buffer.
*
* @param rb_modem Adafruit RockBLOCK Modem
* @param buf_out Pointer to uint8_t array. A return value from this function. Will be changed.
*
* @return 
*       - uint16_t Length of data_out buffer WARNING:(May not be correct data type. Need to find max buffer size in some datasheet)
*/
uint16_t rockblock_get_data_out(rockblock_st *rb_modem, uint8_t *buf_out)
{
  //TODO: Implement
}
#endif 

/**
* @brief Sets the Binary data in the outboud buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param buf New data buffer that data_out should be.
*
* @return 
*       - ESP_OK       Success
*/
esp_err_t rockblock_set_data_out(rockblock_st *rb_modem, uint8_t *buf)
{
  //TODO: Implement
}


/**
* @brief The text within the outbound buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param text Pointer to character array. A return value from this function. Will be changed.
*
* @return 
*       - uint16_t Length of text in number of characters. 
*/
uint16_t rockblock_get_text_out(rockblock_st *rb_modem, char *text)
{
  //TODO: Implement
}


/**
* @brief Sets the text within the outbound buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param str The string data that should be within data_out buffer.
*
* @return 
*       - ESP_OK    Success
*/
esp_err_t rockblock_set_text_out(rockblock_st *rb_modem, char *str)
{
  //TODO: Implement
}


/**
* @brief Gets the binary data in the inbound buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param data A buffer to contain the values in data_in buffer. Values will be changed.
*
* @return 
*       - uint16_t Length of the data buffer.
*/
uint16_t rockblock_get_data_in(rockblock_st *rb_modem, uint8_t *data)
{
  //TODO: Implement
}


/**
* @brief Sets the binary data of the inbound buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param buf The binary data that will become the new data_in buffer.
*
* @return 
*       - ESP_OK    Success
*/
esp_err_t rockblock_set_data_in(rockblock_st *rb_modem, uint8_t *buf)
{
  //TODO: Implement
}


/**
* @brief Gets the text information in the inbound data buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param text A buffer to contain the values of data_in buffer. Values will be changed.
*
* @return 
*       - uint16_t Length of the text buffer.
*/
uint16_t rockblock_get_text_in(rockblock_st *rb_modem, char *text)
{
  //TODO: Implement
}


/**
* @brief Sets the text data of the inbound data buffer.
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param str The textual data that will become the new data_in buffer.
*
* @return 
*       - ESP_OK    Success
*/
esp_err_t rockblock_set_text_in(rockblock_st *rb_modem, char *str)
{
  //TODO: Implement
}

 /**
 * @brief Initiate a Short Burst Data transfer with satellites.
 * For full functionality, need to add some form of location specification for +SDBIX command.
 *
 * @param rb_modem Adafruit RockBLOCK modem.
 *
 * @return Resulting status of transfer.
 */
rockblock_status_st rockblock_satellite_transfer(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Return struct of Short Burst Data status.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return Status struct of the RockBLOCK modem.
*/
rockblock_status_st rockblock_status(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Return the modem model.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return Null-terminated String containing the modem model. If not received will return '\0'
*/
char* rockblock_model(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Modem's serial number, also known as the modem's IMEI
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return Null-terminated string containing the serial number. If not received will return '\0'
*/
char* rockblock_serial_number(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Signal Quality also known as the Received Signal Strength Indicator (RSSI).
  *
  * Values returned are 0 to 5, where 0 is no signal (0 bars) and 5 is strong signal (5 bars).
  *
  * Important note: signal strengh may not be fully accurate, so waiting for
  * high signal strength prior to sending a mesasge isn't always recommended.
  * For details see https://docs.rockblock.rock7.com/docs/checking-the-signal-strength
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return uint8_t value of RSSI
*/
uint8_t rockblock_signal_quality(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Modem's internal component firmware revisions.
  *
  * For example: Call Processor Version, Modem DSP Version, DBB Version (ASIC),
  * RFA VersionSRFA2), NVM Version, Hardware Version, BOOT Version.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  * @param revisions Array of Null-terminated strings containing the different firmware revisions.
  *
  * @return uint8_t number of revisions there are. If return 2, then 'revisions' is [2][x]
*/
uint8_t rockblock_revision(rockblock_st *rb_modem, char **revisions)
{
  //TODO: Implement
}


/**
  * @brief The current ring indication mode.
  *
  * False means Ring Alerts are disabled, and True means Ring Alerts are enabled.
  *
  * When SBD ring indication is enabled, the ISU asserts the RI line and issues
  * the unsolicited result code SBDRING when an SBD ring alert is received.
  * (Note: the network can only send ring alerts to the ISU after it has registerd).
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return bool for current ring indication mode.
*/
bool rockblock_get_ring_alert(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Sets the current rind indication mode. See rockblock_get_ring_alert() for more details.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  * @param value New value of ring alert
  *
  * @return 
  *     - ESP_OK    Success
*/
esp_err_t rockblock_set_ring_alert(rockblock_st *rb_modem, bool value)
{
  //TODO: Implement
}


/**
  * @brief The ring indication status.
  *
  * Returns the reason for the most recent assertion of the Ring Indicate signal.
  *
  * The response contains separte indications for telephony and SBD ring indications.
  * The response is in the form:
  * {.tel_ri, .sbd_ri}
  *
  * .tel_ri indicates the telephony ring indication status:
  * 0 No telephony ring alert received.
  * 1 Incoming voice call.
  * 2 Incoming data call.
  * 3 Incoming fax call.
  *
  * .sbd_ri indicates the SBD ring indication status:
  * 0 No SBD ring alert received.
  * 1 SBD ring alert received.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return rockblock_ring_indication_st containing .tel_ri and .sbd_ri as integers
*/
rockblock_ring_indication_st rockblock_ring_indication(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Most recent geolocation of the modem as measured by the Iridium constellation
  *        including a timestamp of when geolocation measurement was made.
  *
  * The response is in the form:
  * (<x>, <y>, <z>, <timestamp>)
  *
  * <x>, <y>, <z> is a geolocation grid code from an earth centered Cartesian coordinate system,
  * using dimensions, x, y, and z, to specify location. The coordinate system is aligned
  * such that the z-axis is aligned with the north and south poles, leaving the x-axis
  * and y-axis to lie in the plane containing the equator. The axes are aligned such that
  * at 0 degrees latitude and 0 degrees longitude, both y znd z are zero and 
  * x is positive (x = +6376, representing the nominal earth radius in kilometres).
  * Each dimension of the geolocation grid code is displayed in decimal form using 
  * units of kilometres. Each dimension of the geolocation grid code has a minimum value
  * of -6376, a maximum value of +6376, and a resolution of 4.
  * This geolocation coordinate system is known as ECEF (acronym earth-centered, earth-fixed),
  * also known as ECR (initialism for earth-centered rotational)
  *
  * <timesteamp> is a uint32_t 
  * The timestamp is assigned by the modem when the geolocation grid code received from 
  * the network is stored to the modem's internal memory.
  *
  * The timestamp used by the modem is Iridium system time, which is a running count of 
  * 90 millisecond intervals, since Sunday May 11, 2014, at 14::23::55 UTC (the most recent
  * Iridium epoch).
  * The timestamp returned by the modem is a 32-bit integer displayed in hexadecimal form.
  * This is the value returned by this function.
  *
  * The system time value is always expressed in UTC time.
  *
  * TODO: Make timestamp an actual time type so it can be realistically used
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return rockblock_geolocation_st with {.x, .y, .z, .timestamp}
*/
rockblock_geolocation_st rockblock_geolocation(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
  * @brief Current date and time as given by the Iridium network.
  *
  * The system time is available and valid only after the ISU has registered with
  * the network and has received the Iridium system time from the network.
  * Once the time is received, the ISU uses its internal clock to increment the counter.
  * In addition, at least every 8 hours, or on location update or other event that
  * requires re-registration, the ISU will obtain a new system time from the network.
  *
  * The timestamp used by the modem is Iridium system time, which is a running count of 
  * 90 millisecond intervals, since Sunday May 11, 2014, at 14:23:55 UTC (the most recent
  * Iridium epoch).
  * The timestamp returned by the modem is a 32-bit integer displayed in hexadecimal form.
  * We conver the modem's timestamp and return it a s time_t.
  * TODO: Actually return a time struct of some sort, not the raw time data from Iridium sat
  *
  * The system time value is always expressed in UTC time.
  *
  * @param rb_modem Adafruit RockBLOCK modem.
  *
  * @return uint32_t number of 90millisend intervals since beginning of Iridium time.
*/
uint32_t rockblock_system_time(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
* @brief The current accumulated energy usage estimate in microamp hours.
*
* Returns the estimate of the charge taken from the +5V supply to the modem,
* in microamp hours (uAh). This is represented internally as s 26-bit unsigned number,
* so in principle will rollover to zero after approx. 67Ah (in practice this is usually
* greater than battery life, if battery-powered).
*
* The accumulator value is set to zero on a power-cycle or on a watchdog reset.
* Note that while +5V power is supplied to the Data Module but the module is powered off
* by its ON/OFF control line, it will still be consuming up to a few tens of microamps, 
* and this current drain will not be estimated in the +GEMON report
*
* The setter will preset the energy monitor accumulator to value n (typically, <n> would
* be specified as 0, to clear the accumulator).
*
* Note: Call Processor/BOOT Version: TA16005 is known to not support the AT+GEMON energy
* monitor command. It is however known to work on TA19002 (newer) and TA12003 (older).
* You may use the revision function to determine which version you have. Function will return TODO: What will return
* if modem cannot retrieve the acumulated energy usage estimate.
*
* @param rb_modem
*
* @return Estimated energy usage in number of microamp hours (uAh)
*/
uint32_t rockblock_energy_monitor(rockblock_st *rb_modem)
{
  //TODO: Implement
}


/**
* @brief Sets the energy monitor value
*
* @param rb_modem Adafruit RockBLOCK modem.
* @param value New value of the energy monitor
*
* @return 
*       - ESP_OK    Success
*/
esp_err_t rockblock_set_energy_monitor(rockblock_st *rb_modem, uint32_t value)
{
  //TODO: Implement
}
