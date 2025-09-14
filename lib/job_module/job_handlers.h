#pragma once

#include <memory>
#include "single_job_handler_base.h"
#include "alarm_job_handler_base.h"

class JobHandlers {
public:
    static JobHandlers& instance();

    static SingleJobHandlerBase* get_single_job_handler();
    static AlarmJobHandlerBase* get_alarm_job_handler();

    static void set_single_job_handler(std::unique_ptr<SingleJobHandlerBase> handler);
    static void set_alarm_job_handler(std::unique_ptr<AlarmJobHandlerBase> handler);

private:
    JobHandlers() = default;
    ~JobHandlers() = default;

    JobHandlers(const JobHandlers&) = delete;
    JobHandlers& operator=(const JobHandlers&) = delete;

    std::unique_ptr<SingleJobHandlerBase> single_handler_;
    std::unique_ptr<AlarmJobHandlerBase> alarm_handler_;
};
