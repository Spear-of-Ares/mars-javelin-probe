#include "Stats_toDataLine.h"

DataLine Stats_task_toDataLine(umsg_Stats_task_run_time_stats_t &data)
{
    DataLine dataline;

    dataline.name = "STATS_TASK";
    dataline.recorded_tick = data.measure_tick;

    std::string name = "";
    for (int i = 0; i < sizeof(data.task_name) && data.task_name[i] != '\0'; i++)
    {
        name += data.task_name[i];
    }

    dataline.data.push_back(name);
    dataline.data.push_back(std::to_string(data.run_time));
    dataline.data.push_back(std::to_string(data.cpu_usage));
    dataline.data.push_back(std::to_string(data.stack_hwm));

    return dataline;
}
DataLine Stats_system_toDataLine(umsg_Stats_system_run_time_stats_t &data)
{
    DataLine dataline;

    dataline.name = "STATS_TASK";
    dataline.recorded_tick = data.measure_tick;
    dataline.data.push_back(std::to_string(data.free_heap_size));

    return dataline;
}