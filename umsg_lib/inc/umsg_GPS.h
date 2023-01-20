// Generated with umsg_gen on 2022-12-21
#pragma once
#include <umsg_types.h>

// msg structure typedefs

typedef enum
{
    GPS_OK,
    GPS_SLEEPING,
    GPS_OFF,
    GPS_DISCONNECTED
} umsg_GPS_gps_state_t;

typedef struct
{
    uint8_t sample_freq_hz;
} umsg_GPS_gps_configuration_t;

typedef struct
{
    int64_t lat_long[2];
    uint32_t altitude;
    int32_t time_ymd_hms[7];
    bool valid_time;
    uint8_t locked_sats;
} umsg_GPS_gps_data_t;

typedef struct
{
    umsg_GPS_gps_state_t state;
    uint8_t initializing : 1, not_enough_sats : 1, receiving_data : 1, reconnecting : 1, not_present : 1;
} umsg_GPS_gps_state_t;

// api function headers
umsg_sub_handle_t umsg_GPS_gps_configuration_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_GPS_gps_configuration_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_GPS_gps_configuration_publish(umsg_GPS_gps_configuration_t* data);
void umsg_GPS_gps_configuration_publish_ch(umsg_GPS_gps_configuration_t* data, uint8_t channel);
uint8_t umsg_GPS_gps_configuration_receive(umsg_sub_handle_t queue, umsg_GPS_gps_configuration_t* data, uint32_t timeout);
uint8_t umsg_GPS_gps_configuration_peek(umsg_GPS_gps_configuration_t* data);

umsg_sub_handle_t umsg_GPS_gps_data_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_GPS_gps_data_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_GPS_gps_data_publish(umsg_GPS_gps_data_t* data);
void umsg_GPS_gps_data_publish_ch(umsg_GPS_gps_data_t* data, uint8_t channel);
uint8_t umsg_GPS_gps_data_receive(umsg_sub_handle_t queue, umsg_GPS_gps_data_t* data, uint32_t timeout);
uint8_t umsg_GPS_gps_data_peek(umsg_GPS_gps_data_t* data);

umsg_sub_handle_t umsg_GPS_gps_state_subscribe(uint32_t prescaler, uint8_t length);
umsg_sub_handle_t umsg_GPS_gps_state_subscribe_ch(uint32_t prescaler, uint8_t length, uint8_t channel);
void umsg_GPS_gps_state_publish(umsg_GPS_gps_state_t* data);
void umsg_GPS_gps_state_publish_ch(umsg_GPS_gps_state_t* data, uint8_t channel);
uint8_t umsg_GPS_gps_state_receive(umsg_sub_handle_t queue, umsg_GPS_gps_state_t* data, uint32_t timeout);
uint8_t umsg_GPS_gps_state_peek(umsg_GPS_gps_state_t* data);

