#include "job_handlers.h"
#include "alarm_job_handler_impl.h"
#include "single_job_handler_impl.h"

JobHandlers& JobHandlers::instance() {
    static JobHandlers s;
    return s;
}

SingleJobHandlerBase* JobHandlers::get_single_job_handler() {
    if (!instance().single_handler_) {
        instance().single_handler_ = std::make_unique<DefaultSingleJobHandler>();
    }
    return instance().single_handler_.get();
}

AlarmJobHandlerBase* JobHandlers::get_alarm_job_handler() {
    if (!instance().alarm_handler_) {
        instance().alarm_handler_ = std::make_unique<DefaultAlarmJobHandler>();
    }
    return instance().alarm_handler_.get();
}

void JobHandlers::set_single_job_handler(std::unique_ptr<SingleJobHandlerBase> handler) {
    instance().single_handler_ = std::move(handler);
}

void JobHandlers::set_alarm_job_handler(std::unique_ptr<AlarmJobHandlerBase> handler) {
    instance().alarm_handler_ = std::move(handler);
}
