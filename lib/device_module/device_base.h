#pragma once

#include <job_params.h>
#include <device_options.h>



class Device
{
public:
    Device(Device* next_device):next_device_(next_device) {}
    virtual ~Device() {}
    virtual void handleJob(const JobParams& job) = 0;
    virtual void collect_states(JsonArray& arr) const = 0;

protected:
    Device* next_device_;
};

