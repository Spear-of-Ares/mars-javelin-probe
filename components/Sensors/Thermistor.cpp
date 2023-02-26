#include "Thermistor.h"

namespace Thermistors
{
    float calcTemp(float To, int Beta, float Ro, float RT)
    {
        return 1.0 / ((1.0 / To) + (log(RT / Ro) / Beta));
    }

    void readThermistors(float &internal_thermistor_c, float &external_thermistor_c)
    {
        float internal_thermistor_voltage = adc1_get_raw(ADC1_CHANNEL_6) * INTERNAL_THERMISTOR_VS / 4095.0;
        float external_thermistor_voltage = adc1_get_raw(ADC1_CHANNEL_7) * EXTERNAL_THERMISTOR_VS / 4095.0;

        float internal_thermistor_RT = CALCULATE_RT(INTERNAL_THERMISTOR_R1,
                                                    INTERNAL_THERMISTOR_VS,
                                                    internal_thermistor_voltage);

        float external_thermistor_RT = CALCULATE_RT(EXTERNAL_THERMISTOR_R1,
                                                    EXTERNAL_THERMISTOR_VS,
                                                    external_thermistor_voltage);

        internal_thermistor_c = KELVIN_TO_CELSIUS(calcTemp(INTERNAL_THERMISTOR_TO,
                                                           INTERNAL_THERMISTOR_B,
                                                           INTERNAL_THERMISTOR_RO,
                                                           internal_thermistor_RT));

        external_thermistor_c = KELVIN_TO_CELSIUS(calcTemp(EXTERNAL_THERMISTOR_TO,
                                                           EXTERNAL_THERMISTOR_B,
                                                           EXTERNAL_THERMISTOR_RO,
                                                           external_thermistor_RT));
    }
}