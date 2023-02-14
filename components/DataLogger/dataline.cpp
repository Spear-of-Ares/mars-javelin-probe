#include "dataline.h"
std::string DataLine::toString()
{
    std::string str = name + " " + std::to_string(recorded_tick);
    for (std::string data : data)
    {
        str += " " + data;
    }
    return str;
}