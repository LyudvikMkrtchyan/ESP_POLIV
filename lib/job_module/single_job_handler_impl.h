#pragma once

#include "single_job_handler_base.h"


class DefaultSingleJobHandler : public SingleJobHandlerBase {

public:
    void handle(const JobParams&) override;

};