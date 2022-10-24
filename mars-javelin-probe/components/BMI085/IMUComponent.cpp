/*********************************************************************************
*     File Name           :     /components/BMI085/IMUComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:43]
*     Last Modified       :     [2022-10-23 19:22]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#include "IMUComponent.h"

void IMUComponent::vMainLoop_Task(void *arg){
  IMUComponent imu_component = *((IMUComponent*)arg);

  for(;;){
    
  }
}

void IMUComponent::setup(){
  _device = BMI085()
}
