#pragma once

#include "time_base.hpp"
#include "job_params.h"
#include "logger.h"
#include <Arduino.h>
#include "utils.h"

class ConfigsJobsManagerBase
{
public:
    // Parameters for the config manager
    struct Params
    {
        int wifi_batr_min = 40;  // Minimum WiFi battery threshold
        int wifi_batr_max = 90;  // Maximum WiFi battery threshold
        int wifi_charge_pin = D2;
    };

    // Constructor with parameters
    explicit ConfigsJobsManagerBase(const Params& params)
        : params_(params)
    {
        // Get a time object
        time_ = get_time_object_with_ownership(); // returns TimeBase*
        if (time_) {
            Logger::log_debug(time_->getFormattedTime()); // log the current formatted time
        }
    }

    // Virtual destructor to release the time object
    virtual ~ConfigsJobsManagerBase() {
        delete time_; // delete the time object if ownership is not transferred
    }

    // Pure virtual function to run the configuration loop
    virtual void run_configs_loop() = 0;

protected:
    TimeBase* time_;  // pointer to a time object
    Params params_;   // configuration parameters
};

// Factory function to create a config job manager
ConfigsJobsManagerBase* get_config_job_manager(const ConfigsJobsManagerBase::Params& params);
