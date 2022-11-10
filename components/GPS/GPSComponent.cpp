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

GPSComponent::GPSComponent(QueueHandle_t dataOutSD, QueueHandle_t dataOutLoRa, QueueHandle_t dataOutIridium){
    _dataOutSD = dataOutSD;
    _dataOutLoRa = dataOutLoRa;
    _dataOutIridium = dataOutIridium;
}

void GPSComponent::vMainLoop_Task(void *arg){
    GPSComponent gps_component = *((GPSComponent*)(arg));
    gps_component.setup();
    
    for(;;){
        gps_component.get_data();
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void GPSComponent::setup(){
  while(_GNSS_1.begin(Wire) == false){
    printf("Could not start _GNSS_1\n");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  printf("GNSS 1 setup successfully\n");

  while(_GNSS_2.begin(Wire1) == false){
    printf("Could not start _GNSS_2\n");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  printf("GNSS 2 setup successfully\n");

  _GNSS_1.setNavigationFrequency(1); // Produce two solutions per second
  _GNSS_1.setAutoPVT(true);

  _GNSS_2.setNavigationFrequency(1); // Produce two solutions per second
  _GNSS_2.setAutoPVT(true);

  printf("Finished GPS startup\n");
}

std::string GPSComponent::getGPS_MSG(int gps)
{
    std::ostringstream data;

    data << xTaskGetTickCount() << " || " << GPS_COMP_ID << " || ";
    int header_len = data.str().length();

    printf("Getting GPS data: %d\n", gps);
    SFE_UBLOX_GNSS *myGNSS;
    switch(gps){
        case 1:
            myGNSS = &_GNSS_1;
            break;
        case 2:
            myGNSS = &_GNSS_2;
            break;
        default:
            myGNSS = &_GNSS_1;
    }
    // Calling getPVT returns true if there actually is a fresh navigation solution available.
    // Start the reading only when valid LLH is available
    if (myGNSS->getPVT() && (myGNSS->getInvalidLlh() == false))
    {
        long latitude = myGNSS->getLatitude();
        data << "Lat: " << latitude;

        long longitude = myGNSS->getLongitude();
        data << " Long: " << longitude << " (degrees * 10^-7) ||";

        long altitude = myGNSS->getAltitude();
        data << " Alt: " << altitude << " (mm) ||";

        byte SIV = myGNSS->getSIV();
        data << " SIV: " << SIV << " ||";

        int PDOP = myGNSS->getPDOP();
        data << " PDOP: " << PDOP << " (10^-2)";

        uint16_t year = myGNSS->getYear();
        uint8_t month = myGNSS->getMonth();
        uint8_t day = myGNSS->getDay();
        uint8_t hour = myGNSS->getHour();
        uint8_t minute = myGNSS->getMinute();
        uint8_t second = myGNSS->getSecond();
        uint16_t msec = myGNSS->getMillisecond();
        data << "\n" << std::string(header_len-3, ' ');
        data << "|| Time: " << year << "-" << month << "-" << day << " " << minute << ":" << second << ":" << msec;

        data << " || Time is ";
        if (myGNSS->getTimeValid() == false)
        {
            data << "not ";
        }
        data << "valid  Date is ";
        if (myGNSS->getDateValid() == false)
        {
            data << "not ";
        }
        data << "valid";

    }
    else{
        data << "No Data";
    }
    return data.str();
}

void GPSComponent::sendData(std::string msg){

    SDData *sddata = new SDData();
    sddata->file_name = new std::string("measure");
    sddata->message = new std::string(msg);

    std::string *loradata = new std::string(msg);
    std::string *iriddata = new std::string(msg);


    if (xQueueSend(_dataOutSD, &(sddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
        printf("Failed to post GPS data to SD\n");
    }

    if (xQueueSend(_dataOutIridium, &(iriddata), 10/portTICK_PERIOD_MS) != pdTRUE)
    {
        printf("Failed to post GPS data to Iridium\n");
    }

    if (xQueueSend(_dataOutLoRa, &(loradata), 10/portTICK_PERIOD_MS) != pdTRUE)
    {
        printf("Failed to post GPS data to LoRa\n");
    }

}

void GPSComponent::get_data(){
  std::string msg1 = getGPS_MSG(1);
  std::string msg2 = getGPS_MSG(2);

    sendData(msg1);
    sendData(msg2);
}
