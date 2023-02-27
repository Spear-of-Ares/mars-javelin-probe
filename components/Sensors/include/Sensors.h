#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdio.h>

#include "esp_log.h"

#include "Wire.h"
#include "Thermistor.h"
#include "Adafruit_BNO055.h"  // IMU
#include "Adafruit_ADXL375.h" // Accelerometer
#include "MS5611.h"           // Barometer

extern "C"
{
#include "umsg_Sensors.h"
#include "umsg_CommandCenter.h"
}

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
    Adafruit_ADXL375 _impact_accel;

    MS5611 _baro;
    float _start_press_hpa;
    float _start_alt_m;

    float _internal_thermistor_c;
    float _external_thermistor_c;

    int sample_rate_hz;

    uint8_t imu_calibration;

    umsg_sub_handle_t cmd_sub_handle;
};

#endif /* __SENSORS_H__*/