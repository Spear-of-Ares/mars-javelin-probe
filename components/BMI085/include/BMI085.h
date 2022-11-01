/*********************************************************************************
*     File Name           :     BMI085/include/BMI085.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-16 03:35]
*     Last Modified       :     [2022-10-27 01:25]
*     Description         :     A driver for the Bosch BMI085 6-axis IMU
**********************************************************************************/

#ifndef __BMI085_H__
#define __BMI085_H__

#include <stdio.h>
#include <math.h>
#include "esp_err.h"
#include "driver/i2c.h"
#include "BMI085_registers.h"

// Sensitivity Temperature Drift for the accelerometer
// units: %/K
#define ACCEL_TCS 0.002

// Sensitivity Change over Temperature for the gyroscope.
// units: %/K
#define GYRO_TCS 0.03

// Sensitivity to acceleration stimuli in all three axis
// units: deg/s/g
#define GYRO_gSENS 0.1

// Temperature Sensor offset error. Measured at 25C
// units: K
#define TEMP_OTs 1

// Temperature Sensor slope
#define TEMP_dTs 0.125

// Timeout in ms for communication with BMI085
#define BMI085_TIMEOUT 10

// Values for ACC_CONF register bytes [7:4] 
typedef enum 
{
  ACCEL_BWP_OSR4 = 0x08, // 4-fold oversampling
  ACCEL_BWP_OSR2 = 0x09, // 2 fold oversampling
  ACCEL_BWP_NORMAL = 0x0a
} accel_bwp_et;

// Values for ACC_CONF register bytes [3:0]
typedef enum 
{
  ACCEL_ODR_12_5 = 0x05,
  ACCEL_ODR_25 = 0x06,
  ACCEL_ODR_50 = 0x07,
  ACCEL_ODR_100 = 0x08,
  ACCEL_ODR_200 = 0x09,
  ACCEL_ODR_400 = 0x0A,
  ACCEL_ODR_800 = 0x0B,
  ACCEL_ODR_1600 = 0x0C,
} accel_odr_et;

// Full Scale range of the accelerometer.
// For ACC_RANGE register bytes [1:0]
// units: g
typedef enum 
{
  ACCEL_gFS2G = 0x00,
  ACCEL_gFS4G = 0x01,
  ACCEL_gFS8G = 0x02,
  ACCEL_gFS16G = 0x03
} accel_range_et;


// Values for the ACC_SELF_TEST register bytes [7:0]
typedef enum 
{
  ACCEL_ST_OFF = 0x00,
  ACCEL_ST_POS = 0x0D,
  ACCEL_ST_NEG = 0x09
} accel_self_test_et;

// Values for the ACC_PWR_CONF register bytes [7:0]
typedef enum 
{
  ACCEL_ACTIVE = 0x00,
  ACCEL_SUSPEND = 0x03
} accel_pwr_save_et;

// Values for the ACC_PWR_CTRL register bytes [7:0]
typedef enum 
{
  ACCEL_ON = 0x04,
  ACCEL_OFF = 0x00,
} accel_enable_et;

// Full Scale range of the gyroscope
// units: deg/sec
typedef enum 
{
  GYRO_rFS125 = 0x04,
  GYRO_rFS250 = 0x03,
  GYRO_rFS500 = 0x02,
  GYRO_rFS1000 = 0x01,
  GYRO_rFS2000 = 0x00
} gyro_range_et;

// Values for the GYRO_BANDWITDH register bytes [7:0]
typedef enum 
{
  GYRO_ODR2K_FB532 = 0x00,
  GYRO_ODR2K_FB230 = 0x01,
  GYRO_ODR1K_FB116 = 0x02,
  GYRO_ODR400_FB47 = 0x03,
  GYRO_ODR200_FB23 = 0x04,
  GYRO_ODR100_FB12 = 0x05,
  GYRO_ODR200_FB64 = 0x06,
  GYRO_ODR100_FB32 = 0x07
} gyro_bandwidth_et;

// Values for the GYRO_LPM1 register bytes [7:0]
typedef enum 
{
  GYRO_NORMAL = 0x00,
  GYRO_SUSPEND = 0x80,
  GYRO_DEEP_SUSPEND = 0x20
} gyro_power_mode_et;

class BMI085
{
  public:
    BMI085(){}
    BMI085(i2c_port_t master_port, 
           uint8_t gyro_i2c_addr = 0x68, uint8_t accel_i2c_addr = 0x18,
           accel_range_et accel_range = ACCEL_gFS16G, gyro_range_et gyro_range = GYRO_rFS2000);

    void     begin();
    void     reset();
    
    void     read_imu();
    void     read_accel();
    void     read_gyro();

    void     set_accel_range(accel_range_et accel_range);
    void     set_accel_enable(accel_enable_et accel_enable);
    void     set_accel_power(accel_pwr_save_et accel_power);
    void     set_accel_self_test(accel_self_test_et accel_self_test);
    void     set_accel_odr(accel_odr_et accel_odr);
    void     set_accel_bwp(accel_bwp_et accel_bwp);
    
    void     set_gyro_range(gyro_range_et gyro_range);
    void     set_gyro_bandwidth(gyro_bandwidth_et gyro_bw);
    void     set_gyro_power(gyro_power_mode_et gyro_power);


    float    get_ax(bool raw=true)  { return raw==true ? _ax : _aax; }
    float    get_ay(bool raw=true)  { return raw==true ? _ay : _aay; }
    float    get_az(bool raw=true)  { return raw==true ? _az : _aaz; }

    float    get_gx(bool raw=true)  { return raw==true ? _gx : _gax; }
    float    get_gy(bool raw=true)  { return raw==true ? _gy : _gay; }
    float    get_gz(bool raw=true)  { return raw==true ? _gz : _gaz; }

    float    get_pitch()            { return _pitch; }
    float    get_roll()             { return _roll; }
    float    get_yaw()              { return _yaw; }

    uint16_t get_temp_raw()         {return _temp; }
    float    get_temp_c()           { return _temp_c; }
    uint32_t get_read_time()        { return _read_time; }


  private:
    esp_err_t send_to_accel(uint8_t *data, size_t data_len, uint8_t *recv, size_t recv_len);
    esp_err_t send_to_gyro(uint8_t *data, size_t data_len, uint8_t *recv, size_t recv_len);
    esp_err_t send_to_BMI085(uint8_t addr, uint8_t *data, size_t data_len, uint8_t *recv, size_t recv_len);

    void adjust_accel();
    void adjust_gyro();
    void calc_PRY();

    i2c_port_t _master_port;
    uint8_t _gaddr;
    uint8_t _aaddr;
    

    uint8_t _accel_range;
    uint16_t _accel_sens;

    uint8_t _gyro_range;
    uint16_t _gyro_sens;

    // Raw data values from the IMU
    int16_t _ax, _ay, _az;
    int16_t _gx, _gy, _gz;

    uint16_t _temp;
    uint32_t _read_time;

    // Adjusted data values for the accelerometer
    float _aax, _aay, _aaz;
    float _temp_c;

    // Adjusted data values for the gyroscope
    float _gax, _gay, _gaz; 
    float _acc_gx, _acc_gy, _acc_gz;

    float _pitch, _roll, _yaw;

    uint32_t _lastMicros;
};

#endif /* __BMI085_H__ */

