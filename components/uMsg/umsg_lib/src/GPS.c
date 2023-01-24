// Generated with umsg_gen on 2023-01-20
#include <umsg.h>
#include <umsg_GPS.h>

// msg instances
static umsg_msg_metadata_t msg_GPS_gps_configuration = {.name = "GPS_gps_configuration"};
static umsg_msg_metadata_t msg_GPS_gps_data = {.name = "GPS_gps_data"};
static umsg_msg_metadata_t msg_GPS_state = {.name = "GPS_state"};

// msg api's
// GPS_gps_configuration
umsg_sub_handle_t umsg_GPS_gps_configuration_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_GPS_gps_configuration, prescaler, sizeof(umsg_GPS_gps_configuration_t), length, 0);
}
umsg_sub_handle_t umsg_GPS_gps_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_GPS_gps_configuration, prescaler, sizeof(umsg_GPS_gps_configuration_t), length, channel);
}
void umsg_GPS_gps_configuration_publish(umsg_GPS_gps_configuration_t* data)
{
    umsg_publish(&msg_GPS_gps_configuration, data, 0);
}
void umsg_GPS_gps_configuration_publish_ch(umsg_GPS_gps_configuration_t* data, uint8_t channel)
{
    umsg_publish(&msg_GPS_gps_configuration, data, channel);
}
uint8_t umsg_GPS_gps_configuration_receive(umsg_sub_handle_t queue, umsg_GPS_gps_configuration_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_GPS_gps_configuration_peek(umsg_GPS_gps_configuration_t* data)
{
    return umsg_peek(&msg_GPS_gps_configuration, data, sizeof(umsg_GPS_gps_configuration_t));
}

// GPS_gps_data
umsg_sub_handle_t umsg_GPS_gps_data_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_GPS_gps_data, prescaler, sizeof(umsg_GPS_gps_data_t), length, 0);
}
umsg_sub_handle_t umsg_GPS_gps_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_GPS_gps_data, prescaler, sizeof(umsg_GPS_gps_data_t), length, channel);
}
void umsg_GPS_gps_data_publish(umsg_GPS_gps_data_t* data)
{
    umsg_publish(&msg_GPS_gps_data, data, 0);
}
void umsg_GPS_gps_data_publish_ch(umsg_GPS_gps_data_t* data, uint8_t channel)
{
    umsg_publish(&msg_GPS_gps_data, data, channel);
}
uint8_t umsg_GPS_gps_data_receive(umsg_sub_handle_t queue, umsg_GPS_gps_data_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_GPS_gps_data_peek(umsg_GPS_gps_data_t* data)
{
    return umsg_peek(&msg_GPS_gps_data, data, sizeof(umsg_GPS_gps_data_t));
}

// GPS_state
umsg_sub_handle_t umsg_GPS_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_GPS_state, prescaler, sizeof(umsg_GPS_state_t), length, 0);
}
umsg_sub_handle_t umsg_GPS_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_GPS_state, prescaler, sizeof(umsg_GPS_state_t), length, channel);
}
void umsg_GPS_state_publish(umsg_GPS_state_t* data)
{
    umsg_publish(&msg_GPS_state, data, 0);
}
void umsg_GPS_state_publish_ch(umsg_GPS_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_GPS_state, data, channel);
}
uint8_t umsg_GPS_state_receive(umsg_sub_handle_t queue, umsg_GPS_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_GPS_state_peek(umsg_GPS_state_t* data)
{
    return umsg_peek(&msg_GPS_state, data, sizeof(umsg_GPS_state_t));
}

