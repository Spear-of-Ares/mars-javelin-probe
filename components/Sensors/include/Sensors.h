#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdio.h>

#include "esp_log.h"

#include "Wire.h"
#include "Thermistor.h"
#include "MPU6050.h"
#include "Adafruit_BME280.h"

#include "umsg_Sensors.h"
#include "umsg_CommandCenter.h"

class Sensors
{
public:
    Sensors() {}
    static void vMainLoop_Task(void *arg);

private:
    void variable_delay(int delay_ms);
    void set_sample_rate_hz(int hz);

    void setup();
    void log_data();

    esp_err_t setup_imu();
    esp_err_t setup_bme();
    esp_err_t setup_therm();

    void log_imu();
    void log_bme();
    void log_therm();

    MPU6050 _imu;
    float _pitch;
    float _roll;
    float _yaw;

    Adafruit_BME280 _bme;
    float _start_press_hpa;
    float _start_alt_m;

    float _internal_thermistor_c;
    float _external_thermistor_c;

    int sample_rate_hz;

    umsg_sub_handle_t cmd_sub_handle;
};

#endif /* __SENSORS_H__*/