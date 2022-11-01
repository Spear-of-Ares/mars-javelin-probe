/*********************************************************************************
*     File Name           :     BMI085/include/BMI085_regsiters.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-16 03:54]
*     Last Modified       :     [2022-10-16 15:41]
*     Description         :     Register Map definitions for the Bosch BMI085 6-axis IMU 
**********************************************************************************/

#ifndef __BMI085_registers_H__
#define __BMI085_registers_H__

// Definitions of the registers within the BMI085. First value in pair is the register address.
// The second value (<register_name>_RST) is the reset value of the register. (ie. default)
// Reset values are only included for writeable registers.

/* Accelerometer Registers */
#define ACC_SOFTRESET           0x7E    // W: Write a value of 0xB6 to reset the sensor after 1ms
#define ACC_SOFTRESET_RST       0x00

#define ACC_PWR_CTRL            0x7D    // RW: Switches accelerometer ON or OFF. Required to do after every reset. ON: 0x04 OFF 0x00
#define ACC_PWR_CTRL_RST        0x00

#define ACC_PWR_CONF            0x7C    // RW: Switches accelerometer into suspend mode for saving power
#define ACC_PWR_CONF_RST        0x03

#define ACC_SELF_TEST           0x6D    // RW: Enables the sensor self-test signal, occuring as a steady offset to the sensor output
#define ACC_SELF_TEST_RST       0x00

#define INT_MAP_DATA            0x58    // RW: Map data ready interrupt to output pin INT1 and or INT2
#define INT_MAP_DATA_RST        0x00

#define INT2_IO_CTRL            0x54    // RW: Configures the input/output pin INT2
#define INT2_IO_CTRL_RST        0x00

#define INT1_IO_CTRL            0x53    // RW: Configures the input/output pin INT1
#define INT1_IO_CTRL_RST        0x00

#define ACC_RANGE               0x41    // RW: Accelerometer range setting register
#define ACC_RANGE_RST           0x01

#define ACC_CONF                0x40    // RW: Accelerometer configuration register
#define ACC_CONF_RST            0xA8

#define TEMP_LSB                0x23    // RO: Registers containing the temperature sensor data output. 
#define TEMP_MSB                0x22    //     Resolution of 0.125C/LSB 
#define ACC_INT_STAT_1          0x1D    // RO: Interrupt status register
#define SENSORTIME_2            0x1A    // RO: Registers containing the value of the internal 24 bit counter
#define SENSORTIME_1            0x19    //     Allows for 655.36 seconds of 40 micro second accuracy
#define SENSORTIME_0            0x18
#define ACC_Z_MSB               0x17    // RO: Registers containing acceleration sensor outpout. 
#define ACC_Z_LSB               0x16    //     Calculate full value: Accel_X_int16 = ACC_X_MSB * 256 + ACC_X_LSB
#define ACC_Y_MSB               0x15    //     Convert from LSB to mg: Accel_X_in_mg = Accel_X_int16 / 32768 * 1000 * 2 ^(ACC_RANGE + 1)
#define ACC_Y_LSB               0x14    //                         Where ACC_RANGE is the value in the ACC_RANGE register ^^^^^^^^^
#define ACC_X_MSB               0x13
#define ACC_X_LSB               0x12    
#define ACC_STATUS              0x03    // RO: Sensor status flag
#define ACC_ERR_REG             0x02    // RO: Reports sensor error conditions
#define ACC_CHIP_ID             0x00    // RO: Contains identifier code of acceleration sensor

/* Gyroscope Registers */
#define GYRO_SELF_TEST              0x3C    // RW: Built-in self-test of gyroscope

#define GYRO_INT3_INT4_IO_MAP       0x18    // RW: Map the data reday interrupt pin to one of the interrupt pins INT3 and/or INT4
#define GYRO_INT3_INT4_IO_MAP_RST   0x00

#define GYRO_INT3_INT4_IO_CONF      0x16    // RW: Sets electrical and logical properties of the interrupt pins
#define GYRO_INT3_INT4_IO_CONF_RST  0x0F 

#define GYRO_INT_CTRL               0x15    // RW: Control for enabling new data interrupt to be triggered on new data
#define GYRO_INT_CTRL_RST           0x00 

#define GYRO_SOFTRESET              0x14    // W: Writing a value of 0xB6 resets the sensor after 30ms

#define GYRO_LPM1                   0x11    // RW: Selection of the main power mode. Normal, suspend, deep suspend
#define GYRO_LPM1_RST               0x00

#define GYRO_BANDWIDTH              0x10    // RW: The register allows the selection of the rate dat filter bandwidth and ouput data rate
#define GYRO_BANDWIDTH_RST          0x80

#define GYRO_RANGE                  0x0F    // RW: Angular rate range and resolution
#define GYRO_RANGE_RST              0x00

#define GYRO_INT_STAT_1             0x0A    // RO: Data ready interrupt status.
#define GYRO_RATE_Z_MSB             0x07    // RO: Registers containing the angular velocity sensor output.
#define GYRO_RATE_Z_LSB             0x06    //     Calculate full value: RATE_X_MSB * 256 + RAGE_X_LSB
#define GYRO_RATE_Y_MSB             0x05    
#define GYRO_RATE_Y_LSB             0x04
#define GYRO_RATE_X_MSB             0x03
#define GYRO_RATE_X_LSB             0x02    
#define GYRO_CHIP_ID                0x00    // RO: Contains identifier code of the gyroscope
#endif /* __BMI085_registers_H__ */


