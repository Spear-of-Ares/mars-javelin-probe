/*********************************************************************************
*     File Name           :     BMI085/BMI085.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-16 04:48]
*     Last Modified       :     [2022-10-16 15:54]
*     Description         :     A driver for the Bosch BMI085 6-axis IMU 
**********************************************************************************/

#include "BMI085.h"

#define RAD2DEGREES (180.0 / 3.141596)

BMI085::BMI085(i2c_port_t master_port, 
               uint8_t gyro_i2c_addr, uint8_t accel_i2c_addr, 
               accel_range_et accel_range, gyro_range_et gyro_range)
{
  _master_port = master_port;
  _gaddr = gyro_i2c_addr;
  _aaddr = accel_i2c_addr;
  _accel_range = accel_range;
  _gyro_range = gyro_range;
  reset();
}

void BMI085::begin()
{

}

void BMI085::reset()
{
  _ax = _ay = _az = 0;
  _aax = _aay = _aaz = 0;
  _gx = _gy = _gz = 0;
  _gax = _gay = _gaz = 0;
  _pitch = _roll = _yaw = 0;
}

void BMI085::read_imu()
{
  read_accel();
  read_gyro();

  #if defined IMU_ADJUST
  calc_PRY();
  #endif
}

void BMI085::read_accel()
{
  size_t accel_data_size = 6;
  uint8_t accel_data[accel_data_size];

  size_t accel_time_size = 3;
  uint8_t accel_time_data[accel_time_size];

  size_t temp_data_size = 2;
  uint8_t temp_data[temp_data_size];

  uint8_t acc_x_lsb_addr = ACC_X_LSB;
  uint8_t sensortime_0_addr = SENSORTIME_0;
  uint8_t temp_lsb_addr = TEMP_LSB;
  ESP_ERROR_CHECK(send_to_accel(&acc_x_lsb_addr, 1, accel_data, accel_data_size));
  ESP_ERROR_CHECK(send_to_accel(&sensortime_0_addr, 1, accel_time_data, accel_time_size));
  ESP_ERROR_CHECK(send_to_accel(&temp_lsb_addr, 1, temp_data, temp_data_size));
  
  _ax = accel_data[0] | (accel_data[1] << 8);
  _ay = accel_data[2] | (accel_data[3] << 8);
  _az = accel_data[4] | (accel_data[5] << 8);

  // Weird shifting here because of how the registers are layed out within accelerometer
  _temp = (temp_data[0]>>5) | (temp_data[1]<<3);
  if (_temp > 1023)
  {
    _temp = _temp - 2048;
  }

  _read_time = accel_time_data[0] | (accel_time_data[1] << 8) | (accel_time_data[2] << 16);

  #if defined IMU_ADJUST
  adjust_accel();
  #endif
}

void BMI085::read_gyro()
{
  size_t gyro_data_size = 6;
  uint8_t gyro_data[gyro_data_size];

  uint8_t gyro_rate_x_lsb_addr = GYRO_RATE_X_LSB;
  ESP_ERROR_CHECK(send_to_gyro(&gyro_rate_x_lsb_addr, 1, gyro_data, gyro_data_size));

  _gx = gyro_data[0] | (gyro_data[1] << 8);
  _gy = gyro_data[2] | (gyro_data[3] << 8);
  _gz = gyro_data[4] | (gyro_data[5] << 8);

  #if defined IMU_ADJUST
  adjust_gyro()
  #endif
}

void BMI085::set_accel_range(accel_range_et accel_range)
{

}

void BMI085::set_accel_enable(accel_enable_et accel_enable)
{

}

void BMI085::set_accel_power(accel_pwr_save_et accel_power)
{

}

void BMI085::set_accel_self_test(accel_self_test_et accel_self_test)
{

}

void BMI085::set_accel_odr(accel_odr_et accel_odr)
{

}

void BMI085::set_accel_bwp(accel_bwp_et accel_bwp)
{

}

void BMI085::set_gyro_range(gyro_range_et gyro_range)
{

}

void BMI085::set_gyro_bandwidth(gyro_bandwidth_et gyro_bw)
{

}

void BMI085::set_gyro_power(gyro_power_mode_et gyro_power)
{

}


/**
* @brief Sends some data to the accelerometer chip. May receive data as well. Leave recv NULL to receive nothing.
*
* @param data       Data array to send
* @param data_len   Size of data array
* @param recv       Array to store received values
  * @param recv_len Size allocated to receiving array
  *
  * @return ESP_ERR_T. Check with ESP_ERROR_CHECK
  */
esp_err_t BMI085::send_to_accel(uint8_t *data, size_t data_len, uint8_t *recv, size_t recv_len)
{
  return send_to_BMI085(_aaddr, data, data_len, recv, recv_len);
}


/**
* @brief Sends some data to the gyroscope chip. May receive data as well. Leave recv NULL to receive nothing.
*
* @param data       Data array to send
* @param data_len   Size of data array
* @param recv       Aarray to store received values
  * @param recv_len Size allocated to receiving array
  *
  * @return         Check with ESP_ERROR_CHECK
  */
esp_err_t BMI085::send_to_gyro(uint8_t *data, size_t data_len, uint8_t *recv, size_t recv_len)
{
  return send_to_BMI085(_gaddr, data, data_len, recv, recv_len);
}


/**
* @brief Sends data to the BMI085 IMU. May receive data as well.
*
  * @param addr     Address of chip to address. May be either the accelerometer or gyroscope
  * @param data     Data array to send
  * @param data_len Size of the data array
  * @param recv     Array to store received values
  * @param recv_len Size allocated to the receiving array
  *
  * @return         Check with ESP_ERROR_CHECK
  */
esp_err_t BMI085::send_to_BMI085(uint8_t addr, 
                                 uint8_t *data, size_t data_len, 
                                 uint8_t *recv, size_t recv_len){
  esp_err_t result;
  if (recv == NULL){
    result = i2c_master_write_to_device(
        _master_port,
        addr,
        data,
        data_len,
        BMI085_TIMEOUT/portTICK_PERIOD_MS
      );
  }
  else{
    result = i2c_master_write_read_device(
        _master_port,
        addr,
        data,
        data_len,
        recv,
        recv_len,
        BMI085_TIMEOUT/portTICK_PERIOD_MS
      );
  }
  return result;
}

/**
* @brief Calculate the adjusted values for accelerometer. Results in units of mg's and C
*
  * @return Check with ESP_ERROR_CHECK
*/
void BMI085::adjust_accel()
{
  _aax = (_ax / 32768) * 1000 * (1 << (_accel_range + 1));
  _aay = (_ay / 32768) * 1000 * (1 << (_accel_range + 1));
  _aaz = (_az / 32768) * 1000 * (1 << (_accel_range + 1));

  _temp_c = (_temp * 0.125) + 23;
}


/**
* @brief Calculate the adjusted values for gyroscope. Results in units of deg/sec
*
  * @return Check with ESP_ERROR_CHECK
*/
void BMI085::adjust_gyro()
{
  _gax = ((_gx / 32768) * 2000) >> _gyro_range;
  _gay = ((_gy / 32768) * 2000) >> _gyro_range;
  _gaz = ((_gz / 32768) * 2000) >> _gyro_range;
}

void BMI085::calc_PRY()
{
  uint32_t now = esp_timer_get_time();
  float duration = ((now - _lastMicros) * 1e-6); // duration in seconds
  _lastMicros = now;

  float _ax2 = _ax * _ax;
  float _ay2 = _ay * _ay;
  float _az2 = _az * _az;

  float atan_ax = atan(      _ay / sqrt(_ax2 * _az2)) * RAD2DEGREES;
  float atan_ay = atan(-1.0 * _ax / sqrt(_ay2 * _az2)) * RAD2DEGREES;
  float atan_az = atan(      _az / sqrt(_ax2 * _ay2)) * RAD2DEGREES;

  _acc_gx += _gx * duration;
  _acc_gy += _gy * duration;
  _acc_gz += _gz * duration;

  _yaw = _acc_gz;
  _pitch = 0.96 * _acc_gy + 0.04 * atan_ay;
  _roll = 0.96 * _acc_gx + 0.04 * atan_ax;
}

