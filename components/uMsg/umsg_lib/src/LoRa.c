// Generated with umsg_gen on 2023-02-26
#include <umsg.h>
#include <umsg_LoRa.h>

// msg instances
static umsg_msg_metadata_t msg_LoRa_received_msg = {.name = "LoRa_received_msg"};
static umsg_msg_metadata_t msg_LoRa_sent_msg = {.name = "LoRa_sent_msg"};
static umsg_msg_metadata_t msg_LoRa_state_msg = {.name = "LoRa_state_msg"};

// msg api's
// LoRa_received_msg
umsg_sub_handle_t umsg_LoRa_received_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_LoRa_received_msg, prescaler, sizeof(umsg_LoRa_received_msg_t), length, 0);
}
umsg_sub_handle_t umsg_LoRa_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_LoRa_received_msg, prescaler, sizeof(umsg_LoRa_received_msg_t), length, channel);
}
void umsg_LoRa_received_msg_publish(umsg_LoRa_received_msg_t* data)
{
    umsg_publish(&msg_LoRa_received_msg, data, 0);
}
void umsg_LoRa_received_msg_publish_ch(umsg_LoRa_received_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_LoRa_received_msg, data, channel);
}
uint8_t umsg_LoRa_received_msg_receive(umsg_sub_handle_t queue, umsg_LoRa_received_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_LoRa_received_msg_peek(umsg_LoRa_received_msg_t* data)
{
    return umsg_peek(&msg_LoRa_received_msg, data, sizeof(umsg_LoRa_received_msg_t));
}

// LoRa_sent_msg
umsg_sub_handle_t umsg_LoRa_sent_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_LoRa_sent_msg, prescaler, sizeof(umsg_LoRa_sent_msg_t), length, 0);
}
umsg_sub_handle_t umsg_LoRa_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_LoRa_sent_msg, prescaler, sizeof(umsg_LoRa_sent_msg_t), length, channel);
}
void umsg_LoRa_sent_msg_publish(umsg_LoRa_sent_msg_t* data)
{
    umsg_publish(&msg_LoRa_sent_msg, data, 0);
}
void umsg_LoRa_sent_msg_publish_ch(umsg_LoRa_sent_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_LoRa_sent_msg, data, channel);
}
uint8_t umsg_LoRa_sent_msg_receive(umsg_sub_handle_t queue, umsg_LoRa_sent_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_LoRa_sent_msg_peek(umsg_LoRa_sent_msg_t* data)
{
    return umsg_peek(&msg_LoRa_sent_msg, data, sizeof(umsg_LoRa_sent_msg_t));
}

// LoRa_state_msg
umsg_sub_handle_t umsg_LoRa_state_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_LoRa_state_msg, prescaler, sizeof(umsg_LoRa_state_msg_t), length, 0);
}
umsg_sub_handle_t umsg_LoRa_state_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_LoRa_state_msg, prescaler, sizeof(umsg_LoRa_state_msg_t), length, channel);
}
void umsg_LoRa_state_msg_publish(umsg_LoRa_state_msg_t* data)
{
    umsg_publish(&msg_LoRa_state_msg, data, 0);
}
void umsg_LoRa_state_msg_publish_ch(umsg_LoRa_state_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_LoRa_state_msg, data, channel);
}
uint8_t umsg_LoRa_state_msg_receive(umsg_sub_handle_t queue, umsg_LoRa_state_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_LoRa_state_msg_peek(umsg_LoRa_state_msg_t* data)
{
    return umsg_peek(&msg_LoRa_state_msg, data, sizeof(umsg_LoRa_state_msg_t));
}

