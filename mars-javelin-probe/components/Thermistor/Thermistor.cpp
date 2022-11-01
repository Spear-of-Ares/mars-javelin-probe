/*********************************************************************************
*     File Name           :     /components/Thermistor/Thermistor.cpp
*     Created By          :     jon
*     Creation Date       :     [2022-10-24 21:07]
*     Last Modified       :     [2022-10-28 01:50]
*     Description         :     Class definition and implementation for gathering data from both thermistors 
**********************************************************************************/
#include "Thermistor.h"

ThermistorComponent::ThermistorComponent(QueueHandle_t dataOutSD){
  _dataOutSD = dataOutSD; 

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);

#ifdef THERM2_ATTACHED
  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
#endif 
}

void ThermistorComponent::vMainLoop_Task(void *arg){
  ThermistorComponent therm_component = *((ThermistorComponent*)arg);

  // delay in ms between samples required for specified sample rate
  const TickType_t xDelay = (1000 / THERM_SAMPLE_HZ) / portTICK_PERIOD_MS;

  for(;;){
    therm_component.logThermistors();
    vTaskDelay(xDelay);
  }
  vTaskDelete(NULL);
}

float ThermistorComponent::calcRT(float R1, float VS, float Vo){
  return R1 * (Vo/(VS - Vo));
}

float ThermistorComponent::calcTemp(float To, int Beta, float Ro, float RT){
  return 1.0 / ( (1.0/To) + (log(RT/Ro)/Beta) );
}

void ThermistorComponent::logThermistors(){
  readThermistors();
  float therm1_RT = calcRT(THERM1_R1, THERM1_VS, _therm1_vo);
  float therm1_c = KELVIN_TO_CELSIUS(calcTemp(THERM1_TO, THERM1_B, THERM1_RO, therm1_RT));
#ifdef THERM2_ATTACHED
  float therm2_RT = calcRT(THERM2_R1, THERM2_VS, _therm2_vo);
  float therm2_c = KELVIN_TO_CELSIUS(calcTemp(THERM2_TO, THERM2_B, THERM2_RO, therm2_RT));
#else
  float therm2_c = 0;
#endif

  std::ostringstream data;

  data << xTaskGetTickCount() << " || " THERM_TASK_ID << " || T1: " << therm1_c << " C | T2: " << therm2_c << " C\n";
  SDData *sddata = new SDData();                    
  sddata->file_name = new std::string("measure");     
  sddata->message = new std::string(data.str());        


  if(xQueueSend(_dataOutSD, &(sddata), 10/portTICK_PERIOD_MS) != pdTRUE){
    printf("Failed to post thermistor data\n");
  }
}

void ThermistorComponent::readThermistors(){
  adc_power_acquire();
  _therm1_vo = adc1_get_raw(ADC1_CHANNEL_6) * THERM1_VS/4095.0;
#ifdef THERM2_ATTACHED
  _therm2_vo = adc1_get_raw(ADC1_CHANNEL_7) * THERM1_VS/4095.0;
#else
  _therm2_vo = 0;
#endif
  adc_power_release();
}
