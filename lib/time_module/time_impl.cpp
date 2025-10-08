#include "time_impl.hpp"
#include "backend_server.h"
#include "backend_server_params.h"
#include "logger.h"


#include <ArduinoJson.h>

TimeBase* get_time_object_with_ownership()
{
    Logger::log_debug("get_time_object_with_ownership() is runned");
    BackendServers backend_server = BackendServers::getInstance();

    backend_server::RequestParams request_params;
    request_params.type = backend_server::RequestType::GET;
    request_params.endpoint = "get_current_time";

    ResponseBase* response = backend_server.send_request(request_params);

    Logger::log_debug("get_time_object_with_ownership(){response = }" + response->get_response_body());

    // Parse JSON response from backend
    DynamicJsonDocument doc(256);
    deserializeJson(doc, response->get_response_body());

    long server_epoch = doc["epoch_time"];     // UNIX timestamp in seconds
    const char* tz = doc["time_zone_gmt"];     // Time zone string, e.g. "+04:00"

    // millis() returns milliseconds since boot → convert to seconds
    time_t local_time = millis() / 1000;

    // offset = difference between server epoch time and local "uptime" in seconds
    time_t offset_ = server_epoch - local_time;

    Logger::log_debug("Calculated offset = " + String(offset_));

    // Create a BackendTime object with this offset
    BackendTime* backend_time = new BackendTime(offset_);

    return backend_time;
};

BackendTime::BackendTime(time_t serverEpochTime) {
    setTime(serverEpochTime);
}

void BackendTime::setTime(time_t epochTime){
    syncMillis_ = millis();
    offset_ = epochTime - (syncMillis_ / 1000);
}

time_t BackendTime::getCurrentTime() const {
    return offset_ + (millis() / 1000);
}

std::string BackendTime::getFormattedTime() const {
    time_t now = getCurrentTime();
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}

