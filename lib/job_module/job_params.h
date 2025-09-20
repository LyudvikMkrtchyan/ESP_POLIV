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
    std::string device_name; // тип устройства для выполнения задачи
    long long start_timestamp;
    int execution_second;
    long long end_timestamp;
    int priority;
    int id;
    int pin;
    String description;

    JobParams();
    
    // Парсинг из JSON
    bool fromJson(const String& jsonStr) ;

    // Сериализация в JSON
    String toJson()const ;

    // Проверка валидности
    static bool isValidJobType(int value) ;
private:
    
    void print() const {
        Serial.println(this->toJson());
    }
};
