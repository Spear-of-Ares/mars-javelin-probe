/*********************************************************************************
*     File Name           :     mars-javelin-probe/main/mars-javelin-probe.c
*     Created By          :     jon
*     Creation Date       :     [2022-10-03 22:40]
*     Last Modified       :     [2022-10-03 22:42]
*     Description         :     Coordinates and controls generation of new tasks 
*                               Using the ESP Arduino library for access to a wider number of
*                               libraries for components, such as the IridiumSBD library.
*
*                               Using Arduino as a component from the following article:
*                               https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/esp-idf_component.html
**********************************************************************************/



#include <stdio.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "IridiumSBD.h"


void vIridiumSBDUpdate(void *pvParameters)
{
    HardwareSerial IridiumSerial(1); // using uart_num 1
    
    IridiumSBD modem(IridiumSerial);
    IridiumSerial.begin(19200, SERIAL_8N1, 4, 2);
    printf("Starting modem...\n");
    int err = modem.begin();
    if (err != ISBD_SUCCESS)
    {
        printf("Begin failed: error %d\n", err);
        if (err == ISBD_NO_MODEM_DETECTED)
        {
            printf("No modem detected: check wiring.");
        }
    }

    // Test getting firmware version
    char version[12];
    err = modem.getFirmwareVersion(version, sizeof(version));
    if (err != ISBD_SUCCESS)
    {
        printf("FirmwareVersion failed: error %d\n", err);
        return;
    }
    // This is assuming that getFirmwareVersion is supplying us with 0-terminated string
    printf("Firmware Version is %s.", version);

    // Test the signal quality. Value between 1 and 5
    int signalQuality = -1;
    err = modem.getSignalQuality(signalQuality);
    if (err != ISBD_SUCCESS)
    {
        printf("SignalQuality failed: error %d\n", err);
        return;
    }

    printf("On a scale of 0 to 5, signal quality is currently %d.", signalQuality);

    // Try to send a message
    printf("Trying to send the message. This might take several minutes.\r\n");
    err = modem.sendSBDText("Hello, world!");
    if (err != ISBD_SUCCESS)
    {
        printf("sendSBDText failed: error %d\n", err);
        if (err == ISBD_SENDRECEIVE_TIMEOUT)
        {
            printf("Try again with a better view of the sky.");
        }
    }
    else
    {
        printf("Hey, it worked!");
    }

    vTaskDelete( NULL) ;
}

extern "C" void app_main(void)
{
    initArduino();
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    xReturned = xTaskCreate(
                        vIridiumSBDUpdate,          // Task
                        "Iridium SBD Connection",   // Name of task
                        1024*4,                     // Stack size
                        NULL,                       // Parameters
                        10,                          // Priority
                        &xHandle                     // Task handle
                        );
    if (xReturned != pdPASS)
    {
        printf("Could not start the Iridium SBD task");
    }
}
