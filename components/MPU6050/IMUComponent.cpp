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
  // Update LoRa every 5 seconds
  if (curr_tick - _lastUpdateLoRa > 5000 / portTICK_PERIOD_MS)
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
  while(!_device.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G, 104, &Wire))
  {
      printf("Could not find a valid MPU6050 Sensor, check wiring!\n");
      vTaskDelay(500/portTICK_PERIOD_MS);
  }

  _device.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  // Disable interupts
  _device.setIntFreeFallEnabled(false);
  _device.setIntZeroMotionEnabled(false);
  _device.setIntMotionEnabled(false);

  _device.calibrateGyro();
  _device.setThreshold(3);
  vTaskDelay(500/portTICK_PERIOD_MS);
  printf("IMU setup complete\n");
}
