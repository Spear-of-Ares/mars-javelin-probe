// Generated with umsg_gen on 2023-03-23
#include <umsg.h>
#include <umsg_Sensors.h>

// msg instances
static umsg_msg_metadata_t msg_Sensors_imu_configuration = {.name = "Sensors_imu_configuration"};
static umsg_msg_metadata_t msg_Sensors_imu_data = {.name = "Sensors_imu_data"};
static umsg_msg_metadata_t msg_Sensors_imu_state = {.name = "Sensors_imu_state"};
static umsg_msg_metadata_t msg_Sensors_accel_configuration = {.name = "Sensors_accel_configuration"};
static umsg_msg_metadata_t msg_Sensors_accel_data = {.name = "Sensors_accel_data"};
static umsg_msg_metadata_t msg_Sensors_accel_state = {.name = "Sensors_accel_state"};
static umsg_msg_metadata_t msg_Sensors_baro_configuration = {.name = "Sensors_baro_configuration"};
static umsg_msg_metadata_t msg_Sensors_baro_data = {.name = "Sensors_baro_data"};
static umsg_msg_metadata_t msg_Sensors_baro_state = {.name = "Sensors_baro_state"};
static umsg_msg_metadata_t msg_Sensors_thermistor_configuration = {.name = "Sensors_thermistor_configuration"};
static umsg_msg_metadata_t msg_Sensors_thermistor_data = {.name = "Sensors_thermistor_data"};
static umsg_msg_metadata_t msg_Sensors_thermistor_state = {.name = "Sensors_thermistor_state"};

// msg api's
// Sensors_imu_configuration
umsg_sub_handle_t umsg_Sensors_imu_configuration_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_imu_configuration, prescaler, sizeof(umsg_Sensors_imu_configuration_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_imu_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_imu_configuration, prescaler, sizeof(umsg_Sensors_imu_configuration_t), length, channel);
}
void umsg_Sensors_imu_configuration_publish(umsg_Sensors_imu_configuration_t* data)
{
    umsg_publish(&msg_Sensors_imu_configuration, data, 0);
}
void umsg_Sensors_imu_configuration_publish_ch(umsg_Sensors_imu_configuration_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_imu_configuration, data, channel);
}
uint8_t umsg_Sensors_imu_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_configuration_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_imu_configuration_peek(umsg_Sensors_imu_configuration_t* data)
{
    return umsg_peek(&msg_Sensors_imu_configuration, data, sizeof(umsg_Sensors_imu_configuration_t));
}

// Sensors_imu_data
umsg_sub_handle_t umsg_Sensors_imu_data_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_imu_data, prescaler, sizeof(umsg_Sensors_imu_data_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_imu_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_imu_data, prescaler, sizeof(umsg_Sensors_imu_data_t), length, channel);
}
void umsg_Sensors_imu_data_publish(umsg_Sensors_imu_data_t* data)
{
    umsg_publish(&msg_Sensors_imu_data, data, 0);
}
void umsg_Sensors_imu_data_publish_ch(umsg_Sensors_imu_data_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_imu_data, data, channel);
}
uint8_t umsg_Sensors_imu_data_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_data_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_imu_data_peek(umsg_Sensors_imu_data_t* data)
{
    return umsg_peek(&msg_Sensors_imu_data, data, sizeof(umsg_Sensors_imu_data_t));
}

// Sensors_imu_state
umsg_sub_handle_t umsg_Sensors_imu_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_imu_state, prescaler, sizeof(umsg_Sensors_imu_state_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_imu_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_imu_state, prescaler, sizeof(umsg_Sensors_imu_state_t), length, channel);
}
void umsg_Sensors_imu_state_publish(umsg_Sensors_imu_state_t* data)
{
    umsg_publish(&msg_Sensors_imu_state, data, 0);
}
void umsg_Sensors_imu_state_publish_ch(umsg_Sensors_imu_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_imu_state, data, channel);
}
uint8_t umsg_Sensors_imu_state_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_imu_state_peek(umsg_Sensors_imu_state_t* data)
{
    return umsg_peek(&msg_Sensors_imu_state, data, sizeof(umsg_Sensors_imu_state_t));
}

// Sensors_accel_configuration
umsg_sub_handle_t umsg_Sensors_accel_configuration_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_accel_configuration, prescaler, sizeof(umsg_Sensors_accel_configuration_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_accel_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_accel_configuration, prescaler, sizeof(umsg_Sensors_accel_configuration_t), length, channel);
}
void umsg_Sensors_accel_configuration_publish(umsg_Sensors_accel_configuration_t* data)
{
    umsg_publish(&msg_Sensors_accel_configuration, data, 0);
}
void umsg_Sensors_accel_configuration_publish_ch(umsg_Sensors_accel_configuration_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_accel_configuration, data, channel);
}
uint8_t umsg_Sensors_accel_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_configuration_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_accel_configuration_peek(umsg_Sensors_accel_configuration_t* data)
{
    return umsg_peek(&msg_Sensors_accel_configuration, data, sizeof(umsg_Sensors_accel_configuration_t));
}

// Sensors_accel_data
umsg_sub_handle_t umsg_Sensors_accel_data_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_accel_data, prescaler, sizeof(umsg_Sensors_accel_data_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_accel_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_accel_data, prescaler, sizeof(umsg_Sensors_accel_data_t), length, channel);
}
void umsg_Sensors_accel_data_publish(umsg_Sensors_accel_data_t* data)
{
    umsg_publish(&msg_Sensors_accel_data, data, 0);
}
void umsg_Sensors_accel_data_publish_ch(umsg_Sensors_accel_data_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_accel_data, data, channel);
}
uint8_t umsg_Sensors_accel_data_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_data_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_accel_data_peek(umsg_Sensors_accel_data_t* data)
{
    return umsg_peek(&msg_Sensors_accel_data, data, sizeof(umsg_Sensors_accel_data_t));
}

// Sensors_accel_state
umsg_sub_handle_t umsg_Sensors_accel_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_accel_state, prescaler, sizeof(umsg_Sensors_accel_state_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_accel_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_accel_state, prescaler, sizeof(umsg_Sensors_accel_state_t), length, channel);
}
void umsg_Sensors_accel_state_publish(umsg_Sensors_accel_state_t* data)
{
    umsg_publish(&msg_Sensors_accel_state, data, 0);
}
void umsg_Sensors_accel_state_publish_ch(umsg_Sensors_accel_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_accel_state, data, channel);
}
uint8_t umsg_Sensors_accel_state_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_accel_state_peek(umsg_Sensors_accel_state_t* data)
{
    return umsg_peek(&msg_Sensors_accel_state, data, sizeof(umsg_Sensors_accel_state_t));
}

// Sensors_baro_configuration
umsg_sub_handle_t umsg_Sensors_baro_configuration_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_baro_configuration, prescaler, sizeof(umsg_Sensors_baro_configuration_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_baro_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_baro_configuration, prescaler, sizeof(umsg_Sensors_baro_configuration_t), length, channel);
}
void umsg_Sensors_baro_configuration_publish(umsg_Sensors_baro_configuration_t* data)
{
    umsg_publish(&msg_Sensors_baro_configuration, data, 0);
}
void umsg_Sensors_baro_configuration_publish_ch(umsg_Sensors_baro_configuration_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_baro_configuration, data, channel);
}
uint8_t umsg_Sensors_baro_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_configuration_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_baro_configuration_peek(umsg_Sensors_baro_configuration_t* data)
{
    return umsg_peek(&msg_Sensors_baro_configuration, data, sizeof(umsg_Sensors_baro_configuration_t));
}

// Sensors_baro_data
umsg_sub_handle_t umsg_Sensors_baro_data_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_baro_data, prescaler, sizeof(umsg_Sensors_baro_data_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_baro_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_baro_data, prescaler, sizeof(umsg_Sensors_baro_data_t), length, channel);
}
void umsg_Sensors_baro_data_publish(umsg_Sensors_baro_data_t* data)
{
    umsg_publish(&msg_Sensors_baro_data, data, 0);
}
void umsg_Sensors_baro_data_publish_ch(umsg_Sensors_baro_data_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_baro_data, data, channel);
}
uint8_t umsg_Sensors_baro_data_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_data_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_baro_data_peek(umsg_Sensors_baro_data_t* data)
{
    return umsg_peek(&msg_Sensors_baro_data, data, sizeof(umsg_Sensors_baro_data_t));
}

// Sensors_baro_state
umsg_sub_handle_t umsg_Sensors_baro_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_baro_state, prescaler, sizeof(umsg_Sensors_baro_state_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_baro_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_baro_state, prescaler, sizeof(umsg_Sensors_baro_state_t), length, channel);
}
void umsg_Sensors_baro_state_publish(umsg_Sensors_baro_state_t* data)
{
    umsg_publish(&msg_Sensors_baro_state, data, 0);
}
void umsg_Sensors_baro_state_publish_ch(umsg_Sensors_baro_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_baro_state, data, channel);
}
uint8_t umsg_Sensors_baro_state_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_baro_state_peek(umsg_Sensors_baro_state_t* data)
{
    return umsg_peek(&msg_Sensors_baro_state, data, sizeof(umsg_Sensors_baro_state_t));
}

// Sensors_thermistor_configuration
umsg_sub_handle_t umsg_Sensors_thermistor_configuration_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_thermistor_configuration, prescaler, sizeof(umsg_Sensors_thermistor_configuration_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_thermistor_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_thermistor_configuration, prescaler, sizeof(umsg_Sensors_thermistor_configuration_t), length, channel);
}
void umsg_Sensors_thermistor_configuration_publish(umsg_Sensors_thermistor_configuration_t* data)
{
    umsg_publish(&msg_Sensors_thermistor_configuration, data, 0);
}
void umsg_Sensors_thermistor_configuration_publish_ch(umsg_Sensors_thermistor_configuration_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_thermistor_configuration, data, channel);
}
uint8_t umsg_Sensors_thermistor_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_configuration_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_thermistor_configuration_peek(umsg_Sensors_thermistor_configuration_t* data)
{
    return umsg_peek(&msg_Sensors_thermistor_configuration, data, sizeof(umsg_Sensors_thermistor_configuration_t));
}

// Sensors_thermistor_data
umsg_sub_handle_t umsg_Sensors_thermistor_data_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_thermistor_data, prescaler, sizeof(umsg_Sensors_thermistor_data_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_thermistor_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_thermistor_data, prescaler, sizeof(umsg_Sensors_thermistor_data_t), length, channel);
}
void umsg_Sensors_thermistor_data_publish(umsg_Sensors_thermistor_data_t* data)
{
    umsg_publish(&msg_Sensors_thermistor_data, data, 0);
}
void umsg_Sensors_thermistor_data_publish_ch(umsg_Sensors_thermistor_data_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_thermistor_data, data, channel);
}
uint8_t umsg_Sensors_thermistor_data_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_data_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_thermistor_data_peek(umsg_Sensors_thermistor_data_t* data)
{
    return umsg_peek(&msg_Sensors_thermistor_data, data, sizeof(umsg_Sensors_thermistor_data_t));
}

// Sensors_thermistor_state
umsg_sub_handle_t umsg_Sensors_thermistor_state_subscribe(uint32_t prescaler, uint8_t length)
{
    return umsg_subscribe(&msg_Sensors_thermistor_state, prescaler, sizeof(umsg_Sensors_thermistor_state_t), length, 0);
}
umsg_sub_handle_t umsg_Sensors_thermistor_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel)
{
    return umsg_subscribe(&msg_Sensors_thermistor_state, prescaler, sizeof(umsg_Sensors_thermistor_state_t), length, channel);
}
void umsg_Sensors_thermistor_state_publish(umsg_Sensors_thermistor_state_t* data)
{
    umsg_publish(&msg_Sensors_thermistor_state, data, 0);
}
void umsg_Sensors_thermistor_state_publish_ch(umsg_Sensors_thermistor_state_t* data, uint8_t channel)
{
    umsg_publish(&msg_Sensors_thermistor_state, data, channel);
}
uint8_t umsg_Sensors_thermistor_state_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_state_t* data, uint32_t timeout)
{
    return umsg_receive(queue, data, timeout);
}
uint8_t umsg_Sensors_thermistor_state_peek(umsg_Sensors_thermistor_state_t* data)
{
    return umsg_peek(&msg_Sensors_thermistor_state, data, sizeof(umsg_Sensors_thermistor_state_t));
}

