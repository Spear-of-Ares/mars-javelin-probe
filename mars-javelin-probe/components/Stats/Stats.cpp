#include <stdio.h>
#include "Stats.h"

esp_err_t RunTimeStats::get_real_time_stats(TickType_t xTicksToWait){

    esp_err_t ret;
    //Allocate array to store current task states
    _start_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    _start_array = (TaskStatus_t*)malloc(sizeof(TaskStatus_t) * _start_array_size);
    if (_start_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        free_arrays();
        return ret;
    }
    //Get current task states
    _start_array_size = uxTaskGetSystemState(_start_array, _start_array_size, &_start_run_time);
    if (_start_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        free_arrays();
        return ret;
    }

    vTaskDelay(xTicksToWait);

    //Allocate array to store tasks states post delay
    _end_array_size = uxTaskGetNumberOfTasks() + ARRAY_SIZE_OFFSET;
    _end_array = (TaskStatus_t*)malloc(sizeof(TaskStatus_t) * _end_array_size);
    if (_end_array == NULL) {
        ret = ESP_ERR_NO_MEM;
        free_arrays();
        return ret;
    }
    //Get post delay task states
    _end_array_size = uxTaskGetSystemState(_end_array, _end_array_size, &_end_run_time);
    if (_end_array_size == 0) {
        ret = ESP_ERR_INVALID_SIZE;
        free_arrays();
        return ret;
    }

    //Calculate total_elapsed_time in units of run time stats clock period.
    _total_elapsed_time = (_end_run_time - _start_run_time);
    if (_total_elapsed_time == 0) {
        ret = ESP_ERR_INVALID_STATE;
        free_arrays();
        return ret;
    }
    ret = ESP_OK;
    return ret;
}

esp_err_t RunTimeStats::print_real_time_stats(TickType_t xTicksToWait, char *pcWriteBuffer)
{
    esp_err_t ret = get_real_time_stats(xTicksToWait);
    if ( ret != ESP_OK){
      return ret; 
    }

    sprintf(pcWriteBuffer, "| Task | Run Time | Percentage | Stack HWM \n");
    //Match each task in _start_array to those in the end_array
    for (int i = 0; i < _start_array_size; i++) {
        int k = -1;
        for (int j = 0; j < _end_array_size; j++) {
            if (_start_array[i].xHandle == _end_array[j].xHandle) {
                k = j;
                //Mark that task have been matched by overwriting their handles
                _start_array[i].xHandle = NULL;
                _end_array[j].xHandle = NULL;
                break;
            }
        }
        //Check if matching task found
        if (k >= 0) {
            uint32_t task_elapsed_time = _end_array[k].ulRunTimeCounter - _start_array[i].ulRunTimeCounter;
            uint32_t percentage_time = (task_elapsed_time * 100UL) / (_total_elapsed_time * portNUM_PROCESSORS);
            uint32_t stack_usage = (_start_array[i].usStackHighWaterMark + _end_array[k].usStackHighWaterMark) / 2;
            sprintf(pcWriteBuffer + strlen(pcWriteBuffer), "| %s | %d | %d%% | %d\n", _start_array[i].pcTaskName, task_elapsed_time, percentage_time, stack_usage);
        }
    }

    //Print unmatched tasks
    for (int i = 0; i < _start_array_size; i++) {
        if (_start_array[i].xHandle != NULL) {
            sprintf(pcWriteBuffer + strlen(pcWriteBuffer), "| %s | Deleted\n", _start_array[i].pcTaskName);
        }
    }
    for (int i = 0; i < _end_array_size; i++) {
        if (_end_array[i].xHandle != NULL) {
            sprintf(pcWriteBuffer + strlen(pcWriteBuffer), "| %s | Created\n", _end_array[i].pcTaskName);
        }
    }
    sprintf(pcWriteBuffer + strlen(pcWriteBuffer), "| Free Heap Size: %d\n", xPortGetFreeHeapSize());
    free_arrays();
    return ret; // Will return ESP_OK due to check at top
}

void RunTimeStats::get_stats(){
  char pcWriteBuffer[1028];

  if (print_real_time_stats(STATS_TICKS, pcWriteBuffer) == ESP_OK) {
    SDData *send_data = new SDData();
    send_data->file_name = new std::string("Stats Component");
    send_data->message = new std::string(pcWriteBuffer);

    if(xQueueSend(_dataOutSD, &(send_data), 10/portTICK_PERIOD_MS) != pdTRUE){
      printf("Failed to post stats data\n");
    }
  } else {
    printf("Error getting real time stats\n");
  }
}

void RunTimeStats::stats_task(void *arg)
{
    RunTimeStats rt_stats = *((RunTimeStats*)arg);
    //Print real time stats periodically
    while (1) {
      rt_stats.get_stats();      
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
