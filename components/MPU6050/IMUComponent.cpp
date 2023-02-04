/*********************************************************************************
*     File Name           :     /components/BMI085/IMUComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:43]
*     Last Modified       :     [2022-11-06 03:46]
*     Description         :     Component for handling the IMU and its data 
**********************************************************************************/

#include "IMUComponent.h"

void IMUComponent::vMainLoop_Task(void *arg){
  IMUComponent imu_component = *((IMUComponent*)arg);
#ifdef IMU_ATTACHED
  imu_component.setup();
#endif
  const TickType_t xDelay = (1000 / SAMPLE_RATE_HZ) / portTICK_PERIOD_MS;
  for(;;){

#ifdef IMU_ATTACHED
    imu_component.logIMU();    
#endif
    vTaskDelay(xDelay);
  }
}

IMUComponent::IMUComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium){
  _dataOutSD = dataOutSD;
  _dataOutLoRa = dataOutLoRa;
  _dataOutIridium = dataOutIridium;

  _lastUpdateIridium = 0x0000;
  _lastUpdateLoRa = 0x0000;
}

void IMUComponent::logIMU(){
  Activites a = _device.readActivites();
  if(!a.isDataReady) return;
  Vector accel_scaled = _device.readScaledAccel();

  // Hardcoded numbers come from the range and scale of accelerometer and gyroscope
  if(accel_scaled.XAxis > 8){accel_scaled.XAxis = accel_scaled.XAxis - 16;}
  if(accel_scaled.YAxis > 8){accel_scaled.YAxis = accel_scaled.YAxis - 16;}
  if(accel_scaled.ZAxis > 8){accel_scaled.ZAxis = accel_scaled.ZAxis - 16;}

  // accel_norm.XAxis = accel_norm.XAxis/3;
  // accel_norm.YAxis = accel_norm.YAxis/3;
  // accel_norm.ZAxis = accel_norm.ZAxis/3;

  Vector gyro_norm = _device.readNormalizeGyro();

  if(gyro_norm.XAxis > 500){gyro_norm.XAxis = gyro_norm.XAxis - 1000;}
  if(gyro_norm.YAxis > 500){gyro_norm.YAxis = gyro_norm.YAxis - 1000;}
  if(gyro_norm.ZAxis > 500){gyro_norm.ZAxis = gyro_norm.ZAxis - 1000;}
  float temp = _device.readTemperature();

  // Time 
  float time_step = 1.0 / SAMPLE_RATE_HZ;
  _pitch = _pitch + gyro_norm.YAxis * time_step;
  _roll = _roll + gyro_norm.XAxis * time_step;
  _yaw = _yaw + gyro_norm.ZAxis * time_step;

  while(_pitch > 360){
    _pitch = _pitch - 360;
  }
  while(_roll > 360){
    _roll = _roll - 360;
  }
  while(_yaw > 360){
    _yaw = _yaw - 360;
  }
  
  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << IMU_TASK_ID << " || ";
  int header_len = data.str().length();
  data << "Ax: " << accel_scaled.XAxis << " Ay: " << accel_scaled.YAxis << " Az: " << accel_scaled.ZAxis << " g | Temp: " << temp << " C\n";
  data << std::string(header_len-3, ' ') << "|| Gx: " << gyro_norm.XAxis << " Gy: " << gyro_norm.YAxis << " Gz: " << gyro_norm.ZAxis << " deg/s\n";
  data << std::string(header_len-3, ' ') << "|| Pitch: " << _pitch << " | Roll: " << _roll << " | Yaw: " << _yaw << "\n";

  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post IMU data\n");
  }

  TickType_t curr_tick = xTaskGetTickCount();
  #ifdef IMU_LOG_IRIDIUM
  // Can't initialize data unless we send it. Otherwise memory leak
  // Update iridium once every 20 seconds
  if (curr_tick - _lastUpdateIridium > 20000 / portTICK_PERIOD_MS)
  {
    std::string *iriddata = new std::string(data.str());
    _lastUpdateIridium = curr_tick;
    if (xQueueSend(_dataOutIridium, &(iriddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Failed to post Thermistor data to Iridium\n");
    }
  }
  #endif

  #ifdef IMU_LOG_LoRa
  // Update LoRa every 1 seconds
  if (curr_tick - _lastUpdateLoRa > 1000 / portTICK_PERIOD_MS)
  {
    std::string *loradata = new std::string(data.str());
    _lastUpdateLoRa = curr_tick;
    if (xQueueSend(_dataOutLoRa, &(loradata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
      printf("Failed to post Thermistor data to LoRa\n");
    }
  }
  #endif
}

void IMUComponent::setup(){
  _device = MPU6050();
  while(!_device.begin(MPU6050_SCALE_500DPS, MPU6050_RANGE_8G, 104, &Wire))
  {
      printf("Could not find a valid MPU6050 Sensor, check wiring!\n");
      vTaskDelay(500/portTICK_PERIOD_MS);
  }

  // Disable interupts
  _device.setIntFreeFallEnabled(false);
  _device.setIntZeroMotionEnabled(false);
  _device.setIntMotionEnabled(false);

  // Calculated by seeing value off norm and the following conversion
  // y = (x*1000) / 2.5
  
  int16_t accel_x_off = (int)-2380;
  int16_t accel_y_off = (int)800;
  int16_t accel_z_off = (int)-880;

  _device.setAccelOffsetX(accel_x_off);
  _device.setAccelOffsetY(accel_y_off);
  _device.setAccelOffsetZ(accel_z_off);

  // Calculated by seeing vaule off norm and the following conversion
  // y = (x*36) / 0.8
  // Fine tuning was done after

  int16_t gyro_x_off = (int)-1080;
  int16_t gyro_y_off = (int)999;
  int16_t gyro_z_off = (int)1272;

  _device.setGyroOffsetX(gyro_x_off);
  _device.setGyroOffsetY(gyro_y_off);
  _device.setGyroOffsetZ(gyro_z_off);

  _device.setDLPFMode(MPU6050_DLPF_4);

  //_device.calibrateGyro(6);
  _device.setThreshold(1);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  printf("IMU setup complete\n");
}
