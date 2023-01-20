// Generated with umsg_gen on 2022-12-21
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    LORA_OK,
    LORA_SLEEPING,
    LORA_OFF,
    LORA_DISCONNECTED
} umsg_LoRa_LoRa_state_t;

typedef enum
{
    MSG_SENSOR_DATA,
    MSG_GPS_DATA,
    MSG_RESPONSE,
    MSG_COMMAND,
    MSG_OTHER
} umsg_LoRa_msg_type_t;

typedef struct
{
    uint32_t receive_tick;
    umsg_LoRa_msg_type_t recv_msg_type;
    int32_t RSSI;
    float Signal To Noise;
    int64_t Frequency Error;
    uint8_t msg_data[10];
} umsg_LoRa_received_msg_t;

typedef struct
{
    uint32_t send_tick;
    umsg_LoRa_msg_type_t sent_msg_type;
} umsg_LoRa_sent_msg_t;

typedef struct
{
    umsg_LoRa_lora_state_t state;
} umsg_LoRa_LoRa_state_t;

// api function headers
umsg_sub_handle_t umsg_LoRa_received_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_LoRa_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_LoRa_received_msg_publish(umsg_LoRa_received_msg_t* data);
void umsg_LoRa_received_msg_publish_ch(umsg_LoRa_received_msg_t* data, uint8_t channel);
uint8_t umsg_LoRa_received_msg_receive(umsg_sub_handle_t queue, umsg_LoRa_received_msg_t* data, uint32_t timeout);
uint8_t umsg_LoRa_received_msg_peek(umsg_LoRa_received_msg_t* data);

umsg_sub_handle_t umsg_LoRa_sent_msg_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_LoRa_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_LoRa_sent_msg_publish(umsg_LoRa_sent_msg_t* data);
void umsg_LoRa_sent_msg_publish_ch(umsg_LoRa_sent_msg_t* data, uint8_t channel);
uint8_t umsg_LoRa_sent_msg_receive(umsg_sub_handle_t queue, umsg_LoRa_sent_msg_t* data, uint32_t timeout);
uint8_t umsg_LoRa_sent_msg_peek(umsg_LoRa_sent_msg_t* data);

umsg_sub_handle_t umsg_LoRa_LoRa_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_LoRa_LoRa_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_LoRa_LoRa_state_publish(umsg_LoRa_LoRa_state_t* data);
void umsg_LoRa_LoRa_state_publish_ch(umsg_LoRa_LoRa_state_t* data, uint8_t channel);
uint8_t umsg_LoRa_LoRa_state_receive(umsg_sub_handle_t queue, umsg_LoRa_LoRa_state_t* data, uint32_t timeout);
uint8_t umsg_LoRa_LoRa_state_peek(umsg_LoRa_LoRa_state_t* data);

