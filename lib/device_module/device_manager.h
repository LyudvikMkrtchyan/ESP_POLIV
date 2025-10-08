#pragma once
#include <memory>
#include <string>
#include "device_impl.h"
#include "mqtt_manager_esp.hpp"
#include "timer_job_manager_base.hpp"
#include "configs_job_manager_base.hpp"

class DeviceManager {
public:
    // Конструктор для работы с Builder
    DeviceManager(std::unique_ptr<Device> devices_head,
                  std::unique_ptr<MqttManagerBase> mqtt_manager,
                  const MqttManagerBase::Params& mqtt_params,
                  std::unique_ptr<TimerJobsManagerBase> timer_job,
                  std::unique_ptr<ConfigsJobsManagerBase> config_job,
                  const std::string& message_topic = "autowatering/module_1/command",
                  const std::string& answer_topic = "autowatering/module_1/status");

    void handle_job(const JobParams& job);
    void print_state();
    void on_mqtt_message(const std::string& topic, const std::string& payload);
    void loop();
    void iterate_over_the_timer_job_list_();
    void configuration_operations();

private:
    void init_devices();
    int get_pin_by_device_name(const std::string& device_name);
    void answer_server_to_mqtt(JobParams);

private:
    std::unique_ptr<Device> devices_head_;
    std::unique_ptr<MqttManagerBase> mqtt_manager_;
    MqttManagerBase::Params mqtt_params_;
    std::string on_mqtt_message_topic_;
    std::string on_mqtt_answer_topic_;
    std::unique_ptr<TimerJobsManagerBase> timer_job_;
    std::unique_ptr<ConfigsJobsManagerBase> config_job_;
};
