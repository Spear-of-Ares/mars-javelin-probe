// Generated with umsg_gen on 2023-03-23
#pragma once
#include <umsg_types.h>

// msg structure typedefs
typedef struct
{
    uint8_t cutdown : 1, low_alt_data_gather : 1, self_test : 1, start_system : 1;
    uint32_t measure_tick;
} umsg_CommandCenter_command_t;

// api function headers
umsg_sub_handle_t umsg_CommandCenter_command_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_CommandCenter_command_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_CommandCenter_command_publish(umsg_CommandCenter_command_t* data);
void umsg_CommandCenter_command_publish_ch(umsg_CommandCenter_command_t* data, uint8_t channel);
uint8_t umsg_CommandCenter_command_receive(umsg_sub_handle_t queue, umsg_CommandCenter_command_t* data, uint32_t timeout);
uint8_t umsg_CommandCenter_command_peek(umsg_CommandCenter_command_t* data);

