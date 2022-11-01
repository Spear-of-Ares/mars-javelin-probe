/*********************************************************************************
*     File Name           :     /components/BMI085/IMUComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:43]
*     Last Modified       :     [2022-10-27 01:23]
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
#ifdef IMU_ATTACHED
  _device.read_imu();
  float ax = _device.get_ax(false);
  float ay = _device.get_ay(false);
  float az = _device.get_az(false);

  float gx = _device.get_gx(false);
  float gy = _device.get_gy(false);
  float gz = _device.get_gz(false);

  float pitch = _device.get_pitch();
  float roll = _device.get_roll();
  float yaw = _device.get_yaw();
  float temp_c = _device.get_temp_c();
  uint32_t read_time = _device.get_read_time();
#else
  float ax = 0;
  float ay = 0;
  float az = 0;

  float gx = 0;
  float gy = 0;
  float gz = 0;

  float pitch = 0;
  float roll = 0;
  float yaw = 0;
  float temp_c = 0;
  uint32_t read_time = 0;
#endif
  
  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << IMU_TASK_ID << " || ";
  int header_len = data.str().length();
  data << "Ax: " << ax << " g | Ay: " << ay << " g | Az: " << az << " g | Temp: " << temp_c << " C | Time: " << read_time << " micro sec\n";
  data << std::string(header_len-3, ' ') << "|| Gx: " << gx << " d/s | Gy: " << gy << " d/s | Gz: " << gz << " d/s\n";
  data << std::string(header_len-3, ' ') << "|| Pitch: " << pitch << " | Roll: " << roll << " | Yaw: " << yaw << "\n";

  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post IMU data\n");
  }
}

void IMUComponent::setup(){
#ifdef IMU_ATTACHED
  _device = BMI085(I2C_MASTER_PORT);
  _device.begin();
  // TODO:: Set ranges and config things
#endif
}
