#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#include "../device_module/device_options.h"

enum class JobType {
    Unknown = 0,
    SINGLE_ON = 1,
    SINGLE_OFF = 2,
    JOB = 3,
    CONFIGURATION = 4
};

enum class JobStatus {
    Pending,
    Accepted, 
    Running,
    Completed,
    Failed
};

std::string jobTypeEnumToStirng(JobType type);

std::string jobStatusEnumToString(JobStatus status);

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
    bool fromJson(const std::string& jsonStr) ;

    // Сериализация в JSON
    String toJson()const ;

    // Проверка валидности
    static bool isValidJobType(int value) ;
private:
    
    void print() const {
        Serial.println(this->toJson());
    }
};
