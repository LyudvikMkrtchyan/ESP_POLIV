#include "timer_job_manager_base.hpp"
#include "job_params.h"
#include "time_base.hpp"
#include "list"



class TimerJobsManager: public TimerJobsManagerBase
{

    typedef std::list<JobParams> list_type;
    typedef list_type::iterator list_iter_type;

    public:
        void add_job_to_list(JobParams job) override;

        void iterate_over_the_list() override;

    private:
        std::list<JobParams> timer_jobs_list_;
        
};