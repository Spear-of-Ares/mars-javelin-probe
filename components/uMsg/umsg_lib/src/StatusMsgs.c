// Generated with umsg_gen on 2023-01-20
#include <umsg.h>
#include <umsg_StatusMsgs.h>

// msg instances
static umsg_msg_metadata_t msg_StatusMsgs_msg = {.name = "StatusMsgs_msg"};

// msg api's
// StatusMsgs_msg
umsg_sub_handle_t umsg_StatusMsgs_msg_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_StatusMsgs_msg, prescaler, sizeof(umsg_StatusMsgs_msg_t), length, 0);
}
umsg_sub_handle_t umsg_StatusMsgs_msg_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_StatusMsgs_msg, prescaler, sizeof(umsg_StatusMsgs_msg_t), length, channel);
}
void umsg_StatusMsgs_msg_publish(umsg_StatusMsgs_msg_t* data)
{
    umsg_publish(&msg_StatusMsgs_msg, data, 0);
}
void umsg_StatusMsgs_msg_publish_ch(umsg_StatusMsgs_msg_t* data, uint8_t channel)
{
    umsg_publish(&msg_StatusMsgs_msg, data, channel);
}
uint8_t umsg_StatusMsgs_msg_receive(umsg_sub_handle_t queue, umsg_StatusMsgs_msg_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_StatusMsgs_msg_peek(umsg_StatusMsgs_msg_t* data)
{
    return umsg_peek(&msg_StatusMsgs_msg, data, sizeof(umsg_StatusMsgs_msg_t));
}

