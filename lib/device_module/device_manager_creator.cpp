#include "device_manager_creator.hpp"


// Инициализация устройств
DeviceManagerCreator& DeviceManagerCreator::init_devices() 
{
    devices_head_ ;
    return *this;
}

// Настройка таймеров
DeviceManagerCreator& DeviceManagerCreator::setup_timer_job() 
{
    timer_job_ = std::unique_ptr<TimerJobsManagerBase>(get_timer_job_manager());
    return *this;
}

// Настройка конфигурации
DeviceManagerCreator& DeviceManagerCreator::setup_config_job() 
{

    ConfigsJobsManagerBase::Params params;

    config_job_ = std::unique_ptr<ConfigsJobsManagerBase>(get_config_job_manager(params));
    return *this;
}

// Настройка MQTT
DeviceManagerCreator& DeviceManagerCreator::setup_mqtt() {

    MqttManagerBase::Params mqtt_params_;

    mqtt_params_.clientId = "ESP_MAIN";
    mqtt_params_.host = "86.107.197.36";
    mqtt_params_.port = 1883;
    mqtt_params_.keepAlive = 60;
    mqtt_params_.cleanSession = true;
    mqtt_params_.username = "myUser";
    mqtt_params_.password = "myPassword";
    
    mqtt_manager_ = std::make_unique<MqttManagerEsp>(mqtt_params_);
    
    return *this;
}

// Построение DeviceManager
std::unique_ptr<DeviceManager> DeviceManagerCreator::build() {
    return std::make_unique<DeviceManager>(
        std::move(devices_head_),
        std::move(mqtt_manager_),
        mqtt_params_,
        std::move(timer_job_),
        std::move(config_job_),
        message_topic_,
        answer_topic_
    );
}
