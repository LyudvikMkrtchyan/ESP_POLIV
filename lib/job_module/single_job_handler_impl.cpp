#include "single_job_handler_impl.h"
#include "logger.h"


void DefaultSingleJobHandler::handle(const JobParams& job) {

    switch(job.job_type) {
        
        case JobType::SINGLE_ON:
            digitalWrite(job.pin, RELAY_ON);
            Logger::log_info("Device turned ON: " + String(job.pin));
            break;
        case JobType::SINGLE_OFF:
            digitalWrite(job.pin, RELAY_OFF);
            Logger::log_info("Device turned OFF: " + String(job.pin));

            break;

        default:
            Logger::log_warning("Unknown job type for device on pin: " + String(job.pin));
            break;
    }

    
}