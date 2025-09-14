#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../device_module/device_options.h"
#include "../config_files/utils.h"

enum class JobType {
    Unknown,
    SINGLE_ON,
    SINGLE_OFF,
    JOB,
    CONFIGURATION
};

enum class JobStatus {
    Pending,
    Running,
    Completed,
    Failed
};

struct JobParams {
    JobType job_type;
    JobStatus job_status;
    DeviceType device_type; // тип устройства для выполнения задачи
    long long start_timestamp;
    int execution_second;
    long long end_timestamp;
    int priority;
    int id;
    int pin;
    String description;

    JobParams()
        : job_type(JobType::Unknown),
          job_status(JobStatus::Pending),
          device_type(DeviceType::UNKNOWN),
          start_timestamp(0),
          execution_second(0),
          end_timestamp(0),
          priority(0),
          id(0),
          description("") {}

    // Парсинг из JSON
    bool fromJson(const String& jsonStr) {
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, jsonStr);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
        }

        id = doc["id"] | 0;
        pin = doc["pin"] | 0;
        priority = doc["priority"] | 0;
        description = doc["description"] | "";
        start_timestamp = doc["start_timestamp"] | 0;
        execution_second = doc["execution_second"] | 0;
        end_timestamp = doc["end_timestamp"] | 0;

        // enums храним как строку
        if(doc["job_type"].is<int>())
        {
            job_type = static_cast<JobType>(doc["job_type"].as<int>());
        }
        else
        {
            Serial.println("job_type is not int");
        }
        
 
        return true;
    }

    // Сериализация в JSON
    String toJson() const {
        StaticJsonDocument<512> doc;
        doc["id"] = id;
        doc["pin"] = pin;
        doc["priority"] = priority;
        doc["description"] = description;
        doc["start_timestamp"] = start_timestamp;
        doc["execution_second"] = execution_second;
        doc["end_timestamp"] = end_timestamp;

        switch (job_type) {
            case JobType::JOB: doc["job_type"] = "JOB"; break;
            case JobType::CONFIGURATION: doc["job_type"] = "CONFIGURATION"; break;
            default: doc["job_type"] = "Unknown"; break;
        }

        switch (job_status) {
            case JobStatus::Running: doc["job_status"] = "Running"; break;
            case JobStatus::Completed: doc["job_status"] = "Completed"; break;
            case JobStatus::Failed: doc["job_status"] = "Failed"; break;
            default: doc["job_status"] = "Pending"; break;
        }

        String output;
        serializeJson(doc, output);
        return output;
    }

    void print() const {
        Serial.println(this->toJson());
    }
};
