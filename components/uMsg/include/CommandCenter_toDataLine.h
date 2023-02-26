#ifndef __COMMANDCENTER_TODATALINE_H__
#define __COMMANDCENTER_TODATALINE_H__

#include <string>
#include "dataline.h"

extern "C"
{
#include "umsg_CommandCenter.h"
}

DataLine Command_toDataLine(umsg_CommandCenter_command_t &data);

#endif