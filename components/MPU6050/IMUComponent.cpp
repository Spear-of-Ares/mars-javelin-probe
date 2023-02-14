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
  Vector gyro_norm = _device.readNormalizeGyro();
  Vector gyro_rad = {gyro_norm.XAxis * (float)DEG_TO_RAD, gyro_norm.YAxis * (float)DEG_TO_RAD, gyro_norm.ZAxis * (float)DEG_TO_RAD };
  set_sensor_data(accel_scaled, gyro_rad);
  update_filter();
  UKF filter = get_UKF();

  Matrix attitude = filter.GetX();

  float temp = _device.readTemperature();


  std::ostringstream data;
  data << xTaskGetTickCount() << " || " << IMU_TASK_ID << " || ";
  int header_len = data.str().length();
  data << "Ax: " << accel_scaled.XAxis << " Ay: " << accel_scaled.YAxis << " Az: " << accel_scaled.ZAxis << " g | Temp: " << temp << " C\n";
  data << std::string(header_len-3, ' ') << "|| Gx: " << gyro_norm.XAxis << " Gy: " << gyro_norm.YAxis << " Gz: " << gyro_norm.ZAxis << " deg/s\n";
  data << std::string(header_len-3, ' ') << "|| Pitch: " << attitude[1][0]*RAD_TO_DEG << " Roll: " << attitude[2][0]*RAD_TO_DEG << " Yaw: " << attitude[3][0]*RAD_TO_DEG << " deg\n";

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
  // Update LoRa every .5 seconds
  if (curr_tick - _lastUpdateLoRa > 500 / portTICK_PERIOD_MS)
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

  // Need to set here because message doesn't get sent occasionally
  while(_device.getScale() != MPU6050_SCALE_1000DPS)
    _device.setScale(MPU6050_SCALE_1000DPS);

  while(_device.getRange() != MPU6050_RANGE_8G)
    _device.setRange(MPU6050_RANGE_8G);

  // Disable interupts
  _device.setIntFreeFallEnabled(false);
  _device.setIntZeroMotionEnabled(false);
  _device.setIntMotionEnabled(false);

  
  _device.setDLPFMode(MPU6050_DLPF_4);

  //_device.calibrateGyro(6);
  _device.setThreshold(1);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  printf("IMU setup complete\n");
}
