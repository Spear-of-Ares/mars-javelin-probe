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

    umsg_GPS_state_t state_data;
    state_data.state = GPS_DISCONNECTED;
    state_data.initializing = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_GPS_state_publish(&state_data);

    while (_GNSS_1.begin(Wire) == false)
    {
        printf("Could not start _GNSS_1\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    state_data.state = GPS_OK;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_GPS_state_publish(&state_data);

    _GNSS_1.setNavigationFrequency(1); // Produce two solutions per second
    _GNSS_1.setAutoPVT(true);

    state_data.initializing = 0;
    state_data.initialized = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_GPS_state_publish(&state_data);
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
    // Get DOP will return true if there is Dilution of Precision Available
    // Start the reading only when valid LLH is available
    if (myGNSS->getPVT() && (myGNSS->getInvalidLlh() == false))
    {
        data.measure_tick = xTaskGetTickCount();
        data.lat_long[0] = myGNSS->getLatitude();
        data.lat_long[1] = myGNSS->getLongitude();

        data.altitude = myGNSS->getAltitudeMSL();

        data.locked_sats = myGNSS->getSIV();

        data.p_dilution_precision = myGNSS->getPDOP();
        data.ground_speed = myGNSS->getGroundSpeed();

        data.time_ymd_hms[0] = myGNSS->getYear();
        data.time_ymd_hms[1] = myGNSS->getMonth();
        data.time_ymd_hms[2] = myGNSS->getDay();
        data.time_ymd_hms[3] = myGNSS->getHour();
        data.time_ymd_hms[4] = myGNSS->getMinute();
        data.time_ymd_hms[5] = myGNSS->getSecond();
        data.time_ymd_hms[6] = myGNSS->getMillisecond();
        data.valid_time = myGNSS->getTimeValid();

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
