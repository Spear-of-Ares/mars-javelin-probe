/*********************************************************************************
*     File Name           :     /components/BMI085/IMUComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:43]
*     Last Modified       :     [2022-11-02 05:04]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#include "IMUComponent.h"

void IMUComponent::vMainLoop_Task(void *arg){
  IMUComponent imu_component = *((IMUComponent*)arg);
  imu_component.setup();
  const TickType_t xDelay = (1000 / SAMPLE_RATE_HZ) / portTICK_PERIOD_MS;
  for(;;){
    imu_component.logIMU();    
    vTaskDelay(xDelay);
  }
}

IMUComponent::IMUComponent(QueueHandle_t dataOutSD){
  _dataOutSD = dataOutSD;
}

void IMUComponent::logIMU(){
  Vector accel_norm = _device.readNormalizeAccel();
  Vector gyro_norm = _device.readNormalizeGyro();
  float temp = _device.readTemperature();

  // Time 
  float time_step = 1.0 / SAMPLE_RATE_HZ;
  _pitch = _pitch + gyro_norm.YAxis * time_step;
  _roll = _roll + gyro_norm.XAxis * time_step;
  _yaw = _yaw + gyro_norm.ZAxis * time_step;
  
  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << IMU_TASK_ID << " || ";
  int header_len = data.str().length();
  data << "Ax: " << accel_norm.XAxis << " m/s^2 | Ay: " << accel_norm.YAxis << " m/s^2 | Az: " << accel_norm.ZAxis << " m/s^2 | Temp: " << temp << " C\n";
  data << std::string(header_len-3, ' ') << "|| Gx: " << gyro_norm.XAxis << " d/s | Gy: " << gyro_norm.YAxis << " d/s | Gz: " << gyro_norm.ZAxis << " d/s\n";
  data << std::string(header_len-3, ' ') << "|| Pitch: " << _pitch << " | Roll: " << _roll << " | Yaw: " << _yaw << "\n";

  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post IMU data\n");
  }
}

void IMUComponent::setup(){
  _device = MPU6050();
  while(!_device.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
      printf("Could not find a valid MPU6050 Sensor, check wiring!");
      vTaskDelay(500/portTICK_PERIOD_MS);
  }

  _device.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  // // Disable interupts 
  // _device.setIntFreeFallEnabled(false);
  // _device.setIntZeroMotionEnabled(false);
  // _device.setIntMotionEnabled(false);

  _device.calibrateGyro();
  _device.setThreshold(3);
  printf("IMU setup complete\n");
}
