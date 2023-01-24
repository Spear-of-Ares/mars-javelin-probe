// Generated with umsg_gen on 2023-01-20
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    IRIDIUM_OK,
    IRIDIUM_SLEEPING,
    IRIDIUM_OFF,
    IRIDIUM_DISCONNECTED
} umsg_Iridium_iridium_state_t;

typedef enum
{
    MSG_GPS_DATA,
    MSG_RESPONSE,
    MSG_COMMAND,
    MSG_OTHER
} umsg_Iridium_msg_type_t;

typedef struct
{
    uint32_t receive_tick;
    umsg_Iridium_msg_type_t recv_msg_type;
    int32_t time_ymd_hms[6];
    uint8_t msg_data[512];
} umsg_Iridium_received_msg_t;

typedef struct
{
    uint32_t send_tick;
    umsg_Iridium_msg_type_t sent_msg_type;
} umsg_Iridium_sent_msg_t;

typedef struct
{
    umsg_Iridium_iridium_state_t state;
} umsg_Iridium_state_t;

// api function headers
umsg_sub_handle_t umsg_Iridium_received_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Iridium_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Iridium_received_msg_publish(umsg_Iridium_received_msg_t* data);
void umsg_Iridium_received_msg_publish_ch(umsg_Iridium_received_msg_t* data, uint8_t channel);
uint8_t umsg_Iridium_received_msg_receive(umsg_sub_handle_t queue, umsg_Iridium_received_msg_t* data, uint32_t timeout);
uint8_t umsg_Iridium_received_msg_peek(umsg_Iridium_received_msg_t* data);

umsg_sub_handle_t umsg_Iridium_sent_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Iridium_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Iridium_sent_msg_publish(umsg_Iridium_sent_msg_t* data);
void umsg_Iridium_sent_msg_publish_ch(umsg_Iridium_sent_msg_t* data, uint8_t channel);
uint8_t umsg_Iridium_sent_msg_receive(umsg_sub_handle_t queue, umsg_Iridium_sent_msg_t* data, uint32_t timeout);
uint8_t umsg_Iridium_sent_msg_peek(umsg_Iridium_sent_msg_t* data);

umsg_sub_handle_t umsg_Iridium_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Iridium_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Iridium_state_publish(umsg_Iridium_state_t* data);
void umsg_Iridium_state_publish_ch(umsg_Iridium_state_t* data, uint8_t channel);
uint8_t umsg_Iridium_state_receive(umsg_sub_handle_t queue, umsg_Iridium_state_t* data, uint32_t timeout);
uint8_t umsg_Iridium_state_peek(umsg_Iridium_state_t* data);

