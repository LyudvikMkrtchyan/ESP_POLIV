#include "device_impl.h"
#include "job_handlers.h"
#include "logger.h"
#include "../config_files/utils.h"
#include <exception>



void LocalDevice::setup(int device_pin) {
    device_pin_ = device_pin;
    pinMode(device_pin, OUTPUT);
    digitalWrite(device_pin, RELAY_OFF); // выключаем устройство по умолчанию
}

void LocalDevice::handleJob(const JobParams& job) {

    internal_handleJob(job);
    if(next_device_) {
        next_device_->handleJob(job);
    }
}

void LocalDevice::internal_handleJob(const JobParams& job) {
    
    int internal_pin = get_internal_pin(job.device_name);
    Logger::log_info("Device on pin " + String(device_pin_) + " checking for job with internal pin " + String(internal_pin));
    if(device_pin_ == internal_pin) {

        JobParams job_copy = job; 
        job_copy.pin = device_pin_; 

        switch_job_type_and_handle(job_copy);
    }
 
}

void LocalDevice::switch_job_type_and_handle(const JobParams& job) {
    switch(job.job_type) {
        case JobType::SINGLE_ON:
        case JobType::SINGLE_OFF:
        {
            try
            {
                JobHandlers::get_single_job_handler()->handle(job);
            }
            catch(const std::exception& ex)
            {
                std::string error_text = "JobHandlers::get_single_job_handler()->handle(job); error text " + std::string(ex.what());
                                            
                Logger::log_error(error_text);
            }

            break;
        }   
        default:
            Logger::log_warning("Unknown job type for device on pin: " + String(device_pin_));
            break;
    }
}

void LocalDevice::collect_states(JsonArray& arr) const {
    add_state(arr);
    if(next_device_) {
        next_device_->collect_states(arr);
    }
}

void LocalDevice::add_state(JsonArray& arr) const {
    JsonObject obj = arr.createNestedObject();
    obj["pin"] = get_external_pin(device_pin_);
    obj["state"] = (digitalRead(device_pin_) == RELAY_ON) ? "ON" : "OFF";
}