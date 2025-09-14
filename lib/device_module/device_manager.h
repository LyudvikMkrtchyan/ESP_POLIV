#pragma once

#include "device_impl.h"

class DeviceManager{
public:
    DeviceManager();
    void handele_job(const JobParams& job);
    void print_state();

private:
    void init_devices();

private:
    Device* devices_head_;

};