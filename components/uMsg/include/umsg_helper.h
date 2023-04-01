#ifndef __UMSG_HELPER_H__
#define __UMSG_HELPER_H__

extern "C"
{
#include "umsg_port.h"
}

#include "freertos/FreeRTOS.h" // For pdPASS

namespace umsg_helpers
{
    template <typename T>
    bool umsg_get_data(umsg_sub_handle_t subscription, T *data, uint8_t (*peek_function)(T *), uint8_t (*receive_function)(umsg_sub_handle_t, T *, uint32_t))
    {
        T temp;
        int timeout = 1;
        if (peek_function(&temp))
        {
            if (receive_function(subscription, data, timeout) == pdPASS)
            {
                return true;
            }
        }
        return false;
    }
}

#endif