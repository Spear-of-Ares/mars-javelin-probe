// Generated with umsg_gen on 2023-01-20
#pragma once
#include <umsg_types.h>

// msg structure typedefs
typedef struct
{
    char task_name[8];
    uint32_t run_time;
    uint32_t cpu_usage;
    uint32_t stack_hwm;
} umsg_Stats_task_run_time_stats_t;

typedef struct
{
    uint32_t free_heap_size;
} umsg_Stats_system_run_time_stats_t;

// api function headers
umsg_sub_handle_t umsg_Stats_task_run_time_stats_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Stats_task_run_time_stats_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Stats_task_run_time_stats_publish(umsg_Stats_task_run_time_stats_t* data);
void umsg_Stats_task_run_time_stats_publish_ch(umsg_Stats_task_run_time_stats_t* data, uint8_t channel);
uint8_t umsg_Stats_task_run_time_stats_receive(umsg_sub_handle_t queue, umsg_Stats_task_run_time_stats_t* data, uint32_t timeout);
uint8_t umsg_Stats_task_run_time_stats_peek(umsg_Stats_task_run_time_stats_t* data);

umsg_sub_handle_t umsg_Stats_system_run_time_stats_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Stats_system_run_time_stats_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Stats_system_run_time_stats_publish(umsg_Stats_system_run_time_stats_t* data);
void umsg_Stats_system_run_time_stats_publish_ch(umsg_Stats_system_run_time_stats_t* data, uint8_t channel);
uint8_t umsg_Stats_system_run_time_stats_receive(umsg_sub_handle_t queue, umsg_Stats_system_run_time_stats_t* data, uint32_t timeout);
uint8_t umsg_Stats_system_run_time_stats_peek(umsg_Stats_system_run_time_stats_t* data);

