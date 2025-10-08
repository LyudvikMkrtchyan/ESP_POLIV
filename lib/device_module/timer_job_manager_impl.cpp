#include "timer_job_manager_impl.hpp"
#include "logger.h"
void TimerJobsManager::add_job_to_list(JobParams job)
{
    timer_jobs_list_.push_back(job);

}

void TimerJobsManager::iterate_over_the_list()
{
    list_iter_type begin = timer_jobs_list_.begin();
    list_iter_type end = timer_jobs_list_.end();

    for(; begin != end; begin++)
    {
    }
}

TimerJobsManagerBase* get_timer_job_manager()
{
    Logger::log_debug("get_timer_job_manager() is runned .");
    TimerJobsManagerBase* timer =  new TimerJobsManager();
    return timer;
}

