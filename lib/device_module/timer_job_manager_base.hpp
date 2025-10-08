#pragma once
#include "time_base.hpp"
#include "job_params.h"
#include "logger.h"

class TimerJobsManagerBase
{
public:
    TimerJobsManagerBase()
    {
        //funkcion return owners
        time = get_time_object_with_ownership();
        Logger::log_debug(time->getFormattedTime());
    }
    virtual void add_job_to_list(JobParams job) = 0;

    virtual void iterate_over_the_list() = 0;
protected:
    TimeBase* time;


};

TimerJobsManagerBase* get_timer_job_manager();



