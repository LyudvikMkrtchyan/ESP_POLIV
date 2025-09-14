#pragma once

#include "job_params.h"

class SingleJobHandlerBase {
public:
    virtual ~SingleJobHandlerBase() = default;
    virtual void handle(const JobParams&) = 0;
};