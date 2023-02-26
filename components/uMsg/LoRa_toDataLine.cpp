#include "LoRa_toDataLine.h"

std::string LoRa_state_toString(umsg_LoRa_lora_state_t state)
{
    switch (state)
    {
    case LORA_OK:
        return "OK";
    case LORA_SLEEPING:
        return "SLEEPING";
    case LORA_OFF:
        return "OFF";
    case LORA_DISCONNECTED:
        return "DISCONNECTED";
    default:
        return "?";
    }
}

std::string LoRa_msg_type_toString(umsg_LoRa_msg_type_t msg_type)
{
    switch (msg_type)
    {
    case LORA_MSG_SENSOR_DATA:
        return "SENSOR_DATA";
    case LORA_MSG_GPS_DATA:
        return "GPS_DATA";
    case LORA_MSG_RESPONSE:
        return "RESPONSE";
    case LORA_MSG_COMMAND:
        return "COMMAND";
    case LORA_MSG_OTHER:
        return "OTHER";
    default:
        return "?";
    }
}

DataLine LoRa_received_toDataLine(umsg_LoRa_received_msg_t &data)
{
    DataLine dataline;
    dataline.name = "LORA_RECV";
    dataline.recorded_tick = data.receive_tick;
    dataline.data.push_back(LoRa_msg_type_toString(data.recv_msg_type));
    dataline.data.push_back(std::to_string(data.RSSI));
    dataline.data.push_back(std::to_string(data.signal_to_noise));
    dataline.data.push_back(std::to_string(data.frequency_error));

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < sizeof(data.msg_data) && data.msg_data[i] != 0x00; i++)
    {
        ss << std::hex << std::setw(2) << static_cast<int>(data.msg_data[i]);
    }
    dataline.data.push_back(ss.str());

    return dataline;
}
DataLine LoRa_sent_toDataLine(umsg_LoRa_sent_msg_t &data)
{
    DataLine dataline;
    dataline.name = "LORA_SEND";
    dataline.recorded_tick = data.send_tick;
    dataline.data.push_back(LoRa_msg_type_toString(data.sent_msg_type));

    return dataline;
}
DataLine LoRa_state_toDataLine(umsg_LoRa_state_msg_t &data)
{
    DataLine dataline;
    dataline.name = "LoRA_STATE";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(LoRa_state_toString(data.state));

    return dataline;
}