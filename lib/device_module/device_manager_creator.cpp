#include "device_manager_creator.hpp"
#include <ArduinoJson.h>
#include "../readers_module/readers_base.hpp"
#include "../3rd_part/logger.h"


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
DeviceManagerCreator& DeviceManagerCreator::setup_mqtt(const std::string& mqtt_config_file_path) {

    atl::File mqtt_config_file;
    mqtt_config_file.file_path = mqtt_config_file_path;
    mqtt_config_file.file_type = atl::FileType::LOCAL_FILE;

    std::string content = read_file_content(mqtt_config_file);

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, content);
    if (error) {
        Logger::log_error("MQTT config JSON parse error: " + String(error.c_str()));
    } else {
        mqtt_params_.clientId     = doc["client_id"].as<std::string>();
        mqtt_params_.host         = doc["host"].as<std::string>();
        mqtt_params_.port         = doc["port"].as<int>();
        mqtt_params_.keepAlive    = doc["keep_alive"].as<int>();
        mqtt_params_.cleanSession = doc["clean_session"].as<bool>();
        mqtt_params_.username     = doc["username"].as<std::string>();
        mqtt_params_.password     = doc["password"].as<std::string>();
        mqtt_params_.message_topic = doc["message_topic"].as<std::string>();
        mqtt_params_.answer_topic  = doc["answer_topic"].as<std::string>();
        Logger::log_info(String("MQTT config loaded: ") + mqtt_params_.host.c_str() + ":" + String(mqtt_params_.port));
    }

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
        std::move(config_job_)
    );
}
