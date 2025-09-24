#pragma once

#include "device_impl.h"
#include "mqtt_manager_esp.hpp"

class DeviceManager {
public:
    DeviceManager(const MqttManagerBase::Params);
    void handle_job(const JobParams& job);
    void print_state();

    // 👉 метод, который будет использоваться как MQTT callback
    void on_mqtt_message(const std::string& topic, const std::string& payload);
    void loop();

private:
    void init_devices();
    int get_pin_by_device_name(const std::string& device_name);
    void answer_server_to_mqtt(JobParams);

private:
    Device* devices_head_;
    MqttManagerBase* mqtt_manager_;
    MqttManagerBase::Params mqtt_params_;
    std::string on_mqtt_message_topic_ = "autowatering/module_1/command";
    std::string on_mqtt_answer_topic_ = "autowatering/module_1/status";


};
