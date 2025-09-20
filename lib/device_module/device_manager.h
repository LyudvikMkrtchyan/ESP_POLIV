#pragma once

#include "device_impl.h"

class DeviceManager{
public:
    DeviceManager();
    void handele_job(const JobParams& job);
    void print_state();

private:
    void init_devices();
    int get_pin_by_device_name(const std::string& device_name);

private:
    Device* devices_head_;

};