// Generated with umsg_gen on 2023-02-13
#include <umsg.h>
#include <umsg_Iridium.h>

// msg instances
static umsg_msg_metadata_t msg_Iridium_received_msg = {.name = "Iridium_received_msg"};
static umsg_msg_metadata_t msg_Iridium_sent_msg = {.name = "Iridium_sent_msg"};
static umsg_msg_metadata_t msg_Iridium_state = {.name = "Iridium_state"};

// msg api's
// Iridium_received_msg
umsg_sub_handle_t umsg_Iridium_received_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Iridium_received_msg, prescaler, sizeof(umsg_Iridium_received_msg_t), length, 0);
}
umsg_sub_handle_t umsg_Iridium_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Iridium_received_msg, prescaler, sizeof(umsg_Iridium_received_msg_t), length, channel);
}
void umsg_Iridium_received_msg_publish(umsg_Iridium_received_msg_t* data)
{
    umsg_publish(&msg_Iridium_received_msg, data, 0);
}
void umsg_Iridium_received_msg_publish_ch(umsg_Iridium_received_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_Iridium_received_msg, data, channel);
}
uint8_t umsg_Iridium_received_msg_receive(umsg_sub_handle_t queue, umsg_Iridium_received_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Iridium_received_msg_peek(umsg_Iridium_received_msg_t* data)
{
    return umsg_peek(&msg_Iridium_received_msg, data, sizeof(umsg_Iridium_received_msg_t));
}

// Iridium_sent_msg
umsg_sub_handle_t umsg_Iridium_sent_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Iridium_sent_msg, prescaler, sizeof(umsg_Iridium_sent_msg_t), length, 0);
}
umsg_sub_handle_t umsg_Iridium_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Iridium_sent_msg, prescaler, sizeof(umsg_Iridium_sent_msg_t), length, channel);
}
void umsg_Iridium_sent_msg_publish(umsg_Iridium_sent_msg_t* data)
{
    umsg_publish(&msg_Iridium_sent_msg, data, 0);
}
void umsg_Iridium_sent_msg_publish_ch(umsg_Iridium_sent_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_Iridium_sent_msg, data, channel);
}
uint8_t umsg_Iridium_sent_msg_receive(umsg_sub_handle_t queue, umsg_Iridium_sent_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Iridium_sent_msg_peek(umsg_Iridium_sent_msg_t* data)
{
    return umsg_peek(&msg_Iridium_sent_msg, data, sizeof(umsg_Iridium_sent_msg_t));
}

// Iridium_state
umsg_sub_handle_t umsg_Iridium_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Iridium_state, prescaler, sizeof(umsg_Iridium_state_t), length, 0);
}
umsg_sub_handle_t umsg_Iridium_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Iridium_state, prescaler, sizeof(umsg_Iridium_state_t), length, channel);
}
void umsg_Iridium_state_publish(umsg_Iridium_state_t* data)
{
    umsg_publish(&msg_Iridium_state, data, 0);
}
void umsg_Iridium_state_publish_ch(umsg_Iridium_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_Iridium_state, data, channel);
}
uint8_t umsg_Iridium_state_receive(umsg_sub_handle_t queue, umsg_Iridium_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Iridium_state_peek(umsg_Iridium_state_t* data)
{
    return umsg_peek(&msg_Iridium_state, data, sizeof(umsg_Iridium_state_t));
}

