// Generated with umsg_gen on 2023-03-23
#include <umsg.h>
#include <umsg_Stats.h>

// msg instances
static umsg_msg_metadata_t msg_Stats_task_run_time_stats = {.name = "Stats_task_run_time_stats"};
static umsg_msg_metadata_t msg_Stats_system_run_time_stats = {.name = "Stats_system_run_time_stats"};

// msg api's
// Stats_task_run_time_stats
umsg_sub_handle_t umsg_Stats_task_run_time_stats_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Stats_task_run_time_stats, prescaler, sizeof(umsg_Stats_task_run_time_stats_t), length, 0);
}
umsg_sub_handle_t umsg_Stats_task_run_time_stats_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Stats_task_run_time_stats, prescaler, sizeof(umsg_Stats_task_run_time_stats_t), length, channel);
}
void umsg_Stats_task_run_time_stats_publish(umsg_Stats_task_run_time_stats_t* data)
{
    umsg_publish(&msg_Stats_task_run_time_stats, data, 0);
}
void umsg_Stats_task_run_time_stats_publish_ch(umsg_Stats_task_run_time_stats_t* data, uint8_t channel)
{
    umsg_publish(&msg_Stats_task_run_time_stats, data, channel);
}
uint8_t umsg_Stats_task_run_time_stats_receive(umsg_sub_handle_t queue, umsg_Stats_task_run_time_stats_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Stats_task_run_time_stats_peek(umsg_Stats_task_run_time_stats_t* data)
{
    return umsg_peek(&msg_Stats_task_run_time_stats, data, sizeof(umsg_Stats_task_run_time_stats_t));
}

// Stats_system_run_time_stats
umsg_sub_handle_t umsg_Stats_system_run_time_stats_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Stats_system_run_time_stats, prescaler, sizeof(umsg_Stats_system_run_time_stats_t), length, 0);
}
umsg_sub_handle_t umsg_Stats_system_run_time_stats_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Stats_system_run_time_stats, prescaler, sizeof(umsg_Stats_system_run_time_stats_t), length, channel);
}
void umsg_Stats_system_run_time_stats_publish(umsg_Stats_system_run_time_stats_t* data)
{
    umsg_publish(&msg_Stats_system_run_time_stats, data, 0);
}
void umsg_Stats_system_run_time_stats_publish_ch(umsg_Stats_system_run_time_stats_t* data, uint8_t channel)
{
    umsg_publish(&msg_Stats_system_run_time_stats, data, channel);
}
uint8_t umsg_Stats_system_run_time_stats_receive(umsg_sub_handle_t queue, umsg_Stats_system_run_time_stats_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Stats_system_run_time_stats_peek(umsg_Stats_system_run_time_stats_t* data)
{
    return umsg_peek(&msg_Stats_system_run_time_stats, data, sizeof(umsg_Stats_system_run_time_stats_t));
}

