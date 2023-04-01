// Generated with umsg_gen on 2023-03-23
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    SENSOR_OK,
    SENSOR_SLEEPING,
    SENSOR_OFF,
    SENSOR_DISCONNECTED
} umsg_Sensors_sensor_state_t;

typedef enum
{
    RAW,
    NORMALIZED,
    SCALED
} umsg_Sensors_imu_processing_level_t;

typedef enum
{
    G_16,
    G_8,
    G_4,
    G_2
} umsg_Sensors_imu_accelerometer_range_t;

typedef enum
{
    DPS_2000,
    DPS_1000,
    DPS_500,
    DPS_250
} umsg_Sensors_imu_gyroscope_scale_t;

typedef struct
{
    uint16_t sample_rate_hz;
    umsg_Sensors_imu_accelerometer_range_t accelerometer_range;
    umsg_Sensors_imu_gyroscope_scale_t gyroscope_scale;
    uint32_t measure_tick;
} umsg_Sensors_imu_configuration_t;

typedef struct
{
    double accelerometer[3];
    double gyroscope[3];
    double magnetometer[3];
    double attitude[3];
    double linear_accel[3];
    double gravity[3];
    double velocity[3];
    double position[3];
    double angle_of_attack;
    double vertical_speed;
    double horizontal_speed;
    double speed;
    uint8_t temperature_c;
    uint32_t measure_tick;
} umsg_Sensors_imu_data_t;

typedef struct
{
    umsg_Sensors_sensor_state_t state;
    uint8_t calibration_state[3];
    uint8_t initializing : 1, initialized : 1, running : 1;
    uint32_t measure_tick;
} umsg_Sensors_imu_state_t;

typedef struct
{
    uint16_t sample_rate_hz;
    uint16_t accelerometer_range;
    uint32_t measure_tick;
} umsg_Sensors_accel_configuration_t;

typedef struct
{
    double acceleration[3];
    uint32_t measure_tick;
} umsg_Sensors_accel_data_t;

typedef struct
{
    umsg_Sensors_sensor_state_t state;
    uint8_t initializing : 1, initialized : 1, running : 1;
    uint32_t measure_tick;
} umsg_Sensors_accel_state_t;

typedef struct
{
    float start_pressure_hpa;
    float sea_level_pressure_hpa;
    float start_alt_m;
    uint32_t measure_tick;
} umsg_Sensors_baro_configuration_t;

typedef struct
{
    float pressure_pa;
    float temperature_c;
    float gained_alt_m;
    float alt_above_sea_m;
    uint32_t measure_tick;
} umsg_Sensors_baro_data_t;

typedef struct
{
    umsg_Sensors_sensor_state_t state;
    uint8_t initializing : 1, initialized : 1, running : 1;
    uint32_t measure_tick;
} umsg_Sensors_baro_state_t;

typedef struct
{
    float R1_value;
    float voltage_source_value;
    float TO_value;
    float RO_value;
    float B_value;
    uint32_t measure_tick;
} umsg_Sensors_thermistor_configuration_t;

typedef struct
{
    float temperature_c;
    uint32_t measure_tick;
} umsg_Sensors_thermistor_data_t;

typedef struct
{
    umsg_Sensors_sensor_state_t state;
    uint8_t initializing : 1, initialized : 1, running : 1;
    uint32_t measure_tick;
} umsg_Sensors_thermistor_state_t;

// api function headers
umsg_sub_handle_t umsg_Sensors_imu_configuration_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_imu_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_imu_configuration_publish(umsg_Sensors_imu_configuration_t* data);
void umsg_Sensors_imu_configuration_publish_ch(umsg_Sensors_imu_configuration_t* data, uint8_t channel);
uint8_t umsg_Sensors_imu_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_configuration_t* data, uint32_t timeout);
uint8_t umsg_Sensors_imu_configuration_peek(umsg_Sensors_imu_configuration_t* data);

umsg_sub_handle_t umsg_Sensors_imu_data_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_imu_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_imu_data_publish(umsg_Sensors_imu_data_t* data);
void umsg_Sensors_imu_data_publish_ch(umsg_Sensors_imu_data_t* data, uint8_t channel);
uint8_t umsg_Sensors_imu_data_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_data_t* data, uint32_t timeout);
uint8_t umsg_Sensors_imu_data_peek(umsg_Sensors_imu_data_t* data);

umsg_sub_handle_t umsg_Sensors_imu_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_imu_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_imu_state_publish(umsg_Sensors_imu_state_t* data);
void umsg_Sensors_imu_state_publish_ch(umsg_Sensors_imu_state_t* data, uint8_t channel);
uint8_t umsg_Sensors_imu_state_receive(umsg_sub_handle_t queue, umsg_Sensors_imu_state_t* data, uint32_t timeout);
uint8_t umsg_Sensors_imu_state_peek(umsg_Sensors_imu_state_t* data);

umsg_sub_handle_t umsg_Sensors_accel_configuration_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_accel_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_accel_configuration_publish(umsg_Sensors_accel_configuration_t* data);
void umsg_Sensors_accel_configuration_publish_ch(umsg_Sensors_accel_configuration_t* data, uint8_t channel);
uint8_t umsg_Sensors_accel_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_configuration_t* data, uint32_t timeout);
uint8_t umsg_Sensors_accel_configuration_peek(umsg_Sensors_accel_configuration_t* data);

umsg_sub_handle_t umsg_Sensors_accel_data_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_accel_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_accel_data_publish(umsg_Sensors_accel_data_t* data);
void umsg_Sensors_accel_data_publish_ch(umsg_Sensors_accel_data_t* data, uint8_t channel);
uint8_t umsg_Sensors_accel_data_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_data_t* data, uint32_t timeout);
uint8_t umsg_Sensors_accel_data_peek(umsg_Sensors_accel_data_t* data);

umsg_sub_handle_t umsg_Sensors_accel_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_accel_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_accel_state_publish(umsg_Sensors_accel_state_t* data);
void umsg_Sensors_accel_state_publish_ch(umsg_Sensors_accel_state_t* data, uint8_t channel);
uint8_t umsg_Sensors_accel_state_receive(umsg_sub_handle_t queue, umsg_Sensors_accel_state_t* data, uint32_t timeout);
uint8_t umsg_Sensors_accel_state_peek(umsg_Sensors_accel_state_t* data);

umsg_sub_handle_t umsg_Sensors_baro_configuration_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_baro_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_baro_configuration_publish(umsg_Sensors_baro_configuration_t* data);
void umsg_Sensors_baro_configuration_publish_ch(umsg_Sensors_baro_configuration_t* data, uint8_t channel);
uint8_t umsg_Sensors_baro_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_configuration_t* data, uint32_t timeout);
uint8_t umsg_Sensors_baro_configuration_peek(umsg_Sensors_baro_configuration_t* data);

umsg_sub_handle_t umsg_Sensors_baro_data_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_baro_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_baro_data_publish(umsg_Sensors_baro_data_t* data);
void umsg_Sensors_baro_data_publish_ch(umsg_Sensors_baro_data_t* data, uint8_t channel);
uint8_t umsg_Sensors_baro_data_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_data_t* data, uint32_t timeout);
uint8_t umsg_Sensors_baro_data_peek(umsg_Sensors_baro_data_t* data);

umsg_sub_handle_t umsg_Sensors_baro_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_baro_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_baro_state_publish(umsg_Sensors_baro_state_t* data);
void umsg_Sensors_baro_state_publish_ch(umsg_Sensors_baro_state_t* data, uint8_t channel);
uint8_t umsg_Sensors_baro_state_receive(umsg_sub_handle_t queue, umsg_Sensors_baro_state_t* data, uint32_t timeout);
uint8_t umsg_Sensors_baro_state_peek(umsg_Sensors_baro_state_t* data);

umsg_sub_handle_t umsg_Sensors_thermistor_configuration_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_thermistor_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_thermistor_configuration_publish(umsg_Sensors_thermistor_configuration_t* data);
void umsg_Sensors_thermistor_configuration_publish_ch(umsg_Sensors_thermistor_configuration_t* data, uint8_t channel);
uint8_t umsg_Sensors_thermistor_configuration_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_configuration_t* data, uint32_t timeout);
uint8_t umsg_Sensors_thermistor_configuration_peek(umsg_Sensors_thermistor_configuration_t* data);

umsg_sub_handle_t umsg_Sensors_thermistor_data_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_thermistor_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_thermistor_data_publish(umsg_Sensors_thermistor_data_t* data);
void umsg_Sensors_thermistor_data_publish_ch(umsg_Sensors_thermistor_data_t* data, uint8_t channel);
uint8_t umsg_Sensors_thermistor_data_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_data_t* data, uint32_t timeout);
uint8_t umsg_Sensors_thermistor_data_peek(umsg_Sensors_thermistor_data_t* data);

umsg_sub_handle_t umsg_Sensors_thermistor_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_Sensors_thermistor_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_Sensors_thermistor_state_publish(umsg_Sensors_thermistor_state_t* data);
void umsg_Sensors_thermistor_state_publish_ch(umsg_Sensors_thermistor_state_t* data, uint8_t channel);
uint8_t umsg_Sensors_thermistor_state_receive(umsg_sub_handle_t queue, umsg_Sensors_thermistor_state_t* data, uint32_t timeout);
uint8_t umsg_Sensors_thermistor_state_peek(umsg_Sensors_thermistor_state_t* data);

