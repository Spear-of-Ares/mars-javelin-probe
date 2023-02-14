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

void GPSComponent::vMainLoop_Task(void *arg)
{
    GPSComponent gps_component = *((GPSComponent *)(arg));
    gps_component.setup();

    for (;;)
    {
        gps_component.get_data();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void GPSComponent::setup()
{

    while (_GNSS_1.begin(Wire) == false)
    {
        printf("Could not start _GNSS_1\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    printf("GNSS 1 setup successfully\n");
    _GNSS_1.setNavigationFrequency(1); // Produce two solutions per second
    _GNSS_1.setAutoPVT(true);
#ifdef DUAL_GPS
    while (_GNSS_2.begin(Wire1) == false)
    {
        printf("Could not start _GNSS_2\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    printf("GNSS 2 setup successfully\n");
    _GNSS_2.setNavigationFrequency(1); // Produce two solutions per second
    _GNSS_2.setAutoPVT(true);
#endif
    printf("Finished GPS startup\n");
}

void GPSComponent::getGPS_MSG(int gps)
{
    umsg_GPS_data_t data;

    SFE_UBLOX_GNSS *myGNSS;
    switch (gps)
    {
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
        data.measure_tick = xTaskGetTickCount();
        data.lat_long[0] = myGNSS->getLatitude();
        data.lat_long[1] = myGNSS->getLongitude();

        data.altitude = myGNSS->getAltitude();

        data.locked_sats = myGNSS->getSIV();

        // int PDOP = myGNSS->getPDOP();
        // data << " PDOP: " << PDOP << " (10^-2)";

        data.time_ymd_hms[0] = myGNSS->getYear();
        data.time_ymd_hms[1] = myGNSS->getMonth();
        data.time_ymd_hms[2] = myGNSS->getDay();
        data.time_ymd_hms[3] = myGNSS->getHour();
        data.time_ymd_hms[4] = myGNSS->getMinute();
        data.time_ymd_hms[5] = myGNSS->getSecond();
        data.time_ymd_hms[6] = myGNSS->getMillisecond();
        data.valid_time = myGNSS->getTimeValid();

        // data << "valid  Date is ";
        // if (myGNSS->getDateValid() == false)
        // {
        //     data << "not ";
        // }
        // data << "valid";

        umsg_GPS_data_publish(&data);
    }
}

void GPSComponent::get_data()
{
    getGPS_MSG(1);
#ifdef DUAL_GPS
    getGPS_MSG(2);
#endif
}
