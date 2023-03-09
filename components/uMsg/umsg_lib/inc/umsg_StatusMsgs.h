// Generated with umsg_gen on 2023-03-03
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    ERROR,
    INFO
} umsg_StatusMsgs_msg_type_t;

typedef struct
{
    char task_name[8];
    umsg_StatusMsgs_msg_type_t msg_type;
    uint32_t tick_time;
    char msg[256];
} umsg_StatusMsgs_msg_t;

// api function headers
umsg_sub_handle_t umsg_StatusMsgs_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_StatusMsgs_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_StatusMsgs_msg_publish(umsg_StatusMsgs_msg_t* data);
void umsg_StatusMsgs_msg_publish_ch(umsg_StatusMsgs_msg_t* data, uint8_t channel);
uint8_t umsg_StatusMsgs_msg_receive(umsg_sub_handle_t queue, umsg_StatusMsgs_msg_t* data, uint32_t timeout);
uint8_t umsg_StatusMsgs_msg_peek(umsg_StatusMsgs_msg_t* data);

