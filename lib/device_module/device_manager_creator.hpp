#pragma once
#include <memory>
#include <string>
#include "device_impl.h"
#include "mqtt_manager_esp.hpp"
#include "timer_job_manager_base.hpp"
#include "configs_job_manager_base.hpp"
#include "device_manager.h"  // сюда нужно подключить DeviceManager

class DeviceManagerCreator {
private:
    std::unique_ptr<Device> devices_head_;
    std::unique_ptr<MqttManagerBase> mqtt_manager_;
    MqttManagerBase::Params mqtt_params_;
    std::string message_topic_ = "autowatering/module_1/command";
    std::string answer_topic_ = "autowatering/module_1/status";
    std::unique_ptr<TimerJobsManagerBase> timer_job_;
    std::unique_ptr<ConfigsJobsManagerBase> config_job_;

public:

    DeviceManagerCreator& init_devices();
    DeviceManagerCreator& setup_timer_job();
    DeviceManagerCreator& setup_config_job();
    DeviceManagerCreator& setup_mqtt();

    std::unique_ptr<DeviceManager> build();
};
