/*********************************************************************************
*     File Name           :     /components/GPS/GPSComponent.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-21 00:39]
*     Last Modified       :     [2022-10-21 00:40]
*     Description         :     Component for handling both GPS modules and their data 
**********************************************************************************/

#include "GPSComponent.h"

#define ADDRESS 0x42

void GPSComponent::vMainLoop_Task(void *arg){
    GPSComponent gps_component = *((GPSComponent*)(arg));
    for(;;){
        gps_component.get_data();
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void GPSComponent::get_data(){
        Wire.beginTransmission(ADDRESS);
        Wire.write(0xFD);
        Wire.endTransmission();

        Wire.beginTransmission(ADDRESS);
        Wire.requestFrom(ADDRESS, 2);
        int high_b = Wire.read();
        int low_b = Wire.read();
        if (high_b == -1 or low_b == -1){
            printf("Could not read GPS data on i2c\n");
        }
        uint16_t data_bytes = high_b << 8 | low_b;
        Wire.endTransmission();

        Wire.beginTransmission(ADDRESS);
        Wire.requestFrom(ADDRESS, data_bytes);
         while(Wire.available()){
            char c = (char)Wire.read();

            bool res = _tiny_gps.encode(c);
            if (res == false){
                printf("Invalid GPS sentence\n");
            }

        }
        

        Wire.endTransmission();
        printf("Got GPS data\n");
        std::ostringstream data;

        data << xTaskGetTickCount() << " || " << _tiny_gps.altitude() << "\n";
        SDData *sddata = new SDData();
        sddata->file_name = new std::string("measure");
        sddata->message = new std::string(data.str());

        if (xQueueSend(_dataOutSD, &(sddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
        {
            printf("Failed to post thermistor data\n");
        }
}