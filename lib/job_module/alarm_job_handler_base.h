#pragma once

#include "job_params.h"


class AlarmJobHandlerBase {
public:
    virtual ~AlarmJobHandlerBase() = default;
    virtual void handle(const JobParams&) = 0;

};