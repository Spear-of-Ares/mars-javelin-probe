#include "CommandCenter_toDataLine.h"

DataLine Command_toDataLine(umsg_CommandCenter_command_t &data)
{
    DataLine dataline;
    dataline.name = "CMDC";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.cutdown) + std::to_string(data.low_alt_data_gather) + std::to_string(data.self_test));
    return dataline;
}