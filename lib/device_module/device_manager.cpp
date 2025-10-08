#include "device_manager.h"
#include "job_params.h"
#include "../3rd_part/logger.h"
#include "../config_files/utils.h"

#include <ArduinoJson.h>   // для парсинга JSON

#include <exception>
#include <string>


DeviceManager::DeviceManager(std::unique_ptr<Device> devices_head,
                             std::unique_ptr<MqttManagerBase> mqtt_manager,
                             const MqttManagerBase::Params& mqtt_params,
                             std::unique_ptr<TimerJobsManagerBase> timer_job,
                             std::unique_ptr<ConfigsJobsManagerBase> config_job,
                             const std::string& message_topic,
                             const std::string& answer_topic)
    : devices_head_(std::move(devices_head))
    , mqtt_manager_(std::move(mqtt_manager))
    , mqtt_params_(mqtt_params)
    , on_mqtt_message_topic_(message_topic)      // сначала члены, которые объявлены раньше
    , on_mqtt_answer_topic_(answer_topic)
    , timer_job_(std::move(timer_job))
    , config_job_(std::move(config_job))

{
    Serial.begin(9600);
    Serial.println("DeviceManager starting...");

    // MQTT callback
    if (mqtt_manager_) {
        mqtt_manager_->set_topic_callback(on_mqtt_message_topic_,
            [this](const std::string& topic, const std::string& payload) {
                this->on_mqtt_message(topic, payload);
            });
        mqtt_manager_->connect();
        mqtt_manager_->subscribe(on_mqtt_message_topic_);
    }

    // Можно инициализировать устройства, если нужно
    if (!devices_head_) {
        init_devices();
    }
}


void DeviceManager::init_devices() {
    // Инициализация устройств и цепочки ответственности
    LocalDevice* device1 = new LocalDevice(nullptr);
    device1->setup(D1); // Пин D1 (GPIO5)

    LocalDevice* device2 = new LocalDevice(device1);
    device2->setup(D2); // Пин D2 (GPIO4)

    LocalDevice* device3 = new LocalDevice(device2);
    device3->setup(D5); // Пин D5 (GPIO14)

    LocalDevice* device4 = new LocalDevice(device3);
    device4->setup(D6); // Пин D6 (GPIO12)

    LocalDevice* device5 = new LocalDevice(device4);
    device5->setup(D7); // Пин D7 (GPIO13)

    devices_head_ = std::unique_ptr<LocalDevice>(device5); 
}

void DeviceManager::handle_job(const JobParams& job) {
    JobParams job_copy = job;
    job_copy.pin = get_pin_by_device_name(job_copy.device_name);

    Logger::log_info("Handling job: " + job_copy.toJson());
    if (devices_head_) {
        try
        {
            devices_head_->handleJob(job_copy);
        }
        catch(const std::exception& ex)
        {
            std::string error_text = "devices_head_->handleJob(job_copy) error text :" + std::string(ex.what());
            Logger::log_error(error_text);
            throw;
        }    

        answer_server_to_mqtt(job);
    }

}

void DeviceManager::print_state() {
    if (devices_head_) {
        StaticJsonDocument<2048> doc;
        JsonArray arr = doc.to<JsonArray>();

        devices_head_->collect_states(arr);

        serializeJson(arr, Serial);
        Serial.println();
    }
}

int DeviceManager::get_pin_by_device_name(const std::string& device_name) {
    return get_internal_pin(device_name);
}

void DeviceManager::on_mqtt_message(const std::string& topic, const std::string& payload) {
    Logger::log_info("[DeviceManager] Received MQTT: " + topic + " -> " + payload);

    JobParams job;
    if (job.fromJson(payload)) {
        Logger::log_info("Новая задача получена: " + job.toJson());
        handle_job(job);
        print_state();
    } else {
        Logger::log_error("Ошибка парсинга MQTT payload: " + payload);
    }
}

void DeviceManager::loop() {
    if (mqtt_manager_) {
        mqtt_manager_->loop();
    }
}

void DeviceManager::iterate_over_the_timer_job_list_()
{
    timer_job_->iterate_over_the_list();
}

void DeviceManager::configuration_operations()
{
    config_job_->run_configs_loop();
}



#include <ArduinoJson.h>

void DeviceManager::answer_server_to_mqtt(JobParams job)
{
    // Определяем статус и время
    switch(job.job_type)
    {
        case JobType::SINGLE_ON:
        case JobType::SINGLE_OFF:
        {
            job.job_status = JobStatus::Completed;
            job.end_timestamp = job.start_timestamp; // или start + execution_second
            break;
        }
        case JobType::JOB:
        {
            job.job_status = JobStatus::Accepted;
            break;
        }
        default:
        {
            job.job_status = JobStatus::Failed;
            break;
        }
    }

    // Формируем JSON
    StaticJsonDocument<512> doc;
    doc["job_id"] = job.id;
    doc["job_type"] = jobTypeEnumToStirng(job.job_type);
    doc["job_status"] = jobStatusEnumToString(job.job_status);
    doc["device_name"] = job.device_name;
    doc["start_timestamp"] = job.start_timestamp;
    doc["end_timestamp"] = job.end_timestamp;
    doc["execution_second"] = job.execution_second;
    doc["priority"] = job.priority;
    doc["description"] = job.description;

    char buffer[512];
    size_t n = serializeJson(doc, buffer);

    mqtt_manager_->publish(on_mqtt_answer_topic_, buffer, n);
}

