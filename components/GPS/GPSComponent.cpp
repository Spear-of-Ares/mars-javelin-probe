/*********************************************************************************
*     File Name           :     /components/GPS/GPSComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:39]
*     Last Modified       :     [2022-11-06 03:46]
*     Description         :     Component for handling both GPS modules and their data 
**********************************************************************************/

#include "GPSComponent.h"

#define ADDRESS_1 0x42
#define ADDRESS_2 0x43
//#define SET_GPS_ADDR

void GPSComponent::vMainLoop_Task(void *arg){
    GPSComponent gps_component = *((GPSComponent*)(arg));
#ifdef SET_GPS_ADDR
    gps_component.setup();
#endif
    
    for(;;){
        gps_component.get_data();
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void GPSComponent::setup(){
#ifdef SET_GPS_ADDR
  if(_GNSS_1.begin(Wire, 0x42, 0xFFFF) == false){
    printf("Could not start _GNSS_1\n");
  }
  else{
    printf("Started _GNSS_1\n");
  }
#endif
  //
  //
  //
  // if(_GNSS_2.begin(Wire, 0x42, 0xFFFF) == false){
  //   printf("Could not start _GNSS_2\n");
  // }
  // else{
  //   _GNSS_2.setMeasurementRate(5000);
  //   printf("Started _GNSS_2\n");
  // }
  //
  // // _GNSS_1.setI2CpollingWait(25);
  // // _GNSS_2.setI2CpollingWait(25);
  //
#ifdef SET_GPS_ADDR
  if(_GNSS_1.setI2CAddress(0x43) == false){
    printf("Could not change the i2c address of _GNSS_1\n");
  }

  if(_GNSS_1.saveConfiguration() == false){
    printf("Unable to save configuration of _GNSS_1\n");
  }
#endif

}

std::string GPSComponent::getGPS_MSG(int addr){
  Wire.beginTransmission(ADDRESS);
  Wire.write(0xFF);
  Wire.endTransmission();

  int val = 0;
  std::string msg = "";
  // Get all data from GPS
  for(int i = 0;; i++){
    Wire.requestFrom(ADDRESS, 1, true);
    val = Wire.read();
    if(val == 0xFF || val == -1){
      if(i == 0){
        return "";
      } 
      break;
    }
    msg += (char)val;
    vTaskDelay(1);
  }

  int i = 0;
  return msg;
}

void GPSComponent::sendData(int gps_num ,std::string msg){
  int i = 0;
  while(i < msg.length() && _tiny_gps.encode(msg[i++]));

  unsigned long date, time, time_age;
  _tiny_gps.get_datetime(&date, &time, &time_age);
  long latitude, longitude;
  unsigned long pos_age;
  _tiny_gps.get_position(&latitude, &longitude, &pos_age);

  std::ostringstream data;
  data << xTaskGetTickCount() << " || " GPS_COMP_ID << " || GPS "<< gps_num << " ||  Date:Time:Age: "<< date << ":"<< time << ":" << time_age << " || Lat:Long:Age: "<<latitude << ":" << longitude << ":" << pos_age<< " || sats: " << _tiny_gps.satellites() << "\n";
  SDData *sddata = new SDData();
  sddata->file_name = new std::string("measure");
  sddata->message = new std::string(data.str());

  if (xQueueSend(_dataOutSD, &(sddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
  {
    printf("Failed to post thermistor data\n");
  }

}

void GPSComponent::get_data(){
  std::string gps1_msg = getGPS_MSG(ADDRESS_1);
  vTaskDelay(100/portTICK_PERIOD_MS);
  std::string gps2_msg = getGPS_MSG(ADDRESS_2);

  sendData(1, gps1_msg);
  sendData(2,gps2_msg);
}
