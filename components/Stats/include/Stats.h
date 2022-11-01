
#ifndef __STATS_H__
#define __STATS_H__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "datalogger.h"

#define STATS_TICKS         pdMS_TO_TICKS(1000)
#define ARRAY_SIZE_OFFSET   5   //Increase this if print_real_time_stats returns ESP_ERR_INVALID_SIZE

#define STATS_TASK_ID       "STATS_COMP"

class RunTimeStats{
public:
  RunTimeStats(QueueHandle_t dataOutSD) {
    _dataOutSD = dataOutSD;
  }
  static void stats_task(void *arg);


private: 
  esp_err_t print_real_time_stats(TickType_t xTicksToWait, char* pcWriteBuffer);
  esp_err_t get_real_time_stats(TickType_t xTicksToWait);
  void      get_stats();

  void free_arrays(){ 
    free(_start_array); 
    free(_end_array); 
  }

  TaskStatus_t *_start_array, *_end_array;
  UBaseType_t _start_array_size, _end_array_size;
  uint32_t _start_run_time, _end_run_time, _total_elapsed_time;
  
  QueueHandle_t _dataOutSD;
};

#endif /* __STATS_H__ */
