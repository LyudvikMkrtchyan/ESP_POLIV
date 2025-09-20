#include "Arduino.h"
#include <ArduinoJson.h>

#include "job_params.h"



JobParams::JobParams()
    : job_type(JobType::Unknown),
        job_status(JobStatus::Pending),
        start_timestamp(0),
        execution_second(0),
        end_timestamp(0),
        priority(0),
        id(0),
        description("") {}


bool JobParams::fromJson(const String& jsonStr) 
{
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonStr);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    Serial.println("Parsed JSON:");
    serializeJsonPretty(doc, Serial);
    Serial.println();

    id = doc["id"] | 0;
    device_name = doc["device_name"] | "";
    priority = doc["priority"] | 0;
    description = doc["description"] | "";
    start_timestamp = doc["start_timestamp"] | 0;
    execution_second = doc["execution_second"] | 0;
    end_timestamp = doc["end_timestamp"] | 0;

    // enums храним как строку
    Serial.println("job_type value:");
    Serial.println(doc["job_type"].as<const char*>());
    Serial.println("job_type as int:");
    Serial.println(doc["job_type"].as<int>());  
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

String JobParams::toJson() const 
{
    StaticJsonDocument<512> doc;
    doc["id"] = id;
    doc["pin"] = pin;
    doc["priority"] = priority;
    doc["description"] = description;
    doc["start_timestamp"] = start_timestamp;
    doc["execution_second"] = execution_second;
    doc["end_timestamp"] = end_timestamp;
    doc["device_name"] = device_name;

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

// Проверка валидности
bool JobParams::isValidJobType(int value) {
    switch (static_cast<JobType>(value)) {
        case JobType::Unknown:
        case JobType::SINGLE_ON:
        case JobType::SINGLE_OFF:
        case JobType::JOB:
        case JobType::CONFIGURATION:
            return true;
        default:
            return false;
    }
}

