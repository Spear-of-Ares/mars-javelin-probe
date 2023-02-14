// Generated with umsg_gen on 2023-02-13
#include <umsg.h>
#include <umsg_CommandCenter.h>

// msg instances
static umsg_msg_metadata_t msg_CommandCenter_command = {.name = "CommandCenter_command"};

// msg api's
// CommandCenter_command
umsg_sub_handle_t umsg_CommandCenter_command_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_CommandCenter_command, prescaler, sizeof(umsg_CommandCenter_command_t), length, 0);
}
umsg_sub_handle_t umsg_CommandCenter_command_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_CommandCenter_command, prescaler, sizeof(umsg_CommandCenter_command_t), length, channel);
}
void umsg_CommandCenter_command_publish(umsg_CommandCenter_command_t* data)
{
    umsg_publish(&msg_CommandCenter_command, data, 0);
}
void umsg_CommandCenter_command_publish_ch(umsg_CommandCenter_command_t* data, uint8_t channel)
{
    umsg_publish(&msg_CommandCenter_command, data, channel);
}
uint8_t umsg_CommandCenter_command_receive(umsg_sub_handle_t queue, umsg_CommandCenter_command_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_CommandCenter_command_peek(umsg_CommandCenter_command_t* data)
{
    return umsg_peek(&msg_CommandCenter_command, data, sizeof(umsg_CommandCenter_command_t));
}

