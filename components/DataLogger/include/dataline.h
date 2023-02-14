#ifndef __DATALINE_H__
#define __DATALINE_H__

#include <string>
#include <vector>
typedef struct
{
    std::string name;
    uint32_t recorded_tick;
    std::vector<std::string> data;

    std::string toString();
} DataLine;

#endif