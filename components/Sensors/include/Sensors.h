#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdio.h>
#include <math.h>

#include "esp_log.h"

#include "Wire.h"
#include "ComBus.h"
#include "Thermistor.h"
#include "Adafruit_BNO055.h"  // IMU
#include "Adafruit_ADXL375.h" // Accelerometer
#include "MS5611.h"           // Barometer
#include "LowPassFilter.hpp"  // Filtering for the linear acceleration to eliminate bouncing of values

extern "C"
{
#include "umsg_Sensors.h"
#include "umsg_CommandCenter.h"
#include "umsg_GPS.h"
}

/* Define to allow for accurate sea level pressure calculation using GPS altitude measurement
    BLOCKS TASK during barometer setup until accurate GPS data is received (PDOP > 7.0 ie > 700)*/
#define ACCURATE_SEA_LEVL_PRESSURE

/* Calculate Velocity: V = V0 + a*dt
   Calculate Position: P = P0 + 1/2a*dt^2
*/
#define ACCEL_VEL_TRANSITION_CALC(last, now) (((now) - (last)) / (1000.00)) // Last and now are time in ms
#define ACCEL_POS_TRANSITION_CALC(dt) (0.5 * dt * dt)

/* Calculate Altitude from barometric equation.
 */
#define PRESS_ALT_TRANSITION(current_pressure, sea_level_pressure) (44330.0 * (1.0 - pow((current_pressure) / (sea_level_pressure), 0.1903)))
#define ALT_SEA_LEVEL_TRANSITION(current_alt, current_pressure) ((current_pressure) / (pow(1.0 - ((current_alt) / 44330.0), 5.255)))

class Sensors
{
public:
    static void vMainLoop_Task(void *arg);

private:
    void variable_delay(int delay_ms);
    void set_sample_rate_hz(int hz);

    void setup();
    void log_data();

    esp_err_t setup_imu();
    esp_err_t setup_accel();
    esp_err_t setup_baro();
    esp_err_t setup_therm();

    void log_imu();
    void log_baro();
    void log_accel();
    void log_therm();

    Adafruit_BNO055 _imu;
    LowPassFilter _low_pass_filters[3]; /*! < Each axis of linear acceleration needs its own low pass filter */
    double position[3];
    imu::Vector<3> velocity;
    int32_t imu_last_read;

    Adafruit_ADXL375 _impact_accel;

    MS5611 _baro;
    float _start_press_hpa;
    float _sea_level_press;
    float _start_alt_m;

    float _internal_thermistor_c;
    float _external_thermistor_c;

    int sample_rate_hz;

    uint8_t imu_calibration;

    umsg_sub_handle_t _cmd_sub_handle;
    umsg_CommandCenter_command_t _cmd_data;

    /* Need an acurate altitude for getting accurate sea level pressure estimate*/
    umsg_sub_handle_t _gps_sub_handle;
    umsg_GPS_data_t _gps_data;
};

#endif /* __SENSORS_H__*/