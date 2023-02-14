#ifndef __LORA_TODATALINE_H__
#define __LORA_TODATALINE_H__

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "dataline.h"

extern "C"
{
#include "umsg_LoRa.h"
}

DataLine LoRa_received_toDataLine(umsg_LoRa_received_msg_t &data);
DataLine LoRa_sent_toDataLine(umsg_LoRa_sent_msg_t &data);
DataLine LoRa_state_toDataLine(umsg_LoRa_state_msg_t &data);

#endif