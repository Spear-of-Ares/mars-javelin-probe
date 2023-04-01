// Generated with umsg_gen on 2023-03-23
#include <umsg.h>
#include <umsg_RFD.h>

// msg instances
static umsg_msg_metadata_t msg_RFD_received_msg = {.name = "RFD_received_msg"};
static umsg_msg_metadata_t msg_RFD_sent_msg = {.name = "RFD_sent_msg"};
static umsg_msg_metadata_t msg_RFD_state = {.name = "RFD_state"};

// msg api's
// RFD_received_msg
umsg_sub_handle_t umsg_RFD_received_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_RFD_received_msg, prescaler, sizeof(umsg_RFD_received_msg_t), length, 0);
}
umsg_sub_handle_t umsg_RFD_received_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_RFD_received_msg, prescaler, sizeof(umsg_RFD_received_msg_t), length, channel);
}
void umsg_RFD_received_msg_publish(umsg_RFD_received_msg_t* data)
{
    umsg_publish(&msg_RFD_received_msg, data, 0);
}
void umsg_RFD_received_msg_publish_ch(umsg_RFD_received_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_RFD_received_msg, data, channel);
}
uint8_t umsg_RFD_received_msg_receive(umsg_sub_handle_t queue, umsg_RFD_received_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_RFD_received_msg_peek(umsg_RFD_received_msg_t* data)
{
    return umsg_peek(&msg_RFD_received_msg, data, sizeof(umsg_RFD_received_msg_t));
}

// RFD_sent_msg
umsg_sub_handle_t umsg_RFD_sent_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_RFD_sent_msg, prescaler, sizeof(umsg_RFD_sent_msg_t), length, 0);
}
umsg_sub_handle_t umsg_RFD_sent_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_RFD_sent_msg, prescaler, sizeof(umsg_RFD_sent_msg_t), length, channel);
}
void umsg_RFD_sent_msg_publish(umsg_RFD_sent_msg_t* data)
{
    umsg_publish(&msg_RFD_sent_msg, data, 0);
}
void umsg_RFD_sent_msg_publish_ch(umsg_RFD_sent_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_RFD_sent_msg, data, channel);
}
uint8_t umsg_RFD_sent_msg_receive(umsg_sub_handle_t queue, umsg_RFD_sent_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_RFD_sent_msg_peek(umsg_RFD_sent_msg_t* data)
{
    return umsg_peek(&msg_RFD_sent_msg, data, sizeof(umsg_RFD_sent_msg_t));
}

// RFD_state
umsg_sub_handle_t umsg_RFD_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_RFD_state, prescaler, sizeof(umsg_RFD_state_t), length, 0);
}
umsg_sub_handle_t umsg_RFD_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_RFD_state, prescaler, sizeof(umsg_RFD_state_t), length, channel);
}
void umsg_RFD_state_publish(umsg_RFD_state_t* data)
{
    umsg_publish(&msg_RFD_state, data, 0);
}
void umsg_RFD_state_publish_ch(umsg_RFD_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_RFD_state, data, channel);
}
uint8_t umsg_RFD_state_receive(umsg_sub_handle_t queue, umsg_RFD_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_RFD_state_peek(umsg_RFD_state_t* data)
{
    return umsg_peek(&msg_RFD_state, data, sizeof(umsg_RFD_state_t));
}

