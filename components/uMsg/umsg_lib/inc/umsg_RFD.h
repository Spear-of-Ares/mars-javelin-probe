// Generated with umsg_gen on 2023-03-23
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    RFD_OK,
    RFD_SLEEPING,
    RFD_OFF,
    RFD_DISCONNECTED
} umsg_RFD_rfd_state_t;

typedef enum
{
    RFD_MSG_GPS_DATA,
    RFD_MSG_RESPONSE,
    RFD_MSG_COMMAND,
    RFD_MSG_OTHER
} umsg_RFD_msg_type_t;

typedef struct
{
    uint32_t receive_tick;
    umsg_RFD_msg_type_t recv_msg_type;
    uint8_t msg_data[512];
} umsg_RFD_received_msg_t;

typedef struct
{
    uint32_t send_tick;
    uint8_t initializing : 1, initialized : 1, running : 1;
    umsg_RFD_msg_type_t sent_msg_type;
} umsg_RFD_sent_msg_t;

typedef struct
{
    umsg_RFD_rfd_state_t state;
    uint8_t initializing : 1, initialized : 1, running : 1;
    uint32_t measure_tick;
} umsg_RFD_state_t;

// api function headers
umsg_sub_handle_t umsg_RFD_received_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_RFD_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_RFD_received_msg_publish(umsg_RFD_received_msg_t* data);
void umsg_RFD_received_msg_publish_ch(umsg_RFD_received_msg_t* data, uint8_t channel);
uint8_t umsg_RFD_received_msg_receive(umsg_sub_handle_t queue, umsg_RFD_received_msg_t* data, uint32_t timeout);
uint8_t umsg_RFD_received_msg_peek(umsg_RFD_received_msg_t* data);

umsg_sub_handle_t umsg_RFD_sent_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_RFD_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_RFD_sent_msg_publish(umsg_RFD_sent_msg_t* data);
void umsg_RFD_sent_msg_publish_ch(umsg_RFD_sent_msg_t* data, uint8_t channel);
uint8_t umsg_RFD_sent_msg_receive(umsg_sub_handle_t queue, umsg_RFD_sent_msg_t* data, uint32_t timeout);
uint8_t umsg_RFD_sent_msg_peek(umsg_RFD_sent_msg_t* data);

umsg_sub_handle_t umsg_RFD_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_RFD_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_RFD_state_publish(umsg_RFD_state_t* data);
void umsg_RFD_state_publish_ch(umsg_RFD_state_t* data, uint8_t channel);
uint8_t umsg_RFD_state_receive(umsg_sub_handle_t queue, umsg_RFD_state_t* data, uint32_t timeout);
uint8_t umsg_RFD_state_peek(umsg_RFD_state_t* data);

