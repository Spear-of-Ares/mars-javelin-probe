
#ifndef __UKF_IMU_ENGL_H__
#define __UKF_IMU_ENGL_H__

#include "konfig.h"
#include "matrix.h"
#include "ukf.h"

#ifndef VECTOR_STRUCT_H
#define VECTOR_STRUCT_H

struct Vector
{
    float XAxis;
    float YAxis;
    float ZAxis;
};
#endif

void setup();
void set_sensor_data(Vector accel, Vector gyro);
void update_filter();
UKF get_UKF();

#endif