#ifndef __STATS_TODATALINE_H__
#define __STATS_TODATALINE_H__

#include <string>
#include "dataline.h"

extern "C"
{
#include "umsg_Stats.h"
}

DataLine Stats_task_toDataLine(umsg_Stats_task_run_time_stats_t &data);
DataLine Stats_system_toDataLine(umsg_Stats_system_run_time_stats_t &data);

#endif