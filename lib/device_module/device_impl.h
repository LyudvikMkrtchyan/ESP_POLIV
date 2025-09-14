#pragma once

#include "device_base.h"

class LocalDevice : public Device
{
public:
    LocalDevice(Device* next_device):Device(next_device){}
    virtual ~LocalDevice() {}
    void setup(int device_pin);
    void handleJob(const JobParams& job)override;
    void collect_states(JsonArray& arr)const override;

    

private:
    void internal_handleJob(const JobParams& job);
    void switch_job_type_and_handle(const JobParams& job);
    void add_state(JsonArray& arr) const;

    
private:
    int device_pin_;
    DeviceType device_type_;
};
