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
    // set to min value so we get immidiate update
    _lastUpdateIridium = 0x0000;
    _lastUpdateLoRa = 0x0000;
  while(_GNSS_1.begin(Wire) == false){
    printf("Could not start _GNSS_1\n");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  printf("GNSS 1 setup successfully\n");
  _GNSS_1.setNavigationFrequency(1); // Produce two solutions per second
  _GNSS_1.setAutoPVT(true);
#ifdef DUAL_GPS
  while(_GNSS_2.begin(Wire1) == false){
    printf("Could not start _GNSS_2\n");
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  printf("GNSS 2 setup successfully\n");
  _GNSS_2.setNavigationFrequency(1); // Produce two solutions per second
  _GNSS_2.setAutoPVT(true);
#endif
  printf("Finished GPS startup\n");
}

std::string GPSComponent::getGPS_MSG(int gps)
{
    std::ostringstream data;


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
        data << " Long: " << longitude << " (degrees * 10^-7) |";

        long altitude = myGNSS->getAltitude();
        data << " Alt: " << altitude/1000.0 << " (m) |";

        int SIV = myGNSS->getSIV();
        data << " SIV: " << SIV << " |";

        int year = myGNSS->getYear();
        int month = myGNSS->getMonth();
        int day = myGNSS->getDay();
        int hour = myGNSS->getHour();
        int minute = myGNSS->getMinute();
        int second = myGNSS->getSecond();
        int msec = myGNSS->getMillisecond();
        data << " | Time: " << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second << ":" << msec;

        data << " | Time is ";
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


        int PDOP = myGNSS->getPDOP();
        data << " PDOP: " << PDOP << " (10^-2) |";

        float ATTroll = myGNSS->getATTroll();
        data << " ATTroll: " << ATTroll << " |";
        float ATTpitch = myGNSS->getATTpitch();
        data << " ATTpitch: " << ATTpitch << " |";
        float ATTheading = myGNSS->getATTheading();
        data << " ATTheading: " << ATTheading << " |";

        int32_t horizAccEst = myGNSS->getHorizontalAccEst();
        data << " horizAccEst: " << horizAccEst << " |";
        int32_t verAccEst = myGNSS->getVerticalAccEst();
        data << " verAccEst: " << verAccEst << " |";
        int32_t nedNorthVel = myGNSS->getNedNorthVel();
        data << " nedNorthVel: " << nedNorthVel << " |";
        int32_t nedEastVel = myGNSS->getNedEastVel();
        data << " nedEastVel: " << nedEastVel << " |";
        int32_t nedDownVel = myGNSS->getNedDownVel();
        data << " nedDownVel: " << nedDownVel << " |";
        int32_t gndSpeed = myGNSS->getGroundSpeed(); // Returns speed in mm/s
        data << "gndSpeed" << gndSpeed << " |";
        int32_t heading = myGNSS->getHeading();     // Returns heading in degrees * 10^-5
        data << " heading: " << heading << " |";
        uint32_t speedAccEst = myGNSS->getSpeedAccEst();
        data << " speedAccEst: " << speedAccEst << " |";
        uint32_t headingAccEst = myGNSS->getHeadingAccEst();
        data << " headingAccEst: " << headingAccEst << " |";

    }
    else{
        data << "No Data";
    }
    return data.str();
}

void GPSComponent::sendData(std::string msg){
    std::ostringstream data;
    data << xTaskGetTickCount() << " || " << GPS_COMP_ID << " || ";
    data << msg;

    SDData *sddata = new SDData();
    sddata->file_name = new std::string("measure");
    sddata->message = new std::string(data.str());



    if (xQueueSend(_dataOutSD, &(sddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
    {
        printf("Failed to post GPS data to SD\n");
    }

    TickType_t curr_tick = xTaskGetTickCount();
    // Update iridium once every 10 seconds
    if (curr_tick - _lastUpdateIridium > 10000/portTICK_PERIOD_MS){
        std::string *iriddata = new std::string(data.str());
        _lastUpdateIridium = curr_tick;
        if (xQueueSend(_dataOutIridium, &(iriddata), 10 / portTICK_PERIOD_MS) != pdTRUE)
        {
            printf("Failed to post GPS data to Iridium\n");
        }
    }
    

    // Update LoRa every 2500
    if (curr_tick - _lastUpdateLoRa > 2500 / portTICK_PERIOD_MS)
    {
        std::string *loradata = new std::string(data.str());
        _lastUpdateLoRa = curr_tick;
        if (xQueueSend(_dataOutLoRa, &(loradata), 10 / portTICK_PERIOD_MS) != pdTRUE)
        {
            printf("Failed to post GPS data to LoRa\n");
        }
    }
}

void GPSComponent::get_data(){
    std::string msg1 = getGPS_MSG(1);
    sendData(msg1);
#ifdef DUAL_GPS
    std::string msg2 = getGPS_MSG(2);
    sendData(msg2);
#endif
}
