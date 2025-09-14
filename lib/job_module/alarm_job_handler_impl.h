#pragma once

#include "alarm_job_handler_base.h"

class DefaultAlarmJobHandler : public AlarmJobHandlerBase {
public:
    void handle(const JobParams&) override ;
};